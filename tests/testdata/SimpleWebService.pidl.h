#pragma once
#include <stencil/stencil.h>

// SECTION START: DECLARATIONS
#if true

namespace SimpleWebService
{
struct Data;
struct TestInterface;
}    // namespace SimpleWebService

template <> struct Stencil::TypeTraits<SimpleWebService::Data>;
template <> struct Stencil::TypeTraits<SimpleWebService::TestInterface>;
#endif
// SECTION END: DECLARATIONS

// SECTION START: Definitions
#if true
namespace SimpleWebService
{
struct Data :
    public Stencil::StructT<Data>
{
    int32_t randomInteger = {};
    shared_string randomString = {};
};
struct TestInterface : public Stencil::InterfaceT<TestInterface>
{
    public:
    struct Dummy
    {};
    struct ObjectStore;

    struct ObjectStore : Dummy
    {};

    TestInterface()          = default;
    virtual ~TestInterface() = default;
    DELETE_COPY_AND_MOVE(TestInterface);
    virtual ::SimpleWebService::Data Create(
        int32_t const& randomInteger
,        shared_string const& randomString
        )
        = 0;
    struct Args_Create
    {
        int32_t arg_randomInteger{};
        shared_string arg_randomString{};
    };

    ::SimpleWebService::Data Create(Args_Create const& args)
    {
        return this->Create(
            args.randomInteger
,            args.randomString
        );
    }

    ::SimpleWebService::Data Create(Args_Create&& args)
    {
        return this->Create(
            std::move(args.randomInteger)
,            std::move(args.randomString)
        );
    }

    ObjectStore objects;
};
}    // namespace SimpleWebService
#endif

// SECTION END: Definitions

// SECTION START: Template specializations
#if true

// SECTION:

template <> struct Stencil::TypeTraits<SimpleWebService::Data>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<SimpleWebService::Data>
{
    enum class Fields
    {
        Invalid,
        Field_randomInteger
,        Field_randomString
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<SimpleWebService::Data>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<SimpleWebService::Data>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "randomInteger"
,        "randomString"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<SimpleWebService::Data>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<SimpleWebService::Data>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<SimpleWebService::Data> : Stencil::TransactionT<SimpleWebService::Data>
{
    using TData  = SimpleWebService::Data;
    using Fields = Stencil::TypeTraitsForIndexable<SimpleWebService::Data>::Fields;

    Transaction<int32_t> _subtracker_randomInteger;
    Transaction<shared_string> _subtracker_randomString;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<SimpleWebService::Data>(ptr)
        ,
        _subtracker_randomInteger(Obj().randomInteger)
        ,
        _subtracker_randomString(Obj().randomString)
    {}

    auto& randomInteger()
    {
        MarkFieldEdited_(Fields::Field_randomInteger);
        return _subtracker_randomInteger;
    }
    auto& randomString()
    {
        MarkFieldEdited_(Fields::Field_randomString);
        return _subtracker_randomString;
    }
    void set_randomInteger(int32_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_randomInteger, Obj().randomInteger, val);
        Obj().randomInteger = std::move(val);
    }

    void set_randomString(shared_string&& val)
    {
        MarkFieldAssigned_(Fields::Field_randomString, Obj().randomString, val);
        Obj().randomString = std::move(val);
    }

    template <typename TLambda> auto Visit(Fields index, TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_randomInteger: return lambda("randomInteger", randomInteger()); return;
        case Fields::Field_randomString: return lambda("randomString", randomString()); return;
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "randomInteger") { return lambda(Fields::Field_randomInteger, randomInteger()); }
        if (fieldName == "randomString") { return lambda(Fields::Field_randomString, randomString()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("randomInteger", Fields::Field_randomInteger, randomInteger(), Obj().randomInteger);
        lambda("randomString", Fields::Field_randomString, randomString(), Obj().randomString);
    }

    void Flush()
    {
        randomInteger().Flush();

        if (IsFieldEdited(Fields::Field_randomInteger))
        {
            if (!randomInteger().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_randomInteger));
        }

        randomString().Flush();

        if (IsFieldEdited(Fields::Field_randomString))
        {
            if (!randomString().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_randomString));
        }

        Stencil::TransactionT<SimpleWebService::Data>::Flush_();
    }
};

template <> struct Stencil::Visitor<SimpleWebService::Data> : Stencil::VisitorT<SimpleWebService::Data>
{
    using TData  = SimpleWebService::Data;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey(T& obj, Fields field, TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_randomInteger: return lambda(obj.randomInteger);
        case Fields::Field_randomString: return lambda(obj.randomString);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies(T& obj, TLambda&& lambda)
    {
        lambda(Fields::Field_randomInteger, obj.randomInteger);
        lambda(Fields::Field_randomString, obj.randomString);
    }
};

template <> struct Stencil::TypeTraits<SimpleWebService::TestInterface_Create_Args>
{
    struct Traits_arg_randomInteger
    {
        using TOwner     = SimpleWebService::TestInterface_Create_Args;
        using TFieldType = int32_t;

        static constexpr std::string_view Name() { return "randomInteger"; }
        static const ::Stencil::Flags     Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TOwner::get_arg_randomInteger; }
        static constexpr auto TPropertySetter() { return &TOwner::set_arg_randomInteger; }
    };
    struct Traits_arg_randomString
    {
        using TOwner     = SimpleWebService::TestInterface_Create_Args;
        using TFieldType = shared_string;

        static constexpr std::string_view Name() { return "randomString"; }
        static const ::Stencil::Flags     Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TOwner::get_arg_randomString; }
        static constexpr auto TPropertySetter() { return &TOwner::set_arg_randomString; }
    };
    static constexpr ::Stencil::DataType Type() { return ::Stencil::DataType::Object; }
    static constexpr std::string_view    Name() { return "Create"; }
    static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

    using ThisType = SimpleWebService::TestInterface_Create_Args;

    using Handler = ::ReflectionServices::ReflectedStructHandler<SimpleWebService::TestInterface_Create_Args
                                                                     Traits_arg_randomInteger
,                                                                     Traits_arg_randomString
                                                                 >;
};

template <>
Stencil::Comparator<SimpleWebService::TestInterface_Create_Args,
                    SimpleWebService::TestInterface_Create_Args>{
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2){
        return true
               && Stencil::AreEqual(obj1.get_carg_randomInteger(), obj2.get_carg_randomInteger())
               && Stencil::AreEqual(obj1.get_carg_randomString(), obj2.get_carg_randomString())
        ;
}
}
;
template <> struct Stencil::InterfaceTraits<SimpleWebService::TestInterface>
{
    struct ApiTraits_Create
    {
        using TOwner = SimpleWebService::TestInterface;
        static const ::Stencil::Flags     Flags() { return {}; }
        static constexpr std::string_view Name() { return "Create"; }
        static constexpr bool             Static = false;
    };
    using Apis = std::tuple<
        ApiTraits_Create
        >;

    using Objects = std::tuple<
        >;
};

template <> struct Stencil::InterfaceApiTraits<Stencil::InterfaceTraits<SimpleWebService::TestInterface>::ApiTraits_Create>
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
template <> struct Stencil::WebServiceHandlerTraits<SimpleWebService::TestInterface>
{
    static constexpr const std::string_view Url() { return std::string_view("TestInterface"); }
    // Name to api enum
    // Name to funcptr mapping
    // Name to argstruct mapping
    // Invoke api for each arg struct
    static void HandleRequest(SimpleWebService::TestInterface& obj,
                              httplib::Request const&               req,
                              httplib::Response&                    res,
                              std::string_view const&               path);
};
#endif

#endif
// SECTION END: Template specializations

// SECTION START: Inline Function Definitions
#if true

#endif
// SECTION END: Inline Function Definitions
