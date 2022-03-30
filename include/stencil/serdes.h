#pragma once
#include "shared_string.h"
#include "visitor.h"

#include <span>

namespace Stencil
{

template <typename T, typename TProt> struct SerDes;
#if 0
{
    template <typename T, typename TIn> static void  Read(T& obj, TIn& in);
    template <typename T, typename TOut> static void Write(TOut& out, T const& obj);
};
#endif

template <typename T, typename TProt> auto Serialize(T const& obj)
{
    typename TProt::OutType out;
    Stencil::SerDes<T, TProt>::Write(out, obj);
    return out;
}

template <typename T, typename TProt> T Deserialize(typename TProt::InType const& in)
{
    T obj;
    Stencil::SerDes<T, TProt>::Read(obj, in);
    return obj;
}

#ifdef TODO1
struct BinarySerDes
{
    template <typename T> static void Serialize(Stencil::Visitor<T const>& visitor, std::ostream& strm)
    {
        Stencil::OStrmWriter writer(strm);
        switch (visitor.GetDataTypeHint())
        {
        case Stencil::DataType::FixedSize:
        {
        }
        break;
        case Stencil::DataType::Blob:
        {
        }
        break;
        case Stencil::DataType::Dictionary:
        {
            for (size_t i = 0; visitor.TrySelect(i); i++)
            {
                Serialize(visitor, writer.strm());
                visitor.GoBackUp();
            }
        }
        break;
        case Stencil::DataType::List:
        {
            for (uint32_t i = 0; visitor.TrySelect(i); i++)
            {
                writer << i + 1;
                Serialize(visitor, writer.strm());
                visitor.GoBackUp();
            }
            writer << uint32_t{0};
        }
        break;
        case Stencil::DataType::Invalid: [[fallthrough]]; throw std::runtime_error("Unsupported Data Type");
        }
    }

    template <typename TVisitor> static void Deserialize(TVisitor& visitor, std::istream& istrm)
    {
        Stencil::IStrmReader reader(istrm);

        switch (visitor.GetDataTypeHint())
        {
        case Stencil::DataType::Value:
        {
            auto valType = static_cast<Value::Type>(reader.template read<uint8_t>());
            switch (valType)
            {
            case Value::Type::Double: visitor.SetValue(Value{reader.template read<double>()}); break;
            case Value::Type::Empty: TODO(); break;
            case Value::Type::Signed: visitor.SetValue(Value{reader.template read<int64_t>()}); break;
            case Value::Type::String:
                // TODO : See if this can be removed and renamed Value to Value64Bit
                // Value::Type::String -> TrivialConstArray
                visitor.SetValue(Value{reader.read_shared_string()});
                break;
            case Value::Type::Unsigned: visitor.SetValue(Value{reader.template read<uint64_t>()}); break;
            case Value::Type::Unknown: throw std::logic_error("Unknown Value Type");
            }
        }
        break;
        case Stencil::DataType::List:
        {
            auto index = reader.read<uint32_t>();
            for (size_t i = 0; index != 0; i++)
            {
                visitor.Select(i);
                Deserialize(visitor, reader.strm());
                visitor.GoBackUp();
                index = reader.read<uint32_t>();
            }
            break;
        }
        case Stencil::DataType::Object:
        {

            for (size_t i = 0; visitor.TrySelect(i); i++)
            {
                Deserialize(visitor, reader.strm());
                visitor.GoBackUp();
            }
        }
        break;
        case Stencil::DataType::Enum: TODO();
        case Stencil::DataType::Variant: TODO();
        case Stencil::DataType::Invalid: [[fallthrough]];
        case Stencil::DataType::Unknown: throw std::runtime_error("Unsupported Data Type");
        }
    }
};
#endif
}    // namespace Stencil
