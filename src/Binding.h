#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <memory>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <vector>

#if !defined TODO
#define TODO(...) throw std::logic_error("Not Implemented")
#endif

namespace Binding
{
template <typename TStr> struct StrOps;

template <> struct StrOps<std::wstring>
{
    using Type = std::wstring;
    using View = std::wstring_view;
    using Char = wchar_t;

    static constexpr auto InvalidIndex = std::wstring::npos;

    static std::wstring const& Value(Type const& str) { return str; }
    static std::wstring        Value(View const& str) { return std::wstring(str); }

    static Type Create(std::wstring_view str) { return Type(str); }
    static Type Copy(View const& str) { return Type(str); }
    static Type Convert(const char* in)
    {
        if (in == nullptr)
            return Type{};
        else
            return Convert(std::string_view(in));
    }
    static Type Convert(std::string_view const& in)
    {
        Type out;
        out.resize(in.size());
        std::transform(in.begin(), in.end(), out.begin(), [](char a) -> wchar_t { return a; });
        return out;
    }

    static Type ToLower(View const& in)
    {
        Type out;
        out.resize(in.size());
        std::transform(in.begin(), in.end(), out.begin(), [](int a) -> wchar_t { return static_cast<wchar_t>(tolower(a)); });
        return out;
    }
    static bool IEqual(View const& l, View const& r)
    {
        return std::equal(l.begin(), l.end(), r.begin(), r.end(), [](auto lc, auto rc) { return std::tolower(lc) == std::tolower(rc); });
    }
    static bool Equal(View const& l, View const& r) { return l == r; }
    static bool IsEmpty(View const& l) { return l.empty(); }

    static size_t Find(View const& l, Char ch, size_t startIndex = 0) { return l.find(ch, startIndex); }
    static size_t Find(View const& l, View const& substr, size_t startIndex = 0) { return l.find(substr, startIndex); }
    static View   SubString(View const& s, size_t startIndex, size_t endIndex = InvalidIndex) { return s.substr(startIndex, endIndex); }
    static size_t Size(View const& s) { return s.size(); }
};
using Str = StrOps<std::wstring>;

enum class Type
{
    String,
    Expr,
    Array,
    Object
};
//
// constexpr inline Str::View Str(const Type& type)
//{
//    switch (type)
//    {
//    case Type::String: return L"String";
//    case Type::Array: return L"Array";
//    case Type::Expr: return L"Expr";
//    case Type::Object: return L"Object";
//    }
//    return L"Unknown";
//}

struct BindingContext;
struct IValue;
struct Expression;

struct BindingExpr
{
    std::vector<Str::Type> binding;
};

struct Expression
{
    ~Expression() = default;
    Expression()  = default;

    Expression(Expression const&) = delete;
    Expression(Expression&& obj) { *this = std::move(obj); };

    Expression& operator=(Expression const&) = delete;

    Expression& operator=(Expression&& obj)
    {
        assert(obj._pieces.size() > 0);
        std::swap(_pieces, obj._pieces);
        return *this;
    };

    template <typename TFunc> Expression Evaluate(TFunc func) const
    {
        assert(!_empty());
        Expression newexpr;

        for (const auto& p : _pieces)
        {
            if (p.piecetype == Piece::PieceType::Expr)
            {
                std::optional<Expression> result = func(*p.expr);
                if (!result.has_value())
                {
                    newexpr._pieces.push_back(p.clone());
                    continue;
                }

                if (result.value().FullyEvaluated())
                {
                    newexpr.AddString(result.value().String());
                }
                else
                {
                    for (auto& p1 : result.value()._pieces)
                    {
                        if (p1.empty())
                        {
                            result = func(*p.expr);
                            throw std::logic_error("Invalid Result");
                        }
                        newexpr._pieces.push_back(p1.clone());
                    }
                }
            }
            else
            {
                newexpr.AddString(Str::Copy(p.text));
            }
        }

        assert(!newexpr._empty());
        return newexpr;
    }

    // Creation Methods
    void AddString(Str::Type&& val)
    {
        assert(!Str::IsEmpty(val));
        _pieces.emplace_back(Piece{Piece::PieceType::String, nullptr, std::move(val)});
    }

    void AddExpressionString(Str::View const& str, wchar_t sep)
    {
        Piece piece;
        piece.piecetype = Piece::PieceType::Expr;
        piece.expr      = std::make_unique<BindingExpr>();
        size_t start    = 0;
        auto   next     = Str::Find(str, sep);
        while (next != Str::InvalidIndex)
        {
            piece.expr->binding.push_back(Str::Create(Str::SubString(str, start, next - start)));
            start = next + 1;
            next  = Str::Find(str, sep, start);
        }
        piece.expr->binding.push_back(Str::Create(Str::SubString(str, start)));
        _pieces.push_back(std::move(piece));
    }

    void AddBindingExpression(std::unique_ptr<BindingExpr>&& expr)
    {
        assert(expr->binding.size() > 0);
        Piece piece{Piece::PieceType::Expr, std::move(expr), {}};
        _pieces.push_back(std::move(piece));
    }

    Expression Clone() const
    {
        Expression newexpr;
        newexpr._pieces.reserve(_pieces.size());
        for (auto& p : _pieces)
        {
            newexpr._pieces.push_back(p.clone());
        }
        return newexpr;
    }

    struct Piece
    {
        enum class PieceType
        {
            Expr,
            String
        } piecetype;

        std::unique_ptr<BindingExpr> expr;
        Str::Type                    text;

        bool empty() const
        {
            return (piecetype == PieceType::Expr && (expr == nullptr || expr->binding.size() == 0))
                   || (piecetype == PieceType::String && Str::IsEmpty(text));
        }

        Piece clone() const
        {

            std::vector<Str::Type> bindings;
            if (expr != nullptr)
            {
                for (auto& s : expr->binding)
                {
                    bindings.push_back(Str::Copy(s));
                }
            }
            return Piece{piecetype,
                         (expr == nullptr ? nullptr : std::unique_ptr<BindingExpr>(new BindingExpr{std::move(bindings)})),
                         Str::Copy(text)};
        }
    };

    std::vector<Piece> _pieces;

    // Expansion Method
    bool FullyEvaluated() const
    {
        for (auto& p : _pieces)
        {
            if (p.piecetype == Piece::PieceType::Expr)
            {
                return false;
            }
        }
        return true;
    }

    Str::Type String() const
    {
        std::wstringstream ss;
        for (auto& p : _pieces)
        {
            if (p.piecetype != Piece::PieceType::String)
            {
                throw std::runtime_error("Expression Has unevaluated bits");
            }
            assert(!Str::IsEmpty(p.text));
            ss << p.text;
        }
        return Str::Create(ss.str());
    }

    static auto Create(Str::View const& str, Str::View const& startMarker, Str::View const& endMarker, const wchar_t sep)
    {
        Expression expr;
        if (Str::IsEmpty(str) || Str::Size(str) == 0)
        {
            return expr;
        }
        size_t index = 0;
        while (index != Str::InvalidIndex)
        {
            auto markerstart = Str::Find(str, startMarker, index);
            if (markerstart == Str::InvalidIndex) break;
            if (index < markerstart) expr.AddString(Str::Create(Str::SubString(str, index, markerstart - index)));
            markerstart += Str::Size(startMarker);
            auto markerend = Str::Find(str, endMarker, markerstart);
            if (markerstart == Str::InvalidIndex) break;
            expr.AddExpressionString(Str::Create(Str::SubString(str, markerstart, markerend - markerstart)), sep);
            index = markerend + Str::Size(endMarker);
        }
        if (index < Str::Size(str))
        {
            expr.AddString(Str::Create(Str::SubString(str, index)));
        }
        return expr;
    }

    bool _empty() const
    {
        if (_pieces.size() == 0) return true;
        for (auto& p : _pieces)
        {
            if (p.empty()) return true;
        }
        return false;
    }

    bool empty() const { return _empty(); }
};    // namespace Binding

// Bindable Dictionary Object with Name and Bindable Named Values.
struct IBindable
{
    // TODO : Can this be Str::View ??
    virtual Str::Type                     ObjectTypeName() const                                                 = 0;
    virtual Str::Type                     ObjectId() const                                                       = 0;
    virtual std::shared_ptr<const IValue> TryLookupOrNull(BindingContext& context, Str::View const& param) const = 0;
};

// TODO : Do we really need it
struct IBindableComponent
{
    virtual ~IBindableComponent() = default;

    virtual size_t                        GetKeyCount() const                                                   = 0;
    virtual Str::Type                     GetKeyAt(size_t index) const                                          = 0;
    virtual Str::Type                     ComponentName() const                                                 = 0;
    virtual std::shared_ptr<const IValue> TryLookupValue(BindingContext& context, Str::View const& param) const = 0;
};

struct IValueArray
{
    struct Iterator
    {
        Iterator() {}
        Iterator(IValueArray const& ptr) : _ptr(ptr) { _CheckEnd(); }
        Iterator& operator++()
        {
            if (!_ptr.has_value()) return *this;
            _index++;
            _CheckEnd();
            return *this;
        }
        void _CheckEnd()
        {
            if (_index >= _ptr->get().GetCount())
            {
                _ptr.reset();
                _index = 0;
            }
        }

        bool operator==(const Iterator& rhs) const
        {
            return _index == rhs._index && _ptr.has_value() == rhs._ptr.has_value()
                   && (!_ptr.has_value() || &_ptr.value().get() == &rhs._ptr.value().get());
        }

        bool             operator!=(const Iterator& rhs) const { return !((*this) == rhs); }
        IBindable const& operator*() const { return _ptr->get().GetObjectAt(_index); }

        private:
        std::optional<std::reference_wrapper<const IValueArray>> _ptr;

        size_t _index{0};
    };

    virtual size_t           GetCount() const                = 0;
    virtual IBindable const& GetObjectAt(size_t index) const = 0;
};

template <typename TBegin, typename TEnd = TBegin> struct RangeT
{
    RangeT(TBegin begin, TEnd end) : _begin(begin), _end(end) {}
    TBegin begin() { return _begin; }
    TEnd   end() { return _end; }
    TBegin _begin;
    TEnd   _end;
};

template <typename TBegin, typename TEnd = TBegin> auto Range(TBegin begin, TEnd end)
{
    return RangeT<TBegin, TEnd>(begin, end);
}

struct IValue
{
    public:
    virtual ~IValue() = default;

    virtual Type               GetType() const     = 0;
    virtual Str::Type const&   GetString() const   = 0;
    virtual Expression const&  GetExpr() const     = 0;
    virtual IValueArray const& GetArray() const    = 0;
    virtual IBindable const&   GetBindable() const = 0;

    struct Getter
    {
        virtual ~Getter()                                 = default;
        virtual std::shared_ptr<const IValue> Get() const = 0;
    };
};

template <Type TType> struct ValueT;
template <> struct ValueT<Type::String> : IValue
{
    // virtual Str::Type const& GetString() const { throw std::logic_error(""); }

    virtual Type               GetType() const override { return Type::String; }
    virtual Expression const&  GetExpr() const override { throw std::logic_error(""); }
    virtual IValueArray const& GetArray() const override { throw std::logic_error(""); }
    virtual IBindable const&   GetBindable() const override { throw std::logic_error(""); }
};

template <Type TType> struct ValueT;
template <> struct ValueT<Type::Array> : IValue
{
    //  virtual IValueArray const & GetArray() const { throw std::logic_error(""); }

    virtual Type              GetType() const override { return Type::Array; }
    virtual Str::Type const&  GetString() const override { throw std::logic_error(""); }
    virtual Expression const& GetExpr() const override { throw std::logic_error(""); }
    virtual IBindable const&  GetBindable() const override { throw std::logic_error(""); }
};

template <Type TType> struct ValueT;
template <> struct ValueT<Type::Expr> : IValue
{
    // virtual Expression const&  GetExpr() const { throw std::logic_error(""); }

    virtual Type               GetType() const override { return Type::Expr; }
    virtual Str::Type const&   GetString() const override { throw std::logic_error(""); }
    virtual IValueArray const& GetArray() const override { throw std::logic_error(""); }
    virtual IBindable const&   GetBindable() const override { throw std::logic_error(""); }
};

template <Type TType> struct ValueT;
template <> struct ValueT<Type::Object> : IValue
{
    // virtual IBindable const&            GetBindable() const { throw std::logic_error(""); }

    virtual Type               GetType() const override { return Type::Object; }
    virtual Str::Type const&   GetString() const override { throw std::logic_error(""); }
    virtual Expression const&  GetExpr() const override { throw std::logic_error(""); }
    virtual IValueArray const& GetArray() const override { throw std::logic_error(""); }
};

// TODO : do we really need a AttributeMap class ?
struct AttributeMap
{
    void AddEntry(Str::Type name, Binding::Expression&& value) { _kvp[std::move(name)] = std::move(value); }

    const auto& GetAttributes() const { return _kvp; }

    private:
    std::unordered_map<Str::Type, Binding::Expression> _kvp;
};

struct BindingContext
{
    BindingContext(const BindingContext&) = delete;
    BindingContext(BindingContext&&)      = delete;
    BindingContext& operator=(BindingContext&&) = delete;
    BindingContext& operator=(const BindingContext&) = delete;
    BindingContext()                                 = default;

    auto GetRange(IBindable const& ptr, BindingExpr const& expr)
    {
        struct Range
        {
            auto begin() { return IValueArray::Iterator(value->GetArray()); }
            auto end() { return IValueArray::Iterator(); }

            std::shared_ptr<const IValue> value;
        };
        return Range{EvaluateArray(ptr, expr)};
    }

    bool _FindBindableByNameInContextStack(Str::View const& name, std::reference_wrapper<const IBindable>& ptr)
    {
        for (auto it = _stack.begin(); it != _stack.end(); ++it)
        {
            auto bindableName = (*it).get().ObjectTypeName();
            if (Str::Equal(bindableName, name))
            {
                ptr = *it;
                return true;
            }
        }
        return false;
    }

    std::shared_ptr<const IValue> _TryEvaluateBindingExprOrNull(BindingExpr const& expr)
    {
        assert(expr.binding.size() > 0);
        if (expr.binding.size() == 1)
        {
            auto val = _stack.back().get().TryLookupOrNull(*this, expr.binding.back());
            if (val->GetType() == Type::Object)
            {
                throw std::logic_error("WTF");
                // _stack.push_back(val->GetBindable());
            }
            return val;
        }

        std::reference_wrapper<const IBindable> ptr = _stack.back();

        auto bit = expr.binding.begin();
        if (_FindBindableByNameInContextStack(*bit, ptr)) ++bit;

        std::shared_ptr<const IValue> val;
        for (; bit != expr.binding.end(); ++bit)
        {
            if (val != nullptr)
            {
                ptr = val->GetBindable();
            }
            val = ptr.get().TryLookupOrNull(*this, *bit);
            if (val == nullptr)
            {
                return nullptr;
            }
        }

        return val;
    }

    Expression _EvaluateExpression(Expression const& expr)
    {
        auto result = expr.Evaluate([this](BindingExpr const& expr) -> std::optional<Expression> {
            auto value = _TryEvaluateBindingExprOrNull(expr);
            if (value == nullptr)
            {
                return {};
            }
            switch (value->GetType())
            {
            case Type::String:
            {
                Expression newexpr;
                newexpr.AddString(Str::Copy(value->GetString()));
                return newexpr;
            }
            case Type::Expr: return value->GetExpr().Clone();
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
        Scope(BindingContext& context, IBindable const& ptr) : Scope(context._stack, ptr) {}
        Scope(std::vector<std::reference_wrapper<const IBindable>>& stack, IBindable const& ptr) : _stack(stack)
        {
            _stack.push_back(std::cref(ptr));
        }
        ~Scope() { _stack.pop_back(); }
        std::vector<std::reference_wrapper<const IBindable>>& _stack;
    };

    std::shared_ptr<const IValue> EvaluateArray(IBindable const& ptr, BindingExpr const& expr)
    {
        Scope scope(_stack, ptr);
        auto  val = _TryEvaluateBindingExprOrNull(expr);
        if (val == nullptr)
        {
            throw std::logic_error("Unable to Evaluate");
        }
        assert(val->GetType() == Type::Array);
        return val;
    }

    Expression EvaluateExpression(IBindable const& ptr, Expression const& expr)
    {
        Scope scope(_stack, ptr);
        return _EvaluateExpression(expr);
    }

    std::shared_ptr<const IValue> TryEvaluateBindingExprOrNull(IBindable const& ptr, BindingExpr const& expr)
    {
        Scope scope(_stack, ptr);
        return _TryEvaluateBindingExprOrNull(expr);
    }

    private:
    std::vector<std::reference_wrapper<const IBindable>> _stack;
};    // namespace Binding

struct BindableBase : public IBindable, public ValueT<Type::Object>
{
    void AddBaseObject(std::shared_ptr<const BindableBase> const& basePtr)
    {
        // assert(basePtr != nullptr);
        _baseptr.push_back(basePtr);
    }

    void Register(std::shared_ptr<const IBindableComponent> const& obj) { _components.push_back(obj); }

    IBindable const& GetBindable() const override { return *this; }

    virtual std::shared_ptr<const IValue> TryLookupOrNull(BindingContext& context, Str::View const& param) const override
    {
        auto val = _TryLookupOrNull(context, param);
        if (val != nullptr && val->GetType() == Type::Expr)
        {
            struct Value : ValueT<Type::Expr>
            {
                Expression _expr;
                Value(Expression&& expr) : _expr(std::move(expr)) {}
                Expression const& GetExpr() const override { return _expr; }
            };

            return std::make_shared<Value>(context.EvaluateExpression(*this, val->GetExpr()));
        }
        return val;
    }

    std::shared_ptr<const IValue> _TryLookupOrNull(BindingContext& context, Str::View const& param) const
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

    virtual Str::Type ObjectId() const override { return Str::Copy(_name); }

    void SetName(Str::Type&& name) { std::swap(_name, name); }

    template <typename TObject> struct TranformationBindableComponent : public IBindableComponent
    {
        struct Value : public ValueT<Type::Expr>
        {
            Value(Expression&& expr) : _expr(std::move(expr)) {}

            virtual Expression const& GetExpr() const override { return _expr; }
            Expression                _expr;
        };

        typedef Expression (TObject::*TFunc)(BindingContext& context, Binding::Expression const& val) const;

        virtual size_t    GetKeyCount() const override { return 0; }
        virtual Str::Type GetKeyAt(size_t /*index*/) const override { return Str::Type(); }
        virtual Str::Type ComponentName() const override { return Str::Create(Str::Value(_base->ObjectTypeName()) + L"Tranform"); }

        virtual std::shared_ptr<const IValue> TryLookupValue(BindingContext& context, Str::View const& key) const override
        {
            auto valout = _base->TryLookupOrNull(context, key);
            if (valout == nullptr)
            {
                return valout;
            }
            assert(valout->GetType() == Type::Expr);
            auto newexpr = (_obj.*_func)(context, valout->GetExpr());
            return std::make_shared<Value>(std::move(newexpr));
        }

        TranformationBindableComponent(std::shared_ptr<const BindableBase> base, const TObject& obj, TFunc func) :
            _base(base), _obj(obj), _func(func)
        {
        }

        std::shared_ptr<const BindableBase> _base;
        const TObject&                      _obj;
        const TFunc                         _func;
    };

    template <typename TObject, typename TFunc>
    void AddObjectWithTranform(std::shared_ptr<const BindableBase> bindable, const TObject& obj, TFunc func)
    {
        _transformations.push_back(std::make_shared<TranformationBindableComponent<TObject>>(bindable, obj, func));
    }

    private:
    Str::Type                   _name;
    std::weak_ptr<BindableBase> _self;
    //   mutable IDLDebug::DebugContext                   _debugcontext;
    std::vector<std::shared_ptr<const BindableBase>>       _baseptr;
    std::vector<std::shared_ptr<const IBindableComponent>> _transformations;
    std::vector<std::shared_ptr<const IBindableComponent>> _components;
};

template <typename TParent, typename TObject> struct BindableParent : public virtual BindableBase
{
    static constexpr Str::View BindingKeyName() { return TParent::BindingKeyName(); }

    struct BindableComponent : public IBindableComponent,
                               public ValueT<Type::Object>,
                               public std::enable_shared_from_this<BindableComponent>
    {
        BindableComponent(TObject& obj) : _owner(obj) {}
        virtual size_t    GetKeyCount() const override { return 3; }
        virtual Str::Type GetKeyAt(size_t index) const override
        {
            switch (index)
            {
            case 0: return Str::Create(L"Parent");
            case 1: return Str::Copy(_ownerName);
            case 2: return Str::Copy(_objectName);
            default: throw std::logic_error("Unexpected Key Index");
            }
        }

        virtual Str::Type ComponentName() const override
        {
            return Str::Create(L"BindableParent_" + Str::Value(_ownerName) + L"_" + Str::Value(_objectName));
        }
        virtual std::shared_ptr<const IValue> TryLookupValue(BindingContext& /*context */, Str::View const& key) const override
        {
            if (key == _parent || key == _ownerName)
            {
                return this->shared_from_this();
            }
            if (key == _objectName)
            {
                return this->shared_from_this();
            }
            return {};
        }

        virtual IBindable const& GetBindable() const override { return _owner.Parent(); }
        Str::Type                _parent{Str::Create(L"Parent")};
        Str::Type                _ownerName{Str::Create(TParent::BindingKeyName())};
        Str::Type                _objectName{Str::Create(TObject::BindingKeyName())};
        TObject&                 _owner;
    };

    BindableParent(TObject& obj) : _bindableComponent(std::make_shared<BindableComponent>(obj)) { Register(_bindableComponent); }

    std::shared_ptr<BindableComponent> _bindableComponent;
};

template <typename TOwner, typename TObject> struct BindableObjectArray : public virtual BindableBase
{
    BindableObjectArray(TOwner& obj) : _bindableComponent(std::make_shared<BindableComponent>(obj)) { Register(_bindableComponent); }

    void AddToArray(std::shared_ptr<const BindableBase> obj) { _bindableComponent->_array.push_back(obj); }

    struct BindableComponent : public IBindableComponent,
                               public ValueT<Type::Array>,
                               public IValueArray,
                               public std::enable_shared_from_this<BindableComponent>

    {
        BindableComponent(TOwner& obj) : _obj(obj) {}
        virtual size_t    GetKeyCount() const override { return 1; }
        virtual Str::Type GetKeyAt([[maybe_unused]] size_t index) const override
        {
            assert(index == 0);
            return Str::Copy(_key);
        }
        virtual Str::Type                     ComponentName() const override { return Str::Copy(_key); }
        virtual std::shared_ptr<const IValue> TryLookupValue(BindingContext& /*context */, Str::View const& key) const override
        {
            if (key == _key)
            {
                return this->shared_from_this();
            }
            return {};
        }

        IValueArray const& GetArray() const override { return *this; }
        size_t             GetCount() const override { return _array.size(); }
        IBindable const&   GetObjectAt(size_t index) const override { return *_array[index]; }
        Str::Type          _key = Str::Create(TObject::BindingKeyName());

        std::vector<std::shared_ptr<const BindableBase>> _array;
        TOwner&                                          _obj;
    };
    std::shared_ptr<BindableComponent> _bindableComponent;
};

template <typename TObject, typename TReturnValue> struct GetterT;

template <typename TObject>
struct GetterT<TObject, IBindable const&>
    : public ValueT<Type::Object>, public IValue::Getter, public std::enable_shared_from_this<GetterT<TObject, IBindable const&>>
{
    typedef IBindable const& (TObject::*TFunc)() const;
    GetterT(const TObject& ptr, TFunc func) : _ptr(ptr), _func(func) {}

    virtual IBindable const& GetBindable() const override { return (_ptr.*_func)(); }

    virtual std::shared_ptr<const IValue> Get() const override { return this->shared_from_this(); }
    TFunc                                 _func;
    const TObject&                        _ptr;
};

template <typename TObject> struct GetterT<TObject, Str::Type> : public IValue::Getter
{
    typedef Str::Type (TObject::*TFunc)() const;
    GetterT(const TObject& ptr, TFunc func) : _ptr(ptr), _func(func) {}

    struct ValueType : public ValueT<Type::String>, public std::enable_shared_from_this<ValueType>
    {
        ValueType(Str::Type&& value) : _value(std::move(value)) {}
        virtual Str::Type const& GetString() const override { return _value; }
        Str::Type                _value;
    };

    virtual std::shared_ptr<const IValue> Get() const override { return std::make_shared<ValueType>((_ptr.*_func)()); }

    const TObject& _ptr;
    TFunc          _func;
};

template <typename TObject>
struct GetterT<TObject, Binding::Expression const&>
    : public ValueT<Type::Expr>, public IValue::Getter, public std::enable_shared_from_this<GetterT<TObject, Binding::Expression const&>>
{
    typedef Binding::Expression const& (TObject::*TFunc)() const;
    GetterT(const TObject& ptr, TFunc func) : _ptr(ptr), _func(func) {}

    virtual Binding::Expression const&    GetExpr() const override { return (_ptr.*_func)(); }
    virtual std::shared_ptr<const IValue> Get() const override { return this->shared_from_this(); }

    TFunc          _func;
    const TObject& _ptr;
};

template <typename TObject> struct GetterT<TObject, Binding::Expression> : public ValueT<Type::Expr>, public IValue::Getter
{
    typedef Binding::Expression (TObject::*TFunc)() const;
    GetterT(const TObject& ptr, TFunc func) : _ptr(ptr), _func(func) {}

    struct ValueType : public ValueT<Type::Expr>, public std::enable_shared_from_this<ValueType>
    {
        ValueType(Expression&& value) : _value(std::move(value)) {}
        virtual Expression const& GetExpr() const override { return _value; }
        Expression                _value;
    };

    virtual std::shared_ptr<const IValue> Get() const override { return std::make_shared<ValueType>((_ptr.*_func)()); }

    const TObject& _ptr;
    TFunc          _func;
};

// Inherit to expose well defined property names and their values
template <typename TOwner, typename TParent = TOwner> struct BindableT : public virtual BindableBase
{
    struct BindableComponent : IBindableComponent, std::enable_shared_from_this<BindableComponent>
    {
        BindableComponent(const TParent& ptr, Str::Type&& key, std::shared_ptr<IValue::Getter>&& getter) :
            _ptr(ptr), _key(std::move(key)), _getter(std::move(getter))
        {
        }
        virtual size_t    GetKeyCount() const override { return 1; }
        virtual Str::Type GetKeyAt([[maybe_unused]] size_t index) const override
        {
            assert(index == 0);
            return Str::Copy(_key);
        }
        virtual Str::Type                     ComponentName() const override { return Str::Copy(_component); }
        virtual std::shared_ptr<const IValue> TryLookupValue(BindingContext& /*context */, Str::View const& key) const override
        {
            return key == _key ? _getter->Get() : nullptr;
        }
        Str::Type                       _component{Str::Create(TOwner::BindingKeyName())};
        const TParent&                  _ptr;
        Str::Type                       _key;
        std::shared_ptr<IValue::Getter> _getter;
    };

    template <typename TFunc, typename... TArgs>
    void _AddBindableComponents(const TParent& ptr, Str::Type&& key, TFunc func, TArgs&&... args)
    {
        _AddBindableComponents(ptr, std::move(key), func);
        _AddBindableComponents(ptr, std::forward<TArgs>(args)...);
    }

    template <typename TFunc> void _AddBindableComponents(const TParent& ptr, Str::Type&& key, TFunc func)
    {
        _bindableComponent.push_back(std::make_shared<BindableComponent>(
            ptr, std::move(key), std::make_shared<GetterT<TParent, decltype((ptr.*func)())>>(ptr, func)));
    }

    template <typename... TArgs> BindableT(const TParent& ptr, TArgs&&... args)
    {
        _AddBindableComponents(ptr, std::forward<TArgs>(args)...);
        for (auto& c : _bindableComponent) Register(c);
    }

    std::vector<std::shared_ptr<BindableComponent>> _bindableComponent;
};

template <typename TOwner, typename TParent = TOwner> struct BindableDictionaryT : public virtual BindableBase
{
    BindableDictionaryT(TOwner& /*owner*/, std::shared_ptr<AttributeMap> map)
    {
        if (map != nullptr)
        {
            AddAttributes(map);
        }
    }

    struct BindableComponent : public IBindableComponent
    {
        struct KeyValuePair : ValueT<Type::Expr>
        {
            KeyValuePair(Str::Type&& keyIn, Binding::Expression&& valueIn) : key(std::move(keyIn)), value(std::move(valueIn)) {}
            Str::Type                          key;
            Binding::Expression                value;
            virtual const Binding::Expression& GetExpr() const override { return value; }
        };

        BindableComponent(std::shared_ptr<AttributeMap> map)
        {
            if (map == nullptr) return;
            for (const auto& it : map->GetAttributes())
            {
                assert(!it.second.empty());
                _values.push_back(std::make_shared<KeyValuePair>(Str::Copy(it.first), it.second.Clone()));
            }
            for (auto& it : _values)
            {
                _map[Str::Copy(it->key)] = it;
            }
        }
        virtual size_t                        GetKeyCount() const override { return _values.size(); }
        virtual Str::Type                     GetKeyAt(size_t index) const override { return Str::Copy(_values[index]->key); }
        virtual Str::Type                     ComponentName() const override { return Str::Copy(_component); }
        virtual std::shared_ptr<const IValue> TryLookupValue(BindingContext& /*context */, Str::View const& key) const override
        {
            auto it = _map.find(key.data());
            return it != _map.end() ? it->second : nullptr;
        }

        std::vector<std::shared_ptr<KeyValuePair>>                   _values;
        std::unordered_map<Str::Type, std::shared_ptr<KeyValuePair>> _map;
        Str::Type                                                    _component{};
    };

    void AddAttributes(std::shared_ptr<AttributeMap> map)
    {
        _bindableComponents.push_back(std::make_shared<BindableComponent>(map));

        Register(_bindableComponents.back());
    }
    std::vector<std::shared_ptr<BindableComponent>> _bindableComponents;
};

}    // namespace Binding
