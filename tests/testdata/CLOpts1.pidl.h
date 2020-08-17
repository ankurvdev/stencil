#pragma once
#include <DataModel.h>
namespace CLOpts1
{
namespace CommandLineOptions
{
struct Data;
}
namespace CommandLineOptions
{

struct Data : public ReflectionBase::ObjMarker
{
    /*template <typename...TArgs> Data(TArgs&& ... args)
    {
        ReflectionBase::Construct<Data>(this, std::forward<TArgs>(args)...);
    }*/

    enum class FieldIndex
    {
        Invalid,
        workingDirectory,
        libraries,
        scan,
        httpsPort,
        daemon
    };

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
    void                 workingDirectory(shared_string&& val) { _workingDirectory = std::move(val); }

    shared_string& get_workingDirectory()
    {
        return _workingDirectory;
        ;
    }
    void set_workingDirectory(shared_string&& val) { _workingDirectory = std::move(val); }

    private:
    std::vector<shared_string> _libraries = {};

    public:
    std::vector<shared_string>&       libraries() { return _libraries; }
    const std::vector<shared_string>& libraries() const { return _libraries; }
    void                              libraries(std::vector<shared_string>&& val) { _libraries = std::move(val); }

    std::vector<shared_string>& get_libraries()
    {
        return _libraries;
        ;
    }
    void set_libraries(std::vector<shared_string>&& val) { _libraries = std::move(val); }

    private:
    std::vector<shared_string> _scan = {};

    public:
    std::vector<shared_string>&       scan() { return _scan; }
    const std::vector<shared_string>& scan() const { return _scan; }
    void                              scan(std::vector<shared_string>&& val) { _scan = std::move(val); }

    std::vector<shared_string>& get_scan()
    {
        return _scan;
        ;
    }
    void set_scan(std::vector<shared_string>&& val) { _scan = std::move(val); }

    private:
    int32_t _httpsPort = 3443;

    public:
    int32_t&       httpsPort() { return _httpsPort; }
    const int32_t& httpsPort() const { return _httpsPort; }
    void           httpsPort(int32_t&& val) { _httpsPort = std::move(val); }

    int32_t& get_httpsPort()
    {
        return _httpsPort;
        ;
    }
    void set_httpsPort(int32_t&& val) { _httpsPort = std::move(val); }

    private:
    bool _daemon = 0;

    public:
    bool&       daemon() { return _daemon; }
    const bool& daemon() const { return _daemon; }
    void        daemon(bool&& val) { _daemon = std::move(val); }

    bool& get_daemon()
    {
        return _daemon;
        ;
    }
    void set_daemon(bool&& val) { _daemon = std::move(val); }
};

}    // namespace CommandLineOptions
}    // namespace CLOpts1

template <> struct ReflectionBase::TypeTraits<CLOpts1::CommandLineOptions::Data&>
{
    struct Traits_workingDirectory
    {
        using TOwner = CLOpts1::CommandLineOptions::Data;

        static constexpr std::string_view Name() { return "workingDirectory"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_workingDirectory; }
        static constexpr auto TPropertySetter() { return &TOwner::set_workingDirectory; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::workingDirectory, key);
        }

        static const ::ReflectionBase::Flags Flags() { return ::ReflectionBase::Flags{::ReflectionBase::Flag::Max}; }
    };
    struct Traits_libraries
    {
        using TOwner = CLOpts1::CommandLineOptions::Data;

        static constexpr std::string_view Name() { return "libraries"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_libraries; }
        static constexpr auto TPropertySetter() { return &TOwner::set_libraries; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::libraries, key);
        }

        static const ::ReflectionBase::Flags Flags() { return ::ReflectionBase::Flags{::ReflectionBase::Flag::Max}; }
    };
    struct Traits_scan
    {
        using TOwner = CLOpts1::CommandLineOptions::Data;

        static constexpr std::string_view Name() { return "scan"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_scan; }
        static constexpr auto TPropertySetter() { return &TOwner::set_scan; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::scan, key);
        }

        static const ::ReflectionBase::Flags Flags() { return ::ReflectionBase::Flags{::ReflectionBase::Flag::Max}; }
    };
    struct Traits_httpsPort
    {
        using TOwner = CLOpts1::CommandLineOptions::Data;

        static constexpr std::string_view Name() { return "httpsPort"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_httpsPort; }
        static constexpr auto TPropertySetter() { return &TOwner::set_httpsPort; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::httpsPort, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{::ReflectionBase::Flag::HasDefaultValue, ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_daemon
    {
        using TOwner = CLOpts1::CommandLineOptions::Data;

        static constexpr std::string_view Name() { return "daemon"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_daemon; }
        static constexpr auto TPropertySetter() { return &TOwner::set_daemon; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::daemon, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{::ReflectionBase::Flag::HasDefaultValue, ::ReflectionBase::Flag::Max};
        }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "CommandLineOptions"; }
    static constexpr std::string_view AttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

    using Handler = ::ReflectionServices::ReflectedStructHandler<CLOpts1::CommandLineOptions::Data,
                                                                 Traits_workingDirectory,
                                                                 Traits_libraries,
                                                                 Traits_scan,
                                                                 Traits_httpsPort,
                                                                 Traits_daemon>;
};
