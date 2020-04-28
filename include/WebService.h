#include <Common.h>
#include <DataHandlerJson.h>
#include <DataModel.h>
#pragma warning(push, 0)
#include <httplib.h>
#pragma warning(pop)

template <typename TInterface> struct WebServiceHandlerTraits
{
    static constexpr const std::string_view Url()
    {
        static_assert(std::is_same<TInterface, TInterface>::value, "Please specialize WebServiceHandlerTraits and provide Url");
        return std::string_view("", 0);
    }
};

namespace impl
{
inline auto tokenizer(std::string_view view, char sep)
{
    struct substr
    {
        size_t                  index = 0;
        size_t                  size  = 0;
        const std::string_view& view;
    };
    struct iterator
    {
        iterator() {}
        iterator(std::string_view view, char sep) : _view(view), _sep(sep) { _validatecurrent(); }
        iterator& operator++()
        {
            _validatecurrent();
            return *this;
        }

        const auto& operator*() const { return _current; }

        bool operator==(const iterator& it) const { return _current == it._current && _sep == it._sep; }
        bool operator!=(const iterator& it) const { return !(*this == it); }

        void _validatecurrent()
        {
            assert(!_view.empty());
            if (_index == _view.size())
            {
                (*this = iterator());
                return;
            }

            auto curptr    = &_view.at(_index);
            auto nextptr   = strchr(curptr, _sep);
            _current.index = _index;
            _current.size  = nextptr == nullptr ? (_view.size() - _index) : (nextptr - curptr);
            _current.view  = _view;
            _index         = _current.index + _current.size;
        }

        struct substr
        {
            size_t           index = 0;
            size_t           size  = 0;
            std::string_view view{};
            bool             operator==(const substr& it) const { return index == it.index && size == it.size && view == it.view; }
        } _current;

        size_t           _index = 0;
        std::string_view _view;
        char             _sep{};
    };

    struct range
    {
        iterator _begin;
        iterator _end;
        auto     begin() { return _begin; }
        auto     end() { return _end; }
    } range = {iterator(view, sep), iterator()};
    return range;
}

struct WebServiceImpl
{
    public:
    struct UnableToStartDaemonException
    {
    };
    struct InvalidUrlException
    {
    };
    struct EmptyUrlException
    {
    };
    struct HandlerNotFound
    {
    };

    ~WebServiceImpl()
    {
        if (_server.is_running())
        {
            _server.stop();
            _listenthread.join();
        }
    }

    void StartOnPort(uint16_t port)
    {
        auto guardscope = std::unique_lock<std::mutex>();
        _server.Get("/.*", [this](const httplib::Request& req, httplib::Response& res) {
            try
            {
                auto response = ServiceRequest(req, req.path.c_str() + 1);
                res.body      = response;
                res.status    = 200;
            }
            catch (std::exception const& ex)
            {
                res.status = 500;
                res.body   = ex.what();
            }
        });
        _server.set_error_handler([](const auto& /*req*/, auto& res) {
            auto fmt = "<p>Error Status: <span style='color:red;'>%d</span></p>";
            char buf[BUFSIZ];
            snprintf(buf, sizeof(buf), fmt, res.status);
            res.set_content(buf, "text/html");
        });
        _port         = port;
        _listenthread = std::thread([this]() { _server.listen("localhost", _port); });

        for (int i = 0; i < 100 && !_server.is_running(); i++) std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (!_server.is_running()) throw std::runtime_error("Unable to start server");
    }

    void StopDaemon()
    {
        {
            auto guardscope = std::unique_lock<std::mutex>();
            _server.stop();
        }
        _cond.notify_all();
    }

    void WaitForStop() { _listenthread.join(); }

    std::thread             _listenthread;
    std::condition_variable _cond;
    std::mutex              _mutex;

    private:
    virtual std::string ServiceRequest(httplib::Request const& request, const char* url) = 0;

    int             _port;
    httplib::Server _server;
};

template <typename TInterface, typename TInterfaceApi, typename... TInterfaceApis>
static auto WebServiceRequestExecuteApis(httplib::Request const& request, const std::string_view& apiname, const char* moreurl)
{
    constexpr const std::string_view name = ::ReflectionBase::InterfaceApiTraits<TInterfaceApi>::Name();
    if (!iequal(name, apiname))
    {
        if constexpr (sizeof...(TInterfaceApis) != 0)
        {
            return WebServiceRequestExecuteApis<TInterface, TInterfaceApis...>(request, apiname, moreurl);
        }
        else
        {
            throw "Api not found";
        }
    }

    using ArgsStruct   = typename ::ReflectionBase::InterfaceApiTraits<TInterfaceApi>::ArgsStruct;
    using StateTracker = ReflectionServices::StateTraker<ArgsStruct, void*>;
    ArgsStruct args;

    if constexpr (!::ReflectionBase::InterfaceApiTraits<TInterfaceApi>::IsStatic())
    {
        if (moreurl && moreurl[0])
        {
            args.instance = TInterface::FindObjectById(std::string_view(moreurl));
        }
    }

    StateTracker tracker(&args, nullptr);
    for (auto const& [key, value] : request.params)
    {
        if (tracker.TryObjKey(Value(shared_string::make(key)), nullptr))
        {
            tracker.HandleValue(Value(shared_string::make(value)), nullptr);
        }
    }

    auto retval = ::ReflectionBase::InterfaceApiTraits<TInterfaceApi>::Invoke(args);
#ifdef USE_RAPIDJSON
    return ::Json::Stringify<decltype(retval)>(retval);
#else
    throw std::logic_error("No json stringifier defined");
#endif
}

template <typename TInterface, typename... TApis>
static auto ProcessWebServiceRequestForInterface(httplib::Request const& request,
                                                          const std::string_view& apiname,
                                                          const char*             moreurl,
                                                          ::ReflectionBase::InterfaceApiPack<TApis...>)
{
    return WebServiceRequestExecuteApis<TInterface, TApis...>(request, apiname, moreurl);
}

template <typename TInterface, typename... TInterfaces>
static auto WebServiceRequest(httplib::Request const& request, const char* url)
{
    using Apis          = typename ::ReflectionBase::InterfaceTraits<TInterface>::Apis;
    constexpr auto name = WebServiceHandlerTraits<TInterface>::Url();
    if (!iequal(std::string_view(url, name.length()), name) || url[name.length()] != '/')
    {
        if constexpr (sizeof...(TInterfaces) != 0)
        {
            return WebServiceRequest<TInterfaces...>(request, url);
        }

        throw "Invalid Service Request";
    }

    const char *ptrs = &url[name.length()] + 1, *ptre = nullptr;
    for (ptre = ptrs; *ptre && *ptre != '/'; ++ptre)
        ;
    std::string_view apiname(ptrs, (ptre - ptrs));
    return ProcessWebServiceRequestForInterface<TInterface>(request, apiname, ptre, Apis());
}

}    // namespace impl

template <typename... TInterfaces> struct WebService : public impl::WebServiceImpl
{
    virtual std::string ServiceRequest(httplib::Request const& request, const char* url) override
    {
        return impl::WebServiceRequest<TInterfaces...>(request, url);
    }
};
