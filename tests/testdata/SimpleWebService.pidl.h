#pragma once
#include <DataModel.h>
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

    static constexpr std::string_view FieldAttributeValue(FieldIndex index, const std::string_view& key)
    {
        switch (index)
        {
        case FieldIndex::randomInteger:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::randomString:
            return ::ReflectionServices::EmptyAttributeValue(key);
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

    int32_t& get_randomInteger()
    {
        return _randomInteger;
        ;
    }
    void set_randomInteger(int32_t&& val) { _randomInteger = std::move(val); }

    private:
    shared_string _randomString = {};

    public:
    shared_string&       randomString() { return _randomString; }
    const shared_string& randomString() const { return _randomString; }
    void                            randomString(shared_string&& val) { _randomString = std::move(val); }

    shared_string& get_randomString()
    {
        return _randomString;
        ;
    }
    void set_randomString(shared_string&& val) { _randomString = std::move(val); }

};

}    // namespace Data
struct TestInterface : public ReflectionBase::Interface<TestInterface>
{
    public:
    TestInterface() : ReflectionBase::Interface<TestInterface>(this) {}
    virtual Data::Data Create(
        int32_t randomInteger
,        shared_string randomString
        )
        = 0;
    // static std::unique_ptr<TestInterface> Create();
};

struct TestInterfaceFactory : public ReflectionBase::InterfaceFactory<TestInterface>
{
    public:
    virtual std::unique_ptr<TestInterface> Activate() = 0;
};

struct TestInterface_Create_Args
{
    TestInterface* instance = nullptr;

    int32_t  arg_randomInteger{};
    int32_t& get_arg_randomInteger() { return arg_randomInteger; }
    void                      set_arg_randomInteger(int32_t&& value) { arg_randomInteger = std::move(value); }
    shared_string  arg_randomString{};
    shared_string& get_arg_randomString() { return arg_randomString; }
    void                      set_arg_randomString(shared_string&& value) { arg_randomString = std::move(value); }
};

}    // namespace SimpleWebService

template <> struct ReflectionBase::TypeTraits<SimpleWebService::TestInterface_Create_Args&>
{
    struct Traits_arg_randomInteger
    {
        using TOwner = SimpleWebService::TestInterface_Create_Args;
        static constexpr std::string_view    Name() { return "randomInteger"; }
        static const ::ReflectionBase::Flags Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TOwner::get_arg_randomInteger; }
        static constexpr auto TPropertySetter() { return &TOwner::set_arg_randomInteger; }
    };
    struct Traits_arg_randomString
    {
        using TOwner = SimpleWebService::TestInterface_Create_Args;
        static constexpr std::string_view    Name() { return "randomString"; }
        static const ::ReflectionBase::Flags Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TOwner::get_arg_randomString; }
        static constexpr auto TPropertySetter() { return &TOwner::set_arg_randomString; }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "Create"; }
    static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

    using Handler = ::ReflectionServices::ReflectedStructHandler<SimpleWebService::TestInterface_Create_Args,
                                                                 Traits_arg_randomInteger
,                                                                 Traits_arg_randomString
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

    static SimpleWebService::Data::Data Invoke(ArgsStruct& args)
    {
        return args.instance->Create(
            args.get_arg_randomInteger()
,            args.get_arg_randomString()
        );
    }
};
template <> struct WebServiceHandlerTraits<SimpleWebService::TestInterface>
{
    static constexpr const std::string_view Url() { return std::string_view("TestInterface"); }
};

template <> struct ReflectionBase::TypeTraits<SimpleWebService::Data::Data&>
{
    struct Traits_randomInteger
    {
        using TOwner = SimpleWebService::Data::Data;

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
        using TOwner = SimpleWebService::Data::Data;

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

    using Handler = ::ReflectionServices::ReflectedStructHandler<SimpleWebService::Data::Data,
                                                                 Traits_randomInteger
,                                                                 Traits_randomString
                                                                 >;
};
