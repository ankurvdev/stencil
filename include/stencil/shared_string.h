#pragma once
#include <algorithm>
#include <compare>
#include <memory>
#include <string>
#include <string_view>

#ifdef HAVE_FMTLIB
#include "CommonMacros.h"
SUPPRESS_WARNINGS_START
SUPPRESS_STL_WARNINGS
SUPPRESS_FMT_WARNINGS
#include <fmt/format.h>
SUPPRESS_WARNINGS_END
#endif

// NOLINTBEGIN(readability-identifier-naming, cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)
namespace std
{
inline std::string to_string(std::wstring_view str)
{
    std::string s(str.size(), 0);
    std::ranges::transform(str, s.begin(), [](wchar_t c) { return static_cast<char>(c); });
    return s;
}
}    // namespace std

template <typename T> struct shared_stringT
{
    using TString     = std::basic_string<T>;
    using TStringView = std::basic_string_view<T>;

    using const_pointer = TString::const_pointer;
    using pointer       = TString::pointer;

    using value_type = TString::value_type;

    template <size_t N> shared_stringT(T const (&str)[N]) { *this = make(str); }    // NOLINT

    explicit shared_stringT(TStringView const& str)
    {
        if (str.empty()) return;
        *this = make(str);
    }

    shared_stringT() = default;
    shared_stringT(std::nullptr_t) {}    // NOLINT

    shared_stringT(std::shared_ptr<TString>& str) : _str(str) {}    // NOLINT
    shared_stringT(shared_stringT const& str) : _str(std::make_shared<TString>(str)) {}
    shared_stringT(shared_stringT&& str) noexcept : _str(std::move(str._str)) {}
    ~shared_stringT() = default;

    public:
    template <size_t N> shared_stringT& operator=(T const (&str)[N])
    {
        *this = make(str);
        return *this;
    }
    shared_stringT& operator=(TStringView const& str)
    {
        if (str.empty()) return *this;
        *this = make(str);
        return *this;
    }
    shared_stringT& operator=(shared_stringT&& str)    // NOLINT
    {
        std::swap(_str, str._str);
        return *this;
    }
    static shared_stringT make(auto&&... args)
    {
        shared_stringT obj;
        obj._str = std::make_shared<TString>(args...);
        return obj;
    }

    bool icompare(TStringView const& str)
    {
        return std::equal(str.begin(), str.end(), _str->begin(), _str->end(), [](char a, char b) { return tolower(a) == tolower(b); });
    }

    // spaceship operator doesnt seem to work on android
    [[nodiscard]] auto _compare(shared_stringT const& str) const
    {
        bool lhsempty = empty();
        bool rhsempty = str.empty();
        if (!lhsempty && !rhsempty) { return *_str.get() <=> *str._str.get(); }
        if (lhsempty == rhsempty) { return std::strong_ordering::equal; }
        if (lhsempty) { return std::strong_ordering::less; }

        return std::strong_ordering::greater;
    }

    [[nodiscard]] TStringView view() const { return empty() ? TStringView() : TStringView(*_str.get()); }

    template <size_t N> bool operator==(T const (&str)[N]) const
    { return view() == str; }    // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)

    bool operator==(shared_stringT const& str) const { return view() == str.view(); }
    bool operator==(TStringView const& str) const { return view() == str; }
    bool operator!=(shared_stringT const& str) const { return view() != str.view(); }
    bool operator!=(TStringView const& str) const { return view() != str; }
    bool operator<(shared_stringT const& str) const { return view() < str.view(); }
    bool operator<(TStringView const& str) const { return view() < str; }
    bool operator>(shared_stringT const& str) const { return view() > str.view(); }
    bool operator>(TStringView const& str) const { return view() > str; }

    /*
    template <size_t N> bool operator==(T const (&str)[N]) const { return *(_str.get()) == str; }

    bool operator!=(const shared_stringT& str) const { return *(_str.get()) != *str.get(); }
    bool operator!=(const TStringView& str) const { return *(_str.get()) != str; }
    bool operator<(const shared_stringT& str) const { return *(_str.get()) < *str.get(); }
    bool operator<(const TStringView& str) const { return *(_str.get()) < str; }
    */

    shared_stringT                     operator+(shared_stringT const& str) const { return make(*_str.get() + *str._str.get()); }
    template <size_t N> shared_stringT operator+(T const (&str)[N]) const { return make(*_str.get() + str); }

    shared_stringT  operator+(std::basic_string_view<T> const& str) const { return make(*_str.get() + std::basic_string<T>(str)); }
    shared_stringT& operator+=(shared_stringT const& str)
    {
        if (_str.get() == nullptr || str._str.get() == nullptr)
        {
            if (_str.get() == nullptr) { *this = str; }
        }
        else
        {
            (*_str.get() += *str._str.get());
        }

        return *this;
    }

    shared_stringT& operator+=(std::basic_string_view<T> const& str)
    {
        (*_str.get() += str);
        return *this;
    }

    [[nodiscard]] auto begin() const { return _str->begin(); }
    [[nodiscard]] auto end() const { return _str->end(); }

    template <typename T1, typename T2> auto find(T1 obj, T2 index) const { return _str->find(obj, index); }
    template <typename T1> auto              find(T1 obj) const { return _str->find(obj); }
    template <typename T1> auto              rfind(T1 obj) const { return _str->rfind(obj); }
    [[nodiscard]] auto                       substr(size_t start, size_t len) const { return shared_stringT(_str->substr(start, len)); }
    [[nodiscard]] auto                       substr(size_t start) const { return shared_stringT(_str->substr(start)); }
    void                                     clear() { _str->reset(); }
    [[nodiscard]] TString const&             str() const { return *_str.get(); }

    shared_stringT& operator=(shared_stringT const& str) = default;

    operator TString const&() const { return *_str.get(); }                                             // NOLINT
    operator std::basic_string_view<T>() const { return std::basic_string_view<T>(data(), size()); }    // NOLINT

    [[nodiscard]] value_type    at(size_t index) const { return _str->at(index); }
    [[nodiscard]] const_pointer c_str() const { return _str.get() == nullptr ? nullptr : _str->c_str(); }
    [[nodiscard]] const_pointer data() const { return _str.get() == nullptr ? nullptr : _str->c_str(); }
    pointer                     data() { return _str.get() == nullptr ? nullptr : _str->data(); }

    [[nodiscard]] size_t length() const { return _str.get() == nullptr ? 0 : _str->length(); }
    [[nodiscard]] size_t size() const { return length(); }
    [[nodiscard]] bool   empty() const { return _str.get() == nullptr || _str->c_str() == nullptr || _str->length() == 0; }

    void resize(size_t size)
    {
        if (_str.get() == nullptr) { _str = std::make_shared<TString>(); }
        _str->resize(size);
    }

    private:
    std::shared_ptr<std::basic_string<T>> _str{};
    // void copy(const std::shared_ptr<TString>& str) { *this = str; }
};

using shared_string  = shared_stringT<char>;
using shared_wstring = shared_stringT<wchar_t>;

inline shared_string shared_wstring_to_string(shared_wstring const& str)
{
    if (str.empty()) return nullptr;
    return shared_string::make(std::to_string(str));
}

inline shared_wstring shared_string_to_wstring(shared_string const& str)
{
    if (str.empty()) return nullptr;
    std::wstring out;
    out.reserve(str.size());
    std::ranges::transform(str, out.begin(), [](auto const a) { return static_cast<wchar_t>(a); });
    return shared_wstring::make(std::move(out));
}

inline shared_string operator+(std::string const& str1, shared_string const& str2)
{ return shared_string::make(str1 + str2.str()); }

namespace std
{

template <typename T> struct hash<shared_stringT<T>>
{
    size_t operator()(shared_stringT<T> const& str) const { return std::hash<typename shared_stringT<T>::TString>{}(str.str()); }
};

}    // namespace std

#ifdef FMT_VERSION
template <> struct fmt::formatter<shared_string> : fmt::formatter<std::string_view>
{
    // Formats the point p using the parsed format specification (presentation)
    // stored in this formatter.
    auto format(shared_string const& item, fmt::format_context& ctx) const    // NOLINT
    { return fmt::format_to(ctx.out(), "{}", item.str()); }
};

#endif
// NOLINTEND(readability-identifier-naming, cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)
