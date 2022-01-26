#pragma once
#include "visitor.h"

namespace Stencil
{
struct TypeHandler
{
    virtual ~TypeHandler() = default;
};

template <typename T> struct _StackVisitor
{
    _StackVisitor(Visitor<T>& visitor) : _root(&visitor) {}

    template <typename T> void AddKey(T const& /*k*/) {}
    template <typename T> void Assign(T const& /*k*/) {}

    void Pop() {}
    void Add() {}

    // TODO: Check if maxdepth available for T and optimize visitor without vector
    std::vector<TypeHandler> _stack;
    Visitor<T>*              _root{nullptr};
};

template <typename T> using StackVisitor = _StackVisitor<T>;

}    // namespace Stencil