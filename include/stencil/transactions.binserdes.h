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
        *this << static_cast<uint32_t>(val.size());
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
        size_t      size = read<uint32_t>();
        std::string str(size, 0);
        _istrm.read(reinterpret_cast<char*>(str.data()), static_cast<std::streamsize>(size));
        return shared_string::make(std::move(str));
    }

    std::istream& _istrm;
};

struct BinaryTransactionSerDes
{
    template <ConceptTransaction T> static auto& _DeserializeTo(T& txn, OStrmWriter& writer)
    {
        if constexpr (ConceptTransactionForIndexable<T> || ConceptTransactionForIterable<T>)
        {
            using ElemType = typename Stencil::TransactionTraits<T>::ElemType;

            txn.VisitChanges([&](auto const& key, uint8_t const& mutator, [[maybe_unused]] auto const& mutatordata, auto& subtxn) {
                writer << static_cast<uint8_t>(mutator);
                Stencil::SerDes<std::remove_cvref_t<decltype(key)>, ProtocolBinary>::Write(writer, key);
                switch (mutator)
                {
                case 0:    // Assign
                {
                    using Valype = std::remove_cvref_t<decltype(subtxn.Elem())>;
                    Stencil::SerDes<Valype, ProtocolBinary>::Write(writer, subtxn.Elem());
                    break;
                }
                case 1:    // List Add
                {
                    using Valype = std::remove_cvref_t<decltype(subtxn.Elem())>;
                    Stencil::SerDes<Valype, ProtocolBinary>::Write(writer, subtxn.Elem());
                    break;
                }
                case 2:    // List Remove
                {
                    // No additional information needs to be serialized in
                }
                case 3:    // Edit
                    _DeserializeTo(subtxn, writer);
                    break;
                default: throw std::logic_error("Unknown mutator");
                }
            });
            writer << std::numeric_limits<uint8_t>::max();
        }
        return writer;
    }

    template <ConceptTransaction T> static std::ostream& Deserialize(T& txn, std::ostream& ostr)
    {
        OStrmWriter writer(ostr);
        _DeserializeTo(txn, writer);
        return ostr;
    }

    template <ConceptTransaction T, typename F = void> struct _StructApplicator
    {
        static void Apply(T& /* txn */, IStrmReader& /* reader */) { throw std::logic_error("Invalid"); }
    };

    template <ConceptTransaction T, typename F = void> struct _ListApplicator
    {
        static void Add(T& /* txn */, size_t /* listindex */, IStrmReader& /* reader */) { throw std::logic_error("Invalid"); }
        static void Remove(T& /* txn */, size_t /* listindex */) { throw std::logic_error("Invalid"); }
        static void Apply(T& /* txn */, IStrmReader& /* reader */) { throw std::logic_error("Invalid"); }
    };

    template <ConceptTransactionForIterable T> struct _ListApplicator<T>
    {
        static void Add(T& txn, size_t /* listindex */, IStrmReader& reader)
        {
            using TObj = typename Stencil::TransactionTraits<T>::ElemType;
            typename Stencil::Mutators<TObj>::ListObj obj;
            Stencil::SerDesRead<ProtocolBinary>(obj, reader);
            txn.Add(std::move(obj));
        }
        static void Remove(T& txn, size_t listindex) { txn.Remove(listindex); }

        static void Apply(T& txn, IStrmReader& reader)
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
                    TODO("DoNotCommit txn.Visit(index, [&](auto /* fieldType */, auto& subtxn) {_Apply(subtxn, reader); });");
                }
                else { throw std::logic_error("invalid mutator"); }
            }
        }
    };

    template <ConceptTransaction T> static void _ListAdd(T& txn, size_t listindex, IStrmReader& reader)
    {
        _ListApplicator<T>::Add(txn, listindex, reader);
    }

    template <ConceptTransaction T> static void _ListRemove(T& txn, size_t listindex) { _ListApplicator<T>::Remove(txn, listindex); }

    // template <ConceptTransactionForIterable T> struct _StructApplicator<T>
    //{
    //     static void Apply(T& /* txn */, IStrmReader& /* reader */)
    //     {
    //         // throw std::logic_error("Invalid");
    //     }
    // };

    // template <ConceptTransactionForPrimitive T> struct _StructApplicator<T>
    //{
    //     static void Apply(T& /* txn */, IStrmReader& /* reader */)
    //     {
    //         // throw std::logic_error("Invalid");
    //     }
    // };

    template <ConceptTransactionForIndexable T> struct _StructApplicator<T>
    {
        static void Apply(T& txn, IStrmReader& reader)
        {
            for (auto mutator = reader.read<uint8_t>(); mutator != std::numeric_limits<uint8_t>::max(); mutator = reader.read<uint8_t>())
            {
                auto fieldIndex = reader.read<uint32_t>();
                using TKey      = typename Stencil::TypeTraitsForIndexable<typename TransactionTraits<T>::ElemType>::Key;
                auto fieldEnum  = static_cast<TKey>(fieldIndex);
                if (mutator == 0)    // Set
                {
                    txn.Edit(fieldEnum, [&](auto& /*subtxn*/) {
                        // Visitor<T>::VisitKey(txn.Obj(), fieldEnum, [&](auto& obj) {
                        //     txn.MarkFieldAssigned_(fieldEnum);
                        // TODO: DoNotCommit Stencil::SerDes<std::remove_cvref_t<decltype(subtxn)>, ProtocolBinary>::Read(subtxn,
                        // reader);
                        // });
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
                    txn.Edit(fieldEnum, [&](auto& subtxn) { _Apply(subtxn, reader); });
                    // throw std::logic_error("Unknown Mutator");
                }
                else { throw std::logic_error("invalid mutator"); }
            }
        }
    };

    template <ConceptTransaction T> static void _ApplyOnStruct(T& txn, IStrmReader& reader) { _StructApplicator<T>::Apply(txn, reader); }
    template <ConceptTransaction T> static void _ApplyOnList(T& txn, IStrmReader& reader) { _ListApplicator<T>::Apply(txn, reader); }

    template <ConceptTransaction T> static void _Apply(T& txn, IStrmReader& reader)
    {
        if constexpr (ConceptTransactionForIterable<T>) { _ApplyOnList(txn, reader); }
        else if constexpr (ConceptTransactionForIndexable<T>) { _ApplyOnStruct(txn, reader); }
        return;
    }

    public:
    template <ConceptTransaction T> static std::istream& Apply(T& txn, std::istream& strm)
    {
        IStrmReader reader(strm);
        _Apply(txn, reader);
        return strm;
    }
};

}    // namespace Stencil
