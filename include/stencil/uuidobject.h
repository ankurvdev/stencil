#pragma once
#include "typetraits.h"
#include <span>
SUPPRESS_WARNINGS_START
SUPPRESS_STL_WARNINGS
#include "uuid.h"
SUPPRESS_WARNINGS_END
#include <array>
#include <chrono>
#include <compare>

template <typename T> struct UuidObjectT;

template <typename T> struct UuidBasedId
{
    static UuidBasedId<T> Create()
    {
        UuidBasedId<T>     uuid;
        std::random_device rd;
        auto               seed_data = std::array<unsigned, std::mt19937::state_size>{};
        std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
        std::seed_seq                seq(std::begin(seed_data), std::end(seed_data));
        std::mt19937                 generator(seq);
        uuids::uuid_random_generator gen{generator};
        uuid.uuid = gen();
        return uuid;
    }

    template <typename TStr> static constexpr UuidBasedId<T> FromString(TStr const& str)
    {
        UuidBasedId<T> uuid;
        uuid.guid = uuids::uuid(str);
    }

    public:
    UuidBasedId() = default;

    uuids::uuid uuid;

    constexpr bool                  Empty() const { return uuid == Invalid().uuid; }
    static constexpr UuidBasedId<T> Invalid() { return UuidBasedId<T>(); }
    friend UuidObjectT<T>;

    friend std::hash<UuidBasedId<T>>;
};

template <typename T> struct UuidObjectT
{
    using Id = UuidBasedId<T>;
    UuidObjectT() {}
    Id id = Id::Create();
};

template <> struct Stencil::TypeTraits<uuids::uuid>
{
    using Categories = std::tuple<Stencil::Category::Primitive>;
};

template <typename T> struct Stencil::TypeTraits<UuidBasedId<T>>
{
    using Categories = std::tuple<Stencil::Category::Primitive>;
};
