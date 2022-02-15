# Runtime
- Cleanup - 2 days
- Variant - 2 days
- Cleanu - 2 days
- Maps - 2 days
- Cleanup - 2 days
- Object-Initialize / Visitor-Initialize (shared_ptr<>) - 3 days
- Cleanup - 2 days

- Command Line Parsing - 5 days
- Cleanup - 2 days
- Binary Packing / Unpacking - 2 days
- Cleanup 2 days
- String Packing / Unpacking - 2 days
- Codegen - 5 days

March - 20 

TODOs
base:
    Try concepts again with gcc11.2
    Use enums for Primitives, Indexable, Iterables

9. ObjectStore
10. Transactions
11. WebService

## Overview

The stencil runtime provides a reference implementation of the code-generator for some widely-used application

The code-generator itself isnt tied to the runtime.
Support for stencil runtime is provided via built-in templates, which can be easily replaced to have the code-generator generate code free from any dependencies.

The main scenarios that the runtime covers are

- Serialization / Deserialization
- Object Storage and CRUD access
- Transactions for object modification, history tracking, logging
- JSON / REST Web-Services based on interfaces

## Type-Traits

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

### PROP2: TypeTraitsForPrimitives, TypeTraitsForIterable and TypeTraitsForIndexable

Current State: Under Investigation

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

## Visitor

The Visitor specialized class

```c++
template<typename T> Stencil::Visitor;
```

provides functions for iterating over Iterables and Indexables

### Iterables

```c++
template<> Stencil::Visitor<Foo>
{

    static Iterator VisitStart(Foo& obj) {}
    static bool VisitNext(Iterator&it, Foo& obj) {}
};
```

### Indexables

```c++
template<> Stencil::Visitor<Foo>
{

    static void VisitKey(Foo& obj, Stencil::TypeTraits<Foo>::Key const& key, TLambda& lambda) { .. lambda(key, value); ....}
    static void VisitAllIndicies(Foo& obj,  TLambda& lambda) { ... lambda(key, value); ....}
};
```

## Primitives And Protocols

Primitives must rely upon specialization for translations to and from various formats like `String`, `Binary` etc.

Thought

- Protocols
  - Arent String and Binary Just different "Protocol" ? Protocol-String, Protocol-Binary
  - Are protocol limitied to Primitives ?
    - Protocols must cover Indexable and Iterables too ?
  - How to have the user specify overrides for only the protocols he needs ?
    - For a custom type if the user is not using Binary, shouldnt really have to provide a specializtion
    - If the user wants to make a Primitive to a Primitive<64> then that should be all thats really needed

- Stringify
  - JsonStringify might be different than CLI-Stringify
  - Sometimes stringified value must be wrapped in "". Depends on the representation
  - Leverage fmt ? How much

- Binary

- Complex Primtives and Large Primitives
  - Primitive64Bit class used for common types that can be wrapped in 64 bit
  - Should there be a Primitive32Bit class and Primitive128Bit ?
  - Should Primitive be templatized to size ? Primitive<32>, Primitive<64>
  - We need support for random types and structs. Is there value in specializing Primitive for 32/64/128 ?

### Stringifier

TODO :

```c++
template <typename T> struct Stringifier {
    static void Stringify(auto ostr, T const& obj);
};

```

### StringParse

### BinarySerializer

### BinaryDeserializer

## Helpers

To help out with various common scenarios certain helpers are provided

### Enums

All enums have a built-in helper

`Stencil::EnumTraits` specialization for enums is requires to specify the string-mapping and index mapping for enums

User can customize the Typing for their enums by specializiung the enum to override the built-in behavior (no overriding of EnumTraits required)

# TODO

# Next Steps

- Primitives
  - Assign
  - Move
  - Blob
  - String
  - 128 bit values

- Indexable
  - Variants