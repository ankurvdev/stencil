#pragma once
#include "CommonMacros.h"
#include "DebugInfo.h"

#include <algorithm>
#include <cassert>
#include <memory>
#include <optional>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

SUPPRESS_WARNINGS_START
SUPPRESS_MSVC_WARNING(4371)    // Object layout under /vd2 will change due to virtual base
#define SUPER(T) (*static_cast<T*>(this))

namespace Binding
{
template <typename TStr> struct StrOps;

template <> struct StrOps<std::wstring>
{
    using Type = std::wstring;
    using View = std::wstring_view;
    using Char = wchar_t;

    static constexpr auto InvalidIndex = std::wstring::npos;

    static std::wstring const& Value(Type const& str LFTBND) { return str; }
    static std::wstring        Value(View const& str) { return std::wstring(str); }

    static Type Create(std::wstring_view str) { return Type(str); }
    static Type Copy(View const& str) { return Type(str); }
    static Type Convert(char const* in)
    {
        if (in == nullptr) return Type{};
        return Convert(std::string_view(in));
    }

    static Type Convert(std::string_view const& in)
    {
        Type out;
        out.resize(in.size());
        std::ranges::transform(in, out.begin(), [](auto const a) { return static_cast<wchar_t>(a); });
        return out;
    }

    static Type ToLower(View const& in)
    {
        Type out;
        out.resize(in.size());
        std::ranges::transform(in, out.begin(), [](int a) -> wchar_t { return static_cast<wchar_t>(tolower(a)); });
        return out;
    }
    static bool IEqual(View const& l, View const& r)
    {
        return std::ranges::equal(
            l, r, [](auto lc, auto rc) { return std::tolower(static_cast<int>(lc)) == std::tolower(static_cast<int>(rc)); });
    }
    static bool Equal(View const& l, View const& r) { return l == r; }
    static bool IsEmpty(View const& l) { return l.empty(); }

    static size_t Find(View const& l, Char ch, size_t startIndex = 0) { return l.find(ch, startIndex); }
    static size_t Find(View const& l, View const& substr, size_t startIndex = 0) { return l.find(substr, startIndex); }
    static View   SubString(View const& s, size_t startIndex, size_t endIndex = InvalidIndex) { return s.substr(startIndex, endIndex); }
    static size_t Size(View const& s) { return s.size(); }
};

template <> struct StrOps<std::string>
{
    using Type = std::string;
    using View = std::string_view;
    using Char = char;

    static constexpr auto InvalidIndex = std::string::npos;

    static std::string const& Value(Type const& str LFTBND) { return str; }
    static std::string        Value(View const& str) { return std::string(str); }

    static Type Create(std::string_view str) { return Type(str); }
    static Type Copy(View const& str) { return Type(str); }
    static Type Convert(wchar_t const* in)
    {
        if (in == nullptr) return Type{};
        return Convert(std::wstring_view(in));
    }

    static Type Convert(std::wstring_view const& in)
    {
        Type out;
        out.resize(in.size());
        std::ranges::transform(in, out.begin(), [](auto const a) { return static_cast<char>(a); });
        return out;
    }

    static Type ToLower(View const& in)
    {
        Type out;
        out.resize(in.size());
        std::ranges::transform(in, out.begin(), [](int a) -> char { return static_cast<char>(tolower(a)); });
        return out;
    }
    static bool IEqual(View const& l, View const& r)
    {
        return std::ranges::equal(
            l, r, [](auto lc, auto rc) { return std::tolower(static_cast<int>(lc)) == std::tolower(static_cast<int>(rc)); });
    }
    static bool Equal(View const& l, View const& r) { return l == r; }
    static bool IsEmpty(View const& l) { return l.empty(); }

    static size_t Find(View const& l, Char ch, size_t startIndex = 0) { return l.find(ch, startIndex); }
    static size_t Find(View const& l, View const& substr, size_t startIndex = 0) { return l.find(substr, startIndex); }
    static View   SubString(View const& s, size_t startIndex, size_t endIndex = InvalidIndex) { return s.substr(startIndex, endIndex); }
    static size_t Size(View const& s) { return s.size(); }
};

using Str = StrOps<std::wstring>;
}    // namespace Binding

#include "DebugInfo.h"

namespace Binding
{
enum class Type
{
    String,
    Expr,
    Array,
    Object,
};

struct BindingContext;
struct IValue;
struct Expression;

struct BindingExpr
{
    std::vector<Str::Type> binding;

    [[nodiscard]] Str::Type Stringify() const
    {
        std::wstringstream ws;
        ws << "%";
        for (auto const& b : binding) { ws << b << "."; }
        ws << "%";
        return Str::Create(ws.str());
    }
};

struct Expression
{
    ~Expression() = default;
    Expression()  = default;

    Expression(Expression const&) = delete;
    Expression(Expression&& obj) noexcept { *this = std::move(obj); }

    Expression& operator=(Expression const&) = delete;

    Expression& operator=(Expression&& obj) noexcept
    {
        assert(!obj.pieces.empty());
        std::swap(pieces, obj.pieces);
        return *this;
    }

    template <typename TFunc> [[nodiscard]] [[nodiscard]] std::shared_ptr<Expression> Evaluate(TFunc func) const
    {
        assert(!Empty());
        auto newexpr = std::make_shared<Expression>();

        for (auto const& p : pieces)
        {
            if (p.piecetype == Piece::PieceType::Expr)
            {
                std::shared_ptr<Expression> result = func(*p.expr);
                if (result == nullptr)
                {
                    newexpr->pieces.push_back(p.Clone());
                    continue;
                }

                if (result->FullyEvaluated()) { newexpr->AddString(result->String()); }
                else
                {
                    for (auto& p1 : result->pieces)
                    {
                        if (p1.Empty())
                        {
                            result = func(*p.expr);
                            throw std::logic_error("Invalid Result");
                        }
                        newexpr->pieces.push_back(p1.Clone());
                    }
                }
            }
            else
            {
                newexpr->AddString(Str::Copy(p.text));
            }
        }

        assert(!newexpr->Empty());
        return newexpr;
    }

    // Creation Methods
    void AddString(Str::Type&& val)
    {
        assert(!Str::IsEmpty(val));
        pieces.emplace_back(std::move(val));
    }

    void AddExpressionString(Str::View const& str, wchar_t sep)
    {
        auto   expr  = std::make_unique<BindingExpr>();
        size_t start = 0;
        auto   next  = Str::Find(str, sep);
        while (next != Str::InvalidIndex)
        {
            expr->binding.push_back(Str::Create(Str::SubString(str, start, next - start)));
            start = next + 1;
            next  = Str::Find(str, sep, start);
        }
        expr->binding.push_back(Str::Create(Str::SubString(str, start)));
        pieces.emplace_back(std::move(expr));
    }

    void AddBindingExpression(std::unique_ptr<BindingExpr>&& expr)
    {
        assert(!expr->binding.empty());
        pieces.emplace_back(std::move(expr));
    }
    static std::shared_ptr<Expression> Clone(Expression const& obj)
    {
        std::shared_ptr<Expression> newexpr = std::make_shared<Expression>();

        newexpr->pieces.reserve(obj.pieces.size());
        for (auto const& p : obj.pieces) { newexpr->pieces.push_back(p.Clone()); }
        return newexpr;
    }

    static std::shared_ptr<Expression> Clone(std::shared_ptr<Expression const> const& obj)
    {
        if (obj == nullptr) { return {}; }
        return Clone(*obj);
    }

    struct Piece
    {
        explicit Piece(std::unique_ptr<BindingExpr>&& exprIn) : piecetype(PieceType::Expr), expr(std::move(exprIn)) {}
        explicit Piece(Str::Type&& textIn) : piecetype(PieceType::String), text(std::move(textIn)) {}
        ~Piece() = default;
        CLASS_ONLY_MOVE_CONSTRUCT(Piece);

        enum class PieceType
        {
            Expr,
            String,
        } piecetype;

        std::unique_ptr<BindingExpr> expr;
        Str::Type                    text;

        [[nodiscard]] bool Empty() const
        {
            return (piecetype == PieceType::Expr && (expr == nullptr || expr->binding.empty()))
                   || (piecetype == PieceType::String && Str::IsEmpty(text));
        }

        [[nodiscard]] Piece Clone() const
        {
            if (expr != nullptr)
            {
                std::vector<Str::Type> bindings;
                for (auto& s : expr->binding) { bindings.push_back(Str::Copy(s)); }
                return Piece(std::make_unique<BindingExpr>(BindingExpr{std::move(bindings)}));
            }
            return Piece{Str::Copy(text)};
        }

        [[nodiscard]] Str::Type Stringify() const
        {
            if (piecetype == PieceType::String) { return text; }
            if (piecetype == PieceType::Expr) { return expr->Stringify(); }
            throw std::logic_error("Invalid Piece Type");
        }
    };

    std::vector<Piece> pieces;

    // Expansion Method
    [[nodiscard]] bool FullyEvaluated() const
    {
        return std::ranges::all_of(pieces, [](auto const& p) { return p.piecetype != Piece::PieceType::Expr; });
    }

    [[nodiscard]] Str::Type Stringify() const
    {
        std::wstringstream ss;
        for (auto const& p : pieces) { ss << p.Stringify(); }
        return Str::Create(ss.str());
    }

    [[nodiscard]] Str::Type String() const
    {
        std::wstringstream ss;
        for (auto const& p : pieces)
        {
            if (p.piecetype != Piece::PieceType::String)
            {
                throw std::runtime_error(fmt::format("Expression Has unevaluated bits: {}", StrOps<std::string>::Convert(p.Stringify())));
            }
            assert(!Str::IsEmpty(p.text));
            ss << p.text;
        }
        return Str::Create(ss.str());
    }

    static auto Create(Str::View const& str, Str::View const& startMarker, Str::View const& endMarker, wchar_t const sep)
    {
        std::shared_ptr<Expression> expr;
        if (Str::IsEmpty(str) || Str::Size(str) == 0) { return expr; }
        size_t index = 0;

        expr = std::make_shared<Expression>();

        while (index != Str::InvalidIndex)
        {
            auto markerstart = Str::Find(str, startMarker, index);
            if (markerstart == Str::InvalidIndex) break;
            if (index < markerstart) expr->AddString(Str::Create(Str::SubString(str, index, markerstart - index)));
            markerstart += Str::Size(startMarker);
            auto markerend = Str::Find(str, endMarker, markerstart);
            if (markerstart == Str::InvalidIndex) break;
            expr->AddExpressionString(Str::Create(Str::SubString(str, markerstart, markerend - markerstart)), sep);
            index = markerend + Str::Size(endMarker);
        }
        if (index < Str::Size(str)) { expr->AddString(Str::Create(Str::SubString(str, index))); }
        return expr;
    }

    [[nodiscard]] bool Empty() const
    {
        if (pieces.empty()) return true;
        return std::ranges::any_of(pieces, [](auto const& p) { return p.Empty(); });
    }

};    // namespace Binding

// Bindable Dictionary Object with Name and Bindable Named Values.
struct IBindable
{
    IBindable() = default;
    virtual ~IBindable() = default;
    CLASS_DELETE_COPY_AND_MOVE(IBindable);

    // TODO : Can this be Str::View ??
    virtual Str::Type               ObjectTypeName()                                                 = 0;
    virtual Str::Type               ObjectId()                                                       = 0;
    virtual std::shared_ptr<IValue> TryLookupOrNull(BindingContext& context, Str::View const& param) = 0;
};

// TODO : Do we really need it
struct IBindableComponent
{
    IBindableComponent() = default;
    virtual ~IBindableComponent() = default;
    CLASS_DELETE_COPY_AND_MOVE(IBindableComponent);

    virtual size_t                  GetKeyCount()                                                   = 0;
    virtual Str::Type               GetKeyAt(size_t index)                                          = 0;
    virtual Str::Type               ComponentName()                                                 = 0;
    virtual std::shared_ptr<IValue> TryLookupValue(BindingContext& context, Str::View const& param) = 0;
};

struct IValueArray
{
    IValueArray() = default;
    virtual ~IValueArray() = default;
    CLASS_DELETE_COPY_AND_MOVE(IValueArray);

    struct Iterator
    {
        Iterator() = default;
        explicit Iterator(IValueArray& ptr) : _ptr(ptr) { CheckEnd(); }
        Iterator& operator++() LFTBND
        {
            if (!_ptr.has_value()) return *this;
            _index++;
            CheckEnd();
            return *this;
        }
        void CheckEnd()
        {
            if (_index >= _ptr->get().GetCount())
            {
                _ptr.reset();
                _index = 0;
            }
        }

        bool operator==(Iterator const& rhs) const
        {
            return _index == rhs._index && _ptr.has_value() == rhs._ptr.has_value()
                   && (!_ptr.has_value() || &_ptr.value().get() == &rhs._ptr.value().get());
        }

        bool       operator!=(Iterator const& rhs) const { return !((*this) == rhs); }
        IBindable& operator*() const { return _ptr->get().GetObjectAt(_index); }

        private:
        std::optional<std::reference_wrapper<IValueArray>> _ptr;

        size_t _index{0};
    };

    virtual size_t     GetCount()                = 0;
    virtual IBindable& GetObjectAt(size_t index) = 0;
};

template <typename TBegin, typename TEnd = TBegin> struct RangeT
{
    RangeT(TBegin beginIn, TEnd endIn) : begin(beginIn), end(endIn) {}
    TBegin Begin() { return begin; }
    TEnd   End() { return end; }
    TBegin begin;
    TEnd   end;
};

template <typename TBegin, typename TEnd = TBegin> auto Range(TBegin begin, TEnd end)
{ return RangeT<TBegin, TEnd>(begin, end); }

struct IValue
{
    public:
    IValue() = default;
    virtual ~IValue() = default;
    CLASS_DELETE_COPY_AND_MOVE(IValue);

    virtual Type              GetType()     = 0;
    virtual Str::Type const&  GetString()   = 0;
    virtual Expression const& GetExpr()     = 0;
    virtual IValueArray&      GetArray()    = 0;
    virtual IBindable&        GetBindable() = 0;

    struct Getter
    {
        Getter() = default;
        virtual ~Getter()                     = default;
        CLASS_DELETE_COPY_AND_MOVE(Getter);

        virtual std::shared_ptr<IValue> Get() = 0;
    };

    Str::Type Stringify()
    {
        switch (GetType())
        {
        case Type::String: return L"Value[String]: " + GetString();
        case Type::Expr: return L"Value[Expr]:" + GetExpr().Stringify();
        case Type::Array:
        {

            if (GetArray().GetCount() == 0) return L"Value[Array] Length: 0 ";

            return L"Value[Array] Length: " + Str::Create(std::to_wstring(GetArray().GetCount())) + L"[Type]: "
                   + GetArray().GetObjectAt(0).ObjectTypeName();
        }
        case Type::Object: return L"Value[Object] Type: " + GetBindable().ObjectTypeName() + L"Id:" + GetBindable().ObjectId();
        default: break;
        }
        throw std::logic_error("Cannot identify Value Type");
    }
};

template <Type TType> struct ValueT;
template <> struct ValueT<Type::String> : IValue
{
    // virtual Str::Type const& GetString() const { throw std::logic_error(""); }

    Type                           GetType() override { return Type::String; }
    [[noreturn]] Expression const& GetExpr() override { throw std::logic_error(""); }
    [[noreturn]] IValueArray&      GetArray() override { throw std::logic_error(""); }
    [[noreturn]] IBindable&        GetBindable() override { throw std::logic_error(""); }
};

template <Type TType> struct ValueT;
template <> struct ValueT<Type::Array> : IValue
{
    //  virtual IValueArray const & GetArray() const { throw std::logic_error(""); }

    Type                           GetType() override { return Type::Array; }
    [[noreturn]] Str::Type const&  GetString() override { throw std::logic_error("Querying Array Value as a String"); }
    [[noreturn]] Expression const& GetExpr() override { throw std::logic_error("Querying Array Value as an Expression"); }
    [[noreturn]] IBindable&        GetBindable() override { throw std::logic_error("Querying Array Value as a BindableObject"); }
};

template <Type TType> struct ValueT;
template <> struct ValueT<Type::Expr> : IValue
{
    // virtual Expression const&  GetExpr() const { throw std::logic_error(""); }

    Type                          GetType() override { return Type::Expr; }
    [[noreturn]] Str::Type const& GetString() override { throw std::logic_error(""); }
    [[noreturn]] IValueArray&     GetArray() override { throw std::logic_error(""); }
    [[noreturn]] IBindable&       GetBindable() override { throw std::logic_error(""); }
};

template <Type TType> struct ValueT;
template <> struct ValueT<Type::Object> : IValue
{
    // virtual IBindable&            GetBindable() const { throw std::logic_error(""); }

    Type                           GetType() override { return Type::Object; }
    [[noreturn]] Str::Type const&  GetString() override { throw std::logic_error(""); }
    [[noreturn]] Expression const& GetExpr() override { throw std::logic_error(""); }
    [[noreturn]] IValueArray&      GetArray() override { throw std::logic_error(""); }
};

// TODO : do we really need a AttributeMap class ?
struct AttributeMap
{
    void AddEntry(Str::Type name, std::shared_ptr<Binding::Expression>&& value) { _kvp[std::move(name)] = std::move(value); }

    [[nodiscard]] auto const& GetAttributes() const LFTBND { return _kvp; }

    private:
    std::unordered_map<Str::Type, std::shared_ptr<Binding::Expression>> _kvp;
};

struct BindingContext
{
    CLASS_DELETE_COPY_AND_MOVE(BindingContext);

    BindingContext() : _previousContext(_currentThreadContext) { _currentThreadContext = this; }

    ~BindingContext() { _currentThreadContext = _previousContext; }

    auto GetRange(IBindable& ptr, BindingExpr const& expr)
    {
        struct Range
        {
            [[nodiscard]] auto        begin() const { return IValueArray::Iterator(value->GetArray()); } //NOLINT
             auto end() { return IValueArray::Iterator(); } //NOLINT

            std::shared_ptr<IValue> value;
        };
        return Range{EvaluateArray(ptr, expr)};
    }

    bool FindBindableByNameInContextStack(Str::View const& name, std::reference_wrapper<IBindable>& ptr)
    {
        // Start looking from the top of the stack to grab the most relevant item for that type
        for (auto& it : std::views::reverse(_stack))
        {
            auto bindableName = it.get().ObjectTypeName();
            if (Str::Equal(bindableName, name))
            {
                ptr = it;
                return true;
            }
        }
        return false;
    }

    std::shared_ptr<IValue> _TryEvaluateBindingExprOrNull(BindingExpr const& expr)
    {
        assert(!expr.binding.empty());
        if (expr.binding.size() == 1)
        {
            auto val = _stack.back().get().TryLookupOrNull(*this, expr.binding.back());
            if (val == nullptr) { return val; }
            if (val->GetType() == Type::Object)
            {
                throw std::logic_error("WTF");
                // _stack.push_back(val->GetBindable());
            }
            return val;
        }

        std::reference_wrapper<IBindable> ptr = _stack.back();

        auto bit = expr.binding.begin();
        if (FindBindableByNameInContextStack(*bit, ptr)) ++bit;

        std::shared_ptr<IValue>             val;
        std::vector<std::unique_ptr<Scope>> scopes;

        scopes.emplace_back(new Scope(*this, ptr.get()));

        for (; bit != expr.binding.end(); ++bit)
        {
            if (val != nullptr)
            {
                ptr = val->GetBindable();
                scopes.emplace_back(new Scope(*this, ptr.get()));
            }
            val = ptr.get().TryLookupOrNull(*this, *bit);
            if (val == nullptr) { return nullptr; }
        }
        SUPPRESS_WARNINGS_START
        SUPPRESS_CLANG_WARNING("-Wnrvo")
        return val;
        SUPPRESS_WARNINGS_END
    }

    std::shared_ptr<Expression> _EvaluateExpression(Expression const& expr)
    {
        auto result = expr.Evaluate([this](BindingExpr const& expr1) -> std::shared_ptr<Expression> {
            auto value = _TryEvaluateBindingExprOrNull(expr1);
            if (value == nullptr) { return {}; }
            switch (value->GetType())
            {
            case Type::String:
            {
                auto newexpr = std::make_shared<Expression>();
                newexpr->AddString(Str::Copy(value->GetString()));
                return newexpr;
            }
            case Type::Expr: return Expression::Clone(value->GetExpr());
            case Type::Object:
            case Type::Array: throw std::logic_error("Expected either string or another expression");
            default: break;
            }
            return {};
        });
        return result;
    }

    struct Scope
    {
        Scope(BindingContext& context, IBindable& ptr) : Scope(context._stack, ptr) {}
        Scope(std::vector<std::reference_wrapper<IBindable>>& stackIn LFTBND, IBindable& ptrIn) : stack(stackIn) { stack.push_back(std::ref(ptrIn)); }
        ~Scope() { stack.pop_back(); }
        CLASS_DELETE_COPY_AND_MOVE(Scope);
        std::vector<std::reference_wrapper<IBindable>>& stack;
    };

    auto ContextScope(IBindable& ptr) { return Scope{*this, ptr}; }

    std::shared_ptr<IValue> EvaluateArray(IBindable& ptr, BindingExpr const& expr)
    {
        ACTION_CONTEXT([&]() { return L"Bindable: " + ptr.ObjectTypeName() + L" Expression:" + expr.Stringify(); });

        Scope scope(_stack, ptr);
        auto  val = _TryEvaluateBindingExprOrNull(expr);
        if (val == nullptr)
        {
            _TryEvaluateBindingExprOrNull(expr);
            throw std::logic_error("Unable to Evaluate");
        }
        assert(val->GetType() == Type::Array);
        return val;
    }

    auto EvaluateExpression(IBindable& ptr, Expression const& expr)
    {
        Scope scope(_stack, ptr);
        return _EvaluateExpression(expr);
    }

    std::shared_ptr<IValue> TryEvaluateBindingExprOrNull(IBindable& ptr, BindingExpr const& expr)
    {
        Scope scope(_stack, ptr);
        return _TryEvaluateBindingExprOrNull(expr);
    }

    static auto EvaluateExpression(Expression const& expr) { return _currentThreadContext->_EvaluateExpression(expr); }

    private:
    SUPPRESS_WARNINGS_START
    SUPPRESS_CLANG_WARNING("-Wunique-object-duplication")
    thread_local static inline BindingContext* _currentThreadContext = nullptr;
    SUPPRESS_WARNINGS_END

    BindingContext*                                _previousContext = nullptr;
    std::vector<std::reference_wrapper<IBindable>> _stack;
};    // namespace Binding

struct BindableBase : public IBindable, public ValueT<Type::Object>
{
    BindableBase()           = default;
    ~BindableBase() override = default;
    CLASS_DELETE_COPY_AND_MOVE(BindableBase);

    void AddBaseObject(std::shared_ptr<BindableBase> const& basePtr)
    {
        // assert(basePtr != nullptr);
        _baseptr.push_back(basePtr);
    }

    void Register(std::shared_ptr<IBindableComponent> const& obj) { _components.push_back(obj); }

    IBindable& GetBindable() LFTBND override { return *this; }

    std::shared_ptr<IValue> TryLookupOrNull(BindingContext& context, Str::View const& param) override
    {
        auto val = _TryLookupOrNull(context, param);
        if (val != nullptr && val->GetType() == Type::Expr)
        {
            struct Value : ValueT<Type::Expr>
            {
                std::shared_ptr<Expression> expr;
                explicit Value(std::shared_ptr<Expression>&& exprIn) : expr(std::move(exprIn)) {}
                CLASS_DELETE_COPY_AND_MOVE(Value);
                Expression const& GetExpr() override { return *expr; }
            };

            return std::make_shared<Value>(context.EvaluateExpression(*this, val->GetExpr()));
        }
        SUPPRESS_WARNINGS_START
        SUPPRESS_CLANG_WARNING("-Wnrvo")
        return val;
        SUPPRESS_WARNINGS_END
    }

    std::shared_ptr<IValue> _TryLookupOrNull(BindingContext& context, Str::View const& param)
    {
        if (Str::IEqual(param, ObjectTypeName()))
        {
            // TODO
            return _self.lock();
        }

        for (auto& c : _components)
        {
            auto value = c->TryLookupValue(context, param);
            if (value) return value;
        }

        for (auto& b : _transformations)
        {
            auto value = b->TryLookupValue(context, param);
            if (value) return value;
        }

        for (auto& b : _baseptr)
        {
            auto value = b->TryLookupOrNull(context, param);
            if (value) return value;
        }
        return nullptr;
    }

    Str::Type ObjectId() override { return Str::Copy(_name); }

    void SetName(Str::Type&& name) { std::swap(_name, name); }

    template <typename TObject> struct TranformationBindableComponent : public IBindableComponent
    {
        struct Value : public ValueT<Type::Expr>
        {
            explicit Value(std::shared_ptr<Binding::Expression>&& exprIn  ) : expr(std::move(exprIn)) {}

            Expression const&                    GetExpr() override { return *expr; }
            std::shared_ptr<Binding::Expression> expr;
        };

        using TFunc = std::shared_ptr<Binding::Expression> (TObject::*)(BindingContext& context, Binding::Expression const& val);

        size_t    GetKeyCount() override { return 0; }
        Str::Type GetKeyAt(size_t /*index*/) override { return {}; }
        Str::Type ComponentName() override { return Str::Create(Str::Value(base->ObjectTypeName()) + L"Tranform"); }

        std::shared_ptr<IValue> TryLookupValue(BindingContext& context, Str::View const& key) override
        {
            auto valout = base->TryLookupOrNull(context, key);
            SUPPRESS_WARNINGS_START
            SUPPRESS_CLANG_WARNING("-Wnrvo")
            if (valout == nullptr) { return valout; }
            SUPPRESS_WARNINGS_END
            assert(valout->GetType() == Type::Expr);
            auto newexpr = (obj.*func)(context, valout->GetExpr());
            return std::make_shared<Value>(std::move(newexpr));
        }

        TranformationBindableComponent(std::shared_ptr<BindableBase> const& baseIn, TObject& objIn LFTBND, TFunc funcIn) :
            base(baseIn), obj(objIn), func(funcIn)
        {}

        std::shared_ptr<BindableBase> base;
        TObject&                      obj;
        TFunc const                   func;
    };

    template <typename TObject, typename TFunc>
    void AddObjectWithTranform(std::shared_ptr<BindableBase> const& bindable, TObject& obj, TFunc func)
    { _transformations.push_back(std::make_shared<TranformationBindableComponent<TObject>>(bindable, obj, func)); }

    private:
    Str::Type                   _name;
    std::weak_ptr<BindableBase> _self;
    //   mutable IDLDebug::DebugContext                   _debugcontext;
    std::vector<std::shared_ptr<BindableBase>>       _baseptr;
    std::vector<std::shared_ptr<IBindableComponent>> _transformations;
    std::vector<std::shared_ptr<IBindableComponent>> _components;
};

template <typename TParent, typename TObject> struct BindableParent : public virtual BindableBase
{
    static constexpr Str::View BindingKeyName() { return TParent::BindingKeyName(); }

    struct BindableComponent : public IBindableComponent,
                               public ValueT<Type::Object>,
                               public std::enable_shared_from_this<BindableComponent>
    {
        explicit BindableComponent(BindableParent<TParent, TObject>& ownerIn LFTBND) : owner(ownerIn) {}
        size_t    GetKeyCount() override { return 3; }
        Str::Type GetKeyAt(size_t index) override
        {
            switch (index)
            {
            case 0: return Str::Create(L"Parent");
            case 1: return Str::Copy(ownerName);
            case 2: return Str::Copy(objectName);
            default: throw std::logic_error("Unexpected Key Index");
            }
        }

        Str::Type ComponentName() override
        { return Str::Create(L"BindableParent_" + Str::Value(ownerName) + L"_" + Str::Value(objectName)); }
        std::shared_ptr<IValue> TryLookupValue(BindingContext& /*context */, Str::View const& key) override
        {
            if (key == parent || key == ownerName) { return this->shared_from_this(); }
            if (key == objectName) { return this->shared_from_this(); }
            return {};
        }

        IBindable& GetBindable() override { return static_cast<TObject&>(owner).Parent(); }

        BindableParent<TParent, TObject>& owner;
        Str::Type                         parent{Str::Create(L"Parent")};
        Str::Type                         ownerName{Str::Create(TParent::BindingKeyName())};
        Str::Type                         objectName{Str::Create(TObject::BindingKeyName())};
    };
    SUPPRESS_WARNINGS_START
    SUPPRESS_MSVC_WARNING(4355)    // this used in base member initializer list
                                   // TODO remove this disable
    public:
    BindableParent() : bindableComponent(std::make_shared<BindableComponent>(*this)) { Register(bindableComponent); }

    public:
    SUPPRESS_WARNINGS_END

    CLASS_DELETE_COPY_AND_MOVE(BindableParent);

    std::shared_ptr<BindableComponent> bindableComponent;
    friend TObject;
};

template <typename TOwner, typename TObject> struct BindableObjectArray : public virtual BindableBase
{
    public:
    BindableObjectArray() { Register(bindableComponent); }

    ~BindableObjectArray() override = default;
    CLASS_DELETE_COPY_AND_MOVE(BindableObjectArray);

    void AddToArray(std::shared_ptr<BindableBase> const& obj) { bindableComponent->array.push_back(obj); }

    struct BindableComponent : public IBindableComponent,
                               public ValueT<Type::Array>,
                               public IValueArray,
                               public std::enable_shared_from_this<BindableComponent>

    {
        CLASS_DELETE_MOVE_AND_COPY_ASSIGNMENT(BindableComponent);

        BindableComponent() = default;
        size_t    GetKeyCount() override { return 1; }
        Str::Type GetKeyAt([[maybe_unused]] size_t index) override
        {
            assert(index == 0);
            return Str::Copy(key);
        }
        Str::Type               ComponentName() override { return Str::Copy(key); }
        std::shared_ptr<IValue> TryLookupValue(BindingContext& /*context */, Str::View const& keyIn) override
        {
            if (key == keyIn) { return this->shared_from_this(); }
            return {};
        }

        IValueArray& GetArray() LFTBND override { return *this; }
        size_t       GetCount() override { return array.size(); }
        IBindable&   GetObjectAt(size_t index) override { return *array[index]; }
        Str::Type    key = Str::Create(TObject::BindingKeyName());

        std::vector<std::shared_ptr<BindableBase>> array;
    };

    std::shared_ptr<BindableComponent> bindableComponent = std::make_shared<BindableComponent>();
    friend TOwner;
};

template <typename TObject, typename TReturnValue> struct GetterT;

template <typename TObject>
struct GetterT<TObject, IBindable&>
    : public ValueT<Type::Object>, public IValue::Getter, public std::enable_shared_from_this<GetterT<TObject, IBindable&>>
{
    using TFunc = IBindable& (TObject::*)() const;
    GetterT(TObject const& ptrIn LFTBND, TFunc funcIn) : func(funcIn), ptr(ptrIn) {}

    IBindable& GetBindable() override { return (ptr.*func)(); }

    std::shared_ptr<IValue> Get() override { return this->shared_from_this(); }

    TFunc const    func;
    TObject const& ptr;
};

template <typename TObject>
struct GetterT<TObject, std::shared_ptr<IBindable>>
    : public ValueT<Type::Object>, public IValue::Getter, public std::enable_shared_from_this<GetterT<TObject, std::shared_ptr<IBindable>>>
{
    using TFunc = std::shared_ptr<IBindable> (TObject::*)() const;
    GetterT(TObject const& ptrIn, TFunc funcIn) : func(funcIn), ptr(ptrIn) {}

    IBindable& GetBindable() override { return (ptr.*func)(); }

    std::shared_ptr<IValue> Get() override { return this->shared_from_this(); }

    TFunc const    func;
    TObject const& ptr;
};

template <typename TObject> struct GetterT<TObject, Str::Type> : public IValue::Getter
{
    using TFunc = Str::Type (TObject::*)() const;
    GetterT(TObject const& ptrIn LFTBND, TFunc funcIn) : ptr(ptrIn), func(funcIn) {}
    ~GetterT() override = default;
    struct ValueType : public ValueT<Type::String>, public std::enable_shared_from_this<ValueType>
    {
        explicit ValueType(Str::Type&& valueIn) : value(std::move(valueIn)) {}
        Str::Type const& GetString() LFTBND override { return value; }
        Str::Type        value;
    };

    std::shared_ptr<IValue> Get() override { return std::make_shared<ValueType>((ptr.*func)()); }

    TObject const& ptr;
    TFunc          func;
};

template <typename TObject>
struct GetterT<TObject, Binding::Expression const&>
    : public ValueT<Type::Expr>, public IValue::Getter, public std::enable_shared_from_this<GetterT<TObject, Binding::Expression const&>>
{
    using TFunc = Binding::Expression const& (TObject::*)() const;
    GetterT(TObject const& ptrIn LFTBND, TFunc funcIn) : func(funcIn), ptr(ptrIn) {}

    Binding::Expression const& GetExpr() override { return (ptr.*func)(); }
    std::shared_ptr<IValue>    Get() override { return this->shared_from_this(); }

    TFunc const    func;
    TObject const& ptr;
};

template <typename TObject> struct GetterT<TObject, Binding::Expression> : public ValueT<Type::Expr>, public IValue::Getter
{
    using TFunc = Binding::Expression (TObject::*)() const;
    GetterT(TObject const& ptrIn, TFunc funcIn) : ptr(ptrIn), func(funcIn) {}

    struct ValueType : public ValueT<Type::Expr>, public std::enable_shared_from_this<ValueType>
    {
        explicit ValueType(Expression&& valueIn) : value(std::move(valueIn)) {}
        Expression const& GetExpr() override { return value; }
        Expression        value;
    };

    std::shared_ptr<IValue> Get() override { return std::make_shared<ValueType>((ptr.*func)()); }

    TObject const& ptr;
    TFunc const    func;
};

// Inherit to expose well defined property names and their values
template <typename TOwner, typename TParent = TOwner> struct BindableT : public virtual BindableBase
{
    struct BindableComponent : IBindableComponent, std::enable_shared_from_this<BindableComponent>
    {
        BindableComponent(TParent const& ptrIn LFTBND, Str::Type&& keyIn, std::shared_ptr<IValue::Getter>&& getterIn) :
            ptr(ptrIn), key(std::move(keyIn)), getter(std::move(getterIn))
        {}
        size_t    GetKeyCount() override { return 1; }
        Str::Type GetKeyAt([[maybe_unused]] size_t index) override
        {
            assert(index == 0);
            return Str::Copy(key);
        }
        Str::Type               ComponentName() override { return Str::Copy(component); }
        std::shared_ptr<IValue> TryLookupValue(BindingContext& /*context */, Str::View const& keyIn) override
        { return key == keyIn ? getter->Get() : nullptr; }
        Str::Type                       component{Str::Create(TOwner::BindingKeyName())};
        TParent const&                  ptr;
        Str::Type                       key;
        std::shared_ptr<IValue::Getter> getter;
    };

    template <typename TFunc, typename... TArgs>
    void AddBindableComponents(TParent const& ptr, Str::Type&& key, TFunc func, TArgs&&... args)
    {
        AddBindableComponents(ptr, std::move(key), func);
        AddBindableComponents(ptr, std::forward<TArgs>(args)...);
    }

    template <typename TFunc> void AddBindableComponents(TParent const& ptr, Str::Type&& key, TFunc func)
    {
        bindableComponent.push_back(std::make_shared<BindableComponent>(
            ptr, std::move(key), std::make_shared<GetterT<TParent, decltype((ptr.*func)())>>(ptr, func)));
    }

    template <typename... TArgs> explicit BindableT(TArgs&&... args)
    {
        AddBindableComponents(SUPER(TOwner), std::forward<TArgs>(args)...);
        for (auto& c : bindableComponent) Register(c);
    }
    ~BindableT() override = default;
    CLASS_DELETE_COPY_AND_MOVE(BindableT);

    std::vector<std::shared_ptr<BindableComponent>> bindableComponent;
};

template <typename TOwner, typename TParent = TOwner> struct BindableDictionaryT : public virtual BindableBase
{
    private:
    explicit BindableDictionaryT(std::shared_ptr<AttributeMap> const& map)
    {
        if (map != nullptr) { AddAttributes(map); }
    }

    public:
    CLASS_DELETE_COPY_AND_MOVE(BindableDictionaryT);

    struct BindableComponent : public IBindableComponent
    {
        struct KeyValuePair : ValueT<Type::Expr>
        {
            KeyValuePair(Str::Type&& keyIn, std::shared_ptr<Binding::Expression>&& valueIn) :
                key(std::move(keyIn)), value(std::move(valueIn))
            {}
            Str::Type                            key;
            std::shared_ptr<Binding::Expression> value;
            Binding::Expression const&           GetExpr() override { return *value; }
        };

        explicit BindableComponent(std::shared_ptr<AttributeMap> const& mapIn)
        {
            if (mapIn == nullptr) return;
            for (auto const& it : mapIn->GetAttributes())
            {
                assert(!it.second->Empty());
                values.push_back(std::make_shared<KeyValuePair>(Str::Copy(it.first), Binding::Expression::Clone(it.second)));
            }
            for (auto& it : values) { map[Str::Copy(it->key)] = it; }
        }
        size_t                  GetKeyCount() override { return values.size(); }
        Str::Type               GetKeyAt(size_t index) override { return Str::Copy(values[index]->key); }
        Str::Type               ComponentName() override { return Str::Copy(component); }
        std::shared_ptr<IValue> TryLookupValue(BindingContext& /*context */, Str::View const& key) override
        {
            auto it = map.find(key.data());
            return it != map.end() ? it->second : nullptr;
        }

        std::vector<std::shared_ptr<KeyValuePair>>                   values;
        std::unordered_map<Str::Type, std::shared_ptr<KeyValuePair>> map;
        Str::Type                                                    component;
    };

    void AddAttributes(std::shared_ptr<AttributeMap> const& map)
    {
        bindableComponents.push_back(std::make_shared<BindableComponent>(map));

        Register(bindableComponents.back());
    }
    std::vector<std::shared_ptr<BindableComponent>> bindableComponents;
    friend TOwner;
};

}    // namespace Binding
SUPPRESS_WARNINGS_END
