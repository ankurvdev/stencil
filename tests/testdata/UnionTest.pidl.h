#pragma once
#include <stencil/stencil.h>
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
    void       set_Type(UnionType&& val) { _type = (UnionType)std::move(val); }

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

    int32_t& get_field1()
    {
        return _field1;
        ;
    }
    void set_field1(int32_t&& val) { _field1 = std::move(val); }

    private:
    int32_t _field2;

    public:
    int32_t&       field2() { return _field2; }
    const int32_t& field2() const { return _field2; }
    void                            field2(const int32_t& val) { _field2 = val; }
    void                            field2(int32_t&& val) { _field2 = std::move(val); }

    int32_t& get_field2()
    {
        return _field2;
        ;
    }
    void set_field2(int32_t&& val) { _field2 = std::move(val); }

};
}    // namespace Union1
}    // namespace UnionTest

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

template <typename T> struct Stencil::DeltaTracker<T, std::enable_if_t<std::is_same_v<T, UnionTest::Struct1::Data>>>
{
    using TData = T;

    // TODO : Tentative: We hate pointers
    TData* const _ptr;
    // TODO : Better way to unify creation interface

    std::bitset<TData::FieldCount() + 1> _fieldtracker;
    DeltaTracker<shared_string> _subtracker_field1;
    DeltaTracker<shared_string> _subtracker_field2;
    DELETE_COPY_AND_MOVE(DeltaTracker);

    DeltaTracker(TData* ptr) :
        _ptr(ptr)
        ,
        _subtracker_field1(&_ptr->field1())
        ,
        _subtracker_field2(&_ptr->field2())
    {
        // TODO: Tentative
        static_assert(std::is_base_of<Stencil::ObservablePropsT<TData>, TData>::value);
    }

    TData& Obj() { return *_ptr; }

    static constexpr auto Type() { return ReflectionBase::TypeTraits<TData&>::Type(); }

    size_t NumFields() const { return TData::FieldCount(); }
    bool   IsChanged() const { return _fieldtracker.any(); }

    uint8_t MutatorIndex() const;
    bool    OnlyHasDefaultMutator() const;

    void MarkFieldChanged(typename TData::FieldIndex index) { _fieldtracker.set(static_cast<size_t>(index)); }
    bool IsFieldChanged(typename TData::FieldIndex index) const { return _fieldtracker.test(static_cast<size_t>(index)); }

    size_t CountFieldsChanged() const { return _fieldtracker.count(); }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::field1: lambda(_subtracker_field1); return;
        case TData::FieldIndex::field2: lambda(_subtracker_field2); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    void set_field1(shared_string&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::field1, _ptr->field1(), val);
        _ptr->set_field1(std::move(val));
    }

    void set_field2(shared_string&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::field2, _ptr->field2(), val);
        _ptr->set_field2(std::move(val));
    }

};

template <> struct ReflectionServices::EnumTraits<UnionTest::Union1::UnionType>
{
    static constexpr const char* EnumStrings[] = {"Invalid",
                                                  "field1",
                                                  "field2",
                                                  0};

    using ValueType = uint32_t;
};

template <> struct ValueTraits<UnionTest::Union1::UnionType>
{
    static constexpr auto ValueType() { return Value::Type::Unsigned; }
    static void           Get(Value& /*obj*/) { throw 1; }
    static void           Get(const Value& /*obj*/) { throw 1; }
    static void           Check() { throw 1; }
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
