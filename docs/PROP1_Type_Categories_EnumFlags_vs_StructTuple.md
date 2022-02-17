Should it be 

```
template <> struct Stencil::TypeTraits<Foo>
{
    using Categories = std::tuple<Stencil::Category::Primitive, Stencil::Category::Iterable>;
};

bool IsPrimitive() { return TupleHasType<Categories, Primitive>; }
```

OR 

```
template <> struct Stencil::TypeTraits<Foo>
{
    constexpr static auto Categories = Stencil::Category::Primitive | Stencil::Category::Iterable;
};

bool IsPrimitive() { return Categories & Primitive; }

```

Both constexprs 

Enum 
    Pros:
        easier switch cases
        Easier to extend and debug
    Cons:
        Tightness : Extra invalid bit fields 
        Inadvertant runtime cost.

Types
    Pros:
        Non mem/var based. Simply type info checks
        Compile-Time only

