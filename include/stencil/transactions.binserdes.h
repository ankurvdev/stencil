#pragma once
#include "protocol_binary.h"
#include "serdes.h"
#include "transactions.h"

#include <stdexcept>
#include <string_view>
#include <type_traits>

namespace Stencil
{

struct OStrmWriter
{
    OStrmWriter(std::ostream& ostr) : _ostr(ostr) {}
    CLASS_DELETE_COPY_AND_MOVE(OStrmWriter);

    template <typename TVal, std::enable_if_t<std::is_trivial<TVal>::value, bool> = true> auto& operator<<(TVal const& val)
    {
        auto spn = AsCSpan(val);
        _ostr.write(reinterpret_cast<char const*>(spn.data()), static_cast<std::streamsize>(spn.size()));
        return *this;
    }

    auto& strm() { return _ostr; }

    auto& operator<<(shared_string const& val)
    {
        *this << val.size();
        if (val.size() > 0) _ostr.write(reinterpret_cast<char const*>(val.data()), static_cast<std::streamsize>(val.size()));
        return *this;
    }

    std::ostream& _ostr;
};

struct IStrmReader
{
    IStrmReader(std::istream& istrm) : _istrm(istrm) {}
    CLASS_DELETE_COPY_AND_MOVE(IStrmReader);

    bool  isEof() { return !_istrm.good(); }
    auto& strm() { return _istrm; }

    template <typename TVal, std::enable_if_t<std::is_trivial<TVal>::value, bool> = true> TVal read()
    {
        TVal val;
        auto spn = AsSpan(val);
        _istrm.read(reinterpret_cast<char*>(spn.data()), static_cast<std::streamsize>(spn.size()));
        return val;
    }

    shared_string read_shared_string()
    {
        size_t      size = read<size_t>();
        std::string str(size, 0);
        _istrm.read(reinterpret_cast<char*>(str.data()), static_cast<std::streamsize>(size));
        return shared_string::make(std::move(str));
    }

    std::istream& _istrm;
};

struct BinaryTransactionSerDes
{
    template <typename T> static auto& _DeserializeTo(Transaction<T>& txn, OStrmWriter& writer)
    {
        using Traits = Stencil::TypeTraits<T&>;

        if constexpr (ConceptIterable<T>)
        {
            txn.VisitChanges(
                [&](auto const& /* name */, auto const& /* type */, uint8_t const& mutator, size_t const& index, auto& subtxn, auto& obj) {
                    using ObjType = std::remove_reference_t<decltype(obj)>;
                    writer << static_cast<uint8_t>(mutator);
                    writer << static_cast<uint32_t>(index);

                    if (mutator == 3) { _DeserializeTo(subtxn, writer); }
                    else if (mutator == 0)
                    {
                        Stencil::SerDes<ObjType, ProtocolBinary>::Write(writer, obj);
                    }
                    else if (mutator == 1)
                    {
                        Stencil::SerDes<ObjType, ProtocolBinary>::Write(writer, obj);
                    }
                    else if (mutator == 2)
                    {
                        // Nothing else needed
                    }
                    else
                    {
                        throw std::logic_error("Unknown mutator");
                    }
                });
            writer << std::numeric_limits<uint8_t>::max();
        }

        if constexpr (ConceptIndexable<T>)
        {
            txn.VisitChanges(
                [&](auto const& /* name */, auto const& type, auto const& mutator, auto const& /* mutatordata */, auto& subtxn, auto& obj) {
                    using ObjType = std::remove_reference_t<decltype(obj)>;

                    writer << static_cast<uint8_t>(mutator);
                    writer << static_cast<uint32_t>(type);

                    if (mutator == 0) { Stencil::SerDes<ObjType, ProtocolBinary>::Write(writer, obj); }
                    else if (mutator == 3)
                    {
                        _DeserializeTo(subtxn, writer);
                    }
                    else
                    {
                        throw std::logic_error("Unknown mutator");
                    }
                });
            writer << std::numeric_limits<uint8_t>::max();
        }
        return writer;
    }

    template <typename T> static std::ostream& Deserialize(Transaction<T>& txn, std::ostream& ostr)
    {
        OStrmWriter writer(ostr);
        _DeserializeTo(txn, writer);
        return ostr;
    }

    template <typename T, typename F = void> struct _StructApplicator
    {
        static void Apply(Transaction<T>& /* txn */, IStrmReader& /* reader */) { throw std::logic_error("Invalid"); }
    };

    template <typename T, typename F = void> struct _ListApplicator
    {
        static void Add(Transaction<T>& /* txn */, size_t /* listindex */, IStrmReader& /* reader */) { throw std::logic_error("Invalid"); }
        static void Remove(Transaction<T>& /* txn */, size_t /* listindex */) { throw std::logic_error("Invalid"); }
        static void Apply(Transaction<T>& /* txn */, IStrmReader& /* reader */) { throw std::logic_error("Invalid"); }
    };

    template <typename T> struct _ListApplicator<T, std::enable_if_t<ConceptIterable<T>>>
    {
        static void Add(Transaction<T>& txn, size_t /* listindex */, IStrmReader& reader)
        {
            typename Stencil::Mutators<T>::ListObj obj;

            Stencil::SerDes<decltype(obj), ProtocolBinary>::Read(obj, reader);
            txn.add(std::move(obj));
        }
        static void Remove(Transaction<T>& txn, size_t listindex) { txn.remove(listindex); }

        static void Apply(Transaction<T>& txn, IStrmReader& reader)
        {
            for (auto mutator = reader.read<uint8_t>(); mutator != std::numeric_limits<uint8_t>::max(); mutator = reader.read<uint8_t>())
            {
                auto index = reader.read<uint32_t>();
                if (mutator == 1)    // List Add
                {
                    Add(txn, index, reader);
                }
                else if (mutator == 2)    // List remove
                {
                    Remove(txn, index);
                }
                else if (mutator == 3)    // Edit
                {
                    txn.Visit(index, [&](auto /* fieldType */, auto& subtxn) { _Apply(subtxn, reader); });
                }
                else
                {
                    throw std::logic_error("invalid mutator");
                }
            }
        }
    };

    template <typename TObj> static void _ListAdd(Transaction<TObj>& txn, size_t listindex, IStrmReader& reader)
    {
        _ListApplicator<TObj>::Add(txn, listindex, reader);
    }

    template <typename TObj> static void _ListRemove(Transaction<TObj>& txn, size_t listindex)
    {
        _ListApplicator<TObj>::Remove(txn, listindex);
    }

    template <typename T> struct _StructApplicator<T, std::enable_if_t<ConceptPrimitive<T>>>
    {
        static void Apply(Transaction<T>& /* txn */, IStrmReader& /* reader */)
        {
            // throw std::logic_error("Invalid");
        }
    };

    template <typename T> struct _StructApplicator<T, std::enable_if_t<ConceptIndexable<T>>>
    {
        static void Apply(Transaction<T>& txn, IStrmReader& reader)
        {
            for (auto mutator = reader.read<uint8_t>(); mutator != std::numeric_limits<uint8_t>::max(); mutator = reader.read<uint8_t>())
            {
                auto fieldIndex = reader.read<uint32_t>();
                using TKey      = typename Stencil::TypeTraitsForIndexable<T>::Key;
                auto fieldEnum  = static_cast<TKey>(fieldIndex);
                if (mutator == 0)    // Set
                {
                    txn.Visit(fieldEnum, [&](auto /* fieldType */, auto& /* subtxn */) {
                        Visitor<T>::VisitKey(txn.Obj(), fieldEnum, [&](auto& obj) {
                            txn.MarkFieldAssigned_(fieldEnum);
                            Stencil::SerDes<std::remove_cvref_t<decltype(obj)>, ProtocolBinary>::Read(obj, reader);
                        });
                    });

                    // txn.Visit(fieldname, [&](auto fieldType, auto& subtxn) { _ApplyJson(subtxn , fieldType, rhs); });
                }
#ifdef TODO1
                else if (mutator == 1)    // List Add
                {
                    txn.Visit(fieldEnum, [&](auto /* fieldType */, auto& subtxn) { _ListAdd(subtxn, reader.read<uint32_t>(), reader); });
                }
                else if (mutator == 2)    // List remove
                {
                    txn.Visit(fieldEnum, [&](auto /* fieldType */, auto& subtxn) {
                        _ListRemove(subtxn, Value(reader.read<uint32_t>()).convert<size_t>());
                    });
                }
#endif
                else if (mutator == 3)    // edit
                {
                    txn.Visit(fieldEnum, [&](auto /* fieldType */, auto& subtxn) { _Apply(subtxn, reader); });
                    // throw std::logic_error("Unknown Mutator");
                }
                else
                {
                    throw std::logic_error("invalid mutator");
                }
            }
        }
    };

    template <typename T> static void _ApplyOnStruct(Transaction<T>& txn, IStrmReader& reader) { _StructApplicator<T>::Apply(txn, reader); }
    template <typename T> static void _ApplyOnList(Transaction<T>& txn, IStrmReader& reader) { _ListApplicator<T>::Apply(txn, reader); }

    template <typename T> static void _Apply(Transaction<T>& txn, IStrmReader& reader)
    {
        if constexpr (ConceptIterable<T>) { _ApplyOnList(txn, reader); }
        else if constexpr (ConceptIndexable<T>)
        {
            _ApplyOnStruct(txn, reader);
        }
        return;
    }

    public:
    template <typename T> static std::istream& Apply(Transaction<T>& txn, std::istream& strm)
    {
        IStrmReader reader(strm);
        _Apply(txn, reader);
        return strm;
    }
};

}    // namespace Stencil
