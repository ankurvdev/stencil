#include "CommandLineArgsReader.h"
#include "Value.h"

#include <assert.h>
#include <iostream>
#include <sstream>
#include <string>

using Definition = CommandLineArgsReader::Definition;

struct DefinitionTree
{
    std::shared_ptr<Definition>                  def;
    std::vector<std::shared_ptr<DefinitionTree>> children;

    DefinitionTree(std::shared_ptr<Definition> defIn) : def(defIn) {}

    template <typename... TArgs> DefinitionTree(TArgs&&... args) : def(std::make_shared<Definition>(std::forward<TArgs>(args)...)) {}

    void                            AddComponent(std::shared_ptr<DefinitionTree> child) { children.push_back(child); }
    std::shared_ptr<DefinitionTree> FindComponent(size_t index) { return index < children.size() ? children[index] : nullptr; }
    std::shared_ptr<DefinitionTree> FindComponent(shared_string str)
    {
        for (auto& it : children)
        {
            if (it->def->name == str)
            {
                return it;
            }
        }

        return nullptr;
    }
};

typedef void* TStackData;

struct StateTraker
{
    enum Mode
    {
        Val,
        List,
        Obj,
        Enum,
        Union,
        ObjKeyValue,
        ListValue
    };
    constexpr std::string_view ModeToStr(Mode mode)
    {
        switch (mode)
        {
        case Mode::Val: return std::string_view("Value");
        case Mode::List: return std::string_view("List");
        case Mode::Obj: return std::string_view("Obj");
        case Mode::Enum: return std::string_view("Enum");
        case Mode::Union: return std::string_view("Union");
        case Mode::ObjKeyValue: return std::string_view("ObjKeyValue");
        case Mode::ListValue: return std::string_view("ListValue");
        }
        return std::string_view("Unknown");
    }

    StateTraker(std::shared_ptr<DefinitionTree> root) { _stack.push_back(StateStack{root, Mode::Obj, nullptr}); }

    Definition::Type GetFieldTypeHint() const { return _stack.back().def->def->GetType(); }

    void HandleValue(Value const& val, TStackData&& data)
    {
        lines.push_back("v:" + _stack.back().def->def->name + "=" + ((shared_string)val).data());
        _Start(Mode::Val, std::move(data));
        _End(Mode::Val);
    }

    void HandleEnum(Value const& val, TStackData&& data)
    {
        lines.push_back("e:" + _stack.back().def->def->name + "=" + ((shared_string)val).data());
        _Start(Mode::Enum, std::move(data));
        _End(Mode::Enum);
    }

    void SetUnionType(Value const& val, TStackData&& data)
    {
        lines.push_back("u:" + _stack.back().def->def->name + "=" + ((shared_string)val).data());
        _Start(Mode::Union, std::move(data));
    }

    void _Start(Mode mode, TStackData&& data)
    {
        if (_stack.back().mode == Mode::List)
        {
            auto sub = _stack.back().def->FindComponent(0);
            if (sub == nullptr) throw std::logic_error("failure");
            _stack.push_back(StateStack{sub, Mode::ListValue, std::move(data)});
        }
        else
        {
            auto back = _stack.back();
            back.mode = mode;
            back.data = std::move(data);
            _stack.push_back(back);
        }
    }

    void _End(Mode mode)
    {
        lines.push_back("end:" + std::string(ModeToStr(mode)));

        _stack.pop_back();
        if (_stack.back().mode == Mode::ObjKeyValue)
        {
            lines.push_back("end:" + std::string(ModeToStr(Mode::ObjKeyValue)));
            _stack.pop_back();
        }
    }

    void ListStart(TStackData&& data)
    {
        lines.push_back("ls:");
        _Start(Mode::List, std::move(data));
    }

    void ListEnd()
    {
        lines.push_back("le:");
        _End(Mode::List);
    }

    void ObjStart(TStackData&& data)
    {
        lines.push_back("os:");
        _Start(Mode::Obj, std::move(data));
    }

    void ObjEnd()
    {
        lines.push_back("oe:");
        _End(Mode::Obj);
    }

    void ObjKey(Value key, TStackData&& data)
    {
        if (!TryObjKey(key, std::move(data)))
        {
            throw "Key not found";
        }
    }

    bool TryObjKey(Value key, TStackData&& data)
    {
        decltype(_stack.back().def->FindComponent(0)) sub;
        if (key.GetType() == Value::Type::Integer)
        {
            sub = _stack.back().def->FindComponent(key.convert<size_t>());
        }
        else
        {
            sub = _stack.back().def->FindComponent((shared_string)key);
        }

        if (sub == nullptr) throw std::logic_error("failure");
        _stack.push_back(StateStack{sub, Mode::ObjKeyValue, std::move(data)});
        return true;
    }

    shared_string GenerateHelp()
    {
        lines.push_back("gh:");
        return "gh:";
    }

    TStackData& GetStackData() { return _stack.back().data; }

    struct StateStack
    {
        //        const ReflectionBase::ITypeHandler *Handler = nullptr;
        std::shared_ptr<DefinitionTree> def;
        Mode                            mode;
        TStackData                      data;
    };

    //    typename ::ReflectionBase::TypeTraits<T&>::Handler _rootHandler;
    std::vector<StateStack>  _stack;
    std::vector<std::string> lines;
};

struct Handler : public CommandLineArgsReader::Handler
{
    Handler(std::shared_ptr<DefinitionTree> root) : _tracker(root) {}

    virtual void HandleValue(bool value) override { _tracker.HandleValue(Value(value), nullptr); }

    virtual void HandleValue(shared_string str) override { _tracker.HandleValue(Value(str), nullptr); }

    virtual void HandleEnum(shared_string str) override { _tracker.HandleEnum(Value(str), nullptr); }
    virtual void UnionType(shared_string str) override { _tracker.SetUnionType(Value(str), nullptr); }
    virtual void ListStart() override
    {
        // auto sub = FindComponent(0);
        // assert(sub != nullptr);
        _tracker.ListStart(nullptr);
    }
    virtual void ListEnd() override { _tracker.ListEnd(); }

    virtual void ObjStart() override { _tracker.ObjStart(nullptr); }
    virtual void ObjEnd() override { _tracker.ObjEnd(); }
    virtual void ObjKey(Value key) override { _tracker.ObjKey(key, nullptr); }

    virtual std::shared_ptr<CommandLineArgsReader::Definition> GetCurrentContext() override
    {
        return std::make_shared<CommandLineArgsReader::Definition>(shared_string(), _tracker.GetFieldTypeHint());
    }

    virtual shared_string GenerateHelp() override { return _tracker.GenerateHelp(); }

    StateTraker _tracker;
};

auto make_list()
{
    std::shared_ptr<DefinitionTree> meta(std::make_shared<DefinitionTree>(shared_string::make("listofint"), Definition::Type::List));
    meta->AddComponent(std::make_shared<DefinitionTree>(shared_string::make(""), Definition::Type::Value));
    return meta;
}

auto make_obj()
{
    std::shared_ptr<DefinitionTree> meta(std::make_shared<DefinitionTree>(shared_string::make("obj"), Definition::Type::Object));
    meta->AddComponent(std::make_shared<DefinitionTree>(shared_string::make("key1"), Definition::Type::Value));
    meta->AddComponent(std::make_shared<DefinitionTree>(shared_string::make("key2"), Definition::Type::Value));
    return meta;
}

auto make_list_of_list()
{
    std::shared_ptr<DefinitionTree> meta(std::make_shared<DefinitionTree>(shared_string::make("listoflist"), Definition::Type::List));
    meta->AddComponent(make_list());
    return meta;
}

auto make_list_of_obj()
{
    std::shared_ptr<DefinitionTree> meta(std::make_shared<DefinitionTree>(shared_string::make("listofobj"), Definition::Type::List));
    meta->AddComponent(make_obj());
    return meta;
}

auto make_obj_of_list()
{
    std::shared_ptr<DefinitionTree> meta(std::make_shared<DefinitionTree>(shared_string::make("objoflist"), Definition::Type::Object));
    //    auto comp = std::shared_ptr<Definition>(std::make_shared<Definition>(shared_string::make("list1"), Definition::Type::List));
    //    comp->AddComponent(());
    meta->AddComponent(make_list());
    meta->AddComponent(std::make_shared<DefinitionTree>(shared_string::make("key2"), Definition::Type::Value));
    return meta;
}
