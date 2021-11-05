#pragma once
#include <stencil/stencil.h>

// SECTION START: DECLARATIONS
#if true
namespace Transactions::SimpleObject1
{
struct Data;
}
template <> struct ReflectionBase::TypeTraits<Transactions::SimpleObject1::Data&>;
namespace Transactions::ListObject
{
struct Data;
}
template <> struct ReflectionBase::TypeTraits<Transactions::ListObject::Data&>;
namespace Transactions::List
{
struct Data;
}
template <> struct ReflectionBase::TypeTraits<Transactions::List::Data&>;
namespace Transactions::SimpleObject2
{
struct Data;
}
template <> struct ReflectionBase::TypeTraits<Transactions::SimpleObject2::Data&>;
namespace Transactions::Object
{
struct Data;
}
template <> struct ReflectionBase::TypeTraits<Transactions::Object::Data&>;
#endif
// SECTION END: DECLARATIONS

// SECTION START: Definitions
#if true
namespace Transactions
{
namespace SimpleObject1
{
struct Data;
}
namespace ListObject
{
struct Data;
}
namespace List
{
struct Data;
}
namespace SimpleObject2
{
struct Data;
}
namespace Object
{
struct Data;
}
namespace SimpleObject1
{

struct Data :
    public ::Stencil::OptionalPropsT<::Transactions::SimpleObject1::Data>,
    public ::Stencil::TimestampedT<::Transactions::SimpleObject1::Data>,
    public ReflectionBase::ObjMarker
{
    /*template <typename...TArgs> Data(TArgs&& ... args)
    {
        ReflectionBase::Construct<Data>(this, std::forward<TArgs>(args)...);
    }*/

    enum class FieldIndex
    {
        Invalid,
        val1
,        val2
,        val3
,        val4
,        val5
    };

    static constexpr size_t FieldCount()
    {
        return 0u
               + 1u
               + 1u
               + 1u
               + 1u
               + 1u
            ;
    }

    static constexpr std::string_view FieldAttributeValue(FieldIndex index, const std::string_view& key)
    {
        switch (index)
        {
        case FieldIndex::val1:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::val2:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::val3:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::val4:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::val5:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::Invalid: break;

        default: break;
        }
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    private:
    int32_t _val1 = {};

    public:
    int32_t&       val1() { return _val1; }
    const int32_t& val1() const { return _val1; }
    void                            val1(int32_t&& val) { _val1 = std::move(val); }
    int32_t&       get_val1() { return _val1; }

    bool isset_val1() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::val1); }

    void set_val1(int32_t&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::val1, _val1, val);
        _val1 = std::move(val);
    }
    private:
    uint32_t _val2 = {};

    public:
    uint32_t&       val2() { return _val2; }
    const uint32_t& val2() const { return _val2; }
    void                            val2(uint32_t&& val) { _val2 = std::move(val); }
    uint32_t&       get_val2() { return _val2; }

    bool isset_val2() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::val2); }

    void set_val2(uint32_t&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::val2, _val2, val);
        _val2 = std::move(val);
    }
    private:
    uint8_t _val3 = {};

    public:
    uint8_t&       val3() { return _val3; }
    const uint8_t& val3() const { return _val3; }
    void                            val3(uint8_t&& val) { _val3 = std::move(val); }
    uint8_t&       get_val3() { return _val3; }

    bool isset_val3() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::val3); }

    void set_val3(uint8_t&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::val3, _val3, val);
        _val3 = std::move(val);
    }
    private:
    shared_string _val4 = {};

    public:
    shared_string&       val4() { return _val4; }
    const shared_string& val4() const { return _val4; }
    void                            val4(shared_string&& val) { _val4 = std::move(val); }
    shared_string&       get_val4() { return _val4; }

    bool isset_val4() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::val4); }

    void set_val4(shared_string&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::val4, _val4, val);
        _val4 = std::move(val);
    }
    private:
    double _val5 = {};

    public:
    double&       val5() { return _val5; }
    const double& val5() const { return _val5; }
    void                            val5(double&& val) { _val5 = std::move(val); }
    double&       get_val5() { return _val5; }

    bool isset_val5() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::val5); }

    void set_val5(double&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::val5, _val5, val);
        _val5 = std::move(val);
    }
};

}    // namespace SimpleObject1
namespace ListObject
{

struct Data :
    public ::Stencil::TimestampedT<::Transactions::ListObject::Data>,
    public ReflectionBase::ObjMarker
{
    /*template <typename...TArgs> Data(TArgs&& ... args)
    {
        ReflectionBase::Construct<Data>(this, std::forward<TArgs>(args)...);
    }*/

    enum class FieldIndex
    {
        Invalid,
        value
,        obj1
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
        case FieldIndex::value:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::obj1:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::Invalid: break;

        default: break;
        }
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    private:
    uint32_t _value = {};

    public:
    uint32_t&       value() { return _value; }
    const uint32_t& value() const { return _value; }
    void                            value(uint32_t&& val) { _value = std::move(val); }
    uint32_t&       get_value() { return _value; }

    bool isset_value() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::value); }

    void set_value(uint32_t&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::value, _value, val);
        _value = std::move(val);
    }
    private:
    ::Transactions::SimpleObject1::Data _obj1 = {};

    public:
    ::Transactions::SimpleObject1::Data&       obj1() { return _obj1; }
    const ::Transactions::SimpleObject1::Data& obj1() const { return _obj1; }
    void                            obj1(::Transactions::SimpleObject1::Data&& val) { _obj1 = std::move(val); }
    ::Transactions::SimpleObject1::Data&       get_obj1() { return _obj1; }

    bool isset_obj1() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::obj1); }

    void set_obj1(::Transactions::SimpleObject1::Data&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::obj1, _obj1, val);
        _obj1 = std::move(val);
    }
};

}    // namespace ListObject
namespace List
{

struct Data :
    public ::Stencil::TimestampedT<::Transactions::List::Data>,
    public ReflectionBase::ObjMarker
{
    /*template <typename...TArgs> Data(TArgs&& ... args)
    {
        ReflectionBase::Construct<Data>(this, std::forward<TArgs>(args)...);
    }*/

    enum class FieldIndex
    {
        Invalid,
        listobj
    };

    static constexpr size_t FieldCount()
    {
        return 0u
               + 1u
            ;
    }

    static constexpr std::string_view FieldAttributeValue(FieldIndex index, const std::string_view& key)
    {
        switch (index)
        {
        case FieldIndex::listobj:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::Invalid: break;

        default: break;
        }
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    private:
    std::vector<::Transactions::ListObject::Data> _listobj = {};

    public:
    std::vector<::Transactions::ListObject::Data>&       listobj() { return _listobj; }
    const std::vector<::Transactions::ListObject::Data>& listobj() const { return _listobj; }
    void                            listobj(std::vector<::Transactions::ListObject::Data>&& val) { _listobj = std::move(val); }
    std::vector<::Transactions::ListObject::Data>&       get_listobj() { return _listobj; }

    bool isset_listobj() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::listobj); }

    void set_listobj(std::vector<::Transactions::ListObject::Data>&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::listobj, _listobj, val);
        _listobj = std::move(val);
    }
};

}    // namespace List
namespace SimpleObject2
{

struct Data :
    public ::Stencil::OptionalPropsT<::Transactions::SimpleObject2::Data>,
    public ::Stencil::TimestampedT<::Transactions::SimpleObject2::Data>,
    public ReflectionBase::ObjMarker
{
    /*template <typename...TArgs> Data(TArgs&& ... args)
    {
        ReflectionBase::Construct<Data>(this, std::forward<TArgs>(args)...);
    }*/

    enum class FieldIndex
    {
        Invalid,
        val1
,        val2
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
        case FieldIndex::val1:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::val2:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::Invalid: break;

        default: break;
        }
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    private:
    bool _val1 = {};

    public:
    bool&       val1() { return _val1; }
    const bool& val1() const { return _val1; }
    void                            val1(bool&& val) { _val1 = std::move(val); }
    bool&       get_val1() { return _val1; }

    bool isset_val1() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::val1); }

    void set_val1(bool&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::val1, _val1, val);
        _val1 = std::move(val);
    }
    private:
    double _val2 = {};

    public:
    double&       val2() { return _val2; }
    const double& val2() const { return _val2; }
    void                            val2(double&& val) { _val2 = std::move(val); }
    double&       get_val2() { return _val2; }

    bool isset_val2() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::val2); }

    void set_val2(double&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::val2, _val2, val);
        _val2 = std::move(val);
    }
};

}    // namespace SimpleObject2
namespace Object
{

struct Data :
    public ::Stencil::TimestampedT<::Transactions::Object::Data>,
    public ReflectionBase::ObjMarker
{
    /*template <typename...TArgs> Data(TArgs&& ... args)
    {
        ReflectionBase::Construct<Data>(this, std::forward<TArgs>(args)...);
    }*/

    enum class FieldIndex
    {
        Invalid,
        obj1
,        obj2
,        obj3
,        list1
    };

    static constexpr size_t FieldCount()
    {
        return 0u
               + 1u
               + 1u
               + 1u
               + 1u
            ;
    }

    static constexpr std::string_view FieldAttributeValue(FieldIndex index, const std::string_view& key)
    {
        switch (index)
        {
        case FieldIndex::obj1:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::obj2:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::obj3:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::list1:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::Invalid: break;

        default: break;
        }
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    private:
    ::Transactions::SimpleObject1::Data _obj1 = {};

    public:
    ::Transactions::SimpleObject1::Data&       obj1() { return _obj1; }
    const ::Transactions::SimpleObject1::Data& obj1() const { return _obj1; }
    void                            obj1(::Transactions::SimpleObject1::Data&& val) { _obj1 = std::move(val); }
    ::Transactions::SimpleObject1::Data&       get_obj1() { return _obj1; }

    bool isset_obj1() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::obj1); }

    void set_obj1(::Transactions::SimpleObject1::Data&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::obj1, _obj1, val);
        _obj1 = std::move(val);
    }
    private:
    ::Transactions::SimpleObject2::Data _obj2 = {};

    public:
    ::Transactions::SimpleObject2::Data&       obj2() { return _obj2; }
    const ::Transactions::SimpleObject2::Data& obj2() const { return _obj2; }
    void                            obj2(::Transactions::SimpleObject2::Data&& val) { _obj2 = std::move(val); }
    ::Transactions::SimpleObject2::Data&       get_obj2() { return _obj2; }

    bool isset_obj2() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::obj2); }

    void set_obj2(::Transactions::SimpleObject2::Data&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::obj2, _obj2, val);
        _obj2 = std::move(val);
    }
    private:
    ::Transactions::ListObject::Data _obj3 = {};

    public:
    ::Transactions::ListObject::Data&       obj3() { return _obj3; }
    const ::Transactions::ListObject::Data& obj3() const { return _obj3; }
    void                            obj3(::Transactions::ListObject::Data&& val) { _obj3 = std::move(val); }
    ::Transactions::ListObject::Data&       get_obj3() { return _obj3; }

    bool isset_obj3() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::obj3); }

    void set_obj3(::Transactions::ListObject::Data&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::obj3, _obj3, val);
        _obj3 = std::move(val);
    }
    private:
    ::Transactions::List::Data _list1 = {};

    public:
    ::Transactions::List::Data&       list1() { return _list1; }
    const ::Transactions::List::Data& list1() const { return _list1; }
    void                            list1(::Transactions::List::Data&& val) { _list1 = std::move(val); }
    ::Transactions::List::Data&       get_list1() { return _list1; }

    bool isset_list1() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::list1); }

    void set_list1(::Transactions::List::Data&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::list1, _list1, val);
        _list1 = std::move(val);
    }
};

}    // namespace Object
}    // namespace Transactions
#endif
// SECTION END: Definitions

// SECTION START: Template specializations
#if true

// SECTION:
template <> struct ReflectionBase::TypeTraits<Transactions::SimpleObject1::Data&>
{
    struct Traits_val1
    {
        using TOwner     = Transactions::SimpleObject1::Data;
        using TFieldType = int32_t;

        static constexpr std::string_view Name() { return "val1"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_val1; }
        static constexpr auto TPropertySetter() { return &TOwner::set_val1; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::val1, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_val2
    {
        using TOwner     = Transactions::SimpleObject1::Data;
        using TFieldType = uint32_t;

        static constexpr std::string_view Name() { return "val2"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_val2; }
        static constexpr auto TPropertySetter() { return &TOwner::set_val2; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::val2, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_val3
    {
        using TOwner     = Transactions::SimpleObject1::Data;
        using TFieldType = uint8_t;

        static constexpr std::string_view Name() { return "val3"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_val3; }
        static constexpr auto TPropertySetter() { return &TOwner::set_val3; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::val3, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_val4
    {
        using TOwner     = Transactions::SimpleObject1::Data;
        using TFieldType = shared_string;

        static constexpr std::string_view Name() { return "val4"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_val4; }
        static constexpr auto TPropertySetter() { return &TOwner::set_val4; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::val4, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_val5
    {
        using TOwner     = Transactions::SimpleObject1::Data;
        using TFieldType = double;

        static constexpr std::string_view Name() { return "val5"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_val5; }
        static constexpr auto TPropertySetter() { return &TOwner::set_val5; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::val5, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "SimpleObject1"; }
    static constexpr std::string_view           AttributeValue(const std::string_view& key)
    {
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    using ThisType = Transactions::SimpleObject1::Data;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && ReflectionBase::AreEqual(obj1.val1(), obj2.val1())
               && ReflectionBase::AreEqual(obj1.val2(), obj2.val2())
               && ReflectionBase::AreEqual(obj1.val3(), obj2.val3())
               && ReflectionBase::AreEqual(obj1.val4(), obj2.val4())
               && ReflectionBase::AreEqual(obj1.val5(), obj2.val5())
            ;
    }

    using Handler = ::ReflectionServices::ReflectedStructHandler<Transactions::SimpleObject1::Data,
                                                                 Traits_val1
,                                                                 Traits_val2
,                                                                 Traits_val3
,                                                                 Traits_val4
,                                                                 Traits_val5
                                                                 >;
};

template <>
struct Stencil::Transaction<Transactions::SimpleObject1::Data> : Stencil::TransactionT<Transactions::SimpleObject1::Data>
{
    using TData = Transactions::SimpleObject1::Data;

    Transaction<int32_t> _subtracker_val1;
    Transaction<uint32_t> _subtracker_val2;
    Transaction<uint8_t> _subtracker_val3;
    Transaction<shared_string> _subtracker_val4;
    Transaction<double> _subtracker_val5;
    DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<Transactions::SimpleObject1::Data>(ptr)
        ,
        _subtracker_val1(Obj().val1())
        ,
        _subtracker_val2(Obj().val2())
        ,
        _subtracker_val3(Obj().val3())
        ,
        _subtracker_val4(Obj().val4())
        ,
        _subtracker_val5(Obj().val5())
    {}

    auto& val1()
    {
        MarkFieldEdited_(TData::FieldIndex::val1);
        return _subtracker_val1;
    }
    auto& val2()
    {
        MarkFieldEdited_(TData::FieldIndex::val2);
        return _subtracker_val2;
    }
    auto& val3()
    {
        MarkFieldEdited_(TData::FieldIndex::val3);
        return _subtracker_val3;
    }
    auto& val4()
    {
        MarkFieldEdited_(TData::FieldIndex::val4);
        return _subtracker_val4;
    }
    auto& val5()
    {
        MarkFieldEdited_(TData::FieldIndex::val5);
        return _subtracker_val5;
    }
    void set_val1(int32_t&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::val1, Obj().val1(), val);
        Obj().set_val1(std::move(val));
    }

    void set_val2(uint32_t&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::val2, Obj().val2(), val);
        Obj().set_val2(std::move(val));
    }

    void set_val3(uint8_t&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::val3, Obj().val3(), val);
        Obj().set_val3(std::move(val));
    }

    void set_val4(shared_string&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::val4, Obj().val4(), val);
        Obj().set_val4(std::move(val));
    }

    void set_val5(double&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::val5, Obj().val5(), val);
        Obj().set_val5(std::move(val));
    }

    template <typename TLambda> auto Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::val1: return lambda("val1", val1()); return;
        case TData::FieldIndex::val2: return lambda("val2", val2()); return;
        case TData::FieldIndex::val3: return lambda("val3", val3()); return;
        case TData::FieldIndex::val4: return lambda("val4", val4()); return;
        case TData::FieldIndex::val5: return lambda("val5", val5()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "val1") { return lambda(TData::FieldIndex::val1, val1()); }
        if (fieldName == "val2") { return lambda(TData::FieldIndex::val2, val2()); }
        if (fieldName == "val3") { return lambda(TData::FieldIndex::val3, val3()); }
        if (fieldName == "val4") { return lambda(TData::FieldIndex::val4, val4()); }
        if (fieldName == "val5") { return lambda(TData::FieldIndex::val5, val5()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("val1", TData::FieldIndex::val1, val1(), Obj().val1());
        lambda("val2", TData::FieldIndex::val2, val2(), Obj().val2());
        lambda("val3", TData::FieldIndex::val3, val3(), Obj().val3());
        lambda("val4", TData::FieldIndex::val4, val4(), Obj().val4());
        lambda("val5", TData::FieldIndex::val5, val5(), Obj().val5());
    }

    void Flush()
    {
        val1().Flush();

        if (IsFieldEdited(TData::FieldIndex::val1))
        {
            if (!val1().IsChanged()) _edittracker.reset(static_cast<uint8_t>(TData::FieldIndex::val1));
        }

        val2().Flush();

        if (IsFieldEdited(TData::FieldIndex::val2))
        {
            if (!val2().IsChanged()) _edittracker.reset(static_cast<uint8_t>(TData::FieldIndex::val2));
        }

        val3().Flush();

        if (IsFieldEdited(TData::FieldIndex::val3))
        {
            if (!val3().IsChanged()) _edittracker.reset(static_cast<uint8_t>(TData::FieldIndex::val3));
        }

        val4().Flush();

        if (IsFieldEdited(TData::FieldIndex::val4))
        {
            if (!val4().IsChanged()) _edittracker.reset(static_cast<uint8_t>(TData::FieldIndex::val4));
        }

        val5().Flush();

        if (IsFieldEdited(TData::FieldIndex::val5))
        {
            if (!val5().IsChanged()) _edittracker.reset(static_cast<uint8_t>(TData::FieldIndex::val5));
        }

        Stencil::TransactionT<Transactions::SimpleObject1::Data>::Flush_();
    }
};

template <>
struct Stencil::Visitor<Transactions::SimpleObject1::Data, void> : Stencil::VisitorT<Transactions::SimpleObject1::Data>
{
    using TData = Transactions::SimpleObject1::Data;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::val1: lambda("val1", _ref.get().val1()); return;
        case TData::FieldIndex::val2: lambda("val2", _ref.get().val2()); return;
        case TData::FieldIndex::val3: lambda("val3", _ref.get().val3()); return;
        case TData::FieldIndex::val4: lambda("val4", _ref.get().val4()); return;
        case TData::FieldIndex::val5: lambda("val5", _ref.get().val5()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::val1: lambda("val1", _ref.get().val1()); return;
        case TData::FieldIndex::val2: lambda("val2", _ref.get().val2()); return;
        case TData::FieldIndex::val3: lambda("val3", _ref.get().val3()); return;
        case TData::FieldIndex::val4: lambda("val4", _ref.get().val4()); return;
        case TData::FieldIndex::val5: lambda("val5", _ref.get().val5()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        lambda("val1", _ref.get().val1());
        lambda("val2", _ref.get().val2());
        lambda("val3", _ref.get().val3());
        lambda("val4", _ref.get().val4());
        lambda("val5", _ref.get().val5());
    }

    std::reference_wrapper<TData> _ref;
};

template <>
struct Stencil::Visitor<const Transactions::SimpleObject1::Data, void>
    : Stencil::VisitorT<const Transactions::SimpleObject1::Data>
{
    using TData = Transactions::SimpleObject1::Data const;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::val1: lambda("val1", _ref.get().val1()); return;
        case TData::FieldIndex::val2: lambda("val2", _ref.get().val2()); return;
        case TData::FieldIndex::val3: lambda("val3", _ref.get().val3()); return;
        case TData::FieldIndex::val4: lambda("val4", _ref.get().val4()); return;
        case TData::FieldIndex::val5: lambda("val5", _ref.get().val5()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        lambda("val1", _ref.get().val1());
        lambda("val2", _ref.get().val2());
        lambda("val3", _ref.get().val3());
        lambda("val4", _ref.get().val4());
        lambda("val5", _ref.get().val5());
    }

    std::reference_wrapper<TData> _ref;
};

template <> struct ReflectionBase::TypeTraits<Transactions::ListObject::Data&>
{
    struct Traits_value
    {
        using TOwner     = Transactions::ListObject::Data;
        using TFieldType = uint32_t;

        static constexpr std::string_view Name() { return "value"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_value; }
        static constexpr auto TPropertySetter() { return &TOwner::set_value; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::value, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_obj1
    {
        using TOwner     = Transactions::ListObject::Data;
        using TFieldType = ::Transactions::SimpleObject1::Data;

        static constexpr std::string_view Name() { return "obj1"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_obj1; }
        static constexpr auto TPropertySetter() { return &TOwner::set_obj1; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::obj1, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "ListObject"; }
    static constexpr std::string_view           AttributeValue(const std::string_view& key)
    {
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    using ThisType = Transactions::ListObject::Data;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && ReflectionBase::AreEqual(obj1.value(), obj2.value())
               && ReflectionBase::AreEqual(obj1.obj1(), obj2.obj1())
            ;
    }

    using Handler = ::ReflectionServices::ReflectedStructHandler<Transactions::ListObject::Data,
                                                                 Traits_value
,                                                                 Traits_obj1
                                                                 >;
};

template <>
struct Stencil::Transaction<Transactions::ListObject::Data> : Stencil::TransactionT<Transactions::ListObject::Data>
{
    using TData = Transactions::ListObject::Data;

    Transaction<uint32_t> _subtracker_value;
    Transaction<::Transactions::SimpleObject1::Data> _subtracker_obj1;
    DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<Transactions::ListObject::Data>(ptr)
        ,
        _subtracker_value(Obj().value())
        ,
        _subtracker_obj1(Obj().obj1())
    {}

    auto& value()
    {
        MarkFieldEdited_(TData::FieldIndex::value);
        return _subtracker_value;
    }
    auto& obj1()
    {
        MarkFieldEdited_(TData::FieldIndex::obj1);
        return _subtracker_obj1;
    }
    void set_value(uint32_t&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::value, Obj().value(), val);
        Obj().set_value(std::move(val));
    }

    void set_obj1(::Transactions::SimpleObject1::Data&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::obj1, Obj().obj1(), val);
        Obj().set_obj1(std::move(val));
    }

    template <typename TLambda> auto Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::value: return lambda("value", value()); return;
        case TData::FieldIndex::obj1: return lambda("obj1", obj1()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "value") { return lambda(TData::FieldIndex::value, value()); }
        if (fieldName == "obj1") { return lambda(TData::FieldIndex::obj1, obj1()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("value", TData::FieldIndex::value, value(), Obj().value());
        lambda("obj1", TData::FieldIndex::obj1, obj1(), Obj().obj1());
    }

    void Flush()
    {
        value().Flush();

        if (IsFieldEdited(TData::FieldIndex::value))
        {
            if (!value().IsChanged()) _edittracker.reset(static_cast<uint8_t>(TData::FieldIndex::value));
        }

        obj1().Flush();

        if (IsFieldEdited(TData::FieldIndex::obj1))
        {
            if (!obj1().IsChanged()) _edittracker.reset(static_cast<uint8_t>(TData::FieldIndex::obj1));
        }

        Stencil::TransactionT<Transactions::ListObject::Data>::Flush_();
    }
};

template <>
struct Stencil::Visitor<Transactions::ListObject::Data, void> : Stencil::VisitorT<Transactions::ListObject::Data>
{
    using TData = Transactions::ListObject::Data;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::value: lambda("value", _ref.get().value()); return;
        case TData::FieldIndex::obj1: lambda("obj1", _ref.get().obj1()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::value: lambda("value", _ref.get().value()); return;
        case TData::FieldIndex::obj1: lambda("obj1", _ref.get().obj1()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        lambda("value", _ref.get().value());
        lambda("obj1", _ref.get().obj1());
    }

    std::reference_wrapper<TData> _ref;
};

template <>
struct Stencil::Visitor<const Transactions::ListObject::Data, void>
    : Stencil::VisitorT<const Transactions::ListObject::Data>
{
    using TData = Transactions::ListObject::Data const;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::value: lambda("value", _ref.get().value()); return;
        case TData::FieldIndex::obj1: lambda("obj1", _ref.get().obj1()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        lambda("value", _ref.get().value());
        lambda("obj1", _ref.get().obj1());
    }

    std::reference_wrapper<TData> _ref;
};

template <> struct ReflectionBase::TypeTraits<Transactions::List::Data&>
{
    struct Traits_listobj
    {
        using TOwner     = Transactions::List::Data;
        using TFieldType = std::vector<::Transactions::ListObject::Data>;

        static constexpr std::string_view Name() { return "listobj"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_listobj; }
        static constexpr auto TPropertySetter() { return &TOwner::set_listobj; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::listobj, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "List"; }
    static constexpr std::string_view           AttributeValue(const std::string_view& key)
    {
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    using ThisType = Transactions::List::Data;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && ReflectionBase::AreEqual(obj1.listobj(), obj2.listobj())
            ;
    }

    using Handler = ::ReflectionServices::ReflectedStructHandler<Transactions::List::Data,
                                                                 Traits_listobj
                                                                 >;
};

template <>
struct Stencil::Transaction<Transactions::List::Data> : Stencil::TransactionT<Transactions::List::Data>
{
    using TData = Transactions::List::Data;

    Transaction<std::vector<::Transactions::ListObject::Data>> _subtracker_listobj;
    DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<Transactions::List::Data>(ptr)
        ,
        _subtracker_listobj(Obj().listobj())
    {}

    auto& listobj()
    {
        MarkFieldEdited_(TData::FieldIndex::listobj);
        return _subtracker_listobj;
    }
    void set_listobj(std::vector<::Transactions::ListObject::Data>&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::listobj, Obj().listobj(), val);
        Obj().set_listobj(std::move(val));
    }

    void add_listobj(::Transactions::ListObject::Data&& args)
    {
        MarkFieldEdited_(TData::FieldIndex::listobj);
        return Stencil::Mutators<std::remove_reference_t<decltype(listobj())>>::add(listobj(), std::move(args));
    }
    void remove_listobj(size_t&& args)
    {
        MarkFieldEdited_(TData::FieldIndex::listobj);
        return Stencil::Mutators<std::remove_reference_t<decltype(listobj())>>::remove(listobj(), std::move(args));
    }
    auto& edit_listobj(size_t&& args)
    {
        MarkFieldEdited_(TData::FieldIndex::listobj);
        return Stencil::Mutators<std::remove_reference_t<decltype(listobj())>>::edit(listobj(), std::move(args));
    }
    template <typename TLambda> auto Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::listobj: return lambda("listobj", listobj()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "listobj") { return lambda(TData::FieldIndex::listobj, listobj()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("listobj", TData::FieldIndex::listobj, listobj(), Obj().listobj());
    }

    void Flush()
    {
        listobj().Flush();

        if (IsFieldEdited(TData::FieldIndex::listobj))
        {
            if (!listobj().IsChanged()) _edittracker.reset(static_cast<uint8_t>(TData::FieldIndex::listobj));
        }

        Stencil::TransactionT<Transactions::List::Data>::Flush_();
    }
};

template <>
struct Stencil::Visitor<Transactions::List::Data, void> : Stencil::VisitorT<Transactions::List::Data>
{
    using TData = Transactions::List::Data;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::listobj: lambda("listobj", _ref.get().listobj()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::listobj: lambda("listobj", _ref.get().listobj()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        lambda("listobj", _ref.get().listobj());
    }

    std::reference_wrapper<TData> _ref;
};

template <>
struct Stencil::Visitor<const Transactions::List::Data, void>
    : Stencil::VisitorT<const Transactions::List::Data>
{
    using TData = Transactions::List::Data const;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::listobj: lambda("listobj", _ref.get().listobj()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        lambda("listobj", _ref.get().listobj());
    }

    std::reference_wrapper<TData> _ref;
};

template <> struct ReflectionBase::TypeTraits<Transactions::SimpleObject2::Data&>
{
    struct Traits_val1
    {
        using TOwner     = Transactions::SimpleObject2::Data;
        using TFieldType = bool;

        static constexpr std::string_view Name() { return "val1"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_val1; }
        static constexpr auto TPropertySetter() { return &TOwner::set_val1; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::val1, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_val2
    {
        using TOwner     = Transactions::SimpleObject2::Data;
        using TFieldType = double;

        static constexpr std::string_view Name() { return "val2"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_val2; }
        static constexpr auto TPropertySetter() { return &TOwner::set_val2; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::val2, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "SimpleObject2"; }
    static constexpr std::string_view           AttributeValue(const std::string_view& key)
    {
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    using ThisType = Transactions::SimpleObject2::Data;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && ReflectionBase::AreEqual(obj1.val1(), obj2.val1())
               && ReflectionBase::AreEqual(obj1.val2(), obj2.val2())
            ;
    }

    using Handler = ::ReflectionServices::ReflectedStructHandler<Transactions::SimpleObject2::Data,
                                                                 Traits_val1
,                                                                 Traits_val2
                                                                 >;
};

template <>
struct Stencil::Transaction<Transactions::SimpleObject2::Data> : Stencil::TransactionT<Transactions::SimpleObject2::Data>
{
    using TData = Transactions::SimpleObject2::Data;

    Transaction<bool> _subtracker_val1;
    Transaction<double> _subtracker_val2;
    DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<Transactions::SimpleObject2::Data>(ptr)
        ,
        _subtracker_val1(Obj().val1())
        ,
        _subtracker_val2(Obj().val2())
    {}

    auto& val1()
    {
        MarkFieldEdited_(TData::FieldIndex::val1);
        return _subtracker_val1;
    }
    auto& val2()
    {
        MarkFieldEdited_(TData::FieldIndex::val2);
        return _subtracker_val2;
    }
    void set_val1(bool&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::val1, Obj().val1(), val);
        Obj().set_val1(std::move(val));
    }

    void set_val2(double&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::val2, Obj().val2(), val);
        Obj().set_val2(std::move(val));
    }

    template <typename TLambda> auto Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::val1: return lambda("val1", val1()); return;
        case TData::FieldIndex::val2: return lambda("val2", val2()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "val1") { return lambda(TData::FieldIndex::val1, val1()); }
        if (fieldName == "val2") { return lambda(TData::FieldIndex::val2, val2()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("val1", TData::FieldIndex::val1, val1(), Obj().val1());
        lambda("val2", TData::FieldIndex::val2, val2(), Obj().val2());
    }

    void Flush()
    {
        val1().Flush();

        if (IsFieldEdited(TData::FieldIndex::val1))
        {
            if (!val1().IsChanged()) _edittracker.reset(static_cast<uint8_t>(TData::FieldIndex::val1));
        }

        val2().Flush();

        if (IsFieldEdited(TData::FieldIndex::val2))
        {
            if (!val2().IsChanged()) _edittracker.reset(static_cast<uint8_t>(TData::FieldIndex::val2));
        }

        Stencil::TransactionT<Transactions::SimpleObject2::Data>::Flush_();
    }
};

template <>
struct Stencil::Visitor<Transactions::SimpleObject2::Data, void> : Stencil::VisitorT<Transactions::SimpleObject2::Data>
{
    using TData = Transactions::SimpleObject2::Data;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::val1: lambda("val1", _ref.get().val1()); return;
        case TData::FieldIndex::val2: lambda("val2", _ref.get().val2()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::val1: lambda("val1", _ref.get().val1()); return;
        case TData::FieldIndex::val2: lambda("val2", _ref.get().val2()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        lambda("val1", _ref.get().val1());
        lambda("val2", _ref.get().val2());
    }

    std::reference_wrapper<TData> _ref;
};

template <>
struct Stencil::Visitor<const Transactions::SimpleObject2::Data, void>
    : Stencil::VisitorT<const Transactions::SimpleObject2::Data>
{
    using TData = Transactions::SimpleObject2::Data const;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::val1: lambda("val1", _ref.get().val1()); return;
        case TData::FieldIndex::val2: lambda("val2", _ref.get().val2()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        lambda("val1", _ref.get().val1());
        lambda("val2", _ref.get().val2());
    }

    std::reference_wrapper<TData> _ref;
};

template <> struct ReflectionBase::TypeTraits<Transactions::Object::Data&>
{
    struct Traits_obj1
    {
        using TOwner     = Transactions::Object::Data;
        using TFieldType = ::Transactions::SimpleObject1::Data;

        static constexpr std::string_view Name() { return "obj1"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_obj1; }
        static constexpr auto TPropertySetter() { return &TOwner::set_obj1; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::obj1, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_obj2
    {
        using TOwner     = Transactions::Object::Data;
        using TFieldType = ::Transactions::SimpleObject2::Data;

        static constexpr std::string_view Name() { return "obj2"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_obj2; }
        static constexpr auto TPropertySetter() { return &TOwner::set_obj2; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::obj2, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_obj3
    {
        using TOwner     = Transactions::Object::Data;
        using TFieldType = ::Transactions::ListObject::Data;

        static constexpr std::string_view Name() { return "obj3"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_obj3; }
        static constexpr auto TPropertySetter() { return &TOwner::set_obj3; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::obj3, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_list1
    {
        using TOwner     = Transactions::Object::Data;
        using TFieldType = ::Transactions::List::Data;

        static constexpr std::string_view Name() { return "list1"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_list1; }
        static constexpr auto TPropertySetter() { return &TOwner::set_list1; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::list1, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "Object"; }
    static constexpr std::string_view           AttributeValue(const std::string_view& key)
    {
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    using ThisType = Transactions::Object::Data;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && ReflectionBase::AreEqual(obj1.obj1(), obj2.obj1())
               && ReflectionBase::AreEqual(obj1.obj2(), obj2.obj2())
               && ReflectionBase::AreEqual(obj1.obj3(), obj2.obj3())
               && ReflectionBase::AreEqual(obj1.list1(), obj2.list1())
            ;
    }

    using Handler = ::ReflectionServices::ReflectedStructHandler<Transactions::Object::Data,
                                                                 Traits_obj1
,                                                                 Traits_obj2
,                                                                 Traits_obj3
,                                                                 Traits_list1
                                                                 >;
};

template <>
struct Stencil::Transaction<Transactions::Object::Data> : Stencil::TransactionT<Transactions::Object::Data>
{
    using TData = Transactions::Object::Data;

    Transaction<::Transactions::SimpleObject1::Data> _subtracker_obj1;
    Transaction<::Transactions::SimpleObject2::Data> _subtracker_obj2;
    Transaction<::Transactions::ListObject::Data> _subtracker_obj3;
    Transaction<::Transactions::List::Data> _subtracker_list1;
    DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<Transactions::Object::Data>(ptr)
        ,
        _subtracker_obj1(Obj().obj1())
        ,
        _subtracker_obj2(Obj().obj2())
        ,
        _subtracker_obj3(Obj().obj3())
        ,
        _subtracker_list1(Obj().list1())
    {}

    auto& obj1()
    {
        MarkFieldEdited_(TData::FieldIndex::obj1);
        return _subtracker_obj1;
    }
    auto& obj2()
    {
        MarkFieldEdited_(TData::FieldIndex::obj2);
        return _subtracker_obj2;
    }
    auto& obj3()
    {
        MarkFieldEdited_(TData::FieldIndex::obj3);
        return _subtracker_obj3;
    }
    auto& list1()
    {
        MarkFieldEdited_(TData::FieldIndex::list1);
        return _subtracker_list1;
    }
    void set_obj1(::Transactions::SimpleObject1::Data&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::obj1, Obj().obj1(), val);
        Obj().set_obj1(std::move(val));
    }

    void set_obj2(::Transactions::SimpleObject2::Data&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::obj2, Obj().obj2(), val);
        Obj().set_obj2(std::move(val));
    }

    void set_obj3(::Transactions::ListObject::Data&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::obj3, Obj().obj3(), val);
        Obj().set_obj3(std::move(val));
    }

    void set_list1(::Transactions::List::Data&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::list1, Obj().list1(), val);
        Obj().set_list1(std::move(val));
    }

    template <typename TLambda> auto Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::obj1: return lambda("obj1", obj1()); return;
        case TData::FieldIndex::obj2: return lambda("obj2", obj2()); return;
        case TData::FieldIndex::obj3: return lambda("obj3", obj3()); return;
        case TData::FieldIndex::list1: return lambda("list1", list1()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "obj1") { return lambda(TData::FieldIndex::obj1, obj1()); }
        if (fieldName == "obj2") { return lambda(TData::FieldIndex::obj2, obj2()); }
        if (fieldName == "obj3") { return lambda(TData::FieldIndex::obj3, obj3()); }
        if (fieldName == "list1") { return lambda(TData::FieldIndex::list1, list1()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("obj1", TData::FieldIndex::obj1, obj1(), Obj().obj1());
        lambda("obj2", TData::FieldIndex::obj2, obj2(), Obj().obj2());
        lambda("obj3", TData::FieldIndex::obj3, obj3(), Obj().obj3());
        lambda("list1", TData::FieldIndex::list1, list1(), Obj().list1());
    }

    void Flush()
    {
        obj1().Flush();

        if (IsFieldEdited(TData::FieldIndex::obj1))
        {
            if (!obj1().IsChanged()) _edittracker.reset(static_cast<uint8_t>(TData::FieldIndex::obj1));
        }

        obj2().Flush();

        if (IsFieldEdited(TData::FieldIndex::obj2))
        {
            if (!obj2().IsChanged()) _edittracker.reset(static_cast<uint8_t>(TData::FieldIndex::obj2));
        }

        obj3().Flush();

        if (IsFieldEdited(TData::FieldIndex::obj3))
        {
            if (!obj3().IsChanged()) _edittracker.reset(static_cast<uint8_t>(TData::FieldIndex::obj3));
        }

        list1().Flush();

        if (IsFieldEdited(TData::FieldIndex::list1))
        {
            if (!list1().IsChanged()) _edittracker.reset(static_cast<uint8_t>(TData::FieldIndex::list1));
        }

        Stencil::TransactionT<Transactions::Object::Data>::Flush_();
    }
};

template <>
struct Stencil::Visitor<Transactions::Object::Data, void> : Stencil::VisitorT<Transactions::Object::Data>
{
    using TData = Transactions::Object::Data;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::obj1: lambda("obj1", _ref.get().obj1()); return;
        case TData::FieldIndex::obj2: lambda("obj2", _ref.get().obj2()); return;
        case TData::FieldIndex::obj3: lambda("obj3", _ref.get().obj3()); return;
        case TData::FieldIndex::list1: lambda("list1", _ref.get().list1()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::obj1: lambda("obj1", _ref.get().obj1()); return;
        case TData::FieldIndex::obj2: lambda("obj2", _ref.get().obj2()); return;
        case TData::FieldIndex::obj3: lambda("obj3", _ref.get().obj3()); return;
        case TData::FieldIndex::list1: lambda("list1", _ref.get().list1()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        lambda("obj1", _ref.get().obj1());
        lambda("obj2", _ref.get().obj2());
        lambda("obj3", _ref.get().obj3());
        lambda("list1", _ref.get().list1());
    }

    std::reference_wrapper<TData> _ref;
};

template <>
struct Stencil::Visitor<const Transactions::Object::Data, void>
    : Stencil::VisitorT<const Transactions::Object::Data>
{
    using TData = Transactions::Object::Data const;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::obj1: lambda("obj1", _ref.get().obj1()); return;
        case TData::FieldIndex::obj2: lambda("obj2", _ref.get().obj2()); return;
        case TData::FieldIndex::obj3: lambda("obj3", _ref.get().obj3()); return;
        case TData::FieldIndex::list1: lambda("list1", _ref.get().list1()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        lambda("obj1", _ref.get().obj1());
        lambda("obj2", _ref.get().obj2());
        lambda("obj3", _ref.get().obj3());
        lambda("list1", _ref.get().list1());
    }

    std::reference_wrapper<TData> _ref;
};

#endif
// SECTION END: Template specializations

// SECTION START: Inline Function Definitions
#if true

#endif
// SECTION END: Inline Function Definitions
