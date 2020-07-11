#pragma once
#include <cstring>
#include <filesystem>
#include <iostream>
#include <span>
#include <sstream>

struct CommandLineArgsReader
{
    struct Exception : std::exception
    {
        Exception(std::span<std::string_view> const& /*args*/, size_t /*index*/) {}
        const char* what() const override { return _message.c_str(); }

        private:
        std::string _message{"Error Processoing args. TODO: Create a more detailed message"};
    };

    struct Definition
    {
        enum class Type
        {
            Invalid,
            Object,
            List,
            Value,
            Enum,
            Union,
        };
        std::string name;
        Type        type{Type::Invalid};
        std::string description;

        Definition(std::string&& nameIn, Type typeIn) : name(nameIn), type(typeIn) {}

        Type GetType() const { return type; }
        auto GetName() const { return name; }
        auto GetTypeDescription() const { return description.empty() ? name : description; }
    };

    struct Handler
    {
        virtual ~Handler() = default;

        virtual void                        HandleValue(bool value)                  = 0;
        virtual void                        HandleValue(std::string_view const& str) = 0;
        virtual void                        ListStart()                              = 0;
        virtual void                        ListEnd()                                = 0;
        virtual void                        ObjStart()                               = 0;
        virtual void                        ObjEnd()                                 = 0;
        virtual void                        ObjKey(std::string_view const& key)      = 0;
        virtual void                        ObjKey(size_t index)                     = 0;
        virtual void                        HandleEnum(std::string_view const& str)  = 0;
        virtual void                        UnionType(std::string_view const& str)   = 0;
        virtual std::shared_ptr<Definition> GetCurrentContext()                      = 0;
        virtual std::string                 GenerateHelp()                           = 0;
    } * _handler;

    CommandLineArgsReader(Handler* handler) : _handler(handler) {}

    private:
    auto stricompare(std::string_view const& l, std::string_view const& r)
    {
        return std::equal(l.begin(), l.end(), r.begin(), r.end(), [](auto lc, auto rc) { return std::tolower(lc) == std::tolower(rc); });
    }

    void _ProcessLongArg(std::string_view const& argv)
    {

        if (stricompare(argv, "help"))
        {
            auto rslt = _handler->GenerateHelp();
            return;
        }
        auto eqPtr = std::find(argv.begin(), argv.end(), '=');
        char name[128]{};
        if (eqPtr == argv.end())
        {
            // Either its a boolean value or a Object and we'd like to switch context
            _handler->ObjKey(argv);
            if (_handler->GetCurrentContext()->GetType() == Definition::Type::Value)
            {
                _handler->HandleValue(true);
            }
        }
        else
        {
            std::copy(argv.begin(), eqPtr, name);
            _handler->ObjKey(name);
            this->_ProcessValue(argv.substr(static_cast<size_t>(std::distance(argv.begin(), ++eqPtr))));
        }
    }

    void _ProcessShortArg(std::string_view const& argName, std::string_view const& argv)
    {
        _handler->ObjKey(argName);
        this->_ProcessValue(argv);
    }

    void _ProcessRequiredArg(size_t index, std::string_view const& argv)
    {
        switch (_handler->GetCurrentContext()->GetType())
        {
        case Definition::Type::List: this->_ProcessList(argv); break;
        case Definition::Type::Object:
            _handler->ObjKey(0);
            this->_ProcessRequiredArg(index, argv);
            break;
        case Definition::Type::Value: [[fallthrough]];
        case Definition::Type::Enum: [[fallthrough]];
        case Definition::Type::Union: this->_ProcessValue(argv); break;
        case Definition::Type::Invalid:
            _handler->ObjEnd();
            _handler->ObjKey(argv);
            break;
        default: throw std::logic_error("Invalid State Processing Required Arg");
        }
    }

    auto _ValueTokens(std::string_view const& str)
    {
        struct Iterator
        {
            Iterator() = default;
            Iterator(std::string_view const& str) : _str(str) { _MoveNext(); }
            // DEFAULT_COPY_AND_MOVE(Iterator);

            bool operator==(Iterator const& it) const
            {
                return it._str.data() == _str.data() && it._offset == _offset && it._token == _token;
            }
            bool operator!=(Iterator const& it) const { return !(*this == it); }

            auto _substr(std::string_view::iterator b, std::string_view::iterator e)
            {
                return _str.substr(static_cast<size_t>(b - _str.begin()), static_cast<size_t>(e - b));
            }

            Iterator& _MoveNext()
            {
                if (_str.empty()) return *this;
                const char grpstart = '{', grpend = '}';
                auto       its = _str.begin() + static_cast<int>(_offset);

                if (its == _str.end()) return _Terminate();
                if (*its == ':') ++its;
                if (its == _str.end()) return _Terminate();

                if (*its == grpstart)
                {
                    ///{ .... { ... { ... } ... } ...}
                    auto ite   = ++its;
                    int  count = 1;
                    for (; count > 0 && ite != _str.end(); ++ite)
                    {
                        if (*ite == grpstart) count++;
                        if (*ite == grpend) count--;
                    }
                    if (count != 0)
                    {
                        // TODO: TEst
                        throw std::runtime_error("Unclosed parenthesis");
                    }
                    _token  = _substr(its, ite - 1);
                    _offset = static_cast<size_t>(ite - _str.begin());
                }
                else
                {
                    auto ite = std::find(its, _str.end(), ':');
                    _token   = _substr(its, ite);
                    _offset  = static_cast<size_t>(ite - _str.begin());
                }

                return *this;
            }

            Iterator& _Terminate()
            {
                _str    = {};
                _offset = 0;
                _token  = {};
                return *this;
            }

            std::string_view const& operator*() const { return _token; }
            Iterator&               operator++() { return _MoveNext(); }

            std::string_view _str{};
            std::string_view _token{};
            size_t           _offset{};
        };

        struct Range
        {
            Range(std::string_view const& str) : _begin(str) {}
            Iterator _begin;
            Iterator _end{};
            auto     begin() { return _begin; }
            auto     end() { return _end; }
        };
        return Range(str);
    }

    void _ProcessList(std::string_view const& val)
    {
        _handler->ListStart();
        for (auto const& subval : _ValueTokens(val))
        {
            this->_ProcessValue(subval);
        }

        _handler->ListEnd();
    }

    void _ProcessObject(std::string_view const& val)
    {
        _handler->ObjStart();
        size_t index = 0;
        for (auto const& subval : _ValueTokens(val))
        {
            _handler->ObjKey(index++);
            this->_ProcessValue(subval);
        }
        _handler->ObjEnd();
    }

    void _ProcessValue(std::string_view const& str)
    {
        switch (_handler->GetCurrentContext()->GetType())
        {
        case Definition::Type::List: _ProcessList(str); break;
        case Definition::Type::Object:
            // All the following args will be processed in this Context now.
            // DO NOT POP this context
            // Will be popped if this context doesnt accept the arg anymore
            _ProcessObject(str);
            break;
        case Definition::Type::Value: _handler->HandleValue(str); break;
        case Definition::Type::Enum: _handler->HandleEnum(str); break;
        case Definition::Type::Union: _handler->UnionType(str); break;
        case Definition::Type::Invalid: [[fallthrough]];
        default: throw std::logic_error("Error Processing Value");
        }
    }

    /* std::string _Help(const char *commandName)
     {
         std::vector<std::stringstream> subobjects;
         std::stringstream ss;
         ss << "usage:";
         ss << std::experimental::filesystem::path(commandName).filename();
         ss << " ";
         this->_Help(ss, _handler->GetCurrentContext(), subobjects);
         for (auto &subss : subobjects)
         {
             ss << subss.str() << std::endl;
         }
         return std::string::make(ss.str());
     }

     void _Help(std::stringstream &ss, std::shared_ptr<Definition> context, std::vector<std::stringstream> &subobjects)
     {
         ss << context->GetTypeDescription().c_str();
         switch (context->GetType())
         {
         case Definition::Type::Object:
         {
             ss << "..." << std::endl;
             _tra
             for (const auto &sub : context->GetObjectComponents())
             {
                 std::stringstream ss;
                 _Help(ss, sub, subobjects);
                 subobjects.push_back(std::move(ss));
             }
         }
         break;
         case Definition::Type::List:
             ss << "list" << std::endl;
             break;
         case Definition::Type::Value:
             break;
         }
         ss << std::endl;
     }*/
    public:
    void Parse(std::span<std::string_view> const& args)
    {
        size_t requiredArgNum = 0;
        enum class Mode
        {
            Normal,
            ShortArg

        } mode{Mode::Normal};

        char   shortName{};
        size_t index = 0;
        for (auto const& arg : args.subspan(1))
        {
            ++index;
            auto it = arg.begin();
            if (it == arg.end())
            {
                // TODO: Log warning
                continue;
            }

            switch (mode)
            {
            case Mode::Normal:
            {
                if (*it == '-')
                {
                    if ((++it) == arg.end())
                    {
                        throw Exception(args, index);
                    }

                    if (*it == '-')
                    {
                        this->_ProcessLongArg(arg.substr(2));
                        break;
                    }
                    else
                    {
                        for (; it != arg.end(); ++it)
                        {
                            /* Three types of short args
                             * -zxvf
                             * -zValue
                             * -z Value*/
                            shortName = *it;
                            if (shortName == 'h')
                            {
                                auto rslt = _handler->GenerateHelp();
                                return;
                            }

                            // End any current contexts that need values
                            // TODO : Should this be list / something other than invalid
                            // Why would this be invalid
                            while (_handler->GetCurrentContext()->GetType() == Definition::Type::Invalid)
                            {
                                _handler->ObjEnd();
                            }

                            _handler->ObjKey(std::string_view(&shortName, 1));

                            bool needsArg = _handler->GetCurrentContext()->GetType() != Definition::Type::Value;
                            if (needsArg)
                            {
                                if (arg.size() > 2)
                                {
                                    // -zValue
                                    this->_ProcessShortArg(std::string_view(&shortName, 1), arg.substr(2));
                                }
                                else
                                {
                                    // -z Value. Value is in the next arg
                                    mode = Mode::ShortArg;
                                }
                                break;    // Out of the inner loop.
                            }
                            // -zxvf continue for mode
                        }
                    }
                }
                else
                {
                    this->_ProcessRequiredArg(requiredArgNum++, arg);
                }
            }
            break;

            case Mode::ShortArg:
            {
                // -z Value. Value of the short arg
                this->_ProcessShortArg(std::string_view(&shortName, 1), arg);
                mode = Mode::Normal;
            }
            break;
            }
        }
    }

    template <class TDataModel> void ParseWithHelpOnError(TDataModel* pData, std::span<std::string_view> args) noexcept
    {
        auto def = TDataModel::GetModelDefinition();
        try
        {
            _Deserialize(def, pData, args);
        }
        catch (CommandLineArgsReader::Exception const& ex)
        {
            std::cerr << ex.what() << std::endl;
            Help(def, args[0]);
        }
    }
};
