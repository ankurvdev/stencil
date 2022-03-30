# Runtime
- Binary Protocol for Transactions and Mutators
- Transactions


- Protocol
  - Reader/Writer/Streams. Ability to allow protocol to dictate whats the preferred medium
    - Sometimes reading from buffers is faster than streams due to allowing random access and lookahead
    - List Examples
  - Read/Write overrides for Iterables / Indexables
- Code-Cleanup : Reduction : Protocol-String Protocol-Json
- Primitives32Bit Primitives128Bit
- TODOs
  - TypeTraits.Indexable.Key : Should FixedSize Structs also be allowed, how about any type ? Do we care ?
  - Visitor.Indexable.Key needs key to be a temp object outside data scope . How to get rid of it
    - Maybe : But we should validate the json array size and it matches the fixed size
  - StackVisitor: Find a better name


- Maps - 2 days
- Cleanup - 2 days
  - Rapidjson cmake issue
- Object-Initialize / Visitor-Initialize (shared_ptr<>) - 3 days
- Cleanup - 2 days

TODOs

9. ObjectStore
11. WebService

OpenQuestions

- Should Structs be assignable via indexable json

- TypeTraits
  - Categories(Primitive/Iterable/Indexable)
    - Can be multiple. Should we denote a preferred ?
    - Serialiation / Deserialization controlled using Visitors.
    - StackVisitor might not know which one to select.
    - Precedence Indexable > Iterable > Primitive doesnt always work
      - Iterable > Primitive breaks for GUIDs and std::array<char>
      - Indexale > Iterable breaks for ? [OPEN QUESTION]
      -
- Indexable:
  - Key :
  - Justification : [STRONG].
    - Everyone needs to know what indexable types are allowed
    - Can only be a primitive ?
    - What about Types with hash functions
    - Key Visitor requires storage of temp key object IndexableVisitorTypeHandler<T> has a mutable
  - ValueTypes:
    - Justification : [WEAK]
    - Unique only ?
    - What about duplicate ? Codegen wont be able to dedup. Templating technique available for auto dedup
    - Only needed because StackVisitor needs all Type Handlers instantiated . Can we make them ondemand ? [OPEN-QUESTION]
    -
  - Unhandled Types:
    - std::unordered_map<std::string, FixedSize>
    - std::unordered_map<uint64_t, FixedSize>
  -
- Iterable
  - Variable size: // Any length allowed
  - Fixed size : // Validate length (post / pre ?)
  -
  -
  - Primitives
  - Values (64-Bit)
  - Blobs (String/Wide-String)
  - Wht about GUIDs , Crypto-Shas ? 128 Bit ?
  - Custom Conversion Routines
  -
- StackVisitor
  - Rename to something else ?
  - All TypeHandlers for all nested types are available and instantiated
  - What about duplicate types ?
  - TypeTraits

  -

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
There are no Type-Trait requirements

```C++
template <typename T> struct TypeTraitsForIterable
{
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
