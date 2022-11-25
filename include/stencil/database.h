#pragma once
#include "CommonMacros.h"

#include <concepts>
#include <memory>
#include <tuple>
#include <unordered_map>

namespace Stencil::Database    // Type/Trait Declarations
{

template <typename T> struct RecordTraits;

template <typename T>
concept ConceptRecord = requires(typename RecordTraits<T> r) { typename RecordTraits<T>::RecordTypes; };

template <typename T>
concept ConceptComplex = ConceptRecord<T> && std::tuple_size_v<typename RecordTraits<T>::RecordTypes> == 1;

template <typename T>
concept ConceptTrivial = ConceptRecord<T> && !(ConceptComplex<T>);

template <typename T>
concept ConceptBlob = ConceptRecord<T> && RecordTraits<T>::Size() == 0;

template <typename T>
concept ConceptFixedSize = ConceptRecord<T> && RecordTraits<T>::Size() > 0;

template <ConceptRecord T> struct Ref
{
    uint32_t id{0};
};

template <typename T, typename TTup, size_t I = 0> constexpr size_t tuple_index_of()
{
    if constexpr (std::tuple_size<TTup>::value == I) { static_assert(std::is_same_v<T, T>, "Tuple Out of range"); }
    else
    {
        if constexpr (std::is_same_v<std::tuple_element_t<I, TTup>, T>) { return I; }
        else { return tuple_index_of<T, TTup, I + 1>(); }
    }
}
template <ConceptRecord T, typename TDb> static constexpr uint16_t TypeId = tuple_index_of<T, typename TDb::RecordTypes>;

template <ConceptRecord T> struct RecordViewT;
template <ConceptRecord T> using RefAndRecordViewT = std::tuple<Ref<T>, RecordViewT<T>>;

using ROLock = std::unique_lock<std::mutex>;
using RWLock = std::unique_lock<std::mutex>;

template <typename T>
concept ConceptLock = std::is_same_v<T, ROLock> || std::is_same_v<T, RWLock>;

template <ConceptRecord T> struct Iterator
{
    // TODO2
};

}    // namespace Stencil::Database

namespace Stencil::Database    // Class/Inferface
{

template <ConceptRecord... Ts> struct Database
{
    using RecordTypes = std::tuple_cat<RecordTraits<Ts>::RecordTypes...>;
    template <ConceptRecord T> RefAndRecordViewT<T> Create(RWLock const& lock, T const& obj);
    template <ConceptRecord T> RecordViewT<T>       Get(ROLock const& lock, Ref<T> const& ref) { TODO("TODO2"); }
    template <ConceptRecord T> Iterator<T>          Items(ROLock const& lock, Ref<T> const& ref) { TODO("TODO2"); }
    template <ConceptRecord T> void                 Delete(RWLock const& lock, Ref<T> const& ref) { TODO("TODO2"); }

    template <ConceptRecord T> RefAndRecordViewT<T> Create(RWLock const& lock, T const& obj)
    {
        if constexpr (ConceptBlob<T>) {}
        else if constexpr (ConceptFixedSize<T>) {}
        else
        {
            RecordViewT<T> rec;
            rec.VisitAll([&](auto& field) {
                if constexpr (ConceptRef<std::remove_cvref_t<decltype(field)>>) { auto [ref, rec] = Create(lock, field); }
            });
            // Visit-All
        }
    }
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

    using RecordTypes = std::tuple_cat<RecordTraits<K>::RecordTypes, RecordTraits<V>::RecordTypes, RecordTraits<List<MapItem>>>;
};

template <Stencil::Database::ConceptRecord T> struct Stencil::Database::RecordTraits<std::vector<T>>
{
    using RecordTypes = std::tuple_cat<RecordTraits<T>::RecordTypes, RecordTraits<List<Ref<T>>>>;
};

template <Stencil::Database::ConceptRecord T> struct Stencil::Database::RecordTraits<std::unique_ptr<T>>
{
    using RecordTypes = RecordTraits<T>::RecordTypes;
};
