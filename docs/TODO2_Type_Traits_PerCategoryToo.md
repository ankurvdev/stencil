TODO:

Better static_asserts


### PROP2: TypeTraitsForPrimitives, TypeTraitsForIterable and TypeTraitsForIndexable

Current State: Complete

Separate Requirements enforced by Categories (Primitives, Iterables, Indexables) into their own specializations.
Indexable

- Key
Iterable
- ??
Primitive
- ??

```c++
struct Foo
{
  Bar bar1;
  Bar bar2;
};

template <> struct Stencil::TypeTraits<Foo>
{
    using Types = std::tuple<Type::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<Foo>
{
    enum class Fields {
        Invalid,
        Field_bar1,
        Field_bar2
    }
    // Key is a requirement 
    using Key = Fields;
};

```

Pros:

- Helps with Nested(Templated) TypeTraits like std::vector<T>, std::shared_ptr<T>
  - They can inherit TypeTraits from SubTypes without complex constexprs

Cons:
