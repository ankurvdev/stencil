#pragma once
#include "serdes.h"
#include "transactions.h"

#include <stdexcept>
#include <string_view>
#include <type_traits>

namespace Stencil
{
struct BinaryTransactionSerDes
{
    template <typename T> static auto& _DeserializeTo(Transaction<T>& txn, OStrmWriter& writer)
    {
        using Traits = Stencil::TypeTraits<T&>;

        if constexpr (Traits::Type() == Stencil::DataType::List)
        {
            txn.VisitChanges(
                [&](auto const& /* name */, auto const& /* type */, uint8_t const& mutator, size_t const& index, auto& subtxn, auto& obj) {
                    using ObjType = std::remove_reference_t<decltype(obj)>;
                    writer << static_cast<uint8_t>(mutator);
                    writer << static_cast<uint32_t>(index);

                    if (mutator == 3) { _DeserializeTo(subtxn, writer); }
                    else if (mutator == 0)
                    {
                        Visitor<ObjType const> visitor(obj);
                        Stencil::BinarySerDes::Serialize(visitor, writer.strm());
                    }
                    else if (mutator == 1)
                    {
                        Visitor<ObjType const> visitor(obj);
                        Stencil::BinarySerDes::Serialize(visitor, writer.strm());
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

        if constexpr (Stencil::TypeTraits<T&>::Type() == Stencil::DataType::Object)
        {
            txn.VisitChanges(
                [&](auto const& /* name */, auto const& type, auto const& mutator, auto const& /* mutatordata */, auto& subtxn, auto& obj) {
                    using ObjType = std::remove_reference_t<decltype(obj)>;

                    writer << static_cast<uint8_t>(mutator);
                    writer << static_cast<uint32_t>(type);

                    if (mutator == 0)
                    {
                        Visitor<ObjType const> visitor(obj);
                        Stencil::BinarySerDes::Serialize(visitor, writer.strm());
                    }
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

    template <typename T>
    struct _ListApplicator<T, std::enable_if_t<Stencil::TypeTraits<T&>::Type() == Stencil::DataType::List>>
    {
        static void Add(Transaction<T>& txn, size_t /* listindex */, IStrmReader& reader)
        {
            typename Stencil::Mutators<T>::ListObj obj;

            Visitor<decltype(obj)> visitor(obj);
            BinarySerDes::Deserialize(visitor, reader.strm());
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

    template <typename T>
    struct _StructApplicator<T, std::enable_if_t<Stencil::TypeTraits<T&>::Type() == Stencil::DataType::Value>>
    {
        static void Apply(Transaction<T>& /* txn */, IStrmReader& /* reader */)
        {
            // throw std::logic_error("Invalid");
        }
    };

    template <typename T>
    struct _StructApplicator<T, std::enable_if_t<Stencil::TypeTraits<T&>::Type() == Stencil::DataType::Object>>
    {
        static void Apply(Transaction<T>& txn, IStrmReader& reader)
        {
            for (auto mutator = reader.read<uint8_t>(); mutator != std::numeric_limits<uint8_t>::max(); mutator = reader.read<uint8_t>())
            {
                auto fieldIndex = reader.read<uint32_t>();

                using FieldIndex = typename T::FieldIndex;
                auto fieldEnum   = static_cast<FieldIndex>(fieldIndex);
                if (mutator == 0)    // Set
                {
                    txn.Visit(fieldEnum, [&](auto /* fieldType */, auto& /* subtxn */) {
                        Visitor<T> visitor(txn.Obj());
                        visitor.Select(fieldIndex - 1);
                        txn.MarkFieldAssigned_(fieldEnum);
                        BinarySerDes::Deserialize(visitor, reader.strm());
                    });

                    // txn.Visit(fieldname, [&](auto fieldType, auto& subtxn) { _ApplyJson(subtxn , fieldType, rhs); });
                }
#if 0
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
        using Traits = Stencil::TypeTraits<T&>;

        if constexpr (Traits::Type() == Stencil::DataType::List) { _ApplyOnList(txn, reader); }
        if constexpr (Traits::Type() == Stencil::DataType::Object) { _ApplyOnStruct(txn, reader); }
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
