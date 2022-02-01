#pragma once
#include "fixedwidthvalue.h"
#include "visitor.h"
namespace Stencil
{
namespace impl
{
struct TypeHandler;

struct TypeHandlerAndPtr
{
    TypeHandler const* handler{nullptr};
    void*              ptr{nullptr};
};

// Limits choice of primitives but helps with SAX parsers
struct TypeHandler
{
    virtual ~TypeHandler() = default;

    virtual TypeHandlerAndPtr VisitNext(void* ptr) const        = 0;
    virtual TypeHandlerAndPtr VisitKey(void* ptr) const         = 0;
    virtual TypeHandlerAndPtr VisitValueForKey(void* ptr) const = 0;

    virtual void Assign(void* ptr, Value const& val) const             = 0;
    virtual void Assign(void* ptr, std::string_view const& val) const  = 0;
    virtual void Assign(void* ptr, std::wstring_view const& val) const = 0;
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

template <typename T> struct VisitorTypeHandler;

template <typename TTup> struct VisitorTypeHandlerPack;

template <> struct VisitorTypeHandlerPack<std::tuple<>>
{
    std::tuple<> handlers;
};

template <typename... Ts> struct VisitorTypeHandlerPack<std::tuple<Ts...>>
{
    std::tuple<VisitorTypeHandler<Ts>...> handlers;
};

template <typename T> struct IterableVisitorTypeHandler
{
    template <typename T> void Add(T& /*obj*/) const
    {
        throw std::logic_error("Add Not supported on non-iterable types");
    }    // namespace impl
    TypeHandlerAndPtr ElementHandler() const { throw std::logic_error("Not an iterable type"); }
};

template <typename T> struct PrimitiveVisitorTypeHandler
{
    template <typename T2> void Assign(T& /*obj*/, T2& /*obj*/) const { throw std::logic_error("Add Not supported on primitive types"); }
};

template <ConceptPrimitiveOnly T> struct PrimitiveVisitorTypeHandler<T>
{
    template <typename T2> void Assign(T& obj, T2 const& val) const { Stencil::Assign<T, T2>{}(obj, val); }
};

template <ConceptIterableNotIndexable T> struct IterableVisitorTypeHandler<T>
{
    template <typename T> void Add(T& /*obj*/) const { TODO(""); }
};

template <typename T> struct IndexableVisitorTypeHandler
{
    TypeHandlerAndPtr                       KeyHandler() const { throw std::logic_error("Not an indexable type"); }
    template <typename T> TypeHandlerAndPtr VisitValueForKey(T& /*obj*/) const { throw std::logic_error("Not an indexable type"); }
};

template <ConceptIndexable T> struct IndexableVisitorTypeHandler<T>
{
    using Traits = typename Stencil::TypeTraits<T>;

    TypeHandlerAndPtr                       KeyHandler() const { return TypeHandlerAndPtr{&_keyhandler, &_key}; }
    template <typename T> TypeHandlerAndPtr VisitValueForKey(T& obj) const
    {
        TypeHandler const* handler = nullptr;
        void*              ptr     = nullptr;
        Traits::VisitKey(obj, std::move(_key), [&](auto& val) {
            typeid(int).hash_code()
            handler = &std::get<VisitorTypeHandler<std::remove_reference_t<decltype(val)>>>(_handlers.handlers);
            ptr     = &val;
        });
        return {handler, ptr};
    }

    mutable Traits::Key                                 _key;
    VisitorTypeHandler<typename Traits::Key>            _keyhandler;
    // TODO: This is causing me 
    //VisitorTypeHandlerPack<typename Traits::ValueTypes> _handlers;
};

template <typename T>
struct VisitorTypeHandler : TypeHandler
{
    static_assert(std::is_default_constructible_v<Stencil::TypeTraits<T>>, "Specialization of TypeTraits required");

    void Add(T& obj) { TODO(""); }

    virtual TypeHandlerAndPtr VisitNext(void* ptr) const override
    {
        T& obj = *reinterpret_cast<T*>(ptr);
        _iterable.Add(obj);
        return _iterable.ElementHandler();
    }

    virtual TypeHandlerAndPtr VisitKey(void* /*ptr*/) const override
    {    // T& obj = *reinterpret_cast<T*>(ptr);
        return _indexable.KeyHandler();
    }
    virtual TypeHandlerAndPtr VisitValueForKey(void* ptr) const override
    {
        T& obj = *reinterpret_cast<T*>(ptr);
        return _indexable.VisitValueForKey(obj);
    }
    virtual void Assign(void* ptr, Value const& val) const override { _primitive.Assign(*reinterpret_cast<T*>(ptr), val); }
    virtual void Assign(void* ptr, std::string_view const& val) const override { _primitive.Assign(*reinterpret_cast<T*>(ptr), val); }
    virtual void Assign(void* ptr, std::wstring_view const& val) const override { _primitive.Assign(*reinterpret_cast<T*>(ptr), val); }

    PrimitiveVisitorTypeHandler<T> _primitive;
    IterableVisitorTypeHandler<T>  _iterable;
    IndexableVisitorTypeHandler<T> _indexable;
};

template <typename T> struct _StackVisitor
{

    _StackVisitor() = default;

    void Start(T& obj)
    {
        _rootObj = &obj;
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

    bool                           _visitingKey = false;
    std::vector<TypeHandlerAndPtr> _stack;
    VisitorTypeHandler<T>          _handler;
    T*                             _rootObj{nullptr};
};
}    // namespace impl

template <typename T> using StackVisitor = impl::_StackVisitor<T>;

}    // namespace Stencil