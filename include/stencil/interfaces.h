#pragma once
namespace Stencil
{
template <typename T> struct InterfaceT
{
    template <typename TArgs> void RaiseEvent(TArgs const& args);
};

template <typename T> struct InterfaceTraits
{};
template <typename T> struct InterfaceApiTraits
{};
template <typename T> struct InterfaceObjectTraits
{};

}    // namespace Stencil