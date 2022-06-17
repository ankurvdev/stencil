#pragma once
#include <algorithm>
#include <memory>
#include <string>

namespace std
{
inline std::string to_string(std::wstring_view str)
{
    std::string s(str.size(), 0);
    std::transform(str.begin(), str.end(), s.begin(), [](wchar_t c) { return static_cast<char>(c); });
    return s;
}
}    // namespace std

template <typename T> struct shared_stringT
{
    using TString     = typename std::basic_string<T>;
    using TStringView = typename std::basic_string_view<T>;

    using const_pointer = typename TString::const_pointer;
    using pointer       = typename TString::pointer;

    using value_type = typename TString::value_type;

    template <size_t N> shared_stringT(T const (&str)[N]) { *this = make(str); }

    explicit shared_stringT(const TStringView& str)
    {
        if (str.length() == 0) return;
        *this = make(str);
    }

    shared_stringT() = default;
    shared_stringT(std::nullptr_t) {}

    shared_stringT(std::shared_ptr<TString>& str) : _str(str) {}
    shared_stringT(const shared_stringT& str) : _str(std::make_shared<TString>(str)) {}
    shared_stringT(shared_stringT&& str) noexcept : _str(std::move(str._str)) {}

    public:
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

    bool operator==(const shared_stringT& str) const { return *(_str.get()) == *str.get(); }
    bool operator==(const TStringView& str) const { return *(_str.get()) == str; }

    template <size_t N> bool operator==(T const (&str)[N]) const { return *(_str.get()) == str; }

    bool operator!=(const shared_stringT& str) const { return *(_str.get()) != *str.get(); }
    bool operator!=(const TStringView& str) const { return *(_str.get()) != str; }
    bool operator<(const shared_stringT& str) const { return *(_str.get()) < *str.get(); }
    bool operator<(const TStringView& str) const { return *(_str.get()) < str; }

    shared_stringT                     operator+(const shared_stringT& str) const { return make(*_str.get() + *str.get()); }
    template <size_t N> shared_stringT operator+(T const (&str)[N]) const { return make(*_str.get() + str); }

    shared_stringT  operator+(const std::basic_string_view<T>& str) const { return make(*_str.get() + std::basic_string<T>(str)); }
    shared_stringT& operator+=(const shared_stringT& str)
    {
        if (_str.get() == nullptr || str.get() == nullptr)
        {
            if (_str.get() == nullptr) { *this = str; }
        }
        else { (*_str.get() += *str.get()); }

        return *this;
    }

    shared_stringT& operator+=(const std::basic_string_view<T>& str)
    {
        (*_str.get() += str);
        return *this;
    }

    auto begin() const { return _str->begin(); }
    auto end() const { return _str->end(); }

    template <typename T1, typename T2> auto find(T1 obj, T2 index) const { return _str->find(obj, index); }
    template <typename T1> auto              find(T1 obj) const { return _str->find(obj); }
    template <typename T1> auto              rfind(T1 obj) const { return _str->rfind(obj); }
    auto                                     substr(size_t start, size_t len) const { return shared_stringT(_str->substr(start, len)); }
    auto                                     substr(size_t start) const { return shared_stringT(_str->substr(start)); }
    void                                     clear() { _str->reset(); }
    const TString&                           str() const { return *_str.get(); }

    shared_stringT& operator=(const shared_stringT& str)
    {
        _str = str._str;
        return *this;
    }

    operator const TString&() const { return *_str.get(); }
    operator std::basic_string_view<T>() const { return std::basic_string_view<T>(data(), size()); }

    value_type    at(size_t index) const { return _str->at(index); }
    const_pointer c_str() const { return _str.get() == nullptr ? nullptr : _str->c_str(); }
    const_pointer data() const { return _str.get() == nullptr ? nullptr : _str->c_str(); }
    pointer       data() { return _str.get() == nullptr ? nullptr : _str->data(); }

    size_t length() const { return _str.get() == nullptr ? 0 : _str->length(); }
    size_t size() const { return length(); }
    bool   empty() const { return _str.get() == nullptr || _str->c_str() == nullptr || _str->length() == 0; }

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

inline shared_string shared_wstring_to_string(const shared_wstring& str)
{
    if (str.empty()) return nullptr;
    return shared_string::make(std::to_string(str));
}

inline shared_wstring shared_string_to_wstring(const shared_string& str)
{
    if (str.empty()) return nullptr;
    std::wstring out;
    out.reserve(str.size());
    std::transform(str.begin(), str.end(), out.begin(), [](auto const a) { return static_cast<wchar_t>(a); });
    return shared_wstring::make(std::move(out));
}

inline shared_string operator+(const std::string& str1, const shared_string& str2)
{
    return shared_string::make(str1 + str2.str());
}
