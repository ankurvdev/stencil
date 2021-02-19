#pragma once
#include <stencil/stencil.h>
namespace CLOpts1
{
namespace CommandLineOptions
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
        Stencil::ObservablePropsT<Data>::OnChangeRequested(*this, FieldIndex::workingDirectory, _workingDirectory, val);
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::workingDirectory, _workingDirectory, val);
        _workingDirectory = std::move(val);
    }

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
        Stencil::ObservablePropsT<Data>::OnChangeRequested(*this, FieldIndex::libraries, _libraries, val);
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::libraries, _libraries, val);
        _libraries = std::move(val);
    }

    void add_libraries(shared_string&& args)
    {
        Stencil::ObservablePropsT<Data>::OnMutationRequested(*this, FieldIndex::libraries, uint8_t{1}, _libraries, args);
        return Stencil::Mutators<std::vector<shared_string>>::add(_libraries, std::move(args));
    }
    void remove_libraries(size_t&& args)
    {
        Stencil::ObservablePropsT<Data>::OnMutationRequested(*this, FieldIndex::libraries, uint8_t{2}, _libraries, args);
        return Stencil::Mutators<std::vector<shared_string>>::remove(_libraries, std::move(args));
    }
    shared_string at_libraries(size_t const& args) const
    {
        return Stencil::Accessors<std::vector<shared_string>>::at(_libraries, args);
    }
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
        Stencil::ObservablePropsT<Data>::OnChangeRequested(*this, FieldIndex::scan, _scan, val);
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::scan, _scan, val);
        _scan = std::move(val);
    }

    void add_scan(shared_string&& args)
    {
        Stencil::ObservablePropsT<Data>::OnMutationRequested(*this, FieldIndex::scan, uint8_t{1}, _scan, args);
        return Stencil::Mutators<std::vector<shared_string>>::add(_scan, std::move(args));
    }
    void remove_scan(size_t&& args)
    {
        Stencil::ObservablePropsT<Data>::OnMutationRequested(*this, FieldIndex::scan, uint8_t{2}, _scan, args);
        return Stencil::Mutators<std::vector<shared_string>>::remove(_scan, std::move(args));
    }
    shared_string at_scan(size_t const& args) const
    {
        return Stencil::Accessors<std::vector<shared_string>>::at(_scan, args);
    }
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
        Stencil::ObservablePropsT<Data>::OnChangeRequested(*this, FieldIndex::httpsPort, _httpsPort, val);
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::httpsPort, _httpsPort, val);
        _httpsPort = std::move(val);
    }

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
        Stencil::ObservablePropsT<Data>::OnChangeRequested(*this, FieldIndex::daemon, _daemon, val);
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::daemon, _daemon, val);
        _daemon = std::move(val);
    }

};

}    // namespace CommandLineOptions
}    // namespace CLOpts1

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

template <typename T> struct Stencil::DeltaTracker<T, std::enable_if_t<std::is_same_v<T, CLOpts1::CommandLineOptions::Data>>>
{
    using TData = T;

    // TODO : Tentative: We hate pointers
    TData const* const _ptr;
    // TODO : Better way to unify creation interface
    bool _changed = false;

    DELETE_COPY_AND_MOVE(DeltaTracker);

    DeltaTracker(TData const* ptr, bool changed) : _ptr(ptr), _changed(changed)
    {
        // TODO: Tentative
        static_assert(std::is_base_of<Stencil::ObservablePropsT<TData>, TData>::value);
    }

    static constexpr auto Type() { return ReflectionBase::TypeTraits<TData&>::Type(); }

    size_t NumFields() const { return TData::FieldCount(); }
    bool   IsChanged() const { return _ptr->_changetracker.any(); }

    uint8_t MutatorIndex() const;
    bool    OnlyHasDefaultMutator() const;

    bool IsFieldChanged(typename TData::FieldIndex index) const { return _ptr->_changetracker.test(static_cast<size_t>(index)); }

    size_t CountFieldsChanged() const { return _ptr->_changetracker.count(); }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::workingDirectory:
            lambda(DeltaTracker<shared_string>(&_ptr->workingDirectory(), IsFieldChanged(TData::FieldIndex::workingDirectory)));
            return;
        case TData::FieldIndex::libraries:
            lambda(DeltaTracker<std::vector<shared_string>>(&_ptr->libraries(), IsFieldChanged(TData::FieldIndex::libraries)));
            return;
        case TData::FieldIndex::scan:
            lambda(DeltaTracker<std::vector<shared_string>>(&_ptr->scan(), IsFieldChanged(TData::FieldIndex::scan)));
            return;
        case TData::FieldIndex::httpsPort:
            lambda(DeltaTracker<int32_t>(&_ptr->httpsPort(), IsFieldChanged(TData::FieldIndex::httpsPort)));
            return;
        case TData::FieldIndex::daemon:
            lambda(DeltaTracker<bool>(&_ptr->daemon(), IsFieldChanged(TData::FieldIndex::daemon)));
            return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }
};

