#pragma once
#include "typetraits.h"
#include "visitor.h"

#include <compare>

namespace Stencil
{
template <typename T> struct Ref
{
    uint32_t objId{0};

    bool           Valid() const { return objId != 0; }
    constexpr auto operator<=>(Ref<T> const& rhs) const = default;

    static Ref<T> Invalid() { return Ref<T>{}; }
};

template <typename T> struct RefMap
{};

}    // namespace Stencil

template <typename T> struct Stencil::TypeTraits<Stencil::Ref<T>>
{
    using Categories = std::tuple<Stencil::Category::Primitive>;
};

template <Stencil::ConceptIndexable T> struct Stencil::TypeTraitsForIndexable<Stencil::Ref<T>>
{
    using Key = typename Stencil::TypeTraitsForIndexable<T>::Key;
};

template <Stencil::ConceptIterable T> struct Stencil::TypeTraitsForIterable<Stencil::Ref<T>>
{};

template <Stencil::ConceptPrimitive T> struct Stencil::TypeTraitsForPrimitive<Stencil::Ref<T>>
{};

template <Stencil::ConceptIterable T> struct Stencil::VisitorForIterable<Stencil::Ref<T>>
{
    using Iterator = typename Stencil::Visitor<T>::Iterator;
    using ThisType = Stencil::Ref<T>;

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

template <Stencil::ConceptIndexable T> struct Stencil::VisitorForIndexable<Stencil::Ref<T>>
{
    using ThisType = Stencil::Ref<T>;
};

#if 0
template <typename T>
struct Stencil::Visitor<Stencil::Ref<T>>
    : Stencil::VisitorT<Stencil::Ref<T>>, Stencil::VisitorForIterable<Stencil::Ref<T>>, Stencil::VisitorForIndexable<Stencil::Ref<T>>
{
    using Key = typename Stencil::TypeTraitsForIndexable<T>::Key;

    using ThisType = Stencil::Ref<T>;
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
            else { return; }
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
            else { return; }
        }

        Stencil::Visitor<T>::VisitAllIndicies(*obj.get(), std::forward<TLambda>(lambda));
    }
};
#endif

template <typename T> struct Stencil::TypeTraits<Stencil::RefMap<T>>
{
    using Categories = typename Stencil::TypeTraits<T>::Categories;
};

template <Stencil::ConceptIndexable T> struct Stencil::TypeTraitsForIndexable<Stencil::RefMap<T>>
{
    using Key = uint32_t;
};

template <typename T> struct Stencil::Visitor<Stencil::RefMap<T>> : Stencil::VisitorT<Stencil::RefMap<T>>
{
    using Key = typename Stencil::TypeTraitsForIndexable<T>::Key;

    using ThisType = Stencil::RefMap<T>;
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
            else { return; }
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
            else { return; }
        }

        Stencil::Visitor<T>::VisitAllIndicies(*obj.get(), std::forward<TLambda>(lambda));
    }
};

template <typename T> struct Primitives64Bit::Traits<Stencil::Ref<T>>
{
    using _ThisType = Stencil::Ref<T>;
    static constexpr auto Type() { return Primitives64Bit::Type::Unsigned(4); }
    static void           Assign(Primitives64Bit& obj, _ThisType& val) { obj._uVal = Repr(val); }
    static auto           Get(Primitives64Bit const& obj) { return _ThisType(static_cast<uint32_t>(obj._uVal)); }
    static auto           Convert(uint64_t val) { return _ThisType(static_cast<uint32_t>(val)); }
    static uint64_t       Repr(_ThisType const& val) { return val.objId; }
};
