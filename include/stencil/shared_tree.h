#pragma once
#include "tree.h"
#include "typetraits.h"

#include <memory>

template <typename T> struct SharedTree
{
    using iterator = tree<T>::iterator;

    iterator Rootbegin() const { return tobj->rootbegin(); }
    iterator Rootend() const { return tobj->rootend(); }
    auto     Children(iterator it) const { return tobj->children(it); }
    iterator Addchild(iterator it, T&& data) { tobj->addchild(it, std::move(data)); }
    iterator Addsibling(iterator it, T&& data) { tobj->addsibling(it, std::move(data)); }

    std::shared_ptr<tree<T>> tobj = std::make_shared<::tree<T>>();
};

template <typename T> struct Stencil::TypeTraits<SharedTree<T>>
{
    using Categories = std::tuple<Category::Indexable>;
};

template <Stencil::ConceptIndexable T> struct Stencil::TypeTraitsForIndexable<SharedTree<T>>
{
    enum class Key
    {
        Id,
        Value,
        Children,
    };
};
