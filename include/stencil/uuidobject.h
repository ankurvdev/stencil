#pragma once
#include <uuid.h>

#include <array>
#include <chrono>
#include <compare>

template <typename T> struct UuidObjectT;

template <typename T> struct UuidBasedId
{
    static UuidBasedId<T> Create()
    {
        UuidBasedId<T> uuid;
        uuid.guid = uuids::uuid::create();
        return uuid;
    }

    template <typename TStr> static constexpr UuidBasedId<T> FromString(TStr const& str)
    {
        UuidBasedId<T> uuid;
        uuid.guid = uuids::uuid(str);
    }

    public:
    UuidBasedId() = default;

    uuids::uuid guid;

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
