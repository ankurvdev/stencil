#pragma once
#include "stencil/tuple_utils.h"
#include "timestamped.h"
#include "typetraits.h"
#include "typetraits_std.h"
#include "uuidobject.h"

#include <memory>
#include <tuple>
#include <type_traits>
#include <variant>

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

template <typename T> struct VisitorForVariant
{};

template <typename T> struct VisitorT
{};

template <typename... Ts> struct Visitor<std::variant<Ts...>>
{

    template <size_t N, typename TObj, typename TLambda> static void _SetAndVisit(TObj& obj, size_t const& key, TLambda&& lambda)
    {
        if constexpr (N == sizeof...(Ts)) { throw std::runtime_error("Index out of bounds"); }
        else
        {
            if (N == key)
            {
                using Type = std::remove_cvref_t<decltype(std::get<N>(obj))>;
                obj        = Type{};
                lambda(std::get<N>(obj));
            }
            else { _SetAndVisit<N + 1>(obj, key, lambda); }
        }
    }
    template <typename T1, typename TLambda> static void VisitKey(T1& obj, size_t const& key, TLambda&& lambda)
    {
        if (obj.index() == key)
        {
            std::visit([&](auto&& arg) { lambda(arg); }, obj);
        }
        else { _SetAndVisit<0>(obj, key, lambda); }
    }
    template <typename T1, typename TLambda> static void VisitAll(T1& obj, TLambda&& lambda)
    {
        std::visit([&](auto&& arg) { lambda(obj.index(), arg); }, obj);
    }
};

template <typename T> struct StructFieldsVisitor;

template <typename T, typename... TAttrs> struct StructVisitor
{
    using Key = typename Stencil::TypeTraitsForIndexable<T>::Key;

    template <typename T1> using Fields = typename Stencil::TypeTraitsForIndexable<T1>::Fields;

    // template <typename T1, typename TLambda> static bool _VisitKeyIfVariantMatches(T1& obj, Key const& key, TLambda&& lambda)
    // {
    //    if (!std::holds_alternative<Fields<T1>>(key)) { return false; }
    //    return StructFieldsVisitor<T1>::VisitField(obj, std::get<Fields<T1>>(key), lambda);
    //}

    template <typename T1, typename TLambda> static void VisitKey(T1& /* obj */, Key const& /* key */, TLambda&& /* lambda */)
    {
        TODO("");
        //    bool found = (_VisitKeyIfVariantMatches<TAttrs>(obj, key, lambda) || ...) || _VisitKeyIfVariantMatches<T1>(obj, key, lambda);
        //   if (!found) throw std::runtime_error("Key did not match any of the struct fields or attributes");
    }

    template <typename TAttr, typename T1, typename TLambda> static bool _VisitAllFieldsHelper(T1& obj, TLambda&& lambda)
    {
        StructFieldsVisitor<TAttr>::VisitAllFields(obj, [&](auto&& key, auto&& val) { lambda(Key{key}, val); });
        return false;
    }
    template <typename T1, typename TLambda> static void VisitAll(T1& obj, TLambda&& lambda)
    {
        [[maybe_unused]] bool found = (_VisitAllFieldsHelper<TAttrs>(obj, lambda) || ...);
        StructFieldsVisitor<T>::VisitAllFields(obj, [&](auto&& key, auto&& val) { lambda(key, val); });
    }
};

}    // namespace Stencil

template <typename T> struct Stencil::StructFieldsVisitor<Stencil::TimestampedT<T>>
{
    using Fields = typename TypeTraitsForIndexable<Stencil::TimestampedT<T>>::Fields;
    template <typename T1, typename TLambda> static bool VisitField(T1& obj, Fields fields, TLambda&& lambda)
    {
        switch (fields)
        {
        case Fields::Field_timestamp: lambda(obj.lastmodified); return true;
        case Fields::Invalid: [[fallthrough]];
        default: return false;
        }
    }

    template <typename T1, typename TLambda> static void VisitAllFields(T1& obj, TLambda&& lambda)
    {
        lambda(Fields::Field_timestamp, obj.lastmodified);
    }
};

template <typename T> struct Stencil::StructFieldsVisitor<UuidBasedId<T>>
{
    using Fields = typename TypeTraitsForIndexable<UuidBasedId<T>>::Fields;
    template <typename T1, typename TLambda> static bool VisitField(T1& obj, Fields fields, TLambda&& lambda)
    {
        switch (fields)
        {
        case Fields::Field_uuid: lambda(obj.uuid); return true;
        case Fields::Invalid: [[fallthrough]];
        default: return false;
        }
    }

    template <typename T1, typename TLambda> static void VisitAllFields(T1& obj, TLambda&& lambda) { lambda(Fields::Field_uuid, obj.uuid); }
};

template <Stencil::ConceptIterable T> struct Stencil::VisitorForIterable<std::shared_ptr<T>>
{
    using Iterator = typename Stencil::Visitor<T>::Iterator;
    using ThisType = std::shared_ptr<T>;

    template <typename T1>
        requires std::is_same_v<std::remove_const_t<T1>, ThisType>
    static void IteratorBegin(Iterator& it, T1& obj)
    {
        Stencil::Visitor<T>::IteratorBegin(it, *obj.get());
    }
    template <typename T1>
        requires std::is_same_v<std::remove_const_t<T1>, ThisType>
    static void IteratorMoveNext(Iterator& it, T1& obj)
    {
        Stencil::Visitor<T>::IteratorMoveNext(it, *obj.get());
    }
    template <typename T1>
        requires std::is_same_v<std::remove_const_t<T1>, ThisType>
    static bool IteratorValid(Iterator& it, T1& obj)
    {
        return Stencil::Visitor<T>::IteratorValid(it, *obj.get());
    }

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
    using Key = typename Stencil::TypeTraitsForIndexable<T>::Key;

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
            else { return; }
        }

        Stencil::Visitor<T>::VisitKey(*obj.get(), std::forward<TKey>(key), std::forward<TLambda>(lambda));
    }

    template <typename T1, typename TLambda>
        requires std::is_same_v<std::remove_const_t<T1>, ThisType>
    static void VisitAll(T1& obj, TLambda&& lambda)
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

        Stencil::Visitor<T>::VisitAll(*obj.get(), std::forward<TLambda>(lambda));
    }
};

template <typename T, size_t N> struct Stencil::Visitor<std::array<T, N>> : Stencil::VisitorT<std::array<T, N>>
{
    // So that this works for both const and non-const
    template <typename T1, typename TLambda>
        requires std::is_same_v<std::remove_const_t<T1>, std::array<T, N>>
    static void VisitKey(T1& obj, size_t index, TLambda&& lambda)
    {
        lambda(obj.at(index));
    }

    template <typename T1, typename TLambda>
        requires std::is_same_v<std::remove_const_t<T1>, std::array<T, N>>
    static void VisitAll(T1& obj, TLambda&& lambda)
    {
        for (size_t i = 0; i < N; i++) { lambda(i, obj.at(i)); }
    }
    using Iterator = size_t;

    template <typename T1>
        requires std::is_same_v<std::remove_const_t<T1>, std::array<T, N>>
    static void IteratorBegin(Iterator& it, T1&)
    {
        it = Iterator{};
    }
    template <typename T1>
        requires std::is_same_v<std::remove_const_t<T1>, std::array<T, N>>
    static void IteratorMoveNext(Iterator& it, T1&)
    {
        ++it;
    }
    template <typename T1>
        requires std::is_same_v<std::remove_const_t<T1>, std::array<T, N>>
    static bool IteratorValid(Iterator& it, T1&)
    {
        return it <= N;
    }

    template <typename T1, typename TLambda>
        requires std::is_same_v<std::remove_const_t<T1>, std::array<T, N>>
    static void Visit(Iterator& it, T1& obj, TLambda&& lambda)
    {
        lambda(obj.at(it));
    }
};

template <typename T> struct Stencil::Visitor<std::vector<T>> : Stencil::VisitorT<std::vector<T>>
{
    // So that this works for both const and non-const
    template <typename T1, typename TLambda>
        requires std::is_same_v<std::remove_const_t<T1>, std::vector<T>>
    static void VisitKey(T1& obj, size_t index, TLambda&& lambda)
    {
        lambda(obj.at(index));
    }

    template <typename T1, typename TLambda>
        requires std::is_same_v<std::remove_const_t<T1>, std::vector<T>>
    static void VisitAll(T1& obj, TLambda&& lambda)
    {
        for (size_t i = 0; i < obj.size(); i++) { lambda(i, obj.at(i)); }
    }
    using Iterator = size_t;

    template <typename T1>
        requires std::is_same_v<std::remove_const_t<T1>, std::vector<T>>
    static void IteratorBegin(Iterator& it, T1&)
    {
        it = Iterator{};
    }
    template <typename T1>
        requires std::is_same_v<std::remove_const_t<T1>, std::vector<T>>
    static void IteratorMoveNext(Iterator& it, T1&)
    {
        ++it;
    }
    template <typename T1>
        requires std::is_same_v<std::remove_const_t<T1>, std::vector<T>>
    static bool IteratorValid(Iterator& it, T1& obj)
    {
        return it <= obj.size();
    }

    template <typename T1, typename TLambda>
        requires std::is_same_v<std::remove_const_t<T1>, std::vector<T>>
    static void Visit(Iterator& it, T1& obj, TLambda&& lambda)
    {
        if (obj.size() == it) { obj.resize(it + 1); }
        lambda(obj.at(it));
    }
};

template <typename K, typename V> struct Stencil::Visitor<std::unordered_map<K, V>> : Stencil::VisitorT<std::unordered_map<K, V>>
{
    using TObj = std::unordered_map<K, V>;

    template <typename T1, typename TLambda>
        requires std::is_same_v<std::remove_const_t<T1>, TObj>
    static void VisitAll(T1& obj, TLambda&& lambda)
    {
        for (auto& [k, v] : obj) { lambda(k, v); }
    }

    template <typename T1, typename TLambda>
        requires std::is_same_v<std::remove_const_t<T1>, TObj>
    static void VisitKey(T1& obj, K const& it, TLambda&& lambda)
    {
        // TODO : Visit should not create keys
        lambda(obj[it]);
    }

#if 0
    // So that this works for both const and non-const
    template <typename T1, typename TLambda>
    requires std::is_same_v<std::remove_const_t<T1>, std::vector<T>>
    static void VisitKey(T1& obj, size_t index, TLambda&& lambda) { lambda(obj.at(index)); }

    template <typename T1, typename TLambda>
    requires std::is_same_v<std::remove_const_t<T1>, std::vector<T>>
    static void VisitAll(T1& obj, TLambda&& lambda)
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
#endif
};

template <typename... Ts> struct Stencil::VisitorForVariant<std::variant<std::monostate, Ts...>>
{
    using TObj = std::variant<std::monostate, Ts...>;
    static bool IsMonostate(TObj const& obj) { return obj.index() == 0; }

    template <typename TLambda> static void VisitAlternatives(TObj& /* obj */, TLambda&& lambda)
    {
        using TypeTuple  = std::tuple<Ts...>;
        auto applylambda = [&](size_t i, auto& arg, size_t index) {
            if (i == index) { lambda(i, arg); }
        };
        for (size_t i = 0; i < sizeof...(Ts); i++)
        {
            std::apply(
                [&](auto... args) {
                    size_t index = 0;
                    (applylambda(i, args, index++), ...);
                },
                TypeTuple{});
        }
    }

    template <typename TLambda> static void VisitActiveAlternative(TObj const& obj, TLambda&& lambda)
    {
        std::visit([&](auto val) { lambda(obj.index(), val); }, obj);
    }
    template <typename TLambda> static void VisitActiveAlternative(TObj& obj, TLambda&& lambda)
    {
        std::visit([&](auto val) { lambda(obj.index(), val); }, obj);
    }
};

template <typename... Ts>
    requires(!std::is_same_v<std::monostate, std::tuple_element_t<0, std::tuple<Ts...>>>)
struct Stencil::VisitorForVariant<std::variant<Ts...>>
{
    using TObj = std::variant<Ts...>;
    static bool IsMonostate(TObj const& /* obj */) { return false; }

    template <typename TLambda> static void VisitAlternatives(TObj& /* obj */, TLambda&& lambda)
    {
        using TypeTuple  = std::tuple<Ts...>;
        auto applylambda = [&](size_t i, auto& arg, size_t index) {
            if (i == index) { lambda(i, arg); }
        };
        for (size_t i = 0; i < sizeof...(Ts); i++)
        {
            std::apply(
                [&](auto... args) {
                    size_t index = 0;
                    (applylambda(i, args, index++), ...);
                },
                TypeTuple{});
        }
    }

    template <typename TLambda> static void VisitActiveAlternative(TObj const& obj, TLambda&& lambda)
    {
        std::visit([&](auto const& val) { lambda(obj.index(), val); }, obj);
    }
    template <typename TLambda> static void VisitActiveAlternative(TObj& obj, TLambda&& lambda)
    {
        std::visit([&](auto& val) { lambda(obj.index(), val); }, obj);
    }
};
