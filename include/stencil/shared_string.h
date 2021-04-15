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

template <typename T> struct shared_stringT : private std::shared_ptr<std::basic_string<T>>
{
    using BaseT = std::shared_ptr<std::basic_string<T>>;

    using TString     = typename std::basic_string<T>;
    using TStringView = typename std::basic_string_view<T>;

    using const_pointer = typename TString::const_pointer;
    using value_type    = typename TString::value_type;

    shared_stringT() : BaseT(nullptr) {}

    template <size_t N> shared_stringT(T const (&str)[N]) { *this = make(str); }

    explicit shared_stringT(const TStringView& str)
    {
        if (str.length() == 0) return;
        *this = make(str);
    }

    shared_stringT(std::nullptr_t) : BaseT(nullptr) {}

    shared_stringT(std::shared_ptr<TString>& str) : std::shared_ptr<TString>(str) {}
    shared_stringT(const shared_stringT& str) : std::shared_ptr<TString>(str) {}
    shared_stringT(shared_stringT&& str) noexcept : std::shared_ptr<TString>(std::move(str)) {}

    private:
    shared_stringT(BaseT&& str) : BaseT(std::move(str)) {}

    public:
    static shared_stringT make(const T* str) { return {std::make_shared<TString>(str)}; }
    static shared_stringT make(const T* str, size_t length) { return std::make_shared<TString>(str, length); }
    static shared_stringT make(TString&& str) { return std::make_shared<TString>(std::move(str)); }
    static shared_stringT make(const TStringView& str) { return std::make_shared<TString>(str); }

    bool icompare(TStringView const& str)
    {
        return std::equal(str.begin(), str.end(), this->begin(), this->end(), [](char a, char b) { return tolower(a) == tolower(b); });
    }

    bool operator==(const shared_stringT& str) const { return *(this->get()) == *str.get(); }
    bool operator==(const TStringView& str) const { return *(this->get()) == str; }

    template <size_t N> bool operator==(T const (&str)[N]) const { return *(this->get()) == str; }

    bool operator!=(const shared_stringT& str) const { return *(this->get()) != *str.get(); }
    bool operator!=(const TStringView& str) const { return *(this->get()) != str; }
    bool operator<(const shared_stringT& str) const { return *(this->get()) < *str.get(); }
    bool operator<(const TStringView& str) const { return *(this->get()) < str; }

    shared_stringT                     operator+(const shared_stringT& str) const { return make(*this->get() + *str.get()); }
    template <size_t N> shared_stringT operator+(T const (&str)[N]) const { return make(*this->get() + str); }

    shared_stringT  operator+(const std::basic_string_view<T>& str) const { return make(*this->get() + std::basic_string<T>(str)); }
    shared_stringT& operator+=(const shared_stringT& str)
    {
        if (this->get() == nullptr || str.get() == nullptr)
        {
            if (this->get() == nullptr)
            {
                *this = str;
            }
        }
        else
        {
            (*this->get() += *str.get());
        }

        return *this;
    }

    shared_stringT& operator+=(const std::basic_string_view<T>& str)
    {
        (*this->get() += str);
        return *this;
    }

    auto begin() const { return this->get()->begin(); }
    auto end() const { return this->get()->end(); }

    template <typename T1, typename T2> auto find(T1 obj, T2 index) const { return this->get()->find(obj, index); }
    template <typename T1> auto              find(T1 obj) const { return this->get()->find(obj); }
    template <typename T1> auto              rfind(T1 obj) const { return this->get()->rfind(obj); }
    auto           substr(size_t start, size_t len) const { return shared_stringT(this->get()->substr(start, len)); }
    auto           substr(size_t start) const { return shared_stringT(this->get()->substr(start)); }
    void           clear() { this->reset(); }
    const TString& str() const { return *this->get(); }

    shared_stringT& operator=(const shared_stringT& str)
    {
        *static_cast<BaseT*>(this) = *(static_cast<BaseT const*>(&str));
        return *this;
    }

    operator const TString&() const { return *this->get(); }
    operator std::basic_string_view<T>() const { return std::basic_string_view<T>(data(), size()); }

    value_type    at(size_t index) const { return this->get()->at(index); }
    const_pointer c_str() const { return this->get() == nullptr ? nullptr : this->get()->c_str(); }
    const_pointer data() const { return this->get() == nullptr ? nullptr : this->get()->c_str(); }
    size_t        length() const { return this->get() == nullptr ? 0 : this->get()->length(); }
    size_t        size() const { return length(); }
    bool          empty() const { return this->get() == nullptr || this->get()->c_str() == nullptr || this->get()->length() == 0; }

    private:
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
