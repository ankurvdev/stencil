#include <stencil/stencil.h>

#include <assert.h>
#include <iostream>
#include <sstream>
#include <string>

#if 0
using Definition = CommandLineArgsReader::Definition;

struct DefinitionTree
{
    std::shared_ptr<Definition>                  def;
    std::vector<std::shared_ptr<DefinitionTree>> children;

    std::vector<std::string> tracelines;

    DefinitionTree(std::shared_ptr<Definition> defIn) : def(defIn) {}

    template <typename... TArgs> DefinitionTree(TArgs&&... args) : def(std::make_shared<Definition>(std::forward<TArgs>(args)...)) {}

    void                            AddComponent(std::shared_ptr<DefinitionTree> child) { children.push_back(child); }
    std::shared_ptr<DefinitionTree> FindComponent(size_t index) { return index < children.size() ? children[index] : nullptr; }
    std::shared_ptr<DefinitionTree> FindComponent(std::string_view const& str)
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

auto make_list()
{
    std::shared_ptr<DefinitionTree> meta(std::make_shared<DefinitionTree>(std::string("listofint"), Definition::Type::List));
    meta->AddComponent(std::make_shared<DefinitionTree>(std::string(), Definition::Type::Value));
    return meta;
}

auto make_obj()
{
    std::shared_ptr<DefinitionTree> meta(std::make_shared<DefinitionTree>(std::string("obj"), Definition::Type::Object));
    meta->AddComponent(std::make_shared<DefinitionTree>(std::string("key1"), Definition::Type::Value));
    meta->AddComponent(std::make_shared<DefinitionTree>(std::string("key2"), Definition::Type::Value));
    return meta;
}

auto make_list_of_list()
{
    std::shared_ptr<DefinitionTree> meta(std::make_shared<DefinitionTree>(std::string("listoflist"), Definition::Type::List));
    meta->AddComponent(make_list());
    return meta;
}

auto make_list_of_obj()
{
    std::shared_ptr<DefinitionTree> meta(std::make_shared<DefinitionTree>(std::string("listofobj"), Definition::Type::List));
    meta->AddComponent(make_obj());
    return meta;
}

auto make_obj_of_list()
{
    std::shared_ptr<DefinitionTree> meta(std::make_shared<DefinitionTree>(std::string("objoflist"), Definition::Type::Object));
    //    auto comp = std::shared_ptr<Definition>(std::make_shared<Definition>(std::string const&::make("list1"), Definition::Type::List));
    //    comp->AddComponent(());
    meta->AddComponent(make_list());
    meta->AddComponent(std::make_shared<DefinitionTree>(std::string("key2"), Definition::Type::Value));
    return meta;
}

struct GenericDefinitionTreeHandler : public ReflectionBase::IDataTypeHandler<ReflectionBase::DataType::Unknown>
{
    void Trace(std::string_view const& message) {}
    void Trace(std::string_view const& message, void* ptr) {}
    void Trace(std::string_view const& message, std::string_view const& data) {}

    std::shared_ptr<GenericDefinitionTreeHandler> CurrentDefNestedHandler();
    size_t                                        CurrentDefNestedAllocSize();

    void*         CurrentDefNestedHeapAlloc();
    shared_string CurrentDefName()
    {
        Trace("<type>::Name");
        TODO();
    }
    shared_string CurrentDefDescription()
    {
        Trace("<type>::DEscription");
        TODO();
    }
    shared_string CurrentDefAttributeValue(const std::string_view& key)
    {
        Trace("<type>::AttributeValue", std::string(key));
        TODO();
    }
    bool CurrentDefNestedInRange(size_t index);

    struct GenericType
    {
    };

    struct GenericList
    {
        GenericType subtype;
    };

    struct GenericListHandler : ReflectionBase::IDataTypeHandler<ReflectionBase::DataType::List>
    {
        GenericListHandler(GenericDefinitionTreeHandler& owner) : handler(owner) {}

        virtual SubComponent GetListItemHandler() const override
        {
            handler.Trace("List::GetListItemHandler");
            return SubComponent{handler.CurrentDefNestedHandler().get(), handler.CurrentDefNestedHeapAlloc()};
        }

        virtual SubComponent MoveNext(void* rawptr) const override
        {
            handler.Trace("List::MoveNext", rawptr);
            return SubComponent{handler.CurrentDefNestedHandler().get(), handler.CurrentDefNestedHeapAlloc()};
        }

        virtual shared_string Name() const override { return handler.CurrentDefName(); }
        virtual shared_string Description() const override { return handler.CurrentDefDescription(); }
        virtual shared_string AttributeValue(const std::string_view& key) const override { return handler.CurrentDefAttributeValue(key); }

        virtual void Start() const override { handler.Trace("List::Start"); }
        virtual void End() const override { handler.Trace("List::End"); }

        virtual bool TryGetSubComponent(void* rawptr, size_t index, SubComponent& subcomponent) const override
        {
            handler.Trace("List::TryGetSubComponent", rawptr);
            if (!handler.CurrentDefNestedInRange(index))
            {
                return false;
            }
            subcomponent.handler = handler.CurrentDefNestedHandler().get();
            subcomponent.ptr     = handler.CurrentDefNestedHeapAlloc();
        }

        GenericDefinitionTreeHandler& handler;
    };

    struct GenericValueHandler : ReflectionBase::IDataTypeHandler<ReflectionBase::DataType::Value>
    {
        GenericValueHandler(GenericDefinitionTreeHandler& owner) : handler(owner) {}

        virtual void Write(void* ptr, Value const& value) const { handler.Trace("Value::Write", ptr); }

        virtual Value Read(void* ptr) const
        {
            handler.Trace("Value::Read", ptr);
            throw std::logic_error("Read Unsupported while tracing");
        }

        virtual shared_string Name() const override { return handler.CurrentDefName(); }
        virtual shared_string Description() const override { return handler.CurrentDefDescription(); }
        virtual shared_string AttributeValue(const std::string_view& key) const override { return handler.CurrentDefAttributeValue(key); }

        GenericDefinitionTreeHandler& handler;
    };

    struct GenericObjectHandler : ReflectionBase::IDataTypeHandler<ReflectionBase::DataType::Object>
    {
        GenericObjectHandler(GenericDefinitionTreeHandler& owner) : handler(owner) {}

        virtual void Start() const { handler.Trace("Object::Start"); }

        virtual void End() const { handler.Trace("Object::End"); }

        virtual bool TryGetSubComponent(void* rawptr, Value const& key, SubComponent& subcomponent) const
        {
            handler.Trace("Object::TryGetSubComponent", rawptr);
            TODO();
        }

        virtual size_t GetSubComponentCount() const { handler.Trace("Object::GetSubComponentCount"); }

        virtual SubComponent GetSubComponentAt(void* rawptr, size_t index) const
        {
            handler.Trace("Object::GetSubComponentAt", rawptr);
            TODO();
        }

        virtual shared_string Name() const override { return handler.CurrentDefName(); }
        virtual shared_string Description() const override { return handler.CurrentDefDescription(); }
        virtual shared_string AttributeValue(const std::string_view& key) const override { return handler.CurrentDefAttributeValue(key); }

        GenericDefinitionTreeHandler& handler;
    };

    struct GenericEnumHandler : ReflectionBase::IDataTypeHandler<ReflectionBase::DataType::Enum>
    {
        GenericEnumHandler(GenericDefinitionTreeHandler& owner) : handler(owner) {}

        virtual void Write(void* ptr, Value const& value) const { handler.Trace("Enum::Write", ptr); }

        virtual Value Read(void* ptr) const
        {
            handler.Trace("Enum::Read", ptr);
            throw std::logic_error("Read Unsupported while tracing");
        }

        virtual std::vector<Value> AcceptableValues() const
        {
            handler.Trace("Enum::AcceptableValues");
            throw std::logic_error("Unsupported while tracing");
        }

        virtual shared_string Name() const override { return handler.CurrentDefName(); }
        virtual shared_string Description() const override { return handler.CurrentDefDescription(); }
        virtual shared_string AttributeValue(const std::string_view& key) const override { return handler.CurrentDefAttributeValue(key); }

        GenericDefinitionTreeHandler& handler;
    };

    struct GenericVariantHandler : ReflectionBase::IDataTypeHandler<ReflectionBase::DataType::Variant>
    {
        GenericVariantHandler(GenericDefinitionTreeHandler& owner) : handler(owner) {}

        virtual void Start() const { handler.Trace("Variant::Start"); }

        virtual void End() const { handler.Trace("Variant::End"); }

        virtual VariantTypeSubComponent GetVariantTypesHandler(void* rawptr) const
        {
            handler.Trace("Variant::GetVariantTypesHandler", rawptr);
            TODO();
        }
        virtual VariantSubComponent GetActiveVariantHandler(void* rawptr) const
        {
            handler.Trace("Variant::GetActiveVariantHandler", rawptr);
            TODO();
        }

        virtual shared_string Name() const override { return handler.CurrentDefName(); }
        virtual shared_string Description() const override { return handler.CurrentDefDescription(); }
        virtual shared_string AttributeValue(const std::string_view& key) const override { return handler.CurrentDefAttributeValue(key); }

        virtual std::vector<Value> AcceptableTypeNames() const
        {
            handler.Trace("Variant::AcceptableTypeNames");
            throw std::logic_error("Unsupported while tracing");
        }

        virtual std::vector<VariantSubComponent> SubComponentHandlers() const
        {
            handler.Trace("Variant::SubComponentHandlers");
            throw std::logic_error("Unsupported while tracing");
        }

        GenericDefinitionTreeHandler& handler;
    };

    ~GenericDefinitionTreeHandler() = default;

    GenericDefinitionTreeHandler(std::shared_ptr<DefinitionTree> currentTree) :
        _list(*this), _value(*this), _object(*this), _enum(*this), _union(*this)
    {
    }

    const ReflectionBase::IDataTypeHandler<ReflectionBase::DataType::List>*   ListHandler() const override { return &_list; }
    const ReflectionBase::IDataTypeHandler<ReflectionBase::DataType::Value>*  ValueHandler() const override { return &_value; }
    const ReflectionBase::IDataTypeHandler<ReflectionBase::DataType::Object>* ObjectHandler() const override { return &_object; }
    const ReflectionBase::IDataTypeHandler<ReflectionBase::DataType::Enum>*   EnumHandler() const override { return &_enum; }
    const ReflectionBase::IDataTypeHandler<ReflectionBase::DataType::Variant>*  VariantHandler() const override { return &_union; }

    virtual ReflectionBase::DataType GetDataType() const override { return ReflectionBase::DataType::List; }

    private:
    GenericListHandler   _list;
    GenericValueHandler  _value;
    GenericObjectHandler _object;
    GenericEnumHandler   _enum;
    GenericVariantHandler  _union;

    std::shared_ptr<CommandLineArgsReader::Definition> _currentDef;
};

template <> struct ReflectionBase::TypeTraits<std::shared_ptr<DefinitionTree>&, void>
{
    using Handler = GenericDefinitionTreeHandler;
};
#endif