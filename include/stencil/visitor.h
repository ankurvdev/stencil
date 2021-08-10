#pragma once
#include "base.h"

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
        Union,
        Key,    // List: Integer, Union: Integer, Obj: String/Integer
    };

    struct StateStack
    {
        const ReflectionBase::IDataTypeHandler<ReflectionBase::DataType::Unknown>* Handler = nullptr;
        decltype(GetPtrType())                                                     Ptr     = nullptr;
        Mode                                                                       mode;
    };

    VisitorT(T& obj) { _stack.push_back(StateStack{&_rootHandler, static_cast<decltype(GetPtrType())>(&obj), Mode::Obj}); }

    bool TrySelect(Value const& val)
    {
        switch (GetDataTypeHint())
        {
        case ReflectionBase::DataType::Object:
        {
            auto& state = _stack.back();
            auto  cptr  = const_cast<void*>(state.Ptr);    // Shhh... Thats ok.
            ReflectionBase::IDataTypeHandler<ReflectionBase::DataType::Object>::SubComponent sub;

            if (!state.Handler->ObjectHandler()->TryGetSubComponent(cptr, val, sub)) { return false; }
            _stack.push_back(StateStack{sub.handler, sub.ptr, Mode::Obj});
            return true;
        }

        case ReflectionBase::DataType::List:
        {
            auto& state = _stack.back();
            auto  cptr  = const_cast<void*>(state.Ptr);    // Shhh... Thats ok.
            ReflectionBase::IDataTypeHandler<ReflectionBase::DataType::List>::SubComponent sub;

            if (!state.Handler->ListHandler()->TryGetSubComponent(cptr, val, sub)) { return false; }
            _stack.push_back(StateStack{sub.handler, sub.ptr, Mode::List});
            return true;
        }

        case ReflectionBase::DataType::Union: throw std::runtime_error("Not yet supported. Get to work");
        case ReflectionBase::DataType::Value: [[fallthrough]];
        case ReflectionBase::DataType::Enum: throw std::runtime_error("Unsupported Data Type");

        case ReflectionBase::DataType::Invalid: [[fallthrough]];
        case ReflectionBase::DataType::Unknown: break;
        }
        throw std::runtime_error("Unsupported Data Type");
    }

    VisitorT& Select(Value const& val)
    {
        if (!TrySelect(val)) { throw std::runtime_error("Cannot select into the Visitor Object"); }
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

        case ReflectionBase::DataType::Value:
        {
            auto& state = _stack.back();
            auto  cptr  = const_cast<void*>(state.Ptr);    // Shhh... Thats ok.
            return state.Handler->ValueHandler()->Read(cptr);
        }
        case ReflectionBase::DataType::Enum: [[fallthrough]];

        case ReflectionBase::DataType::Object: [[fallthrough]];
        case ReflectionBase::DataType::List: [[fallthrough]];
        case ReflectionBase::DataType::Union: [[fallthrough]];

        case ReflectionBase::DataType::Invalid: [[fallthrough]];
        case ReflectionBase::DataType::Unknown: break;
        }
        throw std::runtime_error("Unsupported Data Type");
    }

    void SetValue(Value const& val)
    {
        static_assert(!std::is_const_v<T>, "Cannot SetValue into a const type");

        switch (GetDataTypeHint())
        {
        case ReflectionBase::DataType::Union:
        {
            auto& state    = _stack.back();
            auto  cptr     = const_cast<void*>(state.Ptr);    // Shhh... Thats ok.
            auto  typecomp = state.Handler->UnionHandler()->GetUnionTypesHandler(cptr);
            typecomp.uniontypehandler->Write(typecomp.ptr, val);
            auto sub = state.Handler->UnionHandler()->GetActiveUnionHandler(cptr);
            _stack.pop_back();
            _stack.push_back(StateStack{sub.handler, sub.ptr, Mode::Union});
            return;
        }
        case ReflectionBase::DataType::Enum: [[fallthrough]];
        case ReflectionBase::DataType::Value:
        {
            auto& state = _stack.back();
            auto  cptr  = const_cast<void*>(state.Ptr);    // Shhh... Thats ok.
            return state.Handler->ValueHandler()->Write(cptr, val);
        }

        case ReflectionBase::DataType::Object: [[fallthrough]];
        case ReflectionBase::DataType::List: [[fallthrough]];
        case ReflectionBase::DataType::Invalid: [[fallthrough]];
        case ReflectionBase::DataType::Unknown: break;
        }
        throw std::runtime_error("Unsupported Data Type");
    }

    typename ReflectionBase::TypeTraits<std::remove_const_t<T>&>::Handler _rootHandler;
    std::vector<StateStack>                                               _stack;
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

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        for (size_t i = 0; i < _ref.get().size(); i++) { lambda(i, _ref.get().at(i)); }
    }

    std::reference_wrapper<std::vector<T> const> _ref;
};

template <typename T, size_t N> struct Visitor<std::array<T, N> const> : VisitorT<std::array<T, N> const>
{
    using TData = std::array<T, N>;

    Visitor(TData const& obj) : VisitorT<TData const>(obj), _ref(obj) {}

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        for (size_t i = 0; i < _ref.get().size(); i++) { lambda(i, _ref.get().at(i)); }
    }
    std::reference_wrapper<TData> _ref;
};

}    // namespace Stencil
