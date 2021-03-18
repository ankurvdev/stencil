#pragma once
#include <stencil/stencil.h>
namespace SimpleWebService
{
namespace Data
{
struct Data;
}
namespace Data
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
        randomInteger
,        randomString
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
        case FieldIndex::randomInteger:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::randomString:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::Invalid: break;

        default: break;
        }
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    private:
    int32_t _randomInteger = {};

    public:
    int32_t&       randomInteger() { return _randomInteger; }
    const int32_t& randomInteger() const { return _randomInteger; }
    void                            randomInteger(int32_t&& val) { _randomInteger = std::move(val); }
    int32_t&       get_randomInteger() { return _randomInteger; }

    bool isset_randomInteger() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::randomInteger); }

    void set_randomInteger(int32_t&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::randomInteger, _randomInteger, val);
        _randomInteger = std::move(val);
    }

    private:
    shared_string _randomString = {};

    public:
    shared_string&       randomString() { return _randomString; }
    const shared_string& randomString() const { return _randomString; }
    void                            randomString(shared_string&& val) { _randomString = std::move(val); }
    shared_string&       get_randomString() { return _randomString; }

    bool isset_randomString() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::randomString); }

    void set_randomString(shared_string&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::randomString, _randomString, val);
        _randomString = std::move(val);
    }

};

}    // namespace Data
struct TestInterface : public ReflectionBase::Interface<TestInterface>
{
    public:
    TestInterface()          = default;
    virtual ~TestInterface() = default;
    DELETE_COPY_AND_MOVE(TestInterface);
    virtual ::SimpleWebService::Data::Data Create(
        int32_t const& randomInteger
,        shared_string const& randomString
        )
        = 0;
    // static std::unique_ptr<TestInterface> Create();
};

struct TestInterfaceFactory : public ReflectionBase::InterfaceFactory<TestInterface>
{
    public:
    virtual std::unique_ptr<TestInterface> Activate() = 0;
    virtual ~TestInterfaceFactory()                   = default;
};

struct TestInterface_Create_Args
{
    TestInterface* instance = nullptr;

    int32_t        arg_randomInteger{};
    int32_t&       get_arg_randomInteger() { return arg_randomInteger; }
    int32_t const& get_carg_randomInteger() const { return arg_randomInteger; }
    void                            set_arg_randomInteger(int32_t&& value) { arg_randomInteger = std::move(value); }
    shared_string        arg_randomString{};
    shared_string&       get_arg_randomString() { return arg_randomString; }
    shared_string const& get_carg_randomString() const { return arg_randomString; }
    void                            set_arg_randomString(shared_string&& value) { arg_randomString = std::move(value); }
};

}    // namespace SimpleWebService

template <> struct ReflectionBase::TypeTraits<SimpleWebService::TestInterface_Create_Args&>
{
    struct Traits_arg_randomInteger
    {
        using TOwner     = SimpleWebService::TestInterface_Create_Args;
        using TFieldType = int32_t;

        static constexpr std::string_view    Name() { return "randomInteger"; }
        static const ::ReflectionBase::Flags Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TOwner::get_arg_randomInteger; }
        static constexpr auto TPropertySetter() { return &TOwner::set_arg_randomInteger; }
    };
    struct Traits_arg_randomString
    {
        using TOwner     = SimpleWebService::TestInterface_Create_Args;
        using TFieldType = shared_string;

        static constexpr std::string_view    Name() { return "randomString"; }
        static const ::ReflectionBase::Flags Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TOwner::get_arg_randomString; }
        static constexpr auto TPropertySetter() { return &TOwner::set_arg_randomString; }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "Create"; }
    static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

    using ThisType = SimpleWebService::TestInterface_Create_Args;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && ReflectionBase::AreEqual(obj1.get_carg_randomInteger(), obj2.get_carg_randomInteger())
               && ReflectionBase::AreEqual(obj1.get_carg_randomString(), obj2.get_carg_randomString())
            ;
    }

    using Handler = ::ReflectionServices::ReflectedStructHandler<SimpleWebService::TestInterface_Create_Args
                                                                 ,
                                                                 Traits_arg_randomInteger
                                                                 ,
                                                                 Traits_arg_randomString
                                                                 >;
};
template <> struct ReflectionBase::InterfaceTraits<SimpleWebService::TestInterface>
{
    struct ApiTraits_Create
    {
        using TOwner = SimpleWebService::TestInterface;
        static const ::ReflectionBase::Flags Flags() { return {}; }
        static constexpr std::string_view    Name() { return "Create"; }
        static constexpr bool                Static = false;
    };
    using Apis = ::ReflectionBase::InterfaceApiPack<
        ApiTraits_Create
        >;
};

template <>
struct ReflectionBase::InterfaceApiTraits<ReflectionBase::InterfaceTraits<SimpleWebService::TestInterface>::ApiTraits_Create>
{
    using ArgsStruct = SimpleWebService::TestInterface_Create_Args;
    static constexpr bool             IsStatic() { return false; }
    static constexpr std::string_view Name() { return "Create"; }

    static auto Invoke(ArgsStruct& args)
    {
        return args.instance->Create(
            args.get_arg_randomInteger()
,            args.get_arg_randomString()
        );
    }
};

#if ((defined STENCIL_USING_WEBSERVICE) and (STENCIL_USING_WEBSERVICE > 0))
template <> struct WebServiceHandlerTraits<SimpleWebService::TestInterface>
{
    static constexpr const std::string_view Url() { return std::string_view("TestInterface"); }
};
#endif

template <> struct ReflectionBase::TypeTraits<SimpleWebService::Data::Data&>
{
    struct Traits_randomInteger
    {
        using TOwner     = SimpleWebService::Data::Data;
        using TFieldType = int32_t;

        static constexpr std::string_view Name() { return "randomInteger"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_randomInteger; }
        static constexpr auto TPropertySetter() { return &TOwner::set_randomInteger; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::randomInteger, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_randomString
    {
        using TOwner     = SimpleWebService::Data::Data;
        using TFieldType = shared_string;

        static constexpr std::string_view Name() { return "randomString"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_randomString; }
        static constexpr auto TPropertySetter() { return &TOwner::set_randomString; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::randomString, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "Data"; }
    static constexpr std::string_view           AttributeValue(const std::string_view& key)
    {
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    using ThisType = SimpleWebService::Data::Data;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && ReflectionBase::AreEqual(obj1.randomInteger(), obj2.randomInteger())
               && ReflectionBase::AreEqual(obj1.randomString(), obj2.randomString())
            ;
    }

    using Handler = ::ReflectionServices::ReflectedStructHandler<SimpleWebService::Data::Data,
                                                                 Traits_randomInteger
,                                                                 Traits_randomString
                                                                 >;
};

template <typename T> struct Stencil::DeltaTracker<T, std::enable_if_t<std::is_same_v<T, SimpleWebService::Data::Data>>>
{
    using TData = T;

    // TODO : Tentative: We hate pointers
    TData* const _ptr;
    // TODO : Better way to unify creation interface

    std::bitset<TData::FieldCount() + 1> _fieldtracker;
    DeltaTracker<int32_t> _subtracker_randomInteger;
    DeltaTracker<shared_string> _subtracker_randomString;
    DELETE_COPY_AND_MOVE(DeltaTracker);

    DeltaTracker(TData* ptr) :
        _ptr(ptr)
        ,
        _subtracker_randomInteger(&_ptr->randomInteger())
        ,
        _subtracker_randomString(&_ptr->randomString())
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
        case TData::FieldIndex::randomInteger: lambda(_subtracker_randomInteger); return;
        case TData::FieldIndex::randomString: lambda(_subtracker_randomString); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    void set_randomInteger(int32_t&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::randomInteger, _ptr->randomInteger(), val);
        _ptr->set_randomInteger(std::move(val));
    }

    void set_randomString(shared_string&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::randomString, _ptr->randomString(), val);
        _ptr->set_randomString(std::move(val));
    }

};

