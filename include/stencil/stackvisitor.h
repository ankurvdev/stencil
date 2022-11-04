#pragma once
#include "primitives64bit.h"
#include "visitor.h"

#include <unordered_map>

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

    virtual void Assign(void* ptr, Primitives64Bit const& val) = 0;
    // virtual void Assign(void* ptr, std::span<std::byte const> const& val) = 0;
    virtual void Assign(void* ptr, std::string_view const& val)  = 0;
    virtual void Assign(void* ptr, std::wstring_view const& val) = 0;
};

inline auto VisitKey(TypeHandlerAndPtr const& item)
{
    return item.handler->VisitKey(item.ptr);
}

inline auto VisitValueForKey(TypeHandlerAndPtr const& item)
{
    return item.handler->VisitValueForKey(item.ptr);
}

inline auto VisitNext(TypeHandlerAndPtr const& item)
{
    return item.handler->VisitNext(item.ptr);
}

template <typename TProto, typename T> struct ProtocolHelper;

template <typename TProto, ConceptPrimitives64Bit TVal> struct ProtocolHelper<TProto, TVal>
{
    void operator()(TypeHandlerAndPtr const& item, TVal const& val) { return item.handler->Assign(item.ptr, Primitives64Bit(val)); }
};

template <typename TProto, typename T> struct ProtocolHelper<TProto, std::span<T const>>
{
    void operator()(TypeHandlerAndPtr const& item, std::span<T const> const& val)
    {
        return item.handler->Assign(item.ptr, std::as_bytes(val));
    }
};

template <typename TProto, typename T> struct ProtocolHelper<TProto, std::basic_string_view<T>>
{
    void operator()(TypeHandlerAndPtr const& item, std::basic_string_view<T> const& val) { return item.handler->Assign(item.ptr, val); }
};

template <typename TProto, typename TOwner, typename T> struct VisitorTypeHandler;

template <typename TProto, typename TOwner, typename T> struct IterableVisitorTypeHandler
{
    template <typename T1> TypeHandlerAndPtr VisitNext(T1& /*obj*/) const { throw std::logic_error("Not an iterable type"); }

    TOwner* owner;
};

template <typename TProto, typename TOwner, typename T> struct PrimitiveVisitorTypeHandler
{
    template <typename T2> void Assign(T& /*obj*/, T2& /*obj*/) const { throw std::logic_error("Add Not supported on primitive types"); }
    TOwner*                     owner;
};

template <typename TProto, typename TOwner, ConceptPrimitive T> struct PrimitiveVisitorTypeHandler<TProto, TOwner, T>
{
    template <typename T2> void Assign(T& obj, T2 const& val) const { SerDes<T, TProto>::Read(obj, val); }
    // void Assign(T& obj, T2 const& val) const { SerDes<T, TProto>::Read(obj, val); }
    //  void Assign(T& obj, T2 const& val) const { SerDes<T, TProto>::Read(obj, val); }
    //  void Assign(T& obj, T2 const& val) const { SerDes<T, TProto>::Read(obj, val); }

    TOwner* owner;
};

template <typename TProto, typename TOwner, ConceptPreferIterable T> struct IterableVisitorTypeHandler<TProto, TOwner, T>
{
    template <typename T1> TypeHandlerAndPtr VisitNext(T1& obj)
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
            using VisitorHandler = VisitorTypeHandler<TProto, TOwner, std::remove_reference_t<decltype(val)>>;
            handler              = owner->template FindOrCreateHandler<VisitorHandler>();
            ptr                  = &val;
        });

        return {handler, ptr};
    }

    bool                          valid = false;
    typename Visitor<T>::Iterator it;
    TOwner*                       owner;
};

template <typename TProto, typename TOwner, typename T> struct IndexableVisitorTypeHandler
{
    TypeHandlerAndPtr                        KeyHandler() const { throw std::logic_error("Not an indexable type"); }
    template <typename T1> TypeHandlerAndPtr VisitValueForKey(T1& /*obj*/) const { throw std::logic_error("Not an indexable type"); }
    TOwner*                                  owner;
};

template <typename TProto, typename TOwner, ConceptIndexable T> struct IndexableVisitorTypeHandler<TProto, TOwner, T>
{
    using Traits = typename Stencil::TypeTraitsForIndexable<T>;

    TypeHandlerAndPtr KeyHandler() { return TypeHandlerAndPtr{&_keyhandler, &_key}; }

    template <typename T1> TypeHandlerAndPtr VisitValueForKey(T1& obj) const
    {
        TypeHandler* handler = nullptr;
        void*        ptr     = nullptr;
        Visitor<T>::VisitKey(obj, std::move(_key), [&](auto& val) {
            using VisitorHandler = VisitorTypeHandler<TProto, TOwner, std::remove_reference_t<decltype(val)>>;
            handler              = owner->template FindOrCreateHandler<VisitorHandler>();
            ptr                  = &val;
        });
        return {handler, ptr};
    }

    TOwner* owner;

    typename Traits::Key _key{};

    VisitorTypeHandler<TProto, TOwner, typename Traits::Key> _keyhandler;

    // TODO: This is causing me
    // VisitorTypeHandlerPack<typename Traits::ValueTypes> _handlers;
};

template <typename TProto, typename TOwner, typename T> struct VisitorTypeHandler : TypeHandler
{
    static_assert(std::is_default_constructible_v<Stencil::TypeTraits<T>>, "Specialization of TypeTraits required");

    void Add(T& /* obj */) { TODO(""); }

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

    virtual void Assign(void* ptr, Primitives64Bit const& val) override
    {
        if constexpr (ConceptPrimitives64Bit<T>) { *reinterpret_cast<T*>(ptr) = val.cast<T>(); }
        else
        {
            TODO("");
        }
    }
    virtual void Assign(void* ptr, std::string_view const& val) override { primitive.Assign(*reinterpret_cast<T*>(ptr), val); }
    virtual void Assign(void* /*ptr*/, std::wstring_view const& /*val*/) override
    {
        TODO("primitive.Assign(*reinterpret_cast<T*>(ptr), val);");
    }

    TOwner*                                        owner;
    PrimitiveVisitorTypeHandler<TProto, TOwner, T> primitive;
    IterableVisitorTypeHandler<TProto, TOwner, T>  iterable;
    IndexableVisitorTypeHandler<TProto, TOwner, T> indexable;
};

template <typename TProto, typename T> struct _StackVisitor
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

    void AddKey() { _stack.push_back(VisitKey(_stack.back())); }

    void AddValue()
    {
        _stack.pop_back();
        _stack.push_back(VisitValueForKey(_stack.back()));
    }

    template <typename TVal> void Assign(TVal const& k) { ProtocolHelper<TProto, TVal>{}(_stack.back(), k); }

    void Pop() { _stack.pop_back(); }

    void Add() { _stack.push_back(VisitNext(_stack.back())); }

    // TODO : Yuck! Revisit and try to convince PROP1
    template <typename T1> TypeHandler* FindOrCreateHandler()
    {
        auto hashcode = typeid(T1).hash_code();
        auto it       = _allhandlers.find(hashcode);
        if (it == _allhandlers.end())
        {
            auto uptr             = std::make_unique<T1>();
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

    // TODO1: Get rid of TypeHandler maps
    std::unordered_map<size_t, std::unique_ptr<TypeHandler>> _allhandlers;
    std::vector<TypeHandlerAndPtr>                           _stack;
    VisitorTypeHandler<TProto, _StackVisitor<TProto, T>, T>  _handler;
    T*                                                       _rootObj{nullptr};
};
}    // namespace impl

template <typename TProto, typename T> using StackVisitor = impl::_StackVisitor<TProto, T>;

}    // namespace Stencil
