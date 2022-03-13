#pragma once
#include "typetraits_std.h"

SUPPRESS_WARNINGS_START
#include <fmt/chrono.h>
#include <fmt/ostream.h>
SUPPRESS_WARNINGS_END

#include <charconv>

namespace Stencil
{

struct ProtocolString
{};

struct ProtocolJsonVal
{};

struct ProtocolBinary
{};

template <typename T, typename TProtocol> struct SerDes;

// Protocol String

// Protocol JsonVal

// Protocol Binary

}    // namespace Stencil
