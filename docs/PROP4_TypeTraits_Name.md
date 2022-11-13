Every type must have a name

Current status : Rejected.

Variant-Type can be an tuple index int to cover all cases

```cpp
template <> struct TypeTraits<Foo>
{
    static constexpr std::string_view Name();
};
```

Why ?

Variants need it to figure out which one to latch onto

Why cant we have enums ?

Why even bother with variants as a class ? Why not treat each variant separately

Should we limit this to variant of structs ?

Variants of Variants ?
variant<variant<int, float>, double, size_t>

Should it just be index ?

Its a auto thing anyway ?
