#pragma once
#include <tuple>

namespace Stencil
{

template <typename T> struct InterfaceTraits
{};
template <typename T> struct InterfaceApiTraits
{};
template <typename T> struct InterfaceObjectTraits
{};

template <typename T>
concept ConceptInterface = requires { typename Stencil::InterfaceTraits<T>; };

template <typename T> struct InterfaceT
{
    template <typename TImpl, typename TArgStruct> void RaiseEvent(TImpl& impl, TArgStruct const& args) { impl.OnEvent(args); }
};
}    // namespace Stencil
