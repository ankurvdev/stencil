#pragma once
#include <stencil/stencil.h>

// SECTION START: DECLARATIONS
#if true
namespace UnionTest::Struct1
{
struct Data;
}
template <> struct ReflectionBase::TypeTraits<UnionTest::Struct1::Data&>;
#endif
// SECTION END: DECLARATIONS

// SECTION START: Definitions
#if true
namespace UnionTest
{
namespace Struct1
{
struct Data;
}
namespace Struct1
{

struct Data :
    public ReflectionBase::ObjMarker
{
    /*template <typename...TArgs> Data(TArgs&& ... args)
    {
        ReflectionBase::Construct<Data>(this, std::forward<TArgs>(args)...);
    }*/

    enum class FieldIndex
    {
        Invalid,
        field1
,        field2
    };

    static constexpr size_t FieldCount()
    {
        return 0u
               + 1u
               + 1u
            ;
    }

    static constexpr std::string_view FieldAttributeValue(FieldIndex index, const std::string_view& key)
    {
        switch (index)
        {
        case FieldIndex::field1:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::field2:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::Invalid: break;

        default: break;
        }
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    private:
    shared_string _field1 = {};

    public:
    shared_string&       field1() { return _field1; }
    const shared_string& field1() const { return _field1; }
    void                            field1(shared_string&& val) { _field1 = std::move(val); }
    shared_string&       get_field1() { return _field1; }

    bool isset_field1() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::field1); }

    void set_field1(shared_string&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::field1, _field1, val);
        _field1 = std::move(val);
    }

#if 0
#endif
    private:
    shared_string _field2 = {};

    public:
    shared_string&       field2() { return _field2; }
    const shared_string& field2() const { return _field2; }
    void                            field2(shared_string&& val) { _field2 = std::move(val); }
    shared_string&       get_field2() { return _field2; }

    bool isset_field2() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::field2); }

    void set_field2(shared_string&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::field2, _field2, val);
        _field2 = std::move(val);
    }

#if 0
#endif
};

}    // namespace Struct1
namespace Union1
{
struct Union;
}
namespace Union1
{
enum class UnionType
{
    Invalid,
    field1
,    field2
};

struct Data : public ReflectionBase::ObjMarker
{
    UnionType _type;

    public:
    UnionType Type() const { return _type; }

    UnionType& get_Type() { return _type; }
    void       set_Type(UnionType&& val) { _type = std::move(val); }

    Data() : _type(UnionType::Invalid) {}

    public:
    enum class FieldIndex
    {
        Invalid,
        field1
,        field2
    };

    static constexpr std::string_view FieldAttributeValue(FieldIndex index, const std::string_view& key)
    {
        switch (index)
        {
        case FieldIndex::field1:
        {
            return ::ReflectionServices::EmptyAttributeValue(key);
        }
        case FieldIndex::field2:
        {
            return ::ReflectionServices::EmptyAttributeValue(key);
        }
        case FieldIndex::Invalid: break;

        default: break;
        }
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    private:
    int32_t _field1;

    public:
    int32_t&       field1() { return _field1; }
    const int32_t& field1() const { return _field1; }
    void                            field1(const int32_t& val) { _field1 = val; }
    void                            field1(int32_t&& val) { _field1 = std::move(val); }

    int32_t& get_field1() { return _field1; }
    void                      set_field1(int32_t&& val) { _field1 = std::move(val); }

    private:
    int32_t _field2;

    public:
    int32_t&       field2() { return _field2; }
    const int32_t& field2() const { return _field2; }
    void                            field2(const int32_t& val) { _field2 = val; }
    void                            field2(int32_t&& val) { _field2 = std::move(val); }

    int32_t& get_field2() { return _field2; }
    void                      set_field2(int32_t&& val) { _field2 = std::move(val); }

};
}    // namespace Union1
}    // namespace UnionTest
#endif
// SECTION END: Definitions

// SECTION START: Template specializations
#if true

// SECTION:
template <> struct ReflectionBase::TypeTraits<UnionTest::Struct1::Data&>
{
    struct Traits_field1
    {
        using TOwner     = UnionTest::Struct1::Data;
        using TFieldType = shared_string;

        static constexpr std::string_view Name() { return "field1"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_field1; }
        static constexpr auto TPropertySetter() { return &TOwner::set_field1; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::field1, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_field2
    {
        using TOwner     = UnionTest::Struct1::Data;
        using TFieldType = shared_string;

        static constexpr std::string_view Name() { return "field2"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_field2; }
        static constexpr auto TPropertySetter() { return &TOwner::set_field2; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::field2, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "Struct1"; }
    static constexpr std::string_view           AttributeValue(const std::string_view& key)
    {
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    using ThisType = UnionTest::Struct1::Data;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && ReflectionBase::AreEqual(obj1.field1(), obj2.field1())
               && ReflectionBase::AreEqual(obj1.field2(), obj2.field2())
            ;
    }

    using Handler = ::ReflectionServices::ReflectedStructHandler<UnionTest::Struct1::Data,
                                                                 Traits_field1
,                                                                 Traits_field2
                                                                 >;
};

template <>
struct Stencil::Transaction<UnionTest::Struct1::Data> : Stencil::TransactionT<UnionTest::Struct1::Data>
{
    using TData = UnionTest::Struct1::Data;

    Transaction<shared_string> _subtracker_field1;
    Transaction<shared_string> _subtracker_field2;
    DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<UnionTest::Struct1::Data>(ptr)
        ,
        _subtracker_field1(Obj().field1())
        ,
        _subtracker_field2(Obj().field2())
    {}

    auto& field1()
    {
        MarkFieldEdited_(TData::FieldIndex::field1);
        return _subtracker_field1;
    }
    auto& field2()
    {
        MarkFieldEdited_(TData::FieldIndex::field2);
        return _subtracker_field2;
    }
    void set_field1(shared_string&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::field1, Obj().field1(), val);
        Obj().set_field1(std::move(val));
    }

    void set_field2(shared_string&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::field2, Obj().field2(), val);
        Obj().set_field2(std::move(val));
    }

    template <typename TLambda> auto Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::field1: return lambda("field1", field1()); return;
        case TData::FieldIndex::field2: return lambda("field2", field2()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "field1") { return lambda(TData::FieldIndex::field1, field1()); }
        if (fieldName == "field2") { return lambda(TData::FieldIndex::field2, field2()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("field1", TData::FieldIndex::field1, field1(), Obj().field1());
        lambda("field2", TData::FieldIndex::field2, field2(), Obj().field2());
    }

    void Flush()
    {
        field1().Flush();

        if (IsFieldEdited(TData::FieldIndex::field1))
        {
            if (!field1().IsChanged()) _edittracker.reset(static_cast<uint8_t>(TData::FieldIndex::field1));
        }

        field2().Flush();

        if (IsFieldEdited(TData::FieldIndex::field2))
        {
            if (!field2().IsChanged()) _edittracker.reset(static_cast<uint8_t>(TData::FieldIndex::field2));
        }

        Stencil::TransactionT<UnionTest::Struct1::Data>::Flush_();
    }
};

template <>
struct Stencil::Visitor<UnionTest::Struct1::Data, void> : Stencil::VisitorT<UnionTest::Struct1::Data>
{
    using TData = UnionTest::Struct1::Data;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::field1: lambda("field1", _ref.get().field1()); return;
        case TData::FieldIndex::field2: lambda("field2", _ref.get().field2()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::field1: lambda("field1", _ref.get().field1()); return;
        case TData::FieldIndex::field2: lambda("field2", _ref.get().field2()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        lambda("field1", _ref.get().field1());
        lambda("field2", _ref.get().field2());
    }

    std::reference_wrapper<TData> _ref;
};

template <>
struct Stencil::Visitor<const UnionTest::Struct1::Data, void>
    : Stencil::VisitorT<const UnionTest::Struct1::Data>
{
    using TData = UnionTest::Struct1::Data const;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::field1: lambda("field1", _ref.get().field1()); return;
        case TData::FieldIndex::field2: lambda("field2", _ref.get().field2()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        lambda("field1", _ref.get().field1());
        lambda("field2", _ref.get().field2());
    }

    std::reference_wrapper<TData> _ref;
};

template <> struct ReflectionServices::EnumTraits<UnionTest::Union1::UnionType>
{
    static constexpr const char* EnumStrings[] = {"Invalid",
                                                  "field1",
                                                  "field2",
                                                  nullptr};

    using ValueType = uint32_t;
};

template <> struct ValueTraits<UnionTest::Union1::UnionType>
{
    static constexpr auto    ValueType() { return Value::Type::Unsigned; }
    [[noreturn]] static void Get(Value& /*obj*/) { throw std::logic_error("Not Implemented"); }
    [[noreturn]] static void Get(const Value& /*obj*/) { throw std::logic_error("Not Implemented"); }
    [[noreturn]] static void Check() { throw std::logic_error("Not Implemented"); }
};

template <> struct ReflectionBase::TypeTraits<UnionTest::Union1::UnionType&>
{
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Value; }
    static constexpr std::string_view           Name() { return "Union1"; }

    using Handler = ::ReflectionServices::EnumHandler<UnionTest::Union1::UnionType>;
};

template <> struct ReflectionBase::TypeTraits<UnionTest::Union1::Data&>
{
    struct Traits_field1
    {
        using TOwner     = UnionTest::Union1::Data;
        using TFieldType = int32_t;

        static constexpr std::string_view Name() { return "field1"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_field1; }
        static constexpr auto TPropertySetter() { return &TOwner::set_field1; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::field1, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_field2
    {
        using TOwner     = UnionTest::Union1::Data;
        using TFieldType = int32_t;

        static constexpr std::string_view Name() { return "field2"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_field2; }
        static constexpr auto TPropertySetter() { return &TOwner::set_field2; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::field2, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "Union1"; }
    static constexpr std::string_view           AttributeValue(const std::string_view& key)
    {
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    using ThisType = UnionTest::Union1::Data;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && ReflectionBase::AreEqual(obj1.field1(), obj2.field1())
               && ReflectionBase::AreEqual(obj1.field2(), obj2.field2())
            ;
    }

    using Handler = ::ReflectionServices::ReflectedUnionHandler<UnionTest::Union1::Data,
                                                                UnionTest::Union1::UnionType,
                                                                Traits_field1
,                                                                Traits_field2
                                                                >;
};

template <>
struct Stencil::Visitor<UnionTest::Union1::Data, void> : Stencil::VisitorT<UnionTest::Union1::Data>
{
    using TData = UnionTest::Union1::Data;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::field1: lambda("field1", _ref.get().field1()); return;
        case TData::FieldIndex::field2: lambda("field2", _ref.get().field2()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::field1: lambda("field1", _ref.get().field1()); return;
        case TData::FieldIndex::field2: lambda("field2", _ref.get().field2()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        lambda("field1", _ref.get().field1());
        lambda("field2", _ref.get().field2());
    }

    std::reference_wrapper<TData> _ref;
};

template <>
struct Stencil::Visitor<const UnionTest::Union1::Data, void>
    : Stencil::VisitorT<const UnionTest::Union1::Data>
{
    using TData = UnionTest::Union1::Data const;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::field1: lambda("field1", _ref.get().field1()); return;
        case TData::FieldIndex::field2: lambda("field2", _ref.get().field2()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        lambda("field1", _ref.get().field1());
        lambda("field2", _ref.get().field2());
    }

    std::reference_wrapper<TData> _ref;
};

#endif
// SECTION END: Template specializations

// SECTION START: Inline Function Definitions
#if true

#endif
// SECTION END: Inline Function Definitions
