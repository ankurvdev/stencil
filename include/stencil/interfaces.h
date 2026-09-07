#pragma once
#include <tuple>

namespace Stencil
{

template <typename T> struct InterfaceTraits;

template <typename T> struct InterfaceApiTraits
{};
template <typename T> struct InterfaceObjectTraits
{};

template <typename T>
concept ConceptInterface = requires {
    typename Stencil::InterfaceTraits<T>;
    typename Stencil::InterfaceTraits<T>::Interface;
};

template <typename T> struct InterfaceT
{
    template <typename TImpl, typename TArgStruct> void RaiseEvent(TImpl& impl, TArgStruct const& args) { impl.OnEvent(args); }
};

template <typename TImpl, ConceptInterface TInterface> struct InterfaceSvcTraits;

template <typename TImpl, ConceptInterface TInterface>
    requires std::is_base_of_v<typename Stencil::InterfaceTraits<TInterface>::Interface, TImpl>
struct InterfaceSvcTraits<TImpl, TInterface>
{
    static auto& QueryInterface(TImpl& impl LFTBND) { return *static_cast<Stencil::InterfaceTraits<TInterface>::Interface*>(&impl); }
};

}    // namespace Stencil
