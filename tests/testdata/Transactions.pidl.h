#pragma once
#include <stencil/stencil.h>

// SECTION START: DECLARATIONS
#if true

namespace Transactions
{
struct SimpleObject1;
struct ListObject;
struct List;
struct SimpleObject2;
struct Object;
}    // namespace Transactions

template <> struct Stencil::TypeTraits<Transactions::SimpleObject1>;
template <> struct Stencil::TypeTraits<Transactions::ListObject>;
template <> struct Stencil::TypeTraits<Transactions::List>;
template <> struct Stencil::TypeTraits<Transactions::SimpleObject2>;
template <> struct Stencil::TypeTraits<Transactions::Object>;
#endif
// SECTION END: DECLARATIONS

// SECTION START: Definitions
#if true
namespace Transactions
{
struct SimpleObject1 :
    public ::Stencil::OptionalPropsT<::Transactions::SimpleObject1>,
    public ::Stencil::TimestampedT<::Transactions::SimpleObject1>,
    public Stencil::StructT<SimpleObject1>
{
    int32_t val1 = {};
    uint32_t val2 = {};
    uint8_t val3 = {};
    shared_string val4 = {};
    double val5 = {};
};
struct ListObject :
    public ::Stencil::TimestampedT<::Transactions::ListObject>,
    public Stencil::StructT<ListObject>
{
    uint32_t value = {};
    ::Transactions::SimpleObject1 obj1 = {};
};
struct List :
    public ::Stencil::TimestampedT<::Transactions::List>,
    public Stencil::StructT<List>
{
    std::vector<::Transactions::ListObject> listobj = {};
};
struct SimpleObject2 :
    public ::Stencil::OptionalPropsT<::Transactions::SimpleObject2>,
    public ::Stencil::TimestampedT<::Transactions::SimpleObject2>,
    public Stencil::StructT<SimpleObject2>
{
    bool val1 = {};
    double val2 = {};
};
struct Object :
    public ::Stencil::TimestampedT<::Transactions::Object>,
    public Stencil::StructT<Object>
{
    ::Transactions::SimpleObject1 obj1 = {};
    ::Transactions::SimpleObject2 obj2 = {};
    ::Transactions::ListObject obj3 = {};
    ::Transactions::List list1 = {};
};
}    // namespace Transactions
#endif

// SECTION END: Definitions

// SECTION START: Template specializations
#if true

// SECTION:

template <> struct Stencil::TypeTraits<Transactions::SimpleObject1>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<Transactions::SimpleObject1>
{
    enum class Fields
    {
        Invalid,
        Field_val1
,        Field_val2
,        Field_val3
,        Field_val4
,        Field_val5
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<Transactions::SimpleObject1>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<Transactions::SimpleObject1>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "val1"
,        "val2"
,        "val3"
,        "val4"
,        "val5"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<Transactions::SimpleObject1>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<Transactions::SimpleObject1>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<Transactions::SimpleObject1> : Stencil::TransactionT<Transactions::SimpleObject1>
{
    using TData  = Transactions::SimpleObject1;
    using Fields = Stencil::TypeTraitsForIndexable<Transactions::SimpleObject1>::Fields;

    Transaction<int32_t> _subtracker_val1;
    Transaction<uint32_t> _subtracker_val2;
    Transaction<uint8_t> _subtracker_val3;
    Transaction<shared_string> _subtracker_val4;
    Transaction<double> _subtracker_val5;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<Transactions::SimpleObject1>(ptr)
        ,
        _subtracker_val1(Obj().val1)
        ,
        _subtracker_val2(Obj().val2)
        ,
        _subtracker_val3(Obj().val3)
        ,
        _subtracker_val4(Obj().val4)
        ,
        _subtracker_val5(Obj().val5)
    {}

    auto& val1()
    {
        MarkFieldEdited_(Fields::Field_val1);
        return _subtracker_val1;
    }
    auto& val2()
    {
        MarkFieldEdited_(Fields::Field_val2);
        return _subtracker_val2;
    }
    auto& val3()
    {
        MarkFieldEdited_(Fields::Field_val3);
        return _subtracker_val3;
    }
    auto& val4()
    {
        MarkFieldEdited_(Fields::Field_val4);
        return _subtracker_val4;
    }
    auto& val5()
    {
        MarkFieldEdited_(Fields::Field_val5);
        return _subtracker_val5;
    }
    void set_val1(int32_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_val1, Obj().val1, val);
        Obj().val1 = std::move(val);
    }

    void set_val2(uint32_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_val2, Obj().val2, val);
        Obj().val2 = std::move(val);
    }

    void set_val3(uint8_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_val3, Obj().val3, val);
        Obj().val3 = std::move(val);
    }

    void set_val4(shared_string&& val)
    {
        MarkFieldAssigned_(Fields::Field_val4, Obj().val4, val);
        Obj().val4 = std::move(val);
    }

    void set_val5(double&& val)
    {
        MarkFieldAssigned_(Fields::Field_val5, Obj().val5, val);
        Obj().val5 = std::move(val);
    }

    template <typename TLambda> auto Visit(Fields index, TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_val1: return lambda("val1", val1()); return;
        case Fields::Field_val2: return lambda("val2", val2()); return;
        case Fields::Field_val3: return lambda("val3", val3()); return;
        case Fields::Field_val4: return lambda("val4", val4()); return;
        case Fields::Field_val5: return lambda("val5", val5()); return;
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "val1") { return lambda(Fields::Field_val1, val1()); }
        if (fieldName == "val2") { return lambda(Fields::Field_val2, val2()); }
        if (fieldName == "val3") { return lambda(Fields::Field_val3, val3()); }
        if (fieldName == "val4") { return lambda(Fields::Field_val4, val4()); }
        if (fieldName == "val5") { return lambda(Fields::Field_val5, val5()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("val1", Fields::Field_val1, val1(), Obj().val1);
        lambda("val2", Fields::Field_val2, val2(), Obj().val2);
        lambda("val3", Fields::Field_val3, val3(), Obj().val3);
        lambda("val4", Fields::Field_val4, val4(), Obj().val4);
        lambda("val5", Fields::Field_val5, val5(), Obj().val5);
    }

    void Flush()
    {
        val1().Flush();

        if (IsFieldEdited(Fields::Field_val1))
        {
            if (!val1().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_val1));
        }

        val2().Flush();

        if (IsFieldEdited(Fields::Field_val2))
        {
            if (!val2().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_val2));
        }

        val3().Flush();

        if (IsFieldEdited(Fields::Field_val3))
        {
            if (!val3().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_val3));
        }

        val4().Flush();

        if (IsFieldEdited(Fields::Field_val4))
        {
            if (!val4().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_val4));
        }

        val5().Flush();

        if (IsFieldEdited(Fields::Field_val5))
        {
            if (!val5().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_val5));
        }

        Stencil::TransactionT<Transactions::SimpleObject1>::Flush_();
    }
};

template <> struct Stencil::Visitor<Transactions::SimpleObject1> : Stencil::VisitorT<Transactions::SimpleObject1>
{
    using TData  = Transactions::SimpleObject1;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey(T& obj, Fields field, TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_val1: return lambda(obj.val1);
        case Fields::Field_val2: return lambda(obj.val2);
        case Fields::Field_val3: return lambda(obj.val3);
        case Fields::Field_val4: return lambda(obj.val4);
        case Fields::Field_val5: return lambda(obj.val5);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies(T& obj, TLambda&& lambda)
    {
        lambda(Fields::Field_val1, obj.val1);
        lambda(Fields::Field_val2, obj.val2);
        lambda(Fields::Field_val3, obj.val3);
        lambda(Fields::Field_val4, obj.val4);
        lambda(Fields::Field_val5, obj.val5);
    }
};

template <> struct Stencil::TypeTraits<Transactions::ListObject>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<Transactions::ListObject>
{
    enum class Fields
    {
        Invalid,
        Field_value
,        Field_obj1
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<Transactions::ListObject>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<Transactions::ListObject>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "value"
,        "obj1"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<Transactions::ListObject>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<Transactions::ListObject>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<Transactions::ListObject> : Stencil::TransactionT<Transactions::ListObject>
{
    using TData  = Transactions::ListObject;
    using Fields = Stencil::TypeTraitsForIndexable<Transactions::ListObject>::Fields;

    Transaction<uint32_t> _subtracker_value;
    Transaction<::Transactions::SimpleObject1> _subtracker_obj1;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<Transactions::ListObject>(ptr)
        ,
        _subtracker_value(Obj().value)
        ,
        _subtracker_obj1(Obj().obj1)
    {}

    auto& value()
    {
        MarkFieldEdited_(Fields::Field_value);
        return _subtracker_value;
    }
    auto& obj1()
    {
        MarkFieldEdited_(Fields::Field_obj1);
        return _subtracker_obj1;
    }
    void set_value(uint32_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_value, Obj().value, val);
        Obj().value = std::move(val);
    }

    void set_obj1(::Transactions::SimpleObject1&& val)
    {
        MarkFieldAssigned_(Fields::Field_obj1, Obj().obj1, val);
        Obj().obj1 = std::move(val);
    }

    template <typename TLambda> auto Visit(Fields index, TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_value: return lambda("value", value()); return;
        case Fields::Field_obj1: return lambda("obj1", obj1()); return;
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "value") { return lambda(Fields::Field_value, value()); }
        if (fieldName == "obj1") { return lambda(Fields::Field_obj1, obj1()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("value", Fields::Field_value, value(), Obj().value);
        lambda("obj1", Fields::Field_obj1, obj1(), Obj().obj1);
    }

    void Flush()
    {
        value().Flush();

        if (IsFieldEdited(Fields::Field_value))
        {
            if (!value().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_value));
        }

        obj1().Flush();

        if (IsFieldEdited(Fields::Field_obj1))
        {
            if (!obj1().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_obj1));
        }

        Stencil::TransactionT<Transactions::ListObject>::Flush_();
    }
};

template <> struct Stencil::Visitor<Transactions::ListObject> : Stencil::VisitorT<Transactions::ListObject>
{
    using TData  = Transactions::ListObject;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey(T& obj, Fields field, TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_value: return lambda(obj.value);
        case Fields::Field_obj1: return lambda(obj.obj1);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies(T& obj, TLambda&& lambda)
    {
        lambda(Fields::Field_value, obj.value);
        lambda(Fields::Field_obj1, obj.obj1);
    }
};

template <> struct Stencil::TypeTraits<Transactions::List>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<Transactions::List>
{
    enum class Fields
    {
        Invalid,
        Field_listobj
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<Transactions::List>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<Transactions::List>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "listobj"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<Transactions::List>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<Transactions::List>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<Transactions::List> : Stencil::TransactionT<Transactions::List>
{
    using TData  = Transactions::List;
    using Fields = Stencil::TypeTraitsForIndexable<Transactions::List>::Fields;

    Transaction<std::vector<::Transactions::ListObject>> _subtracker_listobj;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<Transactions::List>(ptr)
        ,
        _subtracker_listobj(Obj().listobj)
    {}

    auto& listobj()
    {
        MarkFieldEdited_(Fields::Field_listobj);
        return _subtracker_listobj;
    }
    void set_listobj(std::vector<::Transactions::ListObject>&& val)
    {
        MarkFieldAssigned_(Fields::Field_listobj, Obj().listobj, val);
        Obj().listobj = std::move(val);
    }

    void add_listobj(::Transactions::ListObject&& args)
    {
        MarkFieldEdited_(Fields::Field_listobj);
        return Stencil::Mutators<std::remove_reference_t<decltype(listobj())>>::add(listobj(), std::move(args));
    }
    void remove_listobj(size_t&& args)
    {
        MarkFieldEdited_(Fields::Field_listobj);
        return Stencil::Mutators<std::remove_reference_t<decltype(listobj())>>::remove(listobj(), std::move(args));
    }
    auto& edit_listobj(size_t&& args)
    {
        MarkFieldEdited_(Fields::Field_listobj);
        return Stencil::Mutators<std::remove_reference_t<decltype(listobj())>>::edit(listobj(), std::move(args));
    }
    template <typename TLambda> auto Visit(Fields index, TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_listobj: return lambda("listobj", listobj()); return;
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "listobj") { return lambda(Fields::Field_listobj, listobj()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("listobj", Fields::Field_listobj, listobj(), Obj().listobj);
    }

    void Flush()
    {
        listobj().Flush();

        if (IsFieldEdited(Fields::Field_listobj))
        {
            if (!listobj().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_listobj));
        }

        Stencil::TransactionT<Transactions::List>::Flush_();
    }
};

template <> struct Stencil::Visitor<Transactions::List> : Stencil::VisitorT<Transactions::List>
{
    using TData  = Transactions::List;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey(T& obj, Fields field, TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_listobj: return lambda(obj.listobj);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies(T& obj, TLambda&& lambda)
    {
        lambda(Fields::Field_listobj, obj.listobj);
    }
};

template <> struct Stencil::TypeTraits<Transactions::SimpleObject2>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<Transactions::SimpleObject2>
{
    enum class Fields
    {
        Invalid,
        Field_val1
,        Field_val2
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<Transactions::SimpleObject2>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<Transactions::SimpleObject2>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "val1"
,        "val2"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<Transactions::SimpleObject2>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<Transactions::SimpleObject2>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<Transactions::SimpleObject2> : Stencil::TransactionT<Transactions::SimpleObject2>
{
    using TData  = Transactions::SimpleObject2;
    using Fields = Stencil::TypeTraitsForIndexable<Transactions::SimpleObject2>::Fields;

    Transaction<bool> _subtracker_val1;
    Transaction<double> _subtracker_val2;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<Transactions::SimpleObject2>(ptr)
        ,
        _subtracker_val1(Obj().val1)
        ,
        _subtracker_val2(Obj().val2)
    {}

    auto& val1()
    {
        MarkFieldEdited_(Fields::Field_val1);
        return _subtracker_val1;
    }
    auto& val2()
    {
        MarkFieldEdited_(Fields::Field_val2);
        return _subtracker_val2;
    }
    void set_val1(bool&& val)
    {
        MarkFieldAssigned_(Fields::Field_val1, Obj().val1, val);
        Obj().val1 = std::move(val);
    }

    void set_val2(double&& val)
    {
        MarkFieldAssigned_(Fields::Field_val2, Obj().val2, val);
        Obj().val2 = std::move(val);
    }

    template <typename TLambda> auto Visit(Fields index, TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_val1: return lambda("val1", val1()); return;
        case Fields::Field_val2: return lambda("val2", val2()); return;
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "val1") { return lambda(Fields::Field_val1, val1()); }
        if (fieldName == "val2") { return lambda(Fields::Field_val2, val2()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("val1", Fields::Field_val1, val1(), Obj().val1);
        lambda("val2", Fields::Field_val2, val2(), Obj().val2);
    }

    void Flush()
    {
        val1().Flush();

        if (IsFieldEdited(Fields::Field_val1))
        {
            if (!val1().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_val1));
        }

        val2().Flush();

        if (IsFieldEdited(Fields::Field_val2))
        {
            if (!val2().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_val2));
        }

        Stencil::TransactionT<Transactions::SimpleObject2>::Flush_();
    }
};

template <> struct Stencil::Visitor<Transactions::SimpleObject2> : Stencil::VisitorT<Transactions::SimpleObject2>
{
    using TData  = Transactions::SimpleObject2;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey(T& obj, Fields field, TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_val1: return lambda(obj.val1);
        case Fields::Field_val2: return lambda(obj.val2);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies(T& obj, TLambda&& lambda)
    {
        lambda(Fields::Field_val1, obj.val1);
        lambda(Fields::Field_val2, obj.val2);
    }
};

template <> struct Stencil::TypeTraits<Transactions::Object>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<Transactions::Object>
{
    enum class Fields
    {
        Invalid,
        Field_obj1
,        Field_obj2
,        Field_obj3
,        Field_list1
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<Transactions::Object>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<Transactions::Object>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "obj1"
,        "obj2"
,        "obj3"
,        "list1"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<Transactions::Object>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<Transactions::Object>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<Transactions::Object> : Stencil::TransactionT<Transactions::Object>
{
    using TData  = Transactions::Object;
    using Fields = Stencil::TypeTraitsForIndexable<Transactions::Object>::Fields;

    Transaction<::Transactions::SimpleObject1> _subtracker_obj1;
    Transaction<::Transactions::SimpleObject2> _subtracker_obj2;
    Transaction<::Transactions::ListObject> _subtracker_obj3;
    Transaction<::Transactions::List> _subtracker_list1;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<Transactions::Object>(ptr)
        ,
        _subtracker_obj1(Obj().obj1)
        ,
        _subtracker_obj2(Obj().obj2)
        ,
        _subtracker_obj3(Obj().obj3)
        ,
        _subtracker_list1(Obj().list1)
    {}

    auto& obj1()
    {
        MarkFieldEdited_(Fields::Field_obj1);
        return _subtracker_obj1;
    }
    auto& obj2()
    {
        MarkFieldEdited_(Fields::Field_obj2);
        return _subtracker_obj2;
    }
    auto& obj3()
    {
        MarkFieldEdited_(Fields::Field_obj3);
        return _subtracker_obj3;
    }
    auto& list1()
    {
        MarkFieldEdited_(Fields::Field_list1);
        return _subtracker_list1;
    }
    void set_obj1(::Transactions::SimpleObject1&& val)
    {
        MarkFieldAssigned_(Fields::Field_obj1, Obj().obj1, val);
        Obj().obj1 = std::move(val);
    }

    void set_obj2(::Transactions::SimpleObject2&& val)
    {
        MarkFieldAssigned_(Fields::Field_obj2, Obj().obj2, val);
        Obj().obj2 = std::move(val);
    }

    void set_obj3(::Transactions::ListObject&& val)
    {
        MarkFieldAssigned_(Fields::Field_obj3, Obj().obj3, val);
        Obj().obj3 = std::move(val);
    }

    void set_list1(::Transactions::List&& val)
    {
        MarkFieldAssigned_(Fields::Field_list1, Obj().list1, val);
        Obj().list1 = std::move(val);
    }

    template <typename TLambda> auto Visit(Fields index, TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_obj1: return lambda("obj1", obj1()); return;
        case Fields::Field_obj2: return lambda("obj2", obj2()); return;
        case Fields::Field_obj3: return lambda("obj3", obj3()); return;
        case Fields::Field_list1: return lambda("list1", list1()); return;
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "obj1") { return lambda(Fields::Field_obj1, obj1()); }
        if (fieldName == "obj2") { return lambda(Fields::Field_obj2, obj2()); }
        if (fieldName == "obj3") { return lambda(Fields::Field_obj3, obj3()); }
        if (fieldName == "list1") { return lambda(Fields::Field_list1, list1()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("obj1", Fields::Field_obj1, obj1(), Obj().obj1);
        lambda("obj2", Fields::Field_obj2, obj2(), Obj().obj2);
        lambda("obj3", Fields::Field_obj3, obj3(), Obj().obj3);
        lambda("list1", Fields::Field_list1, list1(), Obj().list1);
    }

    void Flush()
    {
        obj1().Flush();

        if (IsFieldEdited(Fields::Field_obj1))
        {
            if (!obj1().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_obj1));
        }

        obj2().Flush();

        if (IsFieldEdited(Fields::Field_obj2))
        {
            if (!obj2().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_obj2));
        }

        obj3().Flush();

        if (IsFieldEdited(Fields::Field_obj3))
        {
            if (!obj3().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_obj3));
        }

        list1().Flush();

        if (IsFieldEdited(Fields::Field_list1))
        {
            if (!list1().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_list1));
        }

        Stencil::TransactionT<Transactions::Object>::Flush_();
    }
};

template <> struct Stencil::Visitor<Transactions::Object> : Stencil::VisitorT<Transactions::Object>
{
    using TData  = Transactions::Object;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey(T& obj, Fields field, TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_obj1: return lambda(obj.obj1);
        case Fields::Field_obj2: return lambda(obj.obj2);
        case Fields::Field_obj3: return lambda(obj.obj3);
        case Fields::Field_list1: return lambda(obj.list1);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies(T& obj, TLambda&& lambda)
    {
        lambda(Fields::Field_obj1, obj.obj1);
        lambda(Fields::Field_obj2, obj.obj2);
        lambda(Fields::Field_obj3, obj.obj3);
        lambda(Fields::Field_list1, obj.list1);
    }
};

#endif
// SECTION END: Template specializations

// SECTION START: Inline Function Definitions
#if true

#endif
// SECTION END: Inline Function Definitions
