#pragma once
#include <stencil/stencil.h>

// SECTION START: DECLARATIONS
#if true
namespace CLOpts1::CommandLineOptions
{
struct Data;
}
template <> struct ReflectionBase::TypeTraits<CLOpts1::CommandLineOptions::Data&>;
namespace CLOpts1::SimpleObj
{
struct Data;
}
template <> struct ReflectionBase::TypeTraits<CLOpts1::SimpleObj::Data&>;
namespace CLOpts1::ObjWithList
{
struct Data;
}
template <> struct ReflectionBase::TypeTraits<CLOpts1::ObjWithList::Data&>;
namespace CLOpts1::CLOptsTest
{
struct Data;
}
template <> struct ReflectionBase::TypeTraits<CLOpts1::CLOptsTest::Data&>;
#endif
// SECTION END: DECLARATIONS

// SECTION START: Definitions
#if true
namespace CLOpts1
{
namespace CommandLineOptions
{
struct Data;
}
namespace SimpleObj
{
struct Data;
}
namespace ObjWithList
{
struct Data;
}
namespace CLOptsTest
{
struct Data;
}
namespace CommandLineOptions
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
        workingDirectory
,        libraries
,        scan
,        httpsPort
,        daemon
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
        case FieldIndex::workingDirectory:
            if (key == "Description") return "Location of path where it'll store its database and other info";
            if (key == "Name1") return "Working Directory";
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::libraries:
            if (key == "Description") return "Locations of Directories to store files";
            if (key == "Name1") return "Library Directories";
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::scan:
            if (key == "Description") return "List of Directories to scan for Document files";
            if (key == "Name1") return "Scan Directories";
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::httpsPort:
            if (key == "Description") return "TCP port on which to listen for incoming HTTP connections";
            if (key == "Name1") return "Listen Port";
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::daemon:
            if (key == "Description") return "Run as daemon";
            if (key == "Name1") return "Run as Daemon";
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::Invalid: break;

        default: break;
        }
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    private:
    shared_string _workingDirectory = {};

    public:
    shared_string&       workingDirectory() { return _workingDirectory; }
    const shared_string& workingDirectory() const { return _workingDirectory; }
    void                            workingDirectory(shared_string&& val) { _workingDirectory = std::move(val); }
    shared_string&       get_workingDirectory() { return _workingDirectory; }

    bool isset_workingDirectory() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::workingDirectory); }

    void set_workingDirectory(shared_string&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::workingDirectory, _workingDirectory, val);
        _workingDirectory = std::move(val);
    }

#if 0
#endif
    private:
    std::vector<shared_string> _libraries = {};

    public:
    std::vector<shared_string>&       libraries() { return _libraries; }
    const std::vector<shared_string>& libraries() const { return _libraries; }
    void                            libraries(std::vector<shared_string>&& val) { _libraries = std::move(val); }
    std::vector<shared_string>&       get_libraries() { return _libraries; }

    bool isset_libraries() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::libraries); }

    void set_libraries(std::vector<shared_string>&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::libraries, _libraries, val);
        _libraries = std::move(val);
    }

#if 0
    void add_libraries(shared_string&& args);
    void remove_libraries(size_t&& args);
    Stencil::Transaction<shared_string> edit_libraries(size_t&& args);
    shared_string at_libraries(size_t const& args) const;
#endif
    private:
    std::vector<shared_string> _scan = {};

    public:
    std::vector<shared_string>&       scan() { return _scan; }
    const std::vector<shared_string>& scan() const { return _scan; }
    void                            scan(std::vector<shared_string>&& val) { _scan = std::move(val); }
    std::vector<shared_string>&       get_scan() { return _scan; }

    bool isset_scan() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::scan); }

    void set_scan(std::vector<shared_string>&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::scan, _scan, val);
        _scan = std::move(val);
    }

#if 0
    void add_scan(shared_string&& args);
    void remove_scan(size_t&& args);
    Stencil::Transaction<shared_string> edit_scan(size_t&& args);
    shared_string at_scan(size_t const& args) const;
#endif
    private:
    int32_t _httpsPort = 3443;

    public:
    int32_t&       httpsPort() { return _httpsPort; }
    const int32_t& httpsPort() const { return _httpsPort; }
    void                            httpsPort(int32_t&& val) { _httpsPort = std::move(val); }
    int32_t&       get_httpsPort() { return _httpsPort; }

    bool isset_httpsPort() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::httpsPort); }

    void set_httpsPort(int32_t&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::httpsPort, _httpsPort, val);
        _httpsPort = std::move(val);
    }

#if 0
#endif
    private:
    bool _daemon = 0;

    public:
    bool&       daemon() { return _daemon; }
    const bool& daemon() const { return _daemon; }
    void                            daemon(bool&& val) { _daemon = std::move(val); }
    bool&       get_daemon() { return _daemon; }

    bool isset_daemon() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::daemon); }

    void set_daemon(bool&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::daemon, _daemon, val);
        _daemon = std::move(val);
    }

#if 0
#endif
};

}    // namespace CommandLineOptions
namespace SimpleObj
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

}    // namespace SimpleObj
namespace ObjWithList
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
        case FieldIndex::field1:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::Invalid: break;

        default: break;
        }
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    private:
    std::vector<shared_string> _field1 = {};

    public:
    std::vector<shared_string>&       field1() { return _field1; }
    const std::vector<shared_string>& field1() const { return _field1; }
    void                            field1(std::vector<shared_string>&& val) { _field1 = std::move(val); }
    std::vector<shared_string>&       get_field1() { return _field1; }

    bool isset_field1() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::field1); }

    void set_field1(std::vector<shared_string>&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::field1, _field1, val);
        _field1 = std::move(val);
    }

#if 0
    void add_field1(shared_string&& args);
    void remove_field1(size_t&& args);
    Stencil::Transaction<shared_string> edit_field1(size_t&& args);
    shared_string at_field1(size_t const& args) const;
#endif
};

}    // namespace ObjWithList
namespace CLOptsTest
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
        key1
,        listofint
,        listoflist
,        listofobj
,        objoflist
,        key2
    };

    static constexpr size_t FieldCount()
    {
        return 0u
               + 1u
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
        case FieldIndex::key1:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::listofint:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::listoflist:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::listofobj:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::objoflist:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::key2:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::Invalid: break;

        default: break;
        }
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    private:
    shared_string _key1 = {};

    public:
    shared_string&       key1() { return _key1; }
    const shared_string& key1() const { return _key1; }
    void                            key1(shared_string&& val) { _key1 = std::move(val); }
    shared_string&       get_key1() { return _key1; }

    bool isset_key1() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::key1); }

    void set_key1(shared_string&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::key1, _key1, val);
        _key1 = std::move(val);
    }

#if 0
#endif
    private:
    std::vector<int32_t> _listofint = {};

    public:
    std::vector<int32_t>&       listofint() { return _listofint; }
    const std::vector<int32_t>& listofint() const { return _listofint; }
    void                            listofint(std::vector<int32_t>&& val) { _listofint = std::move(val); }
    std::vector<int32_t>&       get_listofint() { return _listofint; }

    bool isset_listofint() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::listofint); }

    void set_listofint(std::vector<int32_t>&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::listofint, _listofint, val);
        _listofint = std::move(val);
    }

#if 0
    void add_listofint(int32_t&& args);
    void remove_listofint(size_t&& args);
    Stencil::Transaction<int32_t> edit_listofint(size_t&& args);
    int32_t at_listofint(size_t const& args) const;
#endif
    private:
    std::vector<std::vector<int32_t>> _listoflist = {};

    public:
    std::vector<std::vector<int32_t>>&       listoflist() { return _listoflist; }
    const std::vector<std::vector<int32_t>>& listoflist() const { return _listoflist; }
    void                            listoflist(std::vector<std::vector<int32_t>>&& val) { _listoflist = std::move(val); }
    std::vector<std::vector<int32_t>>&       get_listoflist() { return _listoflist; }

    bool isset_listoflist() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::listoflist); }

    void set_listoflist(std::vector<std::vector<int32_t>>&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::listoflist, _listoflist, val);
        _listoflist = std::move(val);
    }

#if 0
    void add_listoflist(std::vector<int32_t>&& args);
    void remove_listoflist(size_t&& args);
    Stencil::Transaction<std::vector<int32_t>> edit_listoflist(size_t&& args);
    std::vector<int32_t> at_listoflist(size_t const& args) const;
#endif
    private:
    std::vector<::CLOpts1::SimpleObj::Data> _listofobj = {};

    public:
    std::vector<::CLOpts1::SimpleObj::Data>&       listofobj() { return _listofobj; }
    const std::vector<::CLOpts1::SimpleObj::Data>& listofobj() const { return _listofobj; }
    void                            listofobj(std::vector<::CLOpts1::SimpleObj::Data>&& val) { _listofobj = std::move(val); }
    std::vector<::CLOpts1::SimpleObj::Data>&       get_listofobj() { return _listofobj; }

    bool isset_listofobj() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::listofobj); }

    void set_listofobj(std::vector<::CLOpts1::SimpleObj::Data>&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::listofobj, _listofobj, val);
        _listofobj = std::move(val);
    }

#if 0
    void add_listofobj(::CLOpts1::SimpleObj::Data&& args);
    void remove_listofobj(size_t&& args);
    Stencil::Transaction<::CLOpts1::SimpleObj::Data> edit_listofobj(size_t&& args);
    ::CLOpts1::SimpleObj::Data at_listofobj(size_t const& args) const;
#endif
    private:
    ::CLOpts1::ObjWithList::Data _objoflist = {};

    public:
    ::CLOpts1::ObjWithList::Data&       objoflist() { return _objoflist; }
    const ::CLOpts1::ObjWithList::Data& objoflist() const { return _objoflist; }
    void                            objoflist(::CLOpts1::ObjWithList::Data&& val) { _objoflist = std::move(val); }
    ::CLOpts1::ObjWithList::Data&       get_objoflist() { return _objoflist; }

    bool isset_objoflist() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::objoflist); }

    void set_objoflist(::CLOpts1::ObjWithList::Data&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::objoflist, _objoflist, val);
        _objoflist = std::move(val);
    }

#if 0
#endif
    private:
    shared_string _key2 = {};

    public:
    shared_string&       key2() { return _key2; }
    const shared_string& key2() const { return _key2; }
    void                            key2(shared_string&& val) { _key2 = std::move(val); }
    shared_string&       get_key2() { return _key2; }

    bool isset_key2() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::key2); }

    void set_key2(shared_string&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::key2, _key2, val);
        _key2 = std::move(val);
    }

#if 0
#endif
};

}    // namespace CLOptsTest
}    // namespace CLOpts1
#endif
// SECTION END: Definitions

// SECTION START: Template specializations
#if true

// SECTION:
template <> struct ReflectionBase::TypeTraits<CLOpts1::CommandLineOptions::Data&>
{
    struct Traits_workingDirectory
    {
        using TOwner     = CLOpts1::CommandLineOptions::Data;
        using TFieldType = shared_string;

        static constexpr std::string_view Name() { return "workingDirectory"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_workingDirectory; }
        static constexpr auto TPropertySetter() { return &TOwner::set_workingDirectory; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::workingDirectory, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_libraries
    {
        using TOwner     = CLOpts1::CommandLineOptions::Data;
        using TFieldType = std::vector<shared_string>;

        static constexpr std::string_view Name() { return "libraries"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_libraries; }
        static constexpr auto TPropertySetter() { return &TOwner::set_libraries; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::libraries, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_scan
    {
        using TOwner     = CLOpts1::CommandLineOptions::Data;
        using TFieldType = std::vector<shared_string>;

        static constexpr std::string_view Name() { return "scan"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_scan; }
        static constexpr auto TPropertySetter() { return &TOwner::set_scan; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::scan, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_httpsPort
    {
        using TOwner     = CLOpts1::CommandLineOptions::Data;
        using TFieldType = int32_t;

        static constexpr std::string_view Name() { return "httpsPort"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_httpsPort; }
        static constexpr auto TPropertySetter() { return &TOwner::set_httpsPort; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::httpsPort, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::HasDefaultValue,
                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_daemon
    {
        using TOwner     = CLOpts1::CommandLineOptions::Data;
        using TFieldType = bool;

        static constexpr std::string_view Name() { return "daemon"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_daemon; }
        static constexpr auto TPropertySetter() { return &TOwner::set_daemon; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::daemon, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::HasDefaultValue,
                                           ::ReflectionBase::Flag::Max};
        }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "CommandLineOptions"; }
    static constexpr std::string_view           AttributeValue(const std::string_view& key)
    {
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    using ThisType = CLOpts1::CommandLineOptions::Data;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && ReflectionBase::AreEqual(obj1.workingDirectory(), obj2.workingDirectory())
               && ReflectionBase::AreEqual(obj1.libraries(), obj2.libraries())
               && ReflectionBase::AreEqual(obj1.scan(), obj2.scan())
               && ReflectionBase::AreEqual(obj1.httpsPort(), obj2.httpsPort())
               && ReflectionBase::AreEqual(obj1.daemon(), obj2.daemon())
            ;
    }

    using Handler = ::ReflectionServices::ReflectedStructHandler<CLOpts1::CommandLineOptions::Data,
                                                                 Traits_workingDirectory
,                                                                 Traits_libraries
,                                                                 Traits_scan
,                                                                 Traits_httpsPort
,                                                                 Traits_daemon
                                                                 >;
};

template <>
struct Stencil::Transaction<CLOpts1::CommandLineOptions::Data> : Stencil::TransactionT<CLOpts1::CommandLineOptions::Data>
{
    using TData = CLOpts1::CommandLineOptions::Data;

    Transaction<shared_string> _subtracker_workingDirectory;
    Transaction<std::vector<shared_string>> _subtracker_libraries;
    Transaction<std::vector<shared_string>> _subtracker_scan;
    Transaction<int32_t> _subtracker_httpsPort;
    Transaction<bool> _subtracker_daemon;
    DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<CLOpts1::CommandLineOptions::Data>(ptr)
        ,
        _subtracker_workingDirectory(Obj().workingDirectory())
        ,
        _subtracker_libraries(Obj().libraries())
        ,
        _subtracker_scan(Obj().scan())
        ,
        _subtracker_httpsPort(Obj().httpsPort())
        ,
        _subtracker_daemon(Obj().daemon())
    {}

    auto& workingDirectory()
    {
        MarkFieldEdited_(TData::FieldIndex::workingDirectory);
        return _subtracker_workingDirectory;
    }
    auto& libraries()
    {
        MarkFieldEdited_(TData::FieldIndex::libraries);
        return _subtracker_libraries;
    }
    auto& scan()
    {
        MarkFieldEdited_(TData::FieldIndex::scan);
        return _subtracker_scan;
    }
    auto& httpsPort()
    {
        MarkFieldEdited_(TData::FieldIndex::httpsPort);
        return _subtracker_httpsPort;
    }
    auto& daemon()
    {
        MarkFieldEdited_(TData::FieldIndex::daemon);
        return _subtracker_daemon;
    }
    void set_workingDirectory(shared_string&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::workingDirectory, Obj().workingDirectory(), val);
        Obj().set_workingDirectory(std::move(val));
    }

    void set_libraries(std::vector<shared_string>&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::libraries, Obj().libraries(), val);
        Obj().set_libraries(std::move(val));
    }

    void add_libraries(shared_string&& args)
    {
        MarkFieldEdited_(TData::FieldIndex::libraries);
        libraries().RecordMutation_add_(args);
        return Stencil::Mutators<std::vector<shared_string>>::add(Obj().libraries(), std::move(args));
    }
    void remove_libraries(size_t&& args)
    {
        MarkFieldEdited_(TData::FieldIndex::libraries);
        libraries().RecordMutation_remove_(args);
        return Stencil::Mutators<std::vector<shared_string>>::remove(Obj().libraries(), std::move(args));
    }
    Stencil::Transaction<shared_string> edit_libraries(size_t&& args)
    {
        MarkFieldEdited_(TData::FieldIndex::libraries);
        libraries().RecordMutation_edit_(args);
        return Stencil::Mutators<std::vector<shared_string>>::edit(Obj().libraries(), std::move(args));
    }
    void set_scan(std::vector<shared_string>&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::scan, Obj().scan(), val);
        Obj().set_scan(std::move(val));
    }

    void add_scan(shared_string&& args)
    {
        MarkFieldEdited_(TData::FieldIndex::scan);
        scan().RecordMutation_add_(args);
        return Stencil::Mutators<std::vector<shared_string>>::add(Obj().scan(), std::move(args));
    }
    void remove_scan(size_t&& args)
    {
        MarkFieldEdited_(TData::FieldIndex::scan);
        scan().RecordMutation_remove_(args);
        return Stencil::Mutators<std::vector<shared_string>>::remove(Obj().scan(), std::move(args));
    }
    Stencil::Transaction<shared_string> edit_scan(size_t&& args)
    {
        MarkFieldEdited_(TData::FieldIndex::scan);
        scan().RecordMutation_edit_(args);
        return Stencil::Mutators<std::vector<shared_string>>::edit(Obj().scan(), std::move(args));
    }
    void set_httpsPort(int32_t&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::httpsPort, Obj().httpsPort(), val);
        Obj().set_httpsPort(std::move(val));
    }

    void set_daemon(bool&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::daemon, Obj().daemon(), val);
        Obj().set_daemon(std::move(val));
    }

    template <typename TLambda> auto Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::workingDirectory: return lambda("workingDirectory", workingDirectory()); return;
        case TData::FieldIndex::libraries: return lambda("libraries", libraries()); return;
        case TData::FieldIndex::scan: return lambda("scan", scan()); return;
        case TData::FieldIndex::httpsPort: return lambda("httpsPort", httpsPort()); return;
        case TData::FieldIndex::daemon: return lambda("daemon", daemon()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "workingDirectory") { return lambda(TData::FieldIndex::workingDirectory, workingDirectory()); }
        if (fieldName == "libraries") { return lambda(TData::FieldIndex::libraries, libraries()); }
        if (fieldName == "scan") { return lambda(TData::FieldIndex::scan, scan()); }
        if (fieldName == "httpsPort") { return lambda(TData::FieldIndex::httpsPort, httpsPort()); }
        if (fieldName == "daemon") { return lambda(TData::FieldIndex::daemon, daemon()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("workingDirectory", TData::FieldIndex::workingDirectory, workingDirectory(), Obj().workingDirectory());
        lambda("libraries", TData::FieldIndex::libraries, libraries(), Obj().libraries());
        lambda("scan", TData::FieldIndex::scan, scan(), Obj().scan());
        lambda("httpsPort", TData::FieldIndex::httpsPort, httpsPort(), Obj().httpsPort());
        lambda("daemon", TData::FieldIndex::daemon, daemon(), Obj().daemon());
    }
};

template <>
struct Stencil::Visitor<CLOpts1::CommandLineOptions::Data, void> : Stencil::VisitorT<CLOpts1::CommandLineOptions::Data>
{
    using TData = CLOpts1::CommandLineOptions::Data;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::workingDirectory: lambda("workingDirectory", _ref.get().workingDirectory()); return;
        case TData::FieldIndex::libraries: lambda("libraries", _ref.get().libraries()); return;
        case TData::FieldIndex::scan: lambda("scan", _ref.get().scan()); return;
        case TData::FieldIndex::httpsPort: lambda("httpsPort", _ref.get().httpsPort()); return;
        case TData::FieldIndex::daemon: lambda("daemon", _ref.get().daemon()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::workingDirectory: lambda("workingDirectory", _ref.get().workingDirectory()); return;
        case TData::FieldIndex::libraries: lambda("libraries", _ref.get().libraries()); return;
        case TData::FieldIndex::scan: lambda("scan", _ref.get().scan()); return;
        case TData::FieldIndex::httpsPort: lambda("httpsPort", _ref.get().httpsPort()); return;
        case TData::FieldIndex::daemon: lambda("daemon", _ref.get().daemon()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        lambda("workingDirectory", _ref.get().workingDirectory());
        lambda("libraries", _ref.get().libraries());
        lambda("scan", _ref.get().scan());
        lambda("httpsPort", _ref.get().httpsPort());
        lambda("daemon", _ref.get().daemon());
    }

    std::reference_wrapper<TData> _ref;
};

template <>
struct Stencil::Visitor<const CLOpts1::CommandLineOptions::Data, void>
    : Stencil::VisitorT<const CLOpts1::CommandLineOptions::Data>
{
    using TData = CLOpts1::CommandLineOptions::Data const;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::workingDirectory: lambda("workingDirectory", _ref.get().workingDirectory()); return;
        case TData::FieldIndex::libraries: lambda("libraries", _ref.get().libraries()); return;
        case TData::FieldIndex::scan: lambda("scan", _ref.get().scan()); return;
        case TData::FieldIndex::httpsPort: lambda("httpsPort", _ref.get().httpsPort()); return;
        case TData::FieldIndex::daemon: lambda("daemon", _ref.get().daemon()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        lambda("workingDirectory", _ref.get().workingDirectory());
        lambda("libraries", _ref.get().libraries());
        lambda("scan", _ref.get().scan());
        lambda("httpsPort", _ref.get().httpsPort());
        lambda("daemon", _ref.get().daemon());
    }

    std::reference_wrapper<TData> _ref;
};

template <> struct ReflectionBase::TypeTraits<CLOpts1::SimpleObj::Data&>
{
    struct Traits_field1
    {
        using TOwner     = CLOpts1::SimpleObj::Data;
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
        using TOwner     = CLOpts1::SimpleObj::Data;
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
    static constexpr std::string_view           Name() { return "SimpleObj"; }
    static constexpr std::string_view           AttributeValue(const std::string_view& key)
    {
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    using ThisType = CLOpts1::SimpleObj::Data;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && ReflectionBase::AreEqual(obj1.field1(), obj2.field1())
               && ReflectionBase::AreEqual(obj1.field2(), obj2.field2())
            ;
    }

    using Handler = ::ReflectionServices::ReflectedStructHandler<CLOpts1::SimpleObj::Data,
                                                                 Traits_field1
,                                                                 Traits_field2
                                                                 >;
};

template <>
struct Stencil::Transaction<CLOpts1::SimpleObj::Data> : Stencil::TransactionT<CLOpts1::SimpleObj::Data>
{
    using TData = CLOpts1::SimpleObj::Data;

    Transaction<shared_string> _subtracker_field1;
    Transaction<shared_string> _subtracker_field2;
    DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<CLOpts1::SimpleObj::Data>(ptr)
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
};

template <>
struct Stencil::Visitor<CLOpts1::SimpleObj::Data, void> : Stencil::VisitorT<CLOpts1::SimpleObj::Data>
{
    using TData = CLOpts1::SimpleObj::Data;

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
struct Stencil::Visitor<const CLOpts1::SimpleObj::Data, void>
    : Stencil::VisitorT<const CLOpts1::SimpleObj::Data>
{
    using TData = CLOpts1::SimpleObj::Data const;

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

template <> struct ReflectionBase::TypeTraits<CLOpts1::ObjWithList::Data&>
{
    struct Traits_field1
    {
        using TOwner     = CLOpts1::ObjWithList::Data;
        using TFieldType = std::vector<shared_string>;

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
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "ObjWithList"; }
    static constexpr std::string_view           AttributeValue(const std::string_view& key)
    {
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    using ThisType = CLOpts1::ObjWithList::Data;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && ReflectionBase::AreEqual(obj1.field1(), obj2.field1())
            ;
    }

    using Handler = ::ReflectionServices::ReflectedStructHandler<CLOpts1::ObjWithList::Data,
                                                                 Traits_field1
                                                                 >;
};

template <>
struct Stencil::Transaction<CLOpts1::ObjWithList::Data> : Stencil::TransactionT<CLOpts1::ObjWithList::Data>
{
    using TData = CLOpts1::ObjWithList::Data;

    Transaction<std::vector<shared_string>> _subtracker_field1;
    DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<CLOpts1::ObjWithList::Data>(ptr)
        ,
        _subtracker_field1(Obj().field1())
    {}

    auto& field1()
    {
        MarkFieldEdited_(TData::FieldIndex::field1);
        return _subtracker_field1;
    }
    void set_field1(std::vector<shared_string>&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::field1, Obj().field1(), val);
        Obj().set_field1(std::move(val));
    }

    void add_field1(shared_string&& args)
    {
        MarkFieldEdited_(TData::FieldIndex::field1);
        field1().RecordMutation_add_(args);
        return Stencil::Mutators<std::vector<shared_string>>::add(Obj().field1(), std::move(args));
    }
    void remove_field1(size_t&& args)
    {
        MarkFieldEdited_(TData::FieldIndex::field1);
        field1().RecordMutation_remove_(args);
        return Stencil::Mutators<std::vector<shared_string>>::remove(Obj().field1(), std::move(args));
    }
    Stencil::Transaction<shared_string> edit_field1(size_t&& args)
    {
        MarkFieldEdited_(TData::FieldIndex::field1);
        field1().RecordMutation_edit_(args);
        return Stencil::Mutators<std::vector<shared_string>>::edit(Obj().field1(), std::move(args));
    }
    template <typename TLambda> auto Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::field1: return lambda("field1", field1()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "field1") { return lambda(TData::FieldIndex::field1, field1()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("field1", TData::FieldIndex::field1, field1(), Obj().field1());
    }
};

template <>
struct Stencil::Visitor<CLOpts1::ObjWithList::Data, void> : Stencil::VisitorT<CLOpts1::ObjWithList::Data>
{
    using TData = CLOpts1::ObjWithList::Data;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::field1: lambda("field1", _ref.get().field1()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::field1: lambda("field1", _ref.get().field1()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        lambda("field1", _ref.get().field1());
    }

    std::reference_wrapper<TData> _ref;
};

template <>
struct Stencil::Visitor<const CLOpts1::ObjWithList::Data, void>
    : Stencil::VisitorT<const CLOpts1::ObjWithList::Data>
{
    using TData = CLOpts1::ObjWithList::Data const;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::field1: lambda("field1", _ref.get().field1()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        lambda("field1", _ref.get().field1());
    }

    std::reference_wrapper<TData> _ref;
};

template <> struct ReflectionBase::TypeTraits<CLOpts1::CLOptsTest::Data&>
{
    struct Traits_key1
    {
        using TOwner     = CLOpts1::CLOptsTest::Data;
        using TFieldType = shared_string;

        static constexpr std::string_view Name() { return "key1"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_key1; }
        static constexpr auto TPropertySetter() { return &TOwner::set_key1; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::key1, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_listofint
    {
        using TOwner     = CLOpts1::CLOptsTest::Data;
        using TFieldType = std::vector<int32_t>;

        static constexpr std::string_view Name() { return "listofint"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_listofint; }
        static constexpr auto TPropertySetter() { return &TOwner::set_listofint; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::listofint, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_listoflist
    {
        using TOwner     = CLOpts1::CLOptsTest::Data;
        using TFieldType = std::vector<std::vector<int32_t>>;

        static constexpr std::string_view Name() { return "listoflist"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_listoflist; }
        static constexpr auto TPropertySetter() { return &TOwner::set_listoflist; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::listoflist, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_listofobj
    {
        using TOwner     = CLOpts1::CLOptsTest::Data;
        using TFieldType = std::vector<::CLOpts1::SimpleObj::Data>;

        static constexpr std::string_view Name() { return "listofobj"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_listofobj; }
        static constexpr auto TPropertySetter() { return &TOwner::set_listofobj; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::listofobj, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_objoflist
    {
        using TOwner     = CLOpts1::CLOptsTest::Data;
        using TFieldType = ::CLOpts1::ObjWithList::Data;

        static constexpr std::string_view Name() { return "objoflist"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_objoflist; }
        static constexpr auto TPropertySetter() { return &TOwner::set_objoflist; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::objoflist, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_key2
    {
        using TOwner     = CLOpts1::CLOptsTest::Data;
        using TFieldType = shared_string;

        static constexpr std::string_view Name() { return "key2"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_key2; }
        static constexpr auto TPropertySetter() { return &TOwner::set_key2; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::key2, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "CLOptsTest"; }
    static constexpr std::string_view           AttributeValue(const std::string_view& key)
    {
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    using ThisType = CLOpts1::CLOptsTest::Data;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && ReflectionBase::AreEqual(obj1.key1(), obj2.key1())
               && ReflectionBase::AreEqual(obj1.listofint(), obj2.listofint())
               && ReflectionBase::AreEqual(obj1.listoflist(), obj2.listoflist())
               && ReflectionBase::AreEqual(obj1.listofobj(), obj2.listofobj())
               && ReflectionBase::AreEqual(obj1.objoflist(), obj2.objoflist())
               && ReflectionBase::AreEqual(obj1.key2(), obj2.key2())
            ;
    }

    using Handler = ::ReflectionServices::ReflectedStructHandler<CLOpts1::CLOptsTest::Data,
                                                                 Traits_key1
,                                                                 Traits_listofint
,                                                                 Traits_listoflist
,                                                                 Traits_listofobj
,                                                                 Traits_objoflist
,                                                                 Traits_key2
                                                                 >;
};

template <>
struct Stencil::Transaction<CLOpts1::CLOptsTest::Data> : Stencil::TransactionT<CLOpts1::CLOptsTest::Data>
{
    using TData = CLOpts1::CLOptsTest::Data;

    Transaction<shared_string> _subtracker_key1;
    Transaction<std::vector<int32_t>> _subtracker_listofint;
    Transaction<std::vector<std::vector<int32_t>>> _subtracker_listoflist;
    Transaction<std::vector<::CLOpts1::SimpleObj::Data>> _subtracker_listofobj;
    Transaction<::CLOpts1::ObjWithList::Data> _subtracker_objoflist;
    Transaction<shared_string> _subtracker_key2;
    DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<CLOpts1::CLOptsTest::Data>(ptr)
        ,
        _subtracker_key1(Obj().key1())
        ,
        _subtracker_listofint(Obj().listofint())
        ,
        _subtracker_listoflist(Obj().listoflist())
        ,
        _subtracker_listofobj(Obj().listofobj())
        ,
        _subtracker_objoflist(Obj().objoflist())
        ,
        _subtracker_key2(Obj().key2())
    {}

    auto& key1()
    {
        MarkFieldEdited_(TData::FieldIndex::key1);
        return _subtracker_key1;
    }
    auto& listofint()
    {
        MarkFieldEdited_(TData::FieldIndex::listofint);
        return _subtracker_listofint;
    }
    auto& listoflist()
    {
        MarkFieldEdited_(TData::FieldIndex::listoflist);
        return _subtracker_listoflist;
    }
    auto& listofobj()
    {
        MarkFieldEdited_(TData::FieldIndex::listofobj);
        return _subtracker_listofobj;
    }
    auto& objoflist()
    {
        MarkFieldEdited_(TData::FieldIndex::objoflist);
        return _subtracker_objoflist;
    }
    auto& key2()
    {
        MarkFieldEdited_(TData::FieldIndex::key2);
        return _subtracker_key2;
    }
    void set_key1(shared_string&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::key1, Obj().key1(), val);
        Obj().set_key1(std::move(val));
    }

    void set_listofint(std::vector<int32_t>&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::listofint, Obj().listofint(), val);
        Obj().set_listofint(std::move(val));
    }

    void add_listofint(int32_t&& args)
    {
        MarkFieldEdited_(TData::FieldIndex::listofint);
        listofint().RecordMutation_add_(args);
        return Stencil::Mutators<std::vector<int32_t>>::add(Obj().listofint(), std::move(args));
    }
    void remove_listofint(size_t&& args)
    {
        MarkFieldEdited_(TData::FieldIndex::listofint);
        listofint().RecordMutation_remove_(args);
        return Stencil::Mutators<std::vector<int32_t>>::remove(Obj().listofint(), std::move(args));
    }
    Stencil::Transaction<int32_t> edit_listofint(size_t&& args)
    {
        MarkFieldEdited_(TData::FieldIndex::listofint);
        listofint().RecordMutation_edit_(args);
        return Stencil::Mutators<std::vector<int32_t>>::edit(Obj().listofint(), std::move(args));
    }
    void set_listoflist(std::vector<std::vector<int32_t>>&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::listoflist, Obj().listoflist(), val);
        Obj().set_listoflist(std::move(val));
    }

    void add_listoflist(std::vector<int32_t>&& args)
    {
        MarkFieldEdited_(TData::FieldIndex::listoflist);
        listoflist().RecordMutation_add_(args);
        return Stencil::Mutators<std::vector<std::vector<int32_t>>>::add(Obj().listoflist(), std::move(args));
    }
    void remove_listoflist(size_t&& args)
    {
        MarkFieldEdited_(TData::FieldIndex::listoflist);
        listoflist().RecordMutation_remove_(args);
        return Stencil::Mutators<std::vector<std::vector<int32_t>>>::remove(Obj().listoflist(), std::move(args));
    }
    Stencil::Transaction<std::vector<int32_t>> edit_listoflist(size_t&& args)
    {
        MarkFieldEdited_(TData::FieldIndex::listoflist);
        listoflist().RecordMutation_edit_(args);
        return Stencil::Mutators<std::vector<std::vector<int32_t>>>::edit(Obj().listoflist(), std::move(args));
    }
    void set_listofobj(std::vector<::CLOpts1::SimpleObj::Data>&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::listofobj, Obj().listofobj(), val);
        Obj().set_listofobj(std::move(val));
    }

    void add_listofobj(::CLOpts1::SimpleObj::Data&& args)
    {
        MarkFieldEdited_(TData::FieldIndex::listofobj);
        listofobj().RecordMutation_add_(args);
        return Stencil::Mutators<std::vector<::CLOpts1::SimpleObj::Data>>::add(Obj().listofobj(), std::move(args));
    }
    void remove_listofobj(size_t&& args)
    {
        MarkFieldEdited_(TData::FieldIndex::listofobj);
        listofobj().RecordMutation_remove_(args);
        return Stencil::Mutators<std::vector<::CLOpts1::SimpleObj::Data>>::remove(Obj().listofobj(), std::move(args));
    }
    Stencil::Transaction<::CLOpts1::SimpleObj::Data> edit_listofobj(size_t&& args)
    {
        MarkFieldEdited_(TData::FieldIndex::listofobj);
        listofobj().RecordMutation_edit_(args);
        return Stencil::Mutators<std::vector<::CLOpts1::SimpleObj::Data>>::edit(Obj().listofobj(), std::move(args));
    }
    void set_objoflist(::CLOpts1::ObjWithList::Data&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::objoflist, Obj().objoflist(), val);
        Obj().set_objoflist(std::move(val));
    }

    void set_key2(shared_string&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::key2, Obj().key2(), val);
        Obj().set_key2(std::move(val));
    }

    template <typename TLambda> auto Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::key1: return lambda("key1", key1()); return;
        case TData::FieldIndex::listofint: return lambda("listofint", listofint()); return;
        case TData::FieldIndex::listoflist: return lambda("listoflist", listoflist()); return;
        case TData::FieldIndex::listofobj: return lambda("listofobj", listofobj()); return;
        case TData::FieldIndex::objoflist: return lambda("objoflist", objoflist()); return;
        case TData::FieldIndex::key2: return lambda("key2", key2()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "key1") { return lambda(TData::FieldIndex::key1, key1()); }
        if (fieldName == "listofint") { return lambda(TData::FieldIndex::listofint, listofint()); }
        if (fieldName == "listoflist") { return lambda(TData::FieldIndex::listoflist, listoflist()); }
        if (fieldName == "listofobj") { return lambda(TData::FieldIndex::listofobj, listofobj()); }
        if (fieldName == "objoflist") { return lambda(TData::FieldIndex::objoflist, objoflist()); }
        if (fieldName == "key2") { return lambda(TData::FieldIndex::key2, key2()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("key1", TData::FieldIndex::key1, key1(), Obj().key1());
        lambda("listofint", TData::FieldIndex::listofint, listofint(), Obj().listofint());
        lambda("listoflist", TData::FieldIndex::listoflist, listoflist(), Obj().listoflist());
        lambda("listofobj", TData::FieldIndex::listofobj, listofobj(), Obj().listofobj());
        lambda("objoflist", TData::FieldIndex::objoflist, objoflist(), Obj().objoflist());
        lambda("key2", TData::FieldIndex::key2, key2(), Obj().key2());
    }
};

template <>
struct Stencil::Visitor<CLOpts1::CLOptsTest::Data, void> : Stencil::VisitorT<CLOpts1::CLOptsTest::Data>
{
    using TData = CLOpts1::CLOptsTest::Data;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::key1: lambda("key1", _ref.get().key1()); return;
        case TData::FieldIndex::listofint: lambda("listofint", _ref.get().listofint()); return;
        case TData::FieldIndex::listoflist: lambda("listoflist", _ref.get().listoflist()); return;
        case TData::FieldIndex::listofobj: lambda("listofobj", _ref.get().listofobj()); return;
        case TData::FieldIndex::objoflist: lambda("objoflist", _ref.get().objoflist()); return;
        case TData::FieldIndex::key2: lambda("key2", _ref.get().key2()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::key1: lambda("key1", _ref.get().key1()); return;
        case TData::FieldIndex::listofint: lambda("listofint", _ref.get().listofint()); return;
        case TData::FieldIndex::listoflist: lambda("listoflist", _ref.get().listoflist()); return;
        case TData::FieldIndex::listofobj: lambda("listofobj", _ref.get().listofobj()); return;
        case TData::FieldIndex::objoflist: lambda("objoflist", _ref.get().objoflist()); return;
        case TData::FieldIndex::key2: lambda("key2", _ref.get().key2()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        lambda("key1", _ref.get().key1());
        lambda("listofint", _ref.get().listofint());
        lambda("listoflist", _ref.get().listoflist());
        lambda("listofobj", _ref.get().listofobj());
        lambda("objoflist", _ref.get().objoflist());
        lambda("key2", _ref.get().key2());
    }

    std::reference_wrapper<TData> _ref;
};

template <>
struct Stencil::Visitor<const CLOpts1::CLOptsTest::Data, void>
    : Stencil::VisitorT<const CLOpts1::CLOptsTest::Data>
{
    using TData = CLOpts1::CLOptsTest::Data const;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::key1: lambda("key1", _ref.get().key1()); return;
        case TData::FieldIndex::listofint: lambda("listofint", _ref.get().listofint()); return;
        case TData::FieldIndex::listoflist: lambda("listoflist", _ref.get().listoflist()); return;
        case TData::FieldIndex::listofobj: lambda("listofobj", _ref.get().listofobj()); return;
        case TData::FieldIndex::objoflist: lambda("objoflist", _ref.get().objoflist()); return;
        case TData::FieldIndex::key2: lambda("key2", _ref.get().key2()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        lambda("key1", _ref.get().key1());
        lambda("listofint", _ref.get().listofint());
        lambda("listoflist", _ref.get().listoflist());
        lambda("listofobj", _ref.get().listofobj());
        lambda("objoflist", _ref.get().objoflist());
        lambda("key2", _ref.get().key2());
    }

    std::reference_wrapper<TData> _ref;
};

#endif
// SECTION END: Template specializations

// SECTION START: Inline Function Definitions
#if true

#endif
// SECTION END: Inline Function Definitions
