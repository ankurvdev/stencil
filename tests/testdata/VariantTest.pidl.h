#pragma once
#include <stencil/stencil.h>

// SECTION START: DECLARATIONS
#if true

namespace VariantTest
{
struct Struct1;
struct Variant1;
}    // namespace VariantTest

template <> struct Stencil::TypeTraits<VariantTest::Struct1>;
template <> struct Stencil::TypeTraits<VariantTest::Variant1>;
#endif
// SECTION END: DECLARATIONS

// SECTION START: Definitions
#if true
namespace VariantTest
{
struct Struct1 :
    public Stencil::StructT<Struct1>
{
    shared_string field1 = {};
    shared_string field2 = {};
};
struct Variant1 : public Stencil::VariantT<Variant1>
{
    std::variant<std::monostate,
                 int32_t
,                 int32_t
                 >
        _variant;

    enum class VariantType
    {
        Invalid,
        field1
,        field2
    };

    VariantType Type() const { return static_cast<VariantType>(_variant.index()); }

    int32_t&       field1() { return std::get<int32_t>(_variant); }
    int32_t const& field1() const { return std::get<int32_t>(_variant); }
    int32_t&       field2() { return std::get<int32_t>(_variant); }
    int32_t const& field2() const { return std::get<int32_t>(_variant); }
};
}    // namespace VariantTest
#endif

// SECTION END: Definitions

// SECTION START: Template specializations
#if true

// SECTION:

template <> struct Stencil::TypeTraits<VariantTest::Struct1>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<VariantTest::Struct1>
{
    enum class Fields
    {
        Invalid,
        Field_field1
,        Field_field2
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<VariantTest::Struct1>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<VariantTest::Struct1>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "field1"
,        "field2"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<VariantTest::Struct1>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<VariantTest::Struct1>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<VariantTest::Struct1> : Stencil::TransactionT<VariantTest::Struct1>
{
    using TData  = VariantTest::Struct1;
    using Fields = Stencil::TypeTraitsForIndexable<VariantTest::Struct1>::Fields;

    Transaction<shared_string> _subtracker_field1;
    Transaction<shared_string> _subtracker_field2;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<VariantTest::Struct1>(ptr)
        ,
        _subtracker_field1(Obj().field1)
        ,
        _subtracker_field2(Obj().field2)
    {}

    auto& field1()
    {
        MarkFieldEdited_(Fields::Field_field1);
        return _subtracker_field1;
    }
    auto& field2()
    {
        MarkFieldEdited_(Fields::Field_field2);
        return _subtracker_field2;
    }
    void set_field1(shared_string&& val)
    {
        MarkFieldAssigned_(Fields::Field_field1, Obj().field1, val);
        Obj().field1 = std::move(val);
    }

    void set_field2(shared_string&& val)
    {
        MarkFieldAssigned_(Fields::Field_field2, Obj().field2, val);
        Obj().field2 = std::move(val);
    }

    template <typename TLambda> auto Visit(Fields index, TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_field1: return lambda("field1", field1()); return;
        case Fields::Field_field2: return lambda("field2", field2()); return;
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "field1") { return lambda(Fields::Field_field1, field1()); }
        if (fieldName == "field2") { return lambda(Fields::Field_field2, field2()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("field1", Fields::Field_field1, field1(), Obj().field1);
        lambda("field2", Fields::Field_field2, field2(), Obj().field2);
    }

    void Flush()
    {
        field1().Flush();

        if (IsFieldEdited(Fields::Field_field1))
        {
            if (!field1().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_field1));
        }

        field2().Flush();

        if (IsFieldEdited(Fields::Field_field2))
        {
            if (!field2().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_field2));
        }

        Stencil::TransactionT<VariantTest::Struct1>::Flush_();
    }
};

template <> struct Stencil::Visitor<VariantTest::Struct1> : Stencil::VisitorT<VariantTest::Struct1>
{
    using TData  = VariantTest::Struct1;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey(T& obj, Fields field, TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_field1: return lambda(obj.field1);
        case Fields::Field_field2: return lambda(obj.field2);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies(T& obj, TLambda&& lambda)
    {
        lambda(Fields::Field_field1, obj.field1);
        lambda(Fields::Field_field2, obj.field2);
    }
};

template <> struct Stencil::TypeTraits<VariantTest::Variant1>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<VariantTest::Variant1>
{
    using Key    = VariantTest::Variant1::VariantType;
    using Fields = Key;
};

template <> struct Stencil::EnumTraits<VariantTest::Variant1::VariantType>
{
    using Enum = VariantTest::Variant1::VariantType;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "field1"
,        "field2"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static VariantTest::Variant1::VariantType ForIndex(size_t index)
    {
        return static_cast<VariantTest::Variant1::VariantType>(index);
    }
};

template <> struct Stencil::Visitor<VariantTest::Variant1>
{
    using Fields = VariantTest::Variant1::VariantType;

    template <typename TType, typename TObj, typename TLambda> static void _SetAndVisit(TObj& obj, TLambda&& lambda)
    {
        using Type = std::remove_cvref_t<TType>;
        obj        = Type{};
        lambda(std::get<Type>(obj));
    }

    template <typename T, typename TLambda> static bool VisitKey(T& obj, Fields fields, TLambda&& lambda)
    {
        switch (fields)
        {
        case Fields::field1:
            _SetAndVisit<int32_t>(obj._variant, std::forward<TLambda>(lambda));
            return true;

        case Fields::field2:
            _SetAndVisit<int32_t>(obj._variant, std::forward<TLambda>(lambda));
            return true;

        case Fields::Invalid: [[fallthrough]];
        default: return false;
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies(T& obj, TLambda&& lambda)
    {
        auto fieldType = static_cast<Fields>(obj.index());
        std::visit([&](auto&& arg) { lambda(fieldType, arg); }, obj._variant);
    }
};

// template <> struct Stencil::Visitor<VariantTest::Variant1> : Stencil::StructVisitor<VariantTest::Variant1>
// {};

#endif
// SECTION END: Template specializations

// SECTION START: Inline Function Definitions
#if true

#endif
// SECTION END: Inline Function Definitions
