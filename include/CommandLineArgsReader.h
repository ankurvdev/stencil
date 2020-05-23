#pragma once
#include <cstring>
#include <filesystem>
#include <iostream>
#include <sstream>

struct CommandLineArgsReader
{

    inline bool stricompare(const char* str1, const char* str2)
    {
        auto len1 = strlen(str1);
        auto len2 = strlen(str2);
        return std::equal(str1, str1 + len1, str2, str2 + len2, [](char a, char b) { return tolower(a) == tolower(b); });
    }

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
            Unknown
        };
        std::string name;
        Type        type;
        std::string description;

        Definition(std::string&& nameIn, Type typeIn) : name(nameIn), type(typeIn) {}

        Type GetType() const { return type; }
        auto GetName() const { return name; }
        auto GetTypeDescription() const { return description.empty() ? name : description; }
    };

    struct Handler
    {
        virtual void                        HandleValue(bool value)             = 0;
        virtual void                        HandleValue(std::string_view const& str) = 0;
        virtual void                        ListStart()                         = 0;
        virtual void                        ListEnd()                           = 0;
        virtual void                        ObjStart()                          = 0;
        virtual void                        ObjEnd()                            = 0;
        virtual void                        ObjKey(std::string_view const& key)      = 0;
        virtual void                        ObjKey(size_t index)                = 0;
        virtual void                        HandleEnum(std::string_view const& str)  = 0;
        virtual void                        UnionType(std::string_view const& str)   = 0;
        virtual std::shared_ptr<Definition> GetCurrentContext()                 = 0;
        virtual std::string                 GenerateHelp()                      = 0;
    } * _handler;

    CommandLineArgsReader(Handler* handler) : _handler(handler) {}

    private:
    void _ProcessLongArg(const char* argv)
    {

        if (stricompare(argv, "help"))
        {
            auto rslt = _handler->GenerateHelp();
            return;
        }
        const char* eqPtr = strchr(argv, '=');
        char        name[128]{};
        if (eqPtr == nullptr || eqPtr[1] == '\0')
        {
            // Either its a boolean value or a Object and we'd like to switch context
            _handler->ObjKey(std::string(argv));
            if (_handler->GetCurrentContext()->GetType() == Definition::Type::Value)
            {
                _handler->HandleValue(true);
            }
        }
        else
        {
            std::copy(argv, eqPtr, name);
            _handler->ObjKey(std::string(name));
            this->_ProcessValue(eqPtr + 1);
        }
    }

    void _ProcessShortArg(const char* argName, const char* argv)
    {
        _handler->ObjKey(std::string(argName));
        this->_ProcessValue(argv);
    }

    void _ProcessRequiredArg(size_t index, const char* argv)
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
            _handler->ObjKey(std::string(argv));
            break;
        case Definition::Type::Unknown: throw 1;
        }
    }

    const char* clgettoken(const char* ptr, char* buffer, size_t bufSize)
    {
        const char separator = ':', grpstart = '{', grpend = '}';
        size_t     i = 0;
        if (*ptr == '\0') return nullptr;
        if (*ptr == grpstart)
        {
            int count = 1;
            for (ptr++; i < bufSize && *ptr != '\0'; ptr++, i++)
            {
                if (*ptr == grpstart) count++;
                if (*ptr == grpend) count--;
                if (count == 0)
                {
                    ptr++;
                    break;
                }
                buffer[i] = *ptr;
            }
            if (*ptr == ':') ++ptr;
            buffer[i] = '\0';
            return ptr;
        }
        for (; *ptr != ':' && *ptr != '\0' && i < bufSize; i++, ptr++) buffer[i] = *ptr;
        if (*ptr == ':') ++ptr;
        buffer[i] = '\0';
        return ptr;
    }

    void _ProcessList(std::string_view val)
    {
        _handler->ListStart();
        char buffer[1024];
        for (auto cPtr = clgettoken(val.data(), buffer, 1024); cPtr != nullptr; cPtr = clgettoken(cPtr, buffer, 1024))
        {
            this->_ProcessValue(buffer);
        }
        _handler->ListEnd();
    }

    void _ProcessObject(std::string_view val)
    {
        _handler->ObjStart();
        char   buffer[1024];
        size_t index = 0;
        for (auto cPtr = clgettoken(val.data(), buffer, 1024); cPtr != nullptr; cPtr = clgettoken(cPtr, buffer, 1024))
        {
            _handler->ObjKey(index++);
            this->_ProcessValue(buffer);
        }
        _handler->ObjEnd();
    }

    void _ProcessValue(std::string_view str)
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
        case Definition::Type::Value: _handler->HandleValue(std::string(str)); break;
        case Definition::Type::Enum: _handler->HandleEnum(std::string(str)); break;
        case Definition::Type::Union: _handler->UnionType(std::string(str)); break;
        case Definition::Type::Invalid: [[fallthrough]];
        case Definition::Type::Unknown: throw "Unknown Data Type";
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
    void Parse(size_t argc, const char* argv[])
    {
        const char* cargv;
        size_t      requiredArgNum = 0;
        for (size_t i = 1; i < argc; i++)
        {
            if ((cargv = argv[i]) == nullptr || cargv[0] == '\0') continue;
            switch (argv[i][0])
            {
            case '-':
            {
                if (cargv[1] == '\0') break;
                if (cargv[1] == '-')
                {
                    this->_ProcessLongArg(&cargv[2]);
                    break;
                }
                char shortName[2]{};

            repeat:
                /* Three types of short args
                 * -zxvf
                 * -zValue
                 * -z Value*/
                shortName[0] = cargv[2];
                if (shortName[0] == 'h')
                {
                    auto rslt = _handler->GenerateHelp();
                    return;
                }
                while (true)
                {
                    if (_handler->GetCurrentContext()->GetType() == Definition::Type::Invalid)
                    {
                        _handler->ObjEnd();
                    }
                    else
                    {
                        _handler->ObjKey(std::string(shortName));
                        break;
                    };
                }

                bool needsArg = _handler->GetCurrentContext()->GetType() != Definition::Type::Value;
                if (needsArg)
                {
                    if (cargv[2] == '\0')
                    {
                        i++;
                        if (i == argc)
                        {
                            throw 1;
                        }
                        cargv = argv[i];
                        this->_ProcessShortArg(shortName, cargv);
                    }
                    else
                    {
                        this->_ProcessShortArg(shortName, &cargv[2]);
                    }
                }
                else
                {
                    if (cargv[2] == '\0')
                    {
                        break;
                    }
                    cargv++;
                    // There's another switch in the same switch text (eg: -zxvf)
                    goto repeat;
                }
            }
            default: this->_ProcessRequiredArg(requiredArgNum++, cargv);
            }
        }
    }

    template <class TDataModel> void ParseWithHelpOnError(TDataModel* pData, int argc, const char* argv[]) noexcept
    {
        auto def = TDataModel::GetModelDefinition();
        try
        {
            _Deserialize(def, pData, argc, argv);
        }
        catch (...)
        {
            Help(def, argv[0]);
        }
    }
};
