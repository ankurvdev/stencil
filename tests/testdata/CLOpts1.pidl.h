#pragma once
#include <stencil/stencil.h>

// SECTION START: DECLARATIONS
#if true

namespace CLOpts1
{
struct CommandLineOptions;
struct SimpleObj;
struct ObjWithList;
struct CLOptsTest;
}    // namespace CLOpts1

template <> struct Stencil::TypeTraits<CLOpts1::CommandLineOptions>;
template <> struct Stencil::TypeTraits<CLOpts1::SimpleObj>;
template <> struct Stencil::TypeTraits<CLOpts1::ObjWithList>;
template <> struct Stencil::TypeTraits<CLOpts1::CLOptsTest>;
#endif
// SECTION END: DECLARATIONS

// SECTION START: Definitions
#if true
namespace CLOpts1
{
struct CommandLineOptions :
    public Stencil::StructT<CommandLineOptions>
{
    shared_string workingDirectory = {};
    std::vector<shared_string> libraries = {};
    std::vector<shared_string> scan = {};
    int32_t httpsPort = 3443;
    bool daemon = 0;
};
struct SimpleObj :
    public Stencil::StructT<SimpleObj>
{
    shared_string field1 = {};
    shared_string field2 = {};
};
struct ObjWithList :
    public Stencil::StructT<ObjWithList>
{
    std::vector<shared_string> field1 = {};
};
struct CLOptsTest :
    public Stencil::StructT<CLOptsTest>
{
    shared_string key1 = {};
    std::vector<int32_t> listofint = {};
    std::vector<std::vector<int32_t>> listoflist = {};
    std::vector<::CLOpts1::SimpleObj> listofobj = {};
    ::CLOpts1::ObjWithList objoflist = {};
    shared_string key2 = {};
};
}    // namespace CLOpts1
#endif

// SECTION END: Definitions

// SECTION START: Template specializations
#if true

// SECTION:

template <> struct Stencil::TypeTraits<CLOpts1::CommandLineOptions>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<CLOpts1::CommandLineOptions>
{
    enum class Fields
    {
        Invalid,
        Field_workingDirectory
,        Field_libraries
,        Field_scan
,        Field_httpsPort
,        Field_daemon
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<CLOpts1::CommandLineOptions>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<CLOpts1::CommandLineOptions>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "workingDirectory"
,        "libraries"
,        "scan"
,        "httpsPort"
,        "daemon"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<CLOpts1::CommandLineOptions>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<CLOpts1::CommandLineOptions>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<CLOpts1::CommandLineOptions> : Stencil::TransactionT<CLOpts1::CommandLineOptions>
{
    using TData  = CLOpts1::CommandLineOptions;
    using Fields = Stencil::TypeTraitsForIndexable<CLOpts1::CommandLineOptions>::Fields;

    Transaction<shared_string> _subtracker_workingDirectory;
    Transaction<std::vector<shared_string>> _subtracker_libraries;
    Transaction<std::vector<shared_string>> _subtracker_scan;
    Transaction<int32_t> _subtracker_httpsPort;
    Transaction<bool> _subtracker_daemon;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<CLOpts1::CommandLineOptions>(ptr)
        ,
        _subtracker_workingDirectory(Obj().workingDirectory)
        ,
        _subtracker_libraries(Obj().libraries)
        ,
        _subtracker_scan(Obj().scan)
        ,
        _subtracker_httpsPort(Obj().httpsPort)
        ,
        _subtracker_daemon(Obj().daemon)
    {}

    auto& workingDirectory()
    {
        MarkFieldEdited_(Fields::Field_workingDirectory);
        return _subtracker_workingDirectory;
    }
    auto& libraries()
    {
        MarkFieldEdited_(Fields::Field_libraries);
        return _subtracker_libraries;
    }
    auto& scan()
    {
        MarkFieldEdited_(Fields::Field_scan);
        return _subtracker_scan;
    }
    auto& httpsPort()
    {
        MarkFieldEdited_(Fields::Field_httpsPort);
        return _subtracker_httpsPort;
    }
    auto& daemon()
    {
        MarkFieldEdited_(Fields::Field_daemon);
        return _subtracker_daemon;
    }
    void set_workingDirectory(shared_string&& val)
    {
        MarkFieldAssigned_(Fields::Field_workingDirectory, Obj().workingDirectory, val);
        Obj().workingDirectory = std::move(val);
    }

    void set_libraries(std::vector<shared_string>&& val)
    {
        MarkFieldAssigned_(Fields::Field_libraries, Obj().libraries, val);
        Obj().libraries = std::move(val);
    }

    void add_libraries(shared_string&& args)
    {
        MarkFieldEdited_(Fields::Field_libraries);
        return Stencil::Mutators<std::remove_reference_t<decltype(libraries())>>::add(libraries(), std::move(args));
    }
    void remove_libraries(size_t&& args)
    {
        MarkFieldEdited_(Fields::Field_libraries);
        return Stencil::Mutators<std::remove_reference_t<decltype(libraries())>>::remove(libraries(), std::move(args));
    }
    auto& edit_libraries(size_t&& args)
    {
        MarkFieldEdited_(Fields::Field_libraries);
        return Stencil::Mutators<std::remove_reference_t<decltype(libraries())>>::edit(libraries(), std::move(args));
    }
    void set_scan(std::vector<shared_string>&& val)
    {
        MarkFieldAssigned_(Fields::Field_scan, Obj().scan, val);
        Obj().scan = std::move(val);
    }

    void add_scan(shared_string&& args)
    {
        MarkFieldEdited_(Fields::Field_scan);
        return Stencil::Mutators<std::remove_reference_t<decltype(scan())>>::add(scan(), std::move(args));
    }
    void remove_scan(size_t&& args)
    {
        MarkFieldEdited_(Fields::Field_scan);
        return Stencil::Mutators<std::remove_reference_t<decltype(scan())>>::remove(scan(), std::move(args));
    }
    auto& edit_scan(size_t&& args)
    {
        MarkFieldEdited_(Fields::Field_scan);
        return Stencil::Mutators<std::remove_reference_t<decltype(scan())>>::edit(scan(), std::move(args));
    }
    void set_httpsPort(int32_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_httpsPort, Obj().httpsPort, val);
        Obj().httpsPort = std::move(val);
    }

    void set_daemon(bool&& val)
    {
        MarkFieldAssigned_(Fields::Field_daemon, Obj().daemon, val);
        Obj().daemon = std::move(val);
    }

    template <typename TLambda> auto Visit(Fields index, TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_workingDirectory: return lambda("workingDirectory", workingDirectory()); return;
        case Fields::Field_libraries: return lambda("libraries", libraries()); return;
        case Fields::Field_scan: return lambda("scan", scan()); return;
        case Fields::Field_httpsPort: return lambda("httpsPort", httpsPort()); return;
        case Fields::Field_daemon: return lambda("daemon", daemon()); return;
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "workingDirectory") { return lambda(Fields::Field_workingDirectory, workingDirectory()); }
        if (fieldName == "libraries") { return lambda(Fields::Field_libraries, libraries()); }
        if (fieldName == "scan") { return lambda(Fields::Field_scan, scan()); }
        if (fieldName == "httpsPort") { return lambda(Fields::Field_httpsPort, httpsPort()); }
        if (fieldName == "daemon") { return lambda(Fields::Field_daemon, daemon()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("workingDirectory", Fields::Field_workingDirectory, workingDirectory(), Obj().workingDirectory);
        lambda("libraries", Fields::Field_libraries, libraries(), Obj().libraries);
        lambda("scan", Fields::Field_scan, scan(), Obj().scan);
        lambda("httpsPort", Fields::Field_httpsPort, httpsPort(), Obj().httpsPort);
        lambda("daemon", Fields::Field_daemon, daemon(), Obj().daemon);
    }

    void Flush()
    {
        workingDirectory().Flush();

        if (IsFieldEdited(Fields::Field_workingDirectory))
        {
            if (!workingDirectory().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_workingDirectory));
        }

        libraries().Flush();

        if (IsFieldEdited(Fields::Field_libraries))
        {
            if (!libraries().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_libraries));
        }

        scan().Flush();

        if (IsFieldEdited(Fields::Field_scan))
        {
            if (!scan().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_scan));
        }

        httpsPort().Flush();

        if (IsFieldEdited(Fields::Field_httpsPort))
        {
            if (!httpsPort().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_httpsPort));
        }

        daemon().Flush();

        if (IsFieldEdited(Fields::Field_daemon))
        {
            if (!daemon().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_daemon));
        }

        Stencil::TransactionT<CLOpts1::CommandLineOptions>::Flush_();
    }
};

template <> struct Stencil::Visitor<CLOpts1::CommandLineOptions> : Stencil::VisitorT<CLOpts1::CommandLineOptions>
{
    using TData  = CLOpts1::CommandLineOptions;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey(T& obj, Fields field, TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_workingDirectory: return lambda(obj.workingDirectory);
        case Fields::Field_libraries: return lambda(obj.libraries);
        case Fields::Field_scan: return lambda(obj.scan);
        case Fields::Field_httpsPort: return lambda(obj.httpsPort);
        case Fields::Field_daemon: return lambda(obj.daemon);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies(T& obj, TLambda&& lambda)
    {
        lambda(Fields::Field_workingDirectory, obj.workingDirectory);
        lambda(Fields::Field_libraries, obj.libraries);
        lambda(Fields::Field_scan, obj.scan);
        lambda(Fields::Field_httpsPort, obj.httpsPort);
        lambda(Fields::Field_daemon, obj.daemon);
    }
};

template <> struct Stencil::TypeTraits<CLOpts1::SimpleObj>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<CLOpts1::SimpleObj>
{
    enum class Fields
    {
        Invalid,
        Field_field1
,        Field_field2
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<CLOpts1::SimpleObj>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<CLOpts1::SimpleObj>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "field1"
,        "field2"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<CLOpts1::SimpleObj>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<CLOpts1::SimpleObj>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<CLOpts1::SimpleObj> : Stencil::TransactionT<CLOpts1::SimpleObj>
{
    using TData  = CLOpts1::SimpleObj;
    using Fields = Stencil::TypeTraitsForIndexable<CLOpts1::SimpleObj>::Fields;

    Transaction<shared_string> _subtracker_field1;
    Transaction<shared_string> _subtracker_field2;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<CLOpts1::SimpleObj>(ptr)
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

        Stencil::TransactionT<CLOpts1::SimpleObj>::Flush_();
    }
};

template <> struct Stencil::Visitor<CLOpts1::SimpleObj> : Stencil::VisitorT<CLOpts1::SimpleObj>
{
    using TData  = CLOpts1::SimpleObj;
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

template <> struct Stencil::TypeTraits<CLOpts1::ObjWithList>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<CLOpts1::ObjWithList>
{
    enum class Fields
    {
        Invalid,
        Field_field1
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<CLOpts1::ObjWithList>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<CLOpts1::ObjWithList>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "field1"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<CLOpts1::ObjWithList>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<CLOpts1::ObjWithList>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<CLOpts1::ObjWithList> : Stencil::TransactionT<CLOpts1::ObjWithList>
{
    using TData  = CLOpts1::ObjWithList;
    using Fields = Stencil::TypeTraitsForIndexable<CLOpts1::ObjWithList>::Fields;

    Transaction<std::vector<shared_string>> _subtracker_field1;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<CLOpts1::ObjWithList>(ptr)
        ,
        _subtracker_field1(Obj().field1)
    {}

    auto& field1()
    {
        MarkFieldEdited_(Fields::Field_field1);
        return _subtracker_field1;
    }
    void set_field1(std::vector<shared_string>&& val)
    {
        MarkFieldAssigned_(Fields::Field_field1, Obj().field1, val);
        Obj().field1 = std::move(val);
    }

    void add_field1(shared_string&& args)
    {
        MarkFieldEdited_(Fields::Field_field1);
        return Stencil::Mutators<std::remove_reference_t<decltype(field1())>>::add(field1(), std::move(args));
    }
    void remove_field1(size_t&& args)
    {
        MarkFieldEdited_(Fields::Field_field1);
        return Stencil::Mutators<std::remove_reference_t<decltype(field1())>>::remove(field1(), std::move(args));
    }
    auto& edit_field1(size_t&& args)
    {
        MarkFieldEdited_(Fields::Field_field1);
        return Stencil::Mutators<std::remove_reference_t<decltype(field1())>>::edit(field1(), std::move(args));
    }
    template <typename TLambda> auto Visit(Fields index, TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_field1: return lambda("field1", field1()); return;
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "field1") { return lambda(Fields::Field_field1, field1()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("field1", Fields::Field_field1, field1(), Obj().field1);
    }

    void Flush()
    {
        field1().Flush();

        if (IsFieldEdited(Fields::Field_field1))
        {
            if (!field1().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_field1));
        }

        Stencil::TransactionT<CLOpts1::ObjWithList>::Flush_();
    }
};

template <> struct Stencil::Visitor<CLOpts1::ObjWithList> : Stencil::VisitorT<CLOpts1::ObjWithList>
{
    using TData  = CLOpts1::ObjWithList;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey(T& obj, Fields field, TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_field1: return lambda(obj.field1);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies(T& obj, TLambda&& lambda)
    {
        lambda(Fields::Field_field1, obj.field1);
    }
};

template <> struct Stencil::TypeTraits<CLOpts1::CLOptsTest>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<CLOpts1::CLOptsTest>
{
    enum class Fields
    {
        Invalid,
        Field_key1
,        Field_listofint
,        Field_listoflist
,        Field_listofobj
,        Field_objoflist
,        Field_key2
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<CLOpts1::CLOptsTest>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<CLOpts1::CLOptsTest>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "key1"
,        "listofint"
,        "listoflist"
,        "listofobj"
,        "objoflist"
,        "key2"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<CLOpts1::CLOptsTest>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<CLOpts1::CLOptsTest>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<CLOpts1::CLOptsTest> : Stencil::TransactionT<CLOpts1::CLOptsTest>
{
    using TData  = CLOpts1::CLOptsTest;
    using Fields = Stencil::TypeTraitsForIndexable<CLOpts1::CLOptsTest>::Fields;

    Transaction<shared_string> _subtracker_key1;
    Transaction<std::vector<int32_t>> _subtracker_listofint;
    Transaction<std::vector<std::vector<int32_t>>> _subtracker_listoflist;
    Transaction<std::vector<::CLOpts1::SimpleObj>> _subtracker_listofobj;
    Transaction<::CLOpts1::ObjWithList> _subtracker_objoflist;
    Transaction<shared_string> _subtracker_key2;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<CLOpts1::CLOptsTest>(ptr)
        ,
        _subtracker_key1(Obj().key1)
        ,
        _subtracker_listofint(Obj().listofint)
        ,
        _subtracker_listoflist(Obj().listoflist)
        ,
        _subtracker_listofobj(Obj().listofobj)
        ,
        _subtracker_objoflist(Obj().objoflist)
        ,
        _subtracker_key2(Obj().key2)
    {}

    auto& key1()
    {
        MarkFieldEdited_(Fields::Field_key1);
        return _subtracker_key1;
    }
    auto& listofint()
    {
        MarkFieldEdited_(Fields::Field_listofint);
        return _subtracker_listofint;
    }
    auto& listoflist()
    {
        MarkFieldEdited_(Fields::Field_listoflist);
        return _subtracker_listoflist;
    }
    auto& listofobj()
    {
        MarkFieldEdited_(Fields::Field_listofobj);
        return _subtracker_listofobj;
    }
    auto& objoflist()
    {
        MarkFieldEdited_(Fields::Field_objoflist);
        return _subtracker_objoflist;
    }
    auto& key2()
    {
        MarkFieldEdited_(Fields::Field_key2);
        return _subtracker_key2;
    }
    void set_key1(shared_string&& val)
    {
        MarkFieldAssigned_(Fields::Field_key1, Obj().key1, val);
        Obj().key1 = std::move(val);
    }

    void set_listofint(std::vector<int32_t>&& val)
    {
        MarkFieldAssigned_(Fields::Field_listofint, Obj().listofint, val);
        Obj().listofint = std::move(val);
    }

    void add_listofint(int32_t&& args)
    {
        MarkFieldEdited_(Fields::Field_listofint);
        return Stencil::Mutators<std::remove_reference_t<decltype(listofint())>>::add(listofint(), std::move(args));
    }
    void remove_listofint(size_t&& args)
    {
        MarkFieldEdited_(Fields::Field_listofint);
        return Stencil::Mutators<std::remove_reference_t<decltype(listofint())>>::remove(listofint(), std::move(args));
    }
    auto& edit_listofint(size_t&& args)
    {
        MarkFieldEdited_(Fields::Field_listofint);
        return Stencil::Mutators<std::remove_reference_t<decltype(listofint())>>::edit(listofint(), std::move(args));
    }
    void set_listoflist(std::vector<std::vector<int32_t>>&& val)
    {
        MarkFieldAssigned_(Fields::Field_listoflist, Obj().listoflist, val);
        Obj().listoflist = std::move(val);
    }

    void add_listoflist(std::vector<int32_t>&& args)
    {
        MarkFieldEdited_(Fields::Field_listoflist);
        return Stencil::Mutators<std::remove_reference_t<decltype(listoflist())>>::add(listoflist(), std::move(args));
    }
    void remove_listoflist(size_t&& args)
    {
        MarkFieldEdited_(Fields::Field_listoflist);
        return Stencil::Mutators<std::remove_reference_t<decltype(listoflist())>>::remove(listoflist(), std::move(args));
    }
    auto& edit_listoflist(size_t&& args)
    {
        MarkFieldEdited_(Fields::Field_listoflist);
        return Stencil::Mutators<std::remove_reference_t<decltype(listoflist())>>::edit(listoflist(), std::move(args));
    }
    void set_listofobj(std::vector<::CLOpts1::SimpleObj>&& val)
    {
        MarkFieldAssigned_(Fields::Field_listofobj, Obj().listofobj, val);
        Obj().listofobj = std::move(val);
    }

    void add_listofobj(::CLOpts1::SimpleObj&& args)
    {
        MarkFieldEdited_(Fields::Field_listofobj);
        return Stencil::Mutators<std::remove_reference_t<decltype(listofobj())>>::add(listofobj(), std::move(args));
    }
    void remove_listofobj(size_t&& args)
    {
        MarkFieldEdited_(Fields::Field_listofobj);
        return Stencil::Mutators<std::remove_reference_t<decltype(listofobj())>>::remove(listofobj(), std::move(args));
    }
    auto& edit_listofobj(size_t&& args)
    {
        MarkFieldEdited_(Fields::Field_listofobj);
        return Stencil::Mutators<std::remove_reference_t<decltype(listofobj())>>::edit(listofobj(), std::move(args));
    }
    void set_objoflist(::CLOpts1::ObjWithList&& val)
    {
        MarkFieldAssigned_(Fields::Field_objoflist, Obj().objoflist, val);
        Obj().objoflist = std::move(val);
    }

    void set_key2(shared_string&& val)
    {
        MarkFieldAssigned_(Fields::Field_key2, Obj().key2, val);
        Obj().key2 = std::move(val);
    }

    template <typename TLambda> auto Visit(Fields index, TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_key1: return lambda("key1", key1()); return;
        case Fields::Field_listofint: return lambda("listofint", listofint()); return;
        case Fields::Field_listoflist: return lambda("listoflist", listoflist()); return;
        case Fields::Field_listofobj: return lambda("listofobj", listofobj()); return;
        case Fields::Field_objoflist: return lambda("objoflist", objoflist()); return;
        case Fields::Field_key2: return lambda("key2", key2()); return;
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "key1") { return lambda(Fields::Field_key1, key1()); }
        if (fieldName == "listofint") { return lambda(Fields::Field_listofint, listofint()); }
        if (fieldName == "listoflist") { return lambda(Fields::Field_listoflist, listoflist()); }
        if (fieldName == "listofobj") { return lambda(Fields::Field_listofobj, listofobj()); }
        if (fieldName == "objoflist") { return lambda(Fields::Field_objoflist, objoflist()); }
        if (fieldName == "key2") { return lambda(Fields::Field_key2, key2()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("key1", Fields::Field_key1, key1(), Obj().key1);
        lambda("listofint", Fields::Field_listofint, listofint(), Obj().listofint);
        lambda("listoflist", Fields::Field_listoflist, listoflist(), Obj().listoflist);
        lambda("listofobj", Fields::Field_listofobj, listofobj(), Obj().listofobj);
        lambda("objoflist", Fields::Field_objoflist, objoflist(), Obj().objoflist);
        lambda("key2", Fields::Field_key2, key2(), Obj().key2);
    }

    void Flush()
    {
        key1().Flush();

        if (IsFieldEdited(Fields::Field_key1))
        {
            if (!key1().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_key1));
        }

        listofint().Flush();

        if (IsFieldEdited(Fields::Field_listofint))
        {
            if (!listofint().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_listofint));
        }

        listoflist().Flush();

        if (IsFieldEdited(Fields::Field_listoflist))
        {
            if (!listoflist().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_listoflist));
        }

        listofobj().Flush();

        if (IsFieldEdited(Fields::Field_listofobj))
        {
            if (!listofobj().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_listofobj));
        }

        objoflist().Flush();

        if (IsFieldEdited(Fields::Field_objoflist))
        {
            if (!objoflist().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_objoflist));
        }

        key2().Flush();

        if (IsFieldEdited(Fields::Field_key2))
        {
            if (!key2().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_key2));
        }

        Stencil::TransactionT<CLOpts1::CLOptsTest>::Flush_();
    }
};

template <> struct Stencil::Visitor<CLOpts1::CLOptsTest> : Stencil::VisitorT<CLOpts1::CLOptsTest>
{
    using TData  = CLOpts1::CLOptsTest;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey(T& obj, Fields field, TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_key1: return lambda(obj.key1);
        case Fields::Field_listofint: return lambda(obj.listofint);
        case Fields::Field_listoflist: return lambda(obj.listoflist);
        case Fields::Field_listofobj: return lambda(obj.listofobj);
        case Fields::Field_objoflist: return lambda(obj.objoflist);
        case Fields::Field_key2: return lambda(obj.key2);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies(T& obj, TLambda&& lambda)
    {
        lambda(Fields::Field_key1, obj.key1);
        lambda(Fields::Field_listofint, obj.listofint);
        lambda(Fields::Field_listoflist, obj.listoflist);
        lambda(Fields::Field_listofobj, obj.listofobj);
        lambda(Fields::Field_objoflist, obj.objoflist);
        lambda(Fields::Field_key2, obj.key2);
    }
};

#endif
// SECTION END: Template specializations

// SECTION START: Inline Function Definitions
#if true

#endif
// SECTION END: Inline Function Definitions
