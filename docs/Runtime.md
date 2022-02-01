# Runtime

## Overview

The stencil runtime provides a reference implementation of the code-generator for some widely-used application

The code-generator itself isnt tied to the runtime, but the built-in templates are.

A user is free to replace the built-in templates with their own custom templates that generate code free from any dependencies on the Stencil runtime.

The main scenarios that the runtime covers are

- Serialization / Deserialization
- Object Storage and CRUD access
- Transactions for object modification, history tracking, logging
- JSON / REST Web-Services based on interfaces

## Type-Traits

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

### Primitives

All primitive types must provide Assignment and Move operations from other supported src primitives like integers, string_view etc.

TODO

### Iterable

Provide iterable access to its elements.
Can be hetrogenous or homogenous

Requirements (TODO : Is it really though)

```C++
ElementTypes
```

### Indexable

Provides random access to its elements referened by a Key
A Key can only be of Primitive Type (TODO : Open Question)

For example in a typical struct

```C++
struct Foo
{
  Bar bar1;
  Bar bar2;
};

template<> struct Stencil::TypeTraits<Foo>
{
    enum class Fields {
        Invalid,
        Field_bar1,
        Field_bar2
    }

    using Key = Fields;
};

```
## [Pending Proposals]

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


## Helpers

To help out with various common scenarios certain helpers are provided

### Enums

All enums have a built-in helper

`Stencil::EnumTraits` specialization for enums is requires to specify the string-mapping and index mapping for enums

User can customize the Typing for their enums by specializiung the enum to override the built-in behavior (no overriding of EnumTraits required)

# TODO

- Multiple Categories
  - Precedence Order : Untested, possibly while implementing CLI
- Do we need ElementTypes ? Can we avoid this overhead ?

# Next Steps

- Primitives
  - Assign
  - Move
  - Blob
  - String
  - 128 bit values

-
