#pragma once
#include "base.h"
namespace Stencil
{
template <typename T, typename TP> struct Visitor;

template <typename T, typename TP>
concept ConceptRVisitor = requires(T t)
{
    typename Visitor<T, TP>;
};

}    // namespace Stencil

#if 0
namespace Stencil
{
template <typename T> struct VisitorT
{
    static constexpr auto GetPtrType()
    {
        if constexpr (std::is_const_v<T>)
        {
            void const* ptr{nullptr};
            return ptr;
        }
        else
        {
            void* ptr{nullptr};
            return ptr;
        }
    }
    /*
    ReflectionServices should provide an Object Browser(Mutator/Accessor), (Const)Visitor ?
        - Ability to Dive into List, Objects
        - No need for Handlers.
        - Serialize/Deserialize using a class Handler at any point during the browser.
        - Optionally carry DeltaTracker data but no need to explicity specify

    Usage Spec
        Deserialize : Visit for edit
            ReflectionServices::ObjectVisitor<Avid::GPS::Data const> visitor;
        Serialize: Visit for read
            ReflectionServices::ObjectVisitor<Avid::GPS::Data> visitor;
        Ability to dive into List, objects, maps etc
            visitor.Select(Value{});
            visitor.Select<T, Deserializer>(Value{});
            visitor.Select<T>(T const&);
            visitor.GoBackUp(); // Go back up
        Serialize Deserialize using a class Handler at any point during the browser
            visitor.Serialize<TSerializer>(); // Current DeltaTracker
            visitor.Deserialize<TDeserializer>(TDeserizlier::Input const&) // Current DeltaTracker
        Optionally carry DeltaTracker data but no need to explicitly specify
            ?? template<typename T, typename TDeltaTracker = void *> // Try void but void* should work too.
            ?? All Select variants and constructors have TDeltaTracker ctx = {} optional param
        Note:
        Handlers accept const for browsing and non-const for editing
        Ediiting on non-const throws exception
    */
    // TODO: const T&
    public:    // Declarations
    enum Mode
    {
        Default,
        Enum,
        List,
        Obj,
        Variant,
        Key,    // List: Integer, Variant: Integer, Obj: String/Integer
    };

    struct StateStack
    {
        // const ReflectionBase::IDataTypeHandler<Stencil::DataType::Unknown>* Handler = nullptr;
        decltype(GetPtrType()) Ptr = nullptr;
        Mode                   mode;
    };

    VisitorT(T& obj) { _stack.push_back(StateStack{&_rootHandler, static_cast<decltype(GetPtrType())>(&obj), Mode::Obj}); }

    bool _TrySelect(Value const& val, bool editmode)
    {
        switch (GetDataTypeHint())
        {
        case Stencil::DataType::Object:
        {
            throw std::logic_error("Not implemented");
#if 0
            auto& state = _stack.back();
            auto  cptr  = const_cast<void*>(state.Ptr);    // Shhh... Thats ok.
            ReflectionBase::IDataTypeHandler<Stencil::DataType::Object>::SubComponent sub;

            if (!state.Handler->ObjectHandler()->TryGetSubComponent(cptr, val, sub)) { return false; }
            _stack.push_back(StateStack{sub.handler, sub.ptr, Mode::Obj});
            return true;
#endif
        }

        case Stencil::DataType::List:
        {
            throw std::logic_error("Not implemented");
#if 0
            auto& state = _stack.back();
            auto  cptr  = const_cast<void*>(state.Ptr);    // Shhh... Thats ok.
            if (editmode)
            {
                auto sub = state.Handler->ListHandler()->GetOrCreateAt(cptr, val);
                _stack.push_back(StateStack{sub.handler, sub.ptr, Mode::List});
            }
            else
            {
                ReflectionBase::IDataTypeHandler<Stencil::DataType::List>::SubComponent sub;
                if (!state.Handler->ListHandler()->TryGetAt(cptr, val, sub)) { return false; }
                _stack.push_back(StateStack{sub.handler, sub.ptr, Mode::List});
            }
            return true;
#endif
        }

        case Stencil::DataType::Variant: throw std::runtime_error("Not yet supported. Get to work");
        case Stencil::DataType::Value: [[fallthrough]];
        case Stencil::DataType::Enum: throw std::runtime_error("Unsupported Data Type");

        case Stencil::DataType::Invalid: [[fallthrough]];
        case Stencil::DataType::Unknown: break;
        }
        throw std::runtime_error("Unsupported Data Type");
    }

    bool TrySelect(Value const& val) { return _TrySelect(val, false); }

    VisitorT& Select(Value const& val)
    {
        if (!_TrySelect(val, true)) { throw std::runtime_error("Cannot select into the Visitor Object"); }
        return *this;
    }

    template <typename TKey, typename TSerDes> VisitorT& Select(TKey const& key)
    {
        if constexpr (Value::Supported<TKey>::value) { return Select(Value{key}); }
        else
        {
            TODO();
            return *this;
        }
    }

    VisitorT& GoBackUp()
    {
        _stack.pop_back();
        return *this;
    }

    auto GetDataTypeHint() const { return _stack.back().Handler->GetDataType(); }
    auto GetDataInfo() const { return _stack.back().Handler->GetDataInfo(); }

    Value GetValue() const
    {
        switch (GetDataTypeHint())
        {

        case Stencil::DataType::Value:
        {
            auto& state = _stack.back();
            auto  cptr  = const_cast<void*>(state.Ptr);    // Shhh... Thats ok.
            return state.Handler->ValueHandler()->Read(cptr);
        }
        case Stencil::DataType::Enum: [[fallthrough]];

        case Stencil::DataType::Object: [[fallthrough]];
        case Stencil::DataType::List: [[fallthrough]];
        case Stencil::DataType::Variant: [[fallthrough]];

        case Stencil::DataType::Invalid: [[fallthrough]];
        case Stencil::DataType::Unknown: break;
        }
        throw std::runtime_error("Unsupported Data Type");
    }
#if 0
    void SetValue(Value const& val)
    {
        static_assert(!std::is_const_v<T>, "Cannot SetValue into a const type");

        switch (GetDataTypeHint())
        {
        case Stencil::DataType::Variant:
        {
            auto& state    = _stack.back();
            auto  cptr     = const_cast<void*>(state.Ptr);    // Shhh... Thats ok.
            auto  typecomp = state.Handler->VariantHandler()->GetVariantTypesHandler(cptr);
            typecomp.uniontypehandler->Write(typecomp.ptr, val);
            auto sub = state.Handler->VariantHandler()->GetActiveVariantHandler(cptr);
            _stack.pop_back();
            _stack.push_back(StateStack{sub.handler, sub.ptr, Mode::Variant});
            return;
        }
        //case Stencil::DataType::Enum: [[fallthrough]];
        case Stencil::DataType::Value:
        {
            auto& state = _stack.back();
            auto  cptr  = const_cast<void*>(state.Ptr);    // Shhh... Thats ok.
            return state.Handler->ValueHandler()->Write(cptr, val);
        }
        case Stencil::DataType::List:    // Could be a string => (char list)
        {
            if (val.GetType() != Value::Type::String) break;
            shared_string str     = val;
            auto&         state   = _stack.back();
            auto          cptr    = const_cast<void*>(state.Ptr);    // Shhh... Thats ok.
            auto          handler = state.Handler->ListHandler();
            for (size_t i = 0; i < str.size(); i++)
            {
                auto sub = handler->GetOrCreateAt(cptr, i);
                sub.handler->ValueHandler()->Write(sub.ptr, Value{str.at(i)});
            }
            return;
        }
        case Stencil::DataType::Object: [[fallthrough]];
        case Stencil::DataType::Invalid: [[fallthrough]];
        case Stencil::DataType::Unknown: break;
        }
        throw std::runtime_error("Unsupported Data Type");
    }
#endif
    //typename Stencil::TypeTraits<std::remove_const_t<T>&>::Handler _rootHandler;
    std::vector<StateStack>                                        _stack;
};
template <typename T, typename = void> struct Visitor;

template <typename T> struct Visitor<T, std::enable_if_t<Value::Supported<T>::value>> : VisitorT<T>
{
    Visitor(T& obj) : VisitorT<T>(obj) {}
};

template <typename T> struct Visitor<T const, std::enable_if_t<Value::Supported<T>::value>> : VisitorT<T const>
{
    Visitor(T const& obj) : VisitorT<T const>(obj) {}
};

template <typename T> struct Visitor<std::vector<T> const> : VisitorT<std::vector<T> const>
{
    Visitor(std::vector<T> const& obj) : VisitorT<std::vector<T> const>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(size_t index, TLambda&& lambda) const { lambda(index, _ref.get().at(index)); }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        for (size_t i = 0; i < _ref.get().size(); i++) { lambda(i, _ref.get().at(i)); }
    }

    std::reference_wrapper<std::vector<T> const> _ref;
};

template <typename T> struct Visitor<std::vector<T>> : VisitorT<std::vector<T>>
{
    Visitor(std::vector<T>& obj) : VisitorT<std::vector<T>>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(size_t index, TLambda&& lambda) { lambda(index, _ref.get().at(index)); }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        for (size_t i = 0; i < _ref.get().size(); i++) { lambda(i, _ref.get().at(i)); }
    }

    std::reference_wrapper<std::vector<T>> _ref;
};

template <typename T, size_t N> struct Visitor<std::array<T, N> const> : VisitorT<std::array<T, N> const>
{
    using TData = std::array<T, N>;

    Visitor(TData const& obj) : VisitorT<TData const>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(size_t index, TLambda&& lambda) const { lambda(index, _ref.get().at(index)); }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        for (size_t i = 0; i < _ref.get().size(); i++) { lambda(i, _ref.get().at(i)); }
    }
    std::reference_wrapper<TData const> _ref;
};

template <typename T, size_t N> struct Visitor<std::array<T, N>> : VisitorT<std::array<T, N>>
{
    using TData = std::array<T, N>;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(size_t index, TLambda&& lambda) { lambda(index, _ref.get().at(index)); }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        for (size_t i = 0; i < _ref.get().size(); i++) { lambda(i, _ref.get().at(i)); }
    }
    std::reference_wrapper<TData> _ref;
};

}    // namespace Stencil
#endif