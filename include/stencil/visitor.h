#pragma once
#include "base.h"
#include "timestamped.h"

#include <memory>

namespace Stencil
{
// 3 core datatypes
//     Primitive
//     Iterable
//     Indexable

// [R/W]VisitorWithParent<T, TP>
//      TP& Parent();
//      Visitor<T>

// [R/W]Visitor<T>
//      Primitive
//          [W] Write
//          [R] Read
//      Iterable
//          Iterator Start()
//          bool MoveNext(Iterator)
//      Indexable
//          auto VisitAt(Key k) -> VisitorWithParent
//          void Add(Key k)     -> VisitorWithParent
//          void VisitAll([&](auto k, auto& v)
template <typename T> struct Visitor;

template <typename T> struct VisitorForIterable
{
    // typename Iterator
    // static void IteratorBegin(Iterator& it, T[const]& obj);
    // static void IteratorMoveNext(Iterator& it, T[const]& obj);
    // static bool IteratorValid(Iterator& it, T[const]& obj);
    // static void Visit(Iterator& it, T1& obj, T[const]Lambda&& lambda);
};

template <typename T> struct VisitorForIndexable
{
    // typename Key
    // static void IteratorBegin(Iterator& it, T[const]& obj);
    // static void IteratorMoveNext(Iterator& it, T[const]& obj);
    // static bool IteratorValid(Iterator& it, T[const]& obj);
    // static void Visit(Iterator& it, T1& obj, T[const]Lambda&& lambda);
};

template <typename TP, typename T> struct VisitorWithParent : Visitor<T>
{
    VisitorWithParent() = default;
    CLASS_DELETE_COPY_AND_MOVE(VisitorWithParent);
    // auto Parent() const { return _parent; }
};

template <typename T> struct VisitorT
{};

template <typename TP, typename T> struct VisitorWithParentT
{};

template <typename T> struct StructFieldEnumVisitor
{};

}    // namespace Stencil

template <typename T> struct Stencil::Visitor<Stencil::TimestampedT<T>>
{};

template <typename T> struct Stencil::Visitor<UuidBasedId<T>>
{};

template <Stencil::ConceptIterable T> struct Stencil::VisitorForIterable<std::shared_ptr<T>>
{
    using Iterator = Stencil::Visitor<T>::Iterator;
    using ThisType = std::shared_ptr<T>;

    template <typename T1>
    requires std::is_same_v<std::remove_const_t<T1>, ThisType>
    static void IteratorBegin(Iterator& it, T1& obj) { Stencil::Visitor<T>::IteratorBegin(it, *obj.get()); }
    template <typename T1>
    requires std::is_same_v<std::remove_const_t<T1>, ThisType>
    static void IteratorMoveNext(Iterator& it, T1& obj) { Stencil::Visitor<T>::IteratorMoveNext(it, *obj.get()); }
    template <typename T1>
    requires std::is_same_v<std::remove_const_t<T1>, ThisType>
    static bool IteratorValid(Iterator& it, T1& obj) { return Stencil::Visitor<T>::IteratorValid(it, *obj.get()); }

    template <typename T1, typename TLambda>
    requires std::is_same_v<std::remove_const_t<T1>, ThisType>
    static void Visit(Iterator& it, T1& obj, TLambda&& lambda)
    {
        Stencil::Visitor<T>::Visit(it, *obj.get(), std::forward<TLambda>(lambda));
    }
};

template <Stencil::ConceptIndexable T> struct Stencil::VisitorForIndexable<std::shared_ptr<T>>
{
    using ThisType = std::shared_ptr<T>;
};

template <typename T>
struct Stencil::Visitor<std::shared_ptr<T>> : Stencil::VisitorT<std::shared_ptr<T>>,
                                              Stencil::VisitorForIterable<std::shared_ptr<T>>,
                                              Stencil::VisitorForIndexable<std::shared_ptr<T>>
{
    using Key = Stencil::TypeTraitsForIndexable<T>::Key;

    using ThisType = std::shared_ptr<T>;
    // So that this works for both const and non-const
    template <typename T1, typename TKey, typename TLambda>
    requires std::is_same_v<std::remove_const_t<T1>, ThisType>
    static void VisitKey(T1& obj, TKey&& key, TLambda&& lambda)
    {
        if (obj.get() == nullptr)
        {
            if constexpr (!std::is_const_v<T1>)
            {
                // TODO: Should it really auto-create on demand
                if (obj.get() == nullptr) { obj = std::make_shared<T>(); }
            }
            else
            {
                return;
            }
        }

        Stencil::Visitor<T>::VisitKey(*obj.get(), std::forward<TKey>(key), std::forward<TLambda>(lambda));
    }

    template <typename T1, typename TLambda>
    requires std::is_same_v<std::remove_const_t<T1>, ThisType>
    static void VisitAllIndicies(T1& obj, TLambda&& lambda)
    {
        if (obj.get() == nullptr)
        {
            if constexpr (!std::is_const_v<T1>)
            {
                // TODO: Should it really auto-create on demand
                if (obj.get() == nullptr) { obj = std::make_shared<T>(); }
            }
            else
            {
                return;
            }
        }

        Stencil::Visitor<T>::VisitAllIndicies(*obj.get(), std::forward<TLambda>(lambda));
    }
};

template <typename T, size_t N> struct Stencil::Visitor<std::array<T, N>> : Stencil::VisitorT<std::array<T, N>>
{
    // So that this works for both const and non-const
    template <typename T1, typename TLambda>
    requires std::is_same_v<std::remove_const_t<T1>, std::array<T, N>>
    static void VisitKey(T1& obj, size_t index, TLambda&& lambda) { lambda(obj.at(index)); }

    template <typename T1, typename TLambda>
    requires std::is_same_v<std::remove_const_t<T1>, std::array<T, N>>
    static void VisitAllIndicies(T1& obj, TLambda&& lambda)
    {
        for (size_t i = 0; i < N; i++) { lambda(i, obj.at(i)); }
    }
    using Iterator = size_t;

    template <typename T1>
    requires std::is_same_v<std::remove_const_t<T1>, std::array<T, N>>
    static void IteratorBegin(Iterator& it, T1&) { it = Iterator{}; }
    template <typename T1>
    requires std::is_same_v<std::remove_const_t<T1>, std::array<T, N>>
    static void IteratorMoveNext(Iterator& it, T1&) { ++it; }
    template <typename T1>
    requires std::is_same_v<std::remove_const_t<T1>, std::array<T, N>>
    static bool IteratorValid(Iterator& it, T1&) { return it <= N; }

    template <typename T1, typename TLambda>
    requires std::is_same_v<std::remove_const_t<T1>, std::array<T, N>>
    static void Visit(Iterator& it, T1& obj, TLambda&& lambda) { lambda(obj.at(it)); }
};

template <typename T> struct Stencil::Visitor<std::vector<T>> : Stencil::VisitorT<std::vector<T>>
{
    // So that this works for both const and non-const
    template <typename T1, typename TLambda>
    requires std::is_same_v<std::remove_const_t<T1>, std::vector<T>>
    static void VisitKey(T1& obj, size_t index, TLambda&& lambda) { lambda(obj.at(index)); }

    template <typename T1, typename TLambda>
    requires std::is_same_v<std::remove_const_t<T1>, std::vector<T>>
    static void VisitAllIndicies(T1& obj, TLambda&& lambda)
    {
        for (size_t i = 0; i < obj.size(); i++) { lambda(i, obj.at(i)); }
    }
    using Iterator = size_t;

    template <typename T1>
    requires std::is_same_v<std::remove_const_t<T1>, std::vector<T>>
    static void IteratorBegin(Iterator& it, T1&) { it = Iterator{}; }
    template <typename T1>
    requires std::is_same_v<std::remove_const_t<T1>, std::vector<T>>
    static void IteratorMoveNext(Iterator& it, T1&) { ++it; }
    template <typename T1>
    requires std::is_same_v<std::remove_const_t<T1>, std::vector<T>>
    static bool IteratorValid(Iterator& it, T1& obj) { return it <= obj.size(); }

    template <typename T1, typename TLambda>
    requires std::is_same_v<std::remove_const_t<T1>, std::vector<T>>
    static void Visit(Iterator& it, T1& obj, TLambda&& lambda)
    {
        if (obj.size() == it) { obj.resize(it + 1); }
        lambda(obj.at(it));
    }
};

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