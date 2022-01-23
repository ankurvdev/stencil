#pragma once
#include <stencil/stencil.h>

// SECTION START: DECLARATIONS
#if true
namespace SimpleWebService::Data
{
struct Data;
}
template <> struct Stencil::TypeTraits<SimpleWebService::Data::Data&>;
#endif
// SECTION END: DECLARATIONS

// SECTION START: Definitions
#if true
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
#endif
// SECTION END: Definitions

// SECTION START: Template specializations
#if true

// SECTION:
template <> struct Stencil::TypeTraits<SimpleWebService::TestInterface_Create_Args&>
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
    static constexpr ::Stencil::DataType Type() { return ::Stencil::DataType::Object; }
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

template <> struct Stencil::TypeTraits<SimpleWebService::Data::Data&>
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
    static constexpr ::Stencil::DataType Type() { return ::Stencil::DataType::Object; }
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

template <>
struct Stencil::Transaction<SimpleWebService::Data::Data> : Stencil::TransactionT<SimpleWebService::Data::Data>
{
    using TData = SimpleWebService::Data::Data;

    Transaction<int32_t> _subtracker_randomInteger;
    Transaction<shared_string> _subtracker_randomString;
    DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<SimpleWebService::Data::Data>(ptr)
        ,
        _subtracker_randomInteger(Obj().randomInteger())
        ,
        _subtracker_randomString(Obj().randomString())
    {}

    auto& randomInteger()
    {
        MarkFieldEdited_(TData::FieldIndex::randomInteger);
        return _subtracker_randomInteger;
    }
    auto& randomString()
    {
        MarkFieldEdited_(TData::FieldIndex::randomString);
        return _subtracker_randomString;
    }
    void set_randomInteger(int32_t&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::randomInteger, Obj().randomInteger(), val);
        Obj().set_randomInteger(std::move(val));
    }

    void set_randomString(shared_string&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::randomString, Obj().randomString(), val);
        Obj().set_randomString(std::move(val));
    }

    template <typename TLambda> auto Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::randomInteger: return lambda("randomInteger", randomInteger()); return;
        case TData::FieldIndex::randomString: return lambda("randomString", randomString()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "randomInteger") { return lambda(TData::FieldIndex::randomInteger, randomInteger()); }
        if (fieldName == "randomString") { return lambda(TData::FieldIndex::randomString, randomString()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("randomInteger", TData::FieldIndex::randomInteger, randomInteger(), Obj().randomInteger());
        lambda("randomString", TData::FieldIndex::randomString, randomString(), Obj().randomString());
    }

    void Flush()
    {
        randomInteger().Flush();

        if (IsFieldEdited(TData::FieldIndex::randomInteger))
        {
            if (!randomInteger().IsChanged()) _edittracker.reset(static_cast<uint8_t>(TData::FieldIndex::randomInteger));
        }

        randomString().Flush();

        if (IsFieldEdited(TData::FieldIndex::randomString))
        {
            if (!randomString().IsChanged()) _edittracker.reset(static_cast<uint8_t>(TData::FieldIndex::randomString));
        }

        Stencil::TransactionT<SimpleWebService::Data::Data>::Flush_();
    }
};

template <>
struct Stencil::Visitor<SimpleWebService::Data::Data, void> : Stencil::VisitorT<SimpleWebService::Data::Data>
{
    using TData = SimpleWebService::Data::Data;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::randomInteger: lambda("randomInteger", _ref.get().randomInteger()); return;
        case TData::FieldIndex::randomString: lambda("randomString", _ref.get().randomString()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::randomInteger: lambda("randomInteger", _ref.get().randomInteger()); return;
        case TData::FieldIndex::randomString: lambda("randomString", _ref.get().randomString()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        lambda("randomInteger", _ref.get().randomInteger());
        lambda("randomString", _ref.get().randomString());
    }

    std::reference_wrapper<TData> _ref;
};

template <>
struct Stencil::Visitor<const SimpleWebService::Data::Data, void>
    : Stencil::VisitorT<const SimpleWebService::Data::Data>
{
    using TData = SimpleWebService::Data::Data const;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::randomInteger: lambda("randomInteger", _ref.get().randomInteger()); return;
        case TData::FieldIndex::randomString: lambda("randomString", _ref.get().randomString()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        lambda("randomInteger", _ref.get().randomInteger());
        lambda("randomString", _ref.get().randomString());
    }

    std::reference_wrapper<TData> _ref;
};

#endif
// SECTION END: Template specializations

// SECTION START: Inline Function Definitions
#if true

#endif
// SECTION END: Inline Function Definitions
