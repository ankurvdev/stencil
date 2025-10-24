#pragma once
#include "primitives64bit.h"
#include "protocol_string.h"
#include "visitor.h"

#include <string>
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

template <ConceptProtocol TProto, typename T> struct ProtocolHelper;

template <ConceptProtocol TProto, ConceptPrimitives64Bit TVal> struct ProtocolHelper<TProto, TVal>
{
    void operator()(TypeHandlerAndPtr const& item, TVal const& val) { return item.handler->Assign(item.ptr, Primitives64Bit(val)); }
};

template <ConceptProtocol TProto, typename T> struct ProtocolHelper<TProto, std::span<T const>>
{
    void operator()(TypeHandlerAndPtr const& item, std::span<T const> const& val)
    {
        return item.handler->Assign(item.ptr, std::as_bytes(val));
    }
};

template <ConceptProtocol TProto, typename T> struct ProtocolHelper<TProto, std::basic_string_view<T>>
{
    void operator()(TypeHandlerAndPtr const& item, std::basic_string_view<T> const& val) { return item.handler->Assign(item.ptr, val); }
};

template <ConceptProtocol TProto, typename TOwner, typename T> struct VisitorTypeHandler;

template <ConceptProtocol TProto, typename TOwner, typename T> struct IterableVisitorTypeHandler
{
    template <typename T1> [[noreturn]] TypeHandlerAndPtr VisitNext(T1& /*obj*/) const { throw std::logic_error("Not an iterable type"); }

    TOwner* owner;
};

template <ConceptProtocol TProto, typename TOwner, typename T> struct PrimitiveVisitorTypeHandler
{
    template <typename T2> [[noreturn]] void Assign(T& /*obj*/, T2& /*obj*/) const
    {
        throw std::logic_error("Add Not supported on primitive types");
    }
    TOwner* owner;
};

template <ConceptProtocol TProto, typename TOwner, ConceptPrimitive T> struct PrimitiveVisitorTypeHandler<TProto, TOwner, T>
{
    template <typename T2> void Assign(T& obj, T2 const& val) const { SerDes<T, TProto>::Read(obj, val); }
    // void Assign(T& obj, T2 const& val) const { SerDes<T, TProto>::Read(obj, val); }
    //  void Assign(T& obj, T2 const& val) const { SerDes<T, TProto>::Read(obj, val); }
    //  void Assign(T& obj, T2 const& val) const { SerDes<T, TProto>::Read(obj, val); }

    TOwner* owner;
};

template <ConceptProtocol TProto, typename TOwner, ConceptIterable T> struct IterableVisitorTypeHandler<TProto, TOwner, T>
{
    template <typename T1> TypeHandlerAndPtr VisitNext(T1& obj)
    {
        if (!valid)
        {
            Visitor<T>::IteratorBegin(it, obj);
            valid = true;
        }
        else { Visitor<T>::IteratorMoveNext(it, obj); }

        if (!Visitor<T>::IteratorValid(it, obj)) throw std::runtime_error("Cannot Visit Next Item on the iterable");

        TypeHandler* handler = nullptr;
        void*        ptr     = nullptr;

        Visitor<T>::Visit(it, obj, [&](auto& val) {
            using VisitorHandler = VisitorTypeHandler<TProto, TOwner, std::remove_reference_t<decltype(val)>>;
            ptr                  = &val;
            handler              = owner->template FindOrCreateHandler<VisitorHandler>(ptr);
        });

        return {handler, ptr};
    }

    bool                          valid = false;
    typename Visitor<T>::Iterator it;
    TOwner*                       owner;
};

template <ConceptProtocol TProto, typename TOwner, typename T> struct IndexableVisitorTypeHandler
{
    [[noreturn]] TypeHandlerAndPtr                        KeyHandler() const { throw std::logic_error("Not an indexable type"); }
    template <typename T1> [[noreturn]] TypeHandlerAndPtr VisitValueForKey(T1& /*obj*/) const
    {
        throw std::logic_error("Not an indexable type");
    }
    TOwner* owner;
};

template <ConceptProtocol TProto, typename TOwner, ConceptVariant T> struct IndexableVisitorTypeHandler<TProto, TOwner, T>
{
    using Traits = typename Stencil::TypeTraitsForIndexable<T>;

    struct VariantKeyTypeHandler : TypeHandler
    {
        [[noreturn]] TypeHandlerAndPtr VisitNext(void* /* ptr */) override { TODO(""); }
        [[noreturn]] TypeHandlerAndPtr VisitKey(void* /* ptr */) override { TODO(""); }
        [[noreturn]] TypeHandlerAndPtr VisitValueForKey(void* /* ptr */) override { TODO(""); }

        void Assign(void* /* ptr */, Primitives64Bit const& val) override { _variant = val; }
        void Assign(void* /* ptr */, std::string_view const& val) override { _variant = std::string{val}; }
        void Assign(void* /* ptr */, std::wstring_view const& val) override { _variant = std::wstring{val}; }

        std::variant<std::string, std::wstring, Primitives64Bit> _variant{};
    };

    TypeHandlerAndPtr KeyHandler() { return TypeHandlerAndPtr{&_keyhandler, this}; }

    template <typename T1> TypeHandlerAndPtr VisitValueForKey(T1& obj) const
    {
        TypeHandler* handler = nullptr;
        void*        ptr     = nullptr;

        VisitorForVariant<T>::VisitAlternatives(obj, [&](auto const& k, auto& val) {
            std::stringstream ss;
            SerDes<std::remove_cvref_t<decltype(k)>, ProtocolString>::Write(ss, k);
            if (ss.str() != std::get<std::string>(_keyhandler._variant)) { return; }
            using VisitorHandler = VisitorTypeHandler<TProto, TOwner, std::remove_reference_t<decltype(val)>>;
            obj                  = std::move(val);
            VisitorForVariant<T>::VisitActiveAlternative(obj, [&](auto const& /* k */, auto& val1) { ptr = &val1; });
            handler = owner->template FindOrCreateHandler<VisitorHandler>(ptr);
        });
        return {handler, ptr};
    }

    VariantKeyTypeHandler _keyhandler;
    TOwner*               owner;
    // TODO: This is causing me
    // VisitorTypeHandlerPack<typename Traits::ValueTypes> _handlers;
};

SUPPRESS_WARNINGS_START
SUPPRESS_MSVC_WARNING(4702) /*Unreachable code*/
template <ConceptProtocol TProto, typename TOwner, ConceptIndexable T> struct IndexableVisitorTypeHandler<TProto, TOwner, T>
{
    using Traits = typename Stencil::TypeTraitsForIndexable<T>;

    TypeHandlerAndPtr KeyHandler() { return TypeHandlerAndPtr{&_keyhandler, &_key}; }

    template <typename T1> TypeHandlerAndPtr VisitValueForKey(T1& obj) const
    {
        TypeHandler* handler = nullptr;
        void*        ptr     = nullptr;
        Visitor<T>::VisitKey(obj, std::move(_key), [&](auto& val) {
            using VisitorHandler = VisitorTypeHandler<TProto, TOwner, std::remove_reference_t<decltype(val)>>;
            ptr                  = &val;
            handler              = owner->template FindOrCreateHandler<VisitorHandler>(ptr);
        });
        return {handler, ptr};
    }

    TOwner* owner;

    typename Traits::Key _key{};

    VisitorTypeHandler<TProto, TOwner, typename Traits::Key> _keyhandler;

    // TODO: This is causing me
    // VisitorTypeHandlerPack<typename Traits::ValueTypes> _handlers;
};
SUPPRESS_WARNINGS_END

template <ConceptProtocol TProto, typename TOwner, typename T> struct VisitorTypeHandler : TypeHandler
{
    static_assert(std::is_default_constructible_v<Stencil::TypeTraits<T>>, "Specialization of TypeTraits required");

    [[noreturn]] void Add(T& /* obj */) { TODO(""); }

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
        /* else if constexpr (ConceptTransactionForPrimitive<T>)
         {
             using ElemType = typename Stencil::TransactionTraits<T>::ElemType;
             (*reinterpret_cast<T*>(ptr)).Assign(val.cast<ElemType>());
         }*/
        else { TODO(""); }
    }
    virtual void      Assign(void* ptr, std::string_view const& val) override { primitive.Assign(*reinterpret_cast<T*>(ptr), val); }
    [[noreturn]] void Assign(void* /*ptr*/, std::wstring_view const& /*val*/) override
    {
        TODO("primitive.Assign(*reinterpret_cast<T*>(ptr), val);");
    }

    TOwner*                                        owner;
    PrimitiveVisitorTypeHandler<TProto, TOwner, T> primitive;
    IterableVisitorTypeHandler<TProto, TOwner, T>  iterable;
    IndexableVisitorTypeHandler<TProto, TOwner, T> indexable;
};

template <ConceptProtocol TProto, typename T> struct _StackVisitor
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
    template <typename T1> TypeHandler* FindOrCreateHandler(void* ptr)
    {
        // auto hashcode = typeid(T1).hash_code();
        auto it = _allhandlers.find(ptr);
        if (it == _allhandlers.end())
        {
            auto uptr             = std::make_unique<T1>();
            uptr->owner           = this;
            uptr->primitive.owner = this;
            uptr->iterable.owner  = this;
            uptr->indexable.owner = this;
            auto ptr              = uptr.get();
            _allhandlers.insert(std::make_pair(ptr, std::move(uptr)));
            return ptr;
        }
        return it->second.get();
    }

    std::unordered_map<void*, std::unique_ptr<TypeHandler>> _allhandlers;
    std::vector<TypeHandlerAndPtr>                          _stack;
    VisitorTypeHandler<TProto, _StackVisitor<TProto, T>, T> _handler;
    T*                                                      _rootObj{nullptr};
};
}    // namespace impl

template <ConceptProtocol TProto, typename T> using StackVisitor = impl::_StackVisitor<TProto, T>;

}    // namespace Stencil
