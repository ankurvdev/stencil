#pragma once
#include "CommonMacros.h"
#include "typetraits.h"

#include <filesystem>
#include <stdexcept>
#include <utility>

namespace Stencil
{
#define DEFINE_PATH_CLASS(name) /*NOLINT*/                                                                                     \
    struct name                                                                                                                \
    {                                                                                                                          \
        name()  = default;                                                                                                     \
        ~name() = default;                                                                                                     \
        CLASS_DEFAULT_COPY_AND_MOVE(name);                                                                                     \
        name(std::filesystem::path const& pIn) /*NOLINT*/ :                                                                    \
            p(std::move(pIn)) { /*if (!std::filesystem::exists(p)) { throw std::invalid_argument("Path does not exist"); }*/ } \
                                                                                                                               \
        constexpr             operator std::filesystem::path() const { return p; }                                             \
        constexpr auto        operator<=>(name const& rhs) const = default;                                                    \
        std::filesystem::path p;                                                                                               \
    };    // namespace Stencil

DEFINE_PATH_CLASS(RFPath);
DEFINE_PATH_CLASS(WFPath);
DEFINE_PATH_CLASS(RWFPath);
DEFINE_PATH_CLASS(RDPath);
DEFINE_PATH_CLASS(WDPath);
DEFINE_PATH_CLASS(RWDPath);

}    // namespace Stencil

template <> struct Stencil::TypeTraits<Stencil::RFPath>
{
    using Categories = std::tuple<Stencil::Category::Primitive>;
};

template <> struct Stencil::TypeTraits<Stencil::WFPath>
{
    using Categories = std::tuple<Stencil::Category::Primitive>;
};

template <> struct Stencil::TypeTraits<Stencil::RWFPath>
{
    using Categories = std::tuple<Stencil::Category::Primitive>;
};
