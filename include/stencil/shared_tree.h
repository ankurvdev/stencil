#pragma once
#include "tree.h"
#include "typetraits.h"

#include <memory>

template <typename T> struct shared_tree
{
    using iterator = typename tree<T>::iterator;

    iterator rootbegin() const { return _tree->rootbegin(); }
    iterator rootend() const { return _tree->rootend(); }
    auto     children(iterator it) const { return _tree->children(it); }
    iterator addchild(iterator it, T&& data) { _tree->addsibling(it, std::move(data)); }
    iterator addsibling(iterator it, T&& data) { _tree->addsibling(it, std::move(data)); }

    std::shared_ptr<tree<T>> _tree = std::make_shared<tree<T>>();
};

template <typename T> struct Stencil::TypeTraits<shared_tree<T>>
{
    using Categories = std::tuple<Category::Indexable>;
};

template <Stencil::ConceptIndexable T> struct Stencil::TypeTraitsForIndexable<shared_tree<T>>
{
    enum Key
    {
        Id,
        Value,
        Children,
    };
};
