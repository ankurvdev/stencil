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
        template <typename TStr> Exception(std::string_view const& msg, std::span<TStr> const& args, size_t index)
        {
            std::stringstream ss;
            ss << "Error processing args : " << msg << std::endl;

            for (size_t i = std::max(size_t{3u}, index) - 3; i < index && i < args.size(); i++)
            {
                ss.write(args[i].data(), static_cast<std::streamsize>(args[i].size()));
                ss << " ";
            }

            ss << std::endl;
            ss << " ==> ";
            ss.write(args[index].data(), static_cast<std::streamsize>(args[index].size()));
            ss << " <== " << std::endl;
            for (size_t i = index; i < 3 && i < args.size(); i++)
            {
                ss.write(args[i].data(), static_cast<std::streamsize>(args[i].size()));
                ss << " ";
            }
            _message = ss.str();
        }

        const char* what() const noexcept(true) override { return _message.c_str(); }

        private:
        std::string _message;
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

        virtual void HandleValue(bool value)                  = 0;
        virtual void HandleValue(std::string_view const& str) = 0;
        virtual void ListStart()                              = 0;
        virtual void ListEnd()                                = 0;
        virtual void ObjStart()                               = 0;
        virtual void ObjEnd()                                 = 0;
        virtual void ObjKey(std::string_view const& key)      = 0;
        virtual void ObjKey(size_t index)                     = 0;
        virtual void AddKey(size_t index)                     = 0;
        virtual void HandleEnum(std::string_view const& str)  = 0;
        virtual void UnionType(std::string_view const& str)   = 0;

        virtual std::shared_ptr<Definition> GetCurrentContext() = 0;
        virtual std::string                 GenerateHelp()      = 0;
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
            if (_handler->GetCurrentContext()->GetType() == Definition::Type::Value) { _handler->HandleValue(true); }
        }
        else
        {
            std::copy(argv.begin(), eqPtr, name);
            _handler->ObjKey(name);
            _ProcessValue(argv.substr(static_cast<size_t>(std::distance(argv.begin(), ++eqPtr))));
        }
    }

    void _ProcessShortArg(std::string_view const& argName, std::string_view const& argv)
    {
        _handler->ObjKey(argName);
        _ProcessValue(argv);
    }

    void _ProcessRequiredArg(size_t index, std::string_view const& argv)
    {
        switch (_handler->GetCurrentContext()->GetType())
        {
        case Definition::Type::List: _ProcessList(argv); break;
        case Definition::Type::Object:
            _handler->ObjKey(0);
            _ProcessRequiredArg(index, argv);
            break;
        case Definition::Type::Value: [[fallthrough]];
        case Definition::Type::Enum: [[fallthrough]];
        case Definition::Type::Union: _ProcessValue(argv); break;
        case Definition::Type::Invalid:
            _handler->ObjEnd();
            _handler->ObjKey(argv);
            break;
        default: throw std::logic_error("Invalid State Processing Required Arg");
        }
    }

    int _CountBrackets(std::string_view const& str)
    {
        int count = 0;
        for (auto const& ch : str)
        {
            if (ch == '{') count++;
            if (ch == '}') count--;
        }
        return count;
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
                    if (count != 0) { throw std::logic_error("Unclosed parenthesis"); }
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
        size_t index = 0;
        for (auto const& subval : _ValueTokens(val))
        {
            _handler->AddKey(index++);
            _ProcessValue(subval);
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
            _ProcessValue(subval);
        }
        _handler->ObjEnd();
    }

    void _ProcessValue(std::string_view const& val)
    {
        auto valToUse = val;
        //  if (val.size() > 1 && *val.begin() == '{' && *val.rbegin() == '}') valToUse = val.substr(1, val.size() - 2);

        switch (_handler->GetCurrentContext()->GetType())
        {
        case Definition::Type::List: _ProcessList(valToUse); break;
        case Definition::Type::Object:
            // All the following args will be processed in this Context now.
            // DO NOT POP this context
            // Will be popped if this context doesnt accept the arg anymore
            _ProcessObject(valToUse);
            break;
        case Definition::Type::Value: _handler->HandleValue(valToUse); break;
        case Definition::Type::Enum: _handler->HandleEnum(valToUse); break;
        case Definition::Type::Union: _handler->UnionType(valToUse); break;
        case Definition::Type::Invalid: [[fallthrough]];
        default: throw std::logic_error("Error Processing Value");
        }
    }

    public:
    template <typename TStr> void Parse(std::span<TStr> const& args)
    {
        size_t            requiredArgNum = 0;
        int               bracketCount   = 0;
        std::stringstream accumulation;

        enum class Mode
        {
            Normal,
            ShortArg
        } mode{Mode::Normal};

        char shortName{};

        size_t index = 0;
        for (std::string_view arg : args.subspan(1))
        {
            ++index;
            if (arg.size() == 0) { continue; }
            bool useAccumulated = bracketCount != 0;
            bracketCount += _CountBrackets(arg);
            if (bracketCount < 0) { throw Exception("Illegal Bracket usage", args, index); }

            if (bracketCount > 0)
            {
                accumulation.write(arg.data(), static_cast<std::streamsize>(arg.size()));
                continue;
            }

            std::string      accumlated;
            std::string_view argToUse = arg;
            if (useAccumulated)
            {
                accumlated = accumulation.str();
                accumulation.str("");
                argToUse = accumlated;
            }

            switch (mode)
            {
            case Mode::Normal:
            {
                auto it = arg.begin();
                if (*it == '-')
                {
                    if ((++it) == arg.end()) { throw Exception("Empty Switch", args, index); }

                    if (*it == '-')
                    {
                        _ProcessLongArg(arg.substr(2));
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
                            while (_handler->GetCurrentContext()->GetType() == Definition::Type::Invalid) { _handler->ObjEnd(); }

                            _handler->ObjKey(std::string_view(&shortName, 1));

                            bool needsArg = _handler->GetCurrentContext()->GetType() != Definition::Type::Value;
                            if (needsArg)
                            {
                                if (arg.size() > 2)
                                {
                                    // -zValue
                                    _ProcessShortArg(std::string_view(&shortName, 1), arg.substr(2));
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
                    _ProcessRequiredArg(requiredArgNum++, arg);
                }
            }
            break;
            case Mode::ShortArg:
            {
                // -z Value. Value of the short arg
                _ProcessShortArg(std::string_view(&shortName, 1), argToUse);
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
        } catch (CommandLineArgsReader::Exception const& ex)
        {
            std::cerr << ex.what() << std::endl;
            Help(def, args[0]);
        }
    }
};
