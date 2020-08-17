#include <DataHandlerJson.h>
#include <DataModel.h>

#pragma warning(push, 0)
#pragma warning(disable : 4365)
#include <httplib.h>
#pragma warning(pop)

#define STENCIL_USING_WEBSERVICE 1

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
        size_t           index = 0;
        size_t           size  = 0;
        std::string_view view;
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
    WebServiceImpl() = default;
    DELETE_COPY_AND_MOVE(WebServiceImpl);

    virtual ~WebServiceImpl()
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
            char buf[BUFSIZ];
            snprintf(buf, sizeof(buf), "<p>Error Status: <span style='color:red;'>%d</span></p>", res.status);
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
    virtual std::string ServiceRequest(httplib::Request const& request, const std::string_view& url) = 0;
#if 0
    bool FindDefinition(const shared_string &url, const InterfaceDefinition **pdef = nullptr, const InterfaceApiDefinition **papidef = nullptr) const
    {
        pdef && (*pdef = nullptr);
        papidef && (*papidef = nullptr);

        auto def = this->m_Root;
        auto its = url.begin(1), ite = its;
        // Look for the namespace
        for (; !(ite = its.str().find('/')).empty(); its = ite + 1)
        {
            auto olddef = def;
            for (auto it = def->ChildIteratorBegin(); it != def->ChildIteratorEnd(); ++it)
            {
                if ((*it)->get_Name() == its.str(ite))
                {
                    def = *it;
                    break;
                }
            }
            if (olddef == def) return false;
        }

        // Find api name;
        const InterfaceApiDefinition *apidef = nullptr;
        for (const auto &api : def->get_Apis())
        {
            if (api.get_Name() == its.str())
            {
                apidef = &api;
                break;
            }
        }
        if (apidef == nullptr) return false;

        pdef && (*pdef = def);
        papidef && (*papidef = apidef);
        return true;
    }

    shared_string Eval(const shared_string &url, const UrlArgsHandler::UrlParams &urlParams) const
    {
        const InterfaceDefinition *def = nullptr;
        const InterfaceApiDefinition *apidef = nullptr;
        if (!FindDefinition(url, &def, &apidef)) throw HandlerNotFound();
        UrlArgsHandler inputHandler;
        JsonDataHandler outputHandler;
        auto intrface = TInterfaceFactory::Activate(def);
        return apidef->ActivateAndInvoke(intrface, outputHandler, inputHandler, urlParams);
    }

#endif
    int             _port;
    httplib::Server _server;
};

template <typename T1, typename T2> inline bool iequal(T1 const& a, T2 const& b)
{
    return std::equal(a.begin(), a.end(), b.begin(), b.end(), [](auto a, auto b) { return tolower(a) == tolower(b); });
}

template <typename TInterface, typename TInterfaceApi, typename... TInterfaceApis>
static auto WebServiceRequestExecuteApis(httplib::Request const& request, const std::string_view& apiname, const std::string_view& moreurl)
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
            throw std::runtime_error("Api not found");
        }
    }

    using ArgsStruct   = typename ::ReflectionBase::InterfaceApiTraits<TInterfaceApi>::ArgsStruct;
    using StateTracker = ReflectionServices::StateTraker<ArgsStruct, void*>;
    ArgsStruct args;

    if constexpr (!::ReflectionBase::InterfaceApiTraits<TInterfaceApi>::IsStatic())
    {
        if (!moreurl.empty())
        {
            args.instance = TInterface::FindObjectById(moreurl);
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
#ifdef USE_NLOHMANN_JSON
    return ::Json::Stringify<decltype(retval)>(retval);
#else
    throw std::logic_error("No json stringifier defined");
#endif
}

template <typename TInterface, typename... TApis>
static auto ProcessWebServiceRequestForInterface(httplib::Request const& request,
                                                 const std::string_view& apiname,
                                                 const std::string_view& moreurl,
                                                 ::ReflectionBase::InterfaceApiPack<TApis...>)
{
    return WebServiceRequestExecuteApis<TInterface, TApis...>(request, apiname, moreurl);
}

template <typename TInterface, typename... TInterfaces>
static auto WebServiceRequest(httplib::Request const& request, const std::string_view& url)
{
    using Apis                      = typename ::ReflectionBase::InterfaceTraits<TInterface>::Apis;
    constexpr std::string_view name = WebServiceHandlerTraits<TInterface>::Url();
    if (!iequal(url.substr(0u, name.length()), name) || url[name.length()] != '/')
    {
        if constexpr (sizeof...(TInterfaces) != 0)
        {
            return WebServiceRequest<TInterfaces...>(request, url);
        }

        throw std::runtime_error("Invalid Service Request");
    }

    auto             offsets = name.length() + 1u;
    auto             ite     = std::find(url.begin() + static_cast<int>(offsets), url.end(), '/');
    size_t           offsete = static_cast<size_t>(std::distance(url.begin(), ite));
    std::string_view apiname(url.substr(offsets, static_cast<size_t>(offsete - offsets)));
    return ProcessWebServiceRequestForInterface<TInterface>(request, apiname, url.substr(offsete), Apis());
}

}    // namespace impl

template <typename... TInterfaces> struct WebService : public impl::WebServiceImpl
{
    WebService() = default;
    DELETE_COPY_AND_MOVE(WebService);

    virtual std::string ServiceRequest(httplib::Request const& request, const std::string_view& url) override
    {
        return impl::WebServiceRequest<TInterfaces...>(request, url);
    }
};
