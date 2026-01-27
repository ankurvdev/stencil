#pragma once
#include "CommonMacros.h"

SUPPRESS_WARNINGS_START
SUPPRESS_STL_WARNINGS
SUPPRESS_FMT_WARNINGS
SUPPRESS_MSVC_WARNING(4242)
SUPPRESS_MSVC_WARNING(4702)
SUPPRESS_MSVC_WARNING(5219)
SUPPRESS_MSVC_WARNING(5262)    // implicit fall-through occurs here;

#include <boost/asio.hpp>
#include <boost/beast/http.hpp>
#include <boost/url.hpp>

#include <fmt/format.h>
#include <fmt/ostream.h>

SUPPRESS_WARNINGS_END

template <> struct fmt::formatter<boost::system::error_code> : fmt::formatter<std::string_view>
{
    auto format(boost::system::error_code const& ec, fmt::format_context& ctx) const    // NOLINT
    {
        return fmt::format_to(ctx.out(), "{} {}:{}", ec.message(), ec.value(), ec.category().name());
    }
};

template <> struct fmt::formatter<boost::asio::ip::tcp::endpoint> : fmt::ostream_formatter
{};
template <> struct fmt::formatter<boost::asio::ip::udp::endpoint> : fmt::ostream_formatter
{};
template <> struct fmt::formatter<boost::asio::ip::address> : fmt::ostream_formatter
{};
