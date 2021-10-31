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
        using Traits = ReflectionBase::TypeTraits<T&>;

        if constexpr (Traits::Type() == ReflectionBase::DataType::List)
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
                        Stencil::BinarySerDes::Serialize(visitor, writer);
                    }
                    else if (mutator == 1)
                    {
                        Visitor<ObjType const> visitor(obj);
                        Stencil::BinarySerDes::Serialize(visitor, writer);
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
        }

        if constexpr (ReflectionBase::TypeTraits<T&>::Type() == ReflectionBase::DataType::Object)
        {
            txn.VisitChanges(
                [&](auto const& /* name */, auto const& type, auto const& mutator, auto const& /* mutatordata */, auto& subtxn, auto& obj) {
                    using ObjType = std::remove_reference_t<decltype(obj)>;

                    writer << static_cast<uint8_t>(mutator);
                    writer << static_cast<uint32_t>(type);

                    if (mutator == 0)
                    {
                        Visitor<ObjType const> visitor(obj);
                        Stencil::BinarySerDes::Serialize(visitor, writer);
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
        static void Apply(Transaction<T>& /* txn */, uint32_t /* fieldIndex */, uint8_t /* mutator */, IStrmReader& /* reader */)
        {
            throw std::logic_error("Invalid");
        }
    };

    template <typename T, typename F = void> struct _ListApplicator
    {
        static void Add(Transaction<T>& /* txn */, size_t /* listindex */, IStrmReader& /* reader */) { throw std::logic_error("Invalid"); }
        static void Remove(Transaction<T>& /* txn */, size_t /* listindex */) { throw std::logic_error("Invalid"); }
    };

    template <typename T>
    struct _ListApplicator<T, std::enable_if_t<ReflectionBase::TypeTraits<T&>::Type() == ReflectionBase::DataType::List>>
    {
        static void Add(Transaction<T>& txn, size_t /* listindex */, IStrmReader& reader)
        {
            typename Stencil::Mutators<T>::ListObj obj;

            Visitor<decltype(obj)> visitor(obj);
            BinarySerDes::Deserialize(visitor, reader);
            txn.add(std::move(obj));
        }
        static void Remove(Transaction<T>& txn, size_t listindex) { txn.remove(listindex); }
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
    struct _StructApplicator<T, std::enable_if_t<ReflectionBase::TypeTraits<T&>::Type() == ReflectionBase::DataType::Object>>
    {
        static void Apply(Transaction<T>& txn, uint32_t fieldIndex, uint8_t mutator, IStrmReader& reader)
        {
            using FieldIndex = typename T::FieldIndex;
            auto fieldEnum   = static_cast<FieldIndex>(fieldIndex);
            if (mutator == 0)    // Set
            {
                txn.Visit(fieldEnum, [&](auto /* fieldType */, auto& /* subtxn */) {
                    Visitor<T> visitor(txn.Obj());
                    visitor.Select(fieldIndex);
                    txn.MarkFieldAssigned_(fieldEnum);
                    BinarySerDes::Deserialize(visitor, reader);
                });

                // txn.Visit(fieldname, [&](auto fieldType, auto& subtxn) { _ApplyJson(subtxn , fieldType, rhs); });
            }
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
            else
            {
                txn.Visit(fieldEnum, [&](auto /* fieldType */, auto& subtxn) { _Apply(subtxn, reader); });
                // throw std::logic_error("Unknown Mutator");
            }
        }
    };

    template <typename T> static void _ApplyOnStruct(Transaction<T>& txn, uint32_t fieldIndex, uint8_t mutator, IStrmReader& reader)
    {
        _StructApplicator<T>::Apply(txn, fieldIndex, mutator, reader);
    }

    template <typename T> static void _Apply(Transaction<T>& txn, IStrmReader& reader)
    {
        auto mutatortype = reader.read<uint8_t>();
        auto index       = reader.read<uint32_t>();
        _ApplyOnStruct(txn, index, mutatortype, reader);
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
