# Visitor

The Visitor specialized class

```c++
template<typename T> Stencil::Visitor;
```

provides functions for iterating over Iterables and Indexables

## Iterables

```c++
template<> Stencil::Visitor<Foo>
{

    static Iterator VisitStart(Foo& obj) {}
    static bool VisitNext(Iterator&it, Foo& obj) {}
};
```

## Indexables

```c++
template<> Stencil::Visitor<Foo>
{

    static void VisitKey(Foo& obj, Stencil::TypeTraits<Foo>::Key const& key, TLambda& lambda) { .. lambda(key, value); ....}
    static void VisitAll(Foo& obj,  TLambda& lambda) { ... lambda(key, value); ....}
};
```

## Primitives

Primitives dont really need visitors
