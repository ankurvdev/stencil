#pragma once
#include "fixedwidthvalue.h"
#include "todo.h"
#include "visitor.h"

namespace Stencil
{
namespace impl
{
struct TypeHandler;

struct TypeHandlerAndPtr
{
    TypeHandler* handler{nullptr};
    void*        ptr{nullptr};
};

// Limits choice of primitives but helps with SAX parsers
struct TypeHandler
{
    virtual ~TypeHandler() = default;

    virtual TypeHandlerAndPtr VisitNext(void* ptr)        = 0;
    virtual TypeHandlerAndPtr VisitKey(void* ptr)         = 0;
    virtual TypeHandlerAndPtr VisitValueForKey(void* ptr) = 0;

    virtual void Assign(void* ptr, Value const& val)             = 0;
    virtual void Assign(void* ptr, std::string_view const& val)  = 0;
    virtual void Assign(void* ptr, std::wstring_view const& val) = 0;
};

auto VisitKey(TypeHandlerAndPtr const& item)
{
    return item.handler->VisitKey(item.ptr);
}

auto VisitValueForKey(TypeHandlerAndPtr const& item)
{
    return item.handler->VisitValueForKey(item.ptr);
}

auto VisitNext(TypeHandlerAndPtr const& item)
{
    return item.handler->VisitNext(item.ptr);
}
template <typename T> struct AssignHelper;

template <ConceptValue TVal> struct AssignHelper<TVal>
{
    void operator()(TypeHandlerAndPtr const& item, TVal const& val) { return item.handler->Assign(item.ptr, Value(val)); }
};

template <typename T> struct AssignHelper<std::span<T const>>
{
    void operator()(TypeHandlerAndPtr const& item, std::span<T const> const& val)
    {
        return item.handler->Assign(item.ptr, std::as_bytes(val));
    }
};

template <typename T> struct AssignHelper<std::basic_string_view<T>>
{
    void operator()(TypeHandlerAndPtr const& item, std::basic_string_view<T> const& val) { return item.handler->Assign(item.ptr, val); }
};

template <typename TOwner, typename T> struct VisitorTypeHandler;
#if 0
template <typename TTup> struct VisitorTypeHandlerPack;

template <> struct VisitorTypeHandlerPack<std::tuple<>>
{
    std::tuple<> handlers;
};

template <typename TOWner, typename... Ts> struct VisitorTypeHandlerPack<std::tuple<Ts...>>
{
    std::tuple<VisitorTypeHandler<Ts>...> handlers;
};
#endif
template <typename TOwner, typename T> struct IterableVisitorTypeHandler
{
    template <typename T> TypeHandlerAndPtr VisitNext(T& /*obj*/) const { throw std::logic_error("Not an iterable type"); }

    TOwner* owner;
};

template <typename TOwner, typename T> struct PrimitiveVisitorTypeHandler
{
    template <typename T2> void Assign(T& /*obj*/, T2& /*obj*/) const { throw std::logic_error("Add Not supported on primitive types"); }
    TOwner*                     owner;
};

template <typename TOwner, ConceptPrimitive T> struct PrimitiveVisitorTypeHandler<TOwner, T>
{
    template <typename T2> void Assign(T& obj, T2 const& val) const { Stencil::Assign<T, T2>{}(obj, val); }

    TOwner* owner;
};

template <typename TOwner, ConceptIterableNotIndexable T> struct IterableVisitorTypeHandler<TOwner, T>
{
    template <typename T> TypeHandlerAndPtr VisitNext(T& obj)
    {
        if (!valid)
        {
            Visitor<T>::IteratorBegin(it, obj);
            valid = true;
        }
        else
        {
            Visitor<T>::IteratorMoveNext(it, obj);
        }

        if (!Visitor<T>::IteratorValid(it, obj)) throw std::runtime_error("Cannot Visit Next Item on the iterable");

        TypeHandler* handler = nullptr;
        void*        ptr     = nullptr;

        Visitor<T>::Visit(it, obj, [&](auto& val) {
            using VisitorHandler = VisitorTypeHandler<TOwner, std::remove_reference_t<decltype(val)>>;
            handler              = owner->template FindOrCreateHandler<VisitorHandler>();
            ptr                  = &val;
        });

        return {handler, ptr};
    }

    bool                 valid = false;
    Visitor<T>::Iterator it;
    TOwner*              owner;
};

template <typename TOwner, typename T> struct IndexableVisitorTypeHandler
{
    TypeHandlerAndPtr                       KeyHandler() const { throw std::logic_error("Not an indexable type"); }
    template <typename T> TypeHandlerAndPtr VisitValueForKey(T& /*obj*/) const { throw std::logic_error("Not an indexable type"); }
    TOwner*                                 owner;
};

template <typename TOwner, ConceptIndexable T> struct IndexableVisitorTypeHandler<TOwner, T>
{
    using Traits = typename Stencil::TypeTraitsForIndexable<T>;

    TypeHandlerAndPtr KeyHandler() { return TypeHandlerAndPtr{&_keyhandler, &_key}; }

    template <typename T> TypeHandlerAndPtr VisitValueForKey(T& obj) const
    {
        TypeHandler* handler = nullptr;
        void*        ptr     = nullptr;
        Visitor<T>::VisitKey(obj, std::move(_key), [&](auto& val) {
            using VisitorHandler = VisitorTypeHandler<TOwner, std::remove_reference_t<decltype(val)>>;
            handler              = owner->template FindOrCreateHandler<VisitorHandler>();
            ptr                  = &val;
        });
        return {handler, ptr};
    }
    TOwner* owner;

    Traits::Key _key;

    VisitorTypeHandler<TOwner, typename Traits::Key> _keyhandler;

    // TODO: This is causing me
    // VisitorTypeHandlerPack<typename Traits::ValueTypes> _handlers;
};

template <typename TOwner, typename T> struct VisitorTypeHandler : TypeHandler
{
    static_assert(std::is_default_constructible_v<Stencil::TypeTraits<T>>, "Specialization of TypeTraits required");

    void Add(T& obj) { TODO(""); }

    virtual TypeHandlerAndPtr VisitNext(void* ptr) override
    {
        T& obj = *reinterpret_cast<T*>(ptr);
        return iterable.VisitNext(obj);
    }

    virtual TypeHandlerAndPtr VisitKey(void* /*ptr*/) override
    {    // T& obj = *reinterpret_cast<T*>(ptr);
        return indexable.KeyHandler();
    }

    virtual TypeHandlerAndPtr VisitValueForKey(void* ptr) override
    {
        T& obj = *reinterpret_cast<T*>(ptr);
        return indexable.VisitValueForKey(obj);
    }

    virtual void Assign(void* ptr, Value const& val) override { primitive.Assign(*reinterpret_cast<T*>(ptr), val); }
    virtual void Assign(void* ptr, std::string_view const& val) override { primitive.Assign(*reinterpret_cast<T*>(ptr), val); }
    virtual void Assign(void* ptr, std::wstring_view const& val) override { primitive.Assign(*reinterpret_cast<T*>(ptr), val); }

    TOwner*                                owner;
    PrimitiveVisitorTypeHandler<TOwner, T> primitive;
    IterableVisitorTypeHandler<TOwner, T>  iterable;
    IndexableVisitorTypeHandler<TOwner, T> indexable;
};

template <typename T> struct _StackVisitor
{

    _StackVisitor() = default;

    void Start(T& obj)
    {
        _rootObj                 = &obj;
        _handler.owner           = this;
        _handler.primitive.owner = this;
        _handler.iterable.owner  = this;
        _handler.indexable.owner = this;

        _stack.push_back({&_handler, _rootObj});
    }

    void AddKey()
    {
        _stack.push_back(VisitKey(_stack.back()));
        _visitingKey = true;
    }

    template <typename TVal> void Assign(TVal const& k) { AssignHelper<TVal>{}(_stack.back(), k); }

    void Pop()
    {
        _stack.pop_back();
        if (_visitingKey) { _stack.push_back(VisitValueForKey(_stack.back())); }
        _visitingKey = false;
    }

    void Add() { _stack.push_back(VisitNext(_stack.back())); }

    // TODO : Yuck! Revisit and try to convince PROP1
    template <typename T> TypeHandler* FindOrCreateHandler()
    {
        auto hashcode = typeid(T).hash_code();
        auto it       = _allhandlers.find(hashcode);
        if (it == _allhandlers.end())
        {
            auto uptr             = std::make_unique<T>();
            uptr->owner           = this;
            uptr->primitive.owner = this;
            uptr->iterable.owner  = this;
            uptr->indexable.owner = this;
            auto ptr              = uptr.get();
            _allhandlers.insert(std::make_pair(hashcode, std::move(uptr)));
            return ptr;
        }
        return it->second.get();
    }

    bool                                                     _visitingKey = false;
    std::unordered_map<size_t, std::unique_ptr<TypeHandler>> _allhandlers;
    std::vector<TypeHandlerAndPtr>                           _stack;
    VisitorTypeHandler<_StackVisitor<T>, T>                  _handler;
    T*                                                       _rootObj{nullptr};
};
}    // namespace impl

template <typename T> using StackVisitor = impl::_StackVisitor<T>;

}    // namespace Stencil