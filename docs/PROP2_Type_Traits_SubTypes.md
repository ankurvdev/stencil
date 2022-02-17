
### PROP1: SubTypes

Current State: Denied : Need more justification.

Enforce Sub-Type declaration in Type-Traits

```c++
struct Foo
{
  Bar bar1;
  Bar bar2;
};

template <> struct Stencil::TypeTraits<Foo>
{
    // Preferable Precedence order : Indexable > Iterable > Primitive
    using SubTypes = std::tuple<Bar>;
};
```

Pros:

- A TypeTrait should be sufficient to accumulate recursively all the nested types and instantiated visitors on the stack with memory allocation
- Helps with SAX Parsers like JSON
  - Heaps for Types: Without this the SAX parser would have to create Visitor-Types on demand and as encountered straining the heap
  - [Debatable] Determine Max depth: With Type-Hierachy determined at compile-time. Max Stack depth can be potentially determined at compile-time too
    - What about cycles in Type-Graph

Cons:

- Additional requirement. Not strictly necessary
- The information is derivable from Visitor Visit Return Types;

Should it be optional ?
No: Eliminates the biggest pro. No point introducing if we can find it everywhere

What about primitives ?
No: Not needed, it a primitive is exposed as an iterable. Sub-Type is available there

Most of the concepts below can be found in the runtime base class `stencil/base.h`

All types must be

- Trivially constructible (no custom constructors)
- Assignable/Moveable

Type-Traits are specified by specializing the following Traits struct

```c++
template <typename T> struct Stencil::TypeTraits;
```

For a given type

There are three main categories of types

- [Primitives](#primitives)
- [Iterable](#iterable)
- [Indexable](#indexable)

Types can be tagged with multiple categories with a preference ordering

- For eg: Structs can be Iterable (field indices) and Indexable but they prefer to be indexable (by field names)
- unordered_maps are only indexable not iterable
- UUIds can be both primitives or iterable fixed size char-arrays (preferred as primitives)

Iterable and Indexable Types also require specialization of the [Visitor](Visitor.md) class to traverse through their contents/elements

```c++
template <> struct Stencil::TypeTraits<Foo>
{
    // Preferable Precedence order : Indexable > Iterable > Primitive
    using Types = std::tuple<Stencil::Type::Indexable, Stencil::Type::Iterable, Stencil::Type::Primitive>
};
```
