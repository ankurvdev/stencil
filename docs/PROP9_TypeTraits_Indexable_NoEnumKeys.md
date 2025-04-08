Type Traits for Indexables uses empty-type-structs instead of enum for Keys

Pros:
Concatenation with Inherited Nested Types (Attributes) is easier

Cons:
Requires Variants for reading keys during ser-des

```
enum Fields {
    Invalid,
    Field1,
    Field2

};
using Key = Fields;
```

```
This can be replaced with

struct Field_1 {};
struct Field_2 {};

using Key = std::tuple<Field_1, Field_2>;
```

## Inherited Attributes

```
struct Foo : Base
{
    int a;
    int b;
};
```

struct TypeTraitsForIndexable<Foo>{
struct Field_a{};
struct Field_b{};
using Key = std::tuple_cat<std::tuple<Field_a, Field_b>>

};
TODO :

Dynamic Indexables : `std::unordered_map<Key, Value>`;
Named-Tuples : `struct NamedTuple { ... }`;
