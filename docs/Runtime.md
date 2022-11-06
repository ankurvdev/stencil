# Runtime

- StackVisitor:
- CLI : Command Help and error messaging
- WebService :
  - DataStores and DataStores in WebService
- Transactions for Structs with more than 64 fields
- Timestampd : Created
- Types
  - Iterables : Improve Iteration Visitors
  - unordered_maps, unordered_sets std::unordered_map<std::string, FixedSize>,  std::unordered_map<uint64_t, FixedSize>
  - Explicit Initialization
  - Should structs be iterables ?
  - What about hetrogeneous keys in indexables ?
  - Bifurcation of iterables into Fixed-size iterables and growable iterables ?
- Visitor
  - Enforces a non-state model. Some visitors do have a state for eg: Variant(keys) or string-key visitors
- StackVisitor
  - Find a better name
  - Type-Handler Maps.Get rid of that. All TypeHandlers for all nested types are available and instantiated
- Protocol
  - Read/Write to iostreams

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

### Primitives

Apart for Conversion primitives, there are no type-traits requirements for Primitives

```C++
template <typename T> struct TypeTraitsForPrimitives
{
    // Nothing as of yet.
};
```

### Iterable

Iterables are required to specialize the visitor class for providing access to their iterable items
Iterable are currently presumed to be homogenous and must provide an ElementType.
This is currently used for Transactions and Visitors. This restriction may be removed in future.
use with caution

```C++
template <typename T> struct TypeTraitsForIterable
{
    typename ElementType;
    // Nothing as of yet.
};
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
