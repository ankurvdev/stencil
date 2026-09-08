#pragma once
#include "tree.h"
#include "typetraits.h"

#include <memory>

template <typename T> struct SharedTree
{
    using iterator = Tree<T>::iterator;

    iterator Rootbegin() const { return tree->rootbegin(); }
    iterator Rootend() const { return tree->rootend(); }
    auto     Children(iterator it) const { return tree->children(it); }
    iterator Addchild(iterator it, T&& data) { tree->addsibling(it, std::move(data)); }
    iterator Addsibling(iterator it, T&& data) { tree->addsibling(it, std::move(data)); }

    std::shared_ptr<Tree<T>> tree = std::make_shared<Tree<T>>();
};

template <typename T> struct Stencil::TypeTraits<SharedTree<T>>
{
    using Categories = std::tuple<Category::Indexable>;
};

template <Stencil::ConceptIndexable T> struct Stencil::TypeTraitsForIndexable<SharedTree<T>>
{
    enum Key
    {
        Id,
        Value,
        Children,
    };
};
