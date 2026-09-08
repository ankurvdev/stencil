#pragma once
#include "CommonMacros.h"
#include "typetraits_builtins.h"    // IWYU pragma: keep
#include "typetraits_std.h"         //IWYU pragma: keep
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
    private:
    template <size_t N, typename TObj, typename TLambda> static void SetAndVisit_(TObj& obj, size_t const& key, TLambda const& lambda)
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
            else
            {
                SetAndVisit_<N + 1>(obj, key, lambda);
            }
        }
    }

    public:
    template <typename T1, typename TLambda> static void VisitKey(T1& obj, size_t const& key, TLambda const& lambda)
    {
        if (obj.index() == key)
        {
            std::visit([&](auto&& arg) { lambda(arg); }, obj);
        }
        else
        {
            SetAndVisit_<0>(obj, key, lambda);
        }
    }
    template <typename T1, typename TLambda> static void VisitAll(T1& obj, TLambda const& lambda)
    {
        std::visit([&](auto&& arg) { lambda(obj.index(), arg); }, obj);
    }
};

template <typename T> struct StructFieldsVisitor;

template <typename T, typename... TAttrs> struct StructVisitor
{
    using Key = Stencil::TypeTraitsForIndexable<T>::Key;

    template <typename T1> using Fields = Stencil::TypeTraitsForIndexable<T1>::Fields;

    private:
    template <typename T1, typename TLambda> static bool VisitKeyIfVariantMatches_(T1& obj, Key const& key, TLambda const& lambda)
    {
        if (!std::holds_alternative<Fields<T1>>(key)) { return false; }
        return StructFieldsVisitor<T1>::VisitField(obj, std::get<Fields<T1>>(key), lambda);
    }

    template <typename TAttr, typename T1, typename TLambda> static bool VisitAllFieldsHelper_(T1& obj, TLambda const& lambda)
    {
        StructFieldsVisitor<TAttr>::VisitAllFields(obj, [&](auto&& key, auto&& val) { lambda(Key{key}, val); });
        return false;
    }

    public:
    template <typename T1, typename TLambda> static void VisitKey(T1& obj, Key const& key, TLambda const& lambda)
    {
        bool found = (VisitKeyIfVariantMatches_<TAttrs>(obj, key, lambda) || ...) || VisitKeyIfVariantMatches_<T1>(obj, key, lambda);
        if (!found)
        {
            found = (VisitKeyIfVariantMatches_<TAttrs>(obj, key, lambda) || ...) || VisitKeyIfVariantMatches_<T1>(obj, key, lambda);
            throw std::runtime_error("Key did not match any of the struct fields or attributes");
        }
    }
    template <typename T1, typename TLambda> static void VisitAll(T1& obj, TLambda const& lambda)
    {
        [[maybe_unused]] bool found = (VisitAllFieldsHelper_<TAttrs>(obj, lambda) || ...);
        StructFieldsVisitor<T>::VisitAllFields(obj, [&](auto&& key, auto&& val) { lambda(key, val); });
    }
};

}    // namespace Stencil

template <typename T> struct Stencil::StructFieldsVisitor<Stencil::TimestampedT<T>>
{
    using Fields = TypeTraitsForIndexable<Stencil::TimestampedT<T>>::Fields;
    template <typename T1, typename TLambda> static bool VisitField(T1& obj, Fields fields, TLambda const& lambda)
    {
        switch (fields)
        {
        case Fields::Field_timestamp: lambda(obj.lastmodified); return true;
        case Fields::Invalid: [[fallthrough]];
        default: return false;
        }
    }

    template <typename T1, typename TLambda> static void VisitAllFields(T1& obj, TLambda const& lambda)
    { lambda(Fields::Field_timestamp, obj.lastmodified); }
};

template <typename T> struct Stencil::StructFieldsVisitor<UuidBasedId<T>>
{
    using Fields = TypeTraitsForIndexable<UuidBasedId<T>>::Fields;
    template <typename T1, typename TLambda> static bool VisitField(T1& obj, Fields fields, TLambda const& lambda)
    {
        switch (fields)
        {
        case Fields::Field_uuid: lambda(obj.uuid); return true;
        case Fields::Invalid: [[fallthrough]];
        default: return false;
        }
    }

    template <typename T1, typename TLambda> static void VisitAllFields(T1& obj, TLambda const& lambda)
    { lambda(Fields::Field_uuid, obj.uuid); }
};

template <Stencil::ConceptIterable T> struct Stencil::VisitorForIterable<std::shared_ptr<T>>
{
    using Iterator = Stencil::Visitor<T>::Iterator;
    using ThisType = std::shared_ptr<T>;

    template <typename T1>
        requires std::is_same_v<std::remove_const_t<T1>, ThisType>
    static void IteratorBegin(Iterator& it, T1& obj)
    { Stencil::Visitor<T>::IteratorBegin(it, *obj.get()); }
    template <typename T1>
        requires std::is_same_v<std::remove_const_t<T1>, ThisType>
    static void IteratorMoveNext(Iterator& it, T1& obj)
    { Stencil::Visitor<T>::IteratorMoveNext(it, *obj.get()); }
    template <typename T1>
        requires std::is_same_v<std::remove_const_t<T1>, ThisType>
    static bool IteratorValid(Iterator& it, T1& obj)
    { return Stencil::Visitor<T>::IteratorValid(it, *obj.get()); }

    template <typename T1, typename TLambda>
        requires std::is_same_v<std::remove_const_t<T1>, ThisType>
    static void Visit(Iterator& it, T1& obj, TLambda && lambda)
    { Stencil::Visitor<T>::Visit(it, *obj.get(), std::forward<TLambda>(lambda)); }
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
    static void VisitAll(T1& obj, TLambda && lambda)
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

        Stencil::Visitor<T>::VisitAll(*obj.get(), std::forward<TLambda>(lambda));
    }
};

template <typename T, size_t N> struct Stencil::Visitor<std::array<T, N>> : Stencil::VisitorT<std::array<T, N>>
{
    // So that this works for both const and non-const
    template <typename T1, typename TLambda>
        requires std::is_same_v<std::remove_const_t<T1>, std::array<T, N>>
    static void VisitKey(T1& obj, size_t index, TLambda const& lambda)
    { lambda(obj.at(index)); }

    template <typename T1, typename TLambda>
        requires std::is_same_v<std::remove_const_t<T1>, std::array<T, N>>
    static void VisitAll(T1& obj, TLambda const& lambda)
    {
        for (size_t i = 0; i < N; i++) { lambda(i, obj.at(i)); }
    }
    using Iterator = size_t;

    template <typename T1>
        requires std::is_same_v<std::remove_const_t<T1>, std::array<T, N>>
    static void IteratorBegin(Iterator& it, T1& /*unused*/)
    { it = Iterator{}; }
    template <typename T1>
        requires std::is_same_v<std::remove_const_t<T1>, std::array<T, N>>
    static void IteratorMoveNext(Iterator& it, T1& /*unused*/)
    { ++it; }
    template <typename T1>
        requires std::is_same_v<std::remove_const_t<T1>, std::array<T, N>>
    static bool IteratorValid(Iterator& it, T1& /*unused*/)
    { return it <= N; }

    template <typename T1, typename TLambda>
        requires std::is_same_v<std::remove_const_t<T1>, std::array<T, N>>
    static void Visit(Iterator& it, T1& obj, TLambda const& lambda)
    { lambda(obj.at(it)); }
};

template <typename T> struct Stencil::Visitor<std::vector<T>> : Stencil::VisitorT<std::vector<T>>
{
    // So that this works for both const and non-const
    template <typename T1, typename TLambda>
        requires std::is_same_v<std::remove_const_t<T1>, std::vector<T>>
    static void VisitKey(T1& obj, size_t index, TLambda const& lambda)
    { lambda(obj.at(index)); }

    template <typename T1, typename TLambda>
        requires std::is_same_v<std::remove_const_t<T1>, std::vector<T>>
    static void VisitAll(T1& obj, TLambda const& lambda)
    {
        for (size_t i = 0; i < obj.size(); i++) { lambda(i, obj.at(i)); }
    }
    using Iterator = size_t;

    template <typename T1>
        requires std::is_same_v<std::remove_const_t<T1>, std::vector<T>>
    static void IteratorBegin(Iterator& it, T1& /*unused*/)
    { it = Iterator{}; }
    template <typename T1>
        requires std::is_same_v<std::remove_const_t<T1>, std::vector<T>>
    static void IteratorMoveNext(Iterator& it, T1& /*unused*/)
    { ++it; }
    template <typename T1>
        requires std::is_same_v<std::remove_const_t<T1>, std::vector<T>>
    static bool IteratorValid(Iterator& it, T1& obj)
    { return it <= obj.size(); }

    SUPPRESS_WARNINGS_START
    SUPPRESS_CLANG_WARNING("-Wlifetime-safety-invalidation")
    template <typename T1, typename TLambda>
        requires std::is_same_v<std::remove_const_t<T1>, std::vector<T>>
    static void Visit(Iterator& it, T1& obj, TLambda const& lambda)
    {
        if (obj.size() == it) { obj.resize(it + 1); }
        lambda(obj.at(it));
    }
    SUPPRESS_WARNINGS_END
};

template <typename K, typename V> struct Stencil::Visitor<std::unordered_map<K, V>> : Stencil::VisitorT<std::unordered_map<K, V>>
{
    using TObj = std::unordered_map<K, V>;

    template <typename T1, typename TLambda>
        requires std::is_same_v<std::remove_const_t<T1>, TObj>
    static void VisitAll(T1& obj, TLambda const& lambda)
    {
        for (auto& [k, v] : obj) { lambda(k, v); }
    }

    template <typename T1, typename TLambda>
        requires std::is_same_v<std::remove_const_t<T1>, TObj>
    static void VisitKey(T1& obj, K const& it, TLambda const& lambda)
    {
        // TODO : Visit should not create keys
        lambda(obj[it]);
    }

};

template <typename... Ts> struct Stencil::VisitorForVariant<std::variant<std::monostate, Ts...>>
{
    using TObj = std::variant<std::monostate, Ts...>;
    static bool IsMonostate(TObj const& obj) { return obj.index() == 0; }

    template <typename TLambda> static void VisitAlternatives(TObj& /* obj */, TLambda const& lambda)
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

    template <typename TLambda> static void VisitActiveAlternative(TObj const& obj, TLambda const& lambda)
    {
        std::visit([&](auto val) { lambda(obj.index(), val); }, obj);
    }
    template <typename TLambda> static void VisitActiveAlternative(TObj& obj, TLambda const& lambda)
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

    template <typename TLambda> static void VisitAlternatives(TObj& /* obj */, TLambda const& lambda)
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

    template <typename TLambda> static void VisitActiveAlternative(TObj const& obj, TLambda const& lambda)
    {
        std::visit([&](auto const& val) { lambda(obj.index(), val); }, obj);
    }
    template <typename TLambda> static void VisitActiveAlternative(TObj& obj, TLambda const& lambda)
    {
        std::visit([&](auto& val) { lambda(obj.index(), val); }, obj);
    }
};
