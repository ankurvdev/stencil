#pragma once
#include "CommonMacros.h"

#include <concepts>
#include <memory>
#include <mutex>
#include <tuple>
#include <unordered_map>

namespace Stencil::Database    // Type/Trait Declarations
{

template <typename T> struct RecordTraits;

template <typename T>
concept ConceptRecord = requires
{
    typename RecordTraits<T>::RecordTypes;
};

template <typename T>
concept ConceptComplex = ConceptRecord<T> && std::tuple_size_v<typename RecordTraits<T>::RecordTypes>
== 1;

template <typename T>
concept ConceptTrivial = ConceptRecord<T> && !(ConceptComplex<T>);

template <typename T>
concept ConceptBlob = ConceptRecord<T> && RecordTraits<T>::Size()
== 0;

template <typename T>
concept ConceptFixedSize = ConceptRecord<T> && RecordTraits<T>::Size()
> 0;

template <ConceptRecord T> struct Ref
{
    uint32_t id{0};
};

template <typename T> constexpr bool IsRef         = false;
template <typename T> constexpr bool IsRef<Ref<T>> = true;

template <typename T>
concept ConceptRef = IsRef<T>;

template <typename T, typename TTup, size_t I = 0> constexpr size_t tuple_index_of()
{
    if constexpr (std::tuple_size<TTup>::value == I) { static_assert(std::is_same_v<T, T>, "Tuple Out of range"); }
    else
    {
        if constexpr (std::is_same_v<std::tuple_element_t<I, TTup>, T>) { return I; }
        else { return tuple_index_of<T, TTup, I + 1>(); }
    }
}

template <typename... Tuples> using tuple_cat_t = decltype(std::tuple_cat(std::declval<Tuples>()...));

template <ConceptRecord T, typename TDb> static constexpr uint16_t TypeId = tuple_index_of<T, typename TDb::RecordTypes>;

template <typename T> struct RecordView;

template <ConceptRecord T> using RefAndRecordView = std::tuple<Ref<T>, RecordView<T>>;

using ROLock = std::unique_lock<std::mutex>;
using RWLock = std::unique_lock<std::mutex>;

template <typename T>
concept ConceptLock = std::is_same_v<T, ROLock> || std::is_same_v<T, RWLock>;

template <ConceptRecord T> struct Iterator
{
    // TODO2
};
template <ConceptRecord T, typename TDb, typename TLock> struct RangeForView
{
    RangeForView(TLock const& lock, TDb& db) : _lock(&lock), _db(&db) {}

    CLASS_DELETE_COPY_DEFAULT_MOVE(RangeForView);

    TLock const* _lock{};
    TDb*         _db{};

    Iterator<T> _begin;
    Iterator<T> _end;

    auto begin() const { return _begin; }
    auto end() const { return _end; }
};

template <typename T> struct List
{};
}    // namespace Stencil::Database

namespace Stencil::Database    // Class/Inferface
{

template <ConceptRecord... Ts> struct Database
{
    using ThisT       = Database<Ts...>;
    using RecordTypes = tuple_cat_t<typename RecordTraits<Ts>::RecordTypes...>;

    // template <ConceptRecord T> RefAndRecordView<T> Create(RWLock const& lock, T const& obj);
    template <ConceptRecord T> RecordView<T> Get(ROLock const& lock, Ref<T> const& ref) { TODO("TODO2"); }
    template <ConceptRecord T> auto          Items(ROLock const& lock) { return RangeForView<T, ThisT, ROLock>(lock, *this); }
    template <ConceptRecord T> void          Delete(RWLock const& lock, Ref<T> const& ref) { TODO("TODO2"); }

    template <ConceptRecord T> RefAndRecordView<T> Create(RWLock const& lock, T const& obj)
    {
        if constexpr (ConceptBlob<T>) {}
        else if constexpr (ConceptFixedSize<T>) {}
        else
        {
            RecordView<T> rec;
            rec.VisitAll([&](auto& field) {
                if constexpr (ConceptRef<std::remove_cvref_t<decltype(field)>>) { auto [ref, rec] = Create(lock, field); }
            });
            // Visit-All
        }
    }

    RWLock LockForEdit() { return std::unique_lock<std::mutex>(_mutex); }
    ROLock LockForRead() { return std::unique_lock<std::mutex>(_mutex); }

    void Init(std::string_view const& fname);

    std::mutex _mutex;
};
}    // namespace Stencil::Database

// Specializations

template <Stencil::Database::ConceptRecord K, Stencil::Database::ConceptRecord V>
struct Stencil::Database::RecordTraits<std::unordered_map<K, V>>
{
    struct MapItem
    {
        Ref<K> k;
        Ref<V> v;
    };

    using RecordTypes = tuple_cat_t<typename RecordTraits<K>::RecordTypes,
                                    typename RecordTraits<V>::RecordTypes,
                                    std::tuple<Stencil::Database::List<MapItem>>>;
};

template <Stencil::Database::ConceptRecord T> struct Stencil::Database::RecordTraits<std::vector<T>>
{
    using RecordTypes = tuple_cat_t<typename RecordTraits<T>::RecordTypes, std::tuple<Stencil::Database::List<Ref<T>>>>;
};

template <Stencil::Database::ConceptRecord T> struct Stencil::Database::RecordTraits<std::unique_ptr<T>>
{
    using RecordTypes = typename RecordTraits<T>::RecordTypes;
};

template <> struct Stencil::Database::RecordTraits<shared_string>
{
    using RecordTypes = std::tuple<shared_string>;
};

template <> struct Stencil::Database::RecordTraits<shared_wstring>
{
    using RecordTypes = std::tuple<shared_wstring>;
};

template <Stencil::ConceptPrimitive T> struct Stencil::Database::RecordTraits<T>
{
    using RecordTypes = std::tuple<T>;
};

template <Stencil::Database::ConceptRecord K, Stencil::Database::ConceptRecord V>
struct Stencil::Database::RecordView<std::unordered_map<K, V>>
{};

template <Stencil::Database::ConceptRecord T> struct Stencil::Database::RecordView<std::vector<T>>
{};

template <Stencil::Database::ConceptRecord T> struct Stencil::Database::RecordView<std::unique_ptr<T>>
{};

template <> struct Stencil::Database::RecordView<shared_string>
{};

template <> struct Stencil::Database::RecordView<shared_wstring>
{};

template <Stencil::ConceptPrimitive T> struct Stencil::Database::RecordView<T>
{};
