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

namespace impl::Interface
{
template <typename TEventArgs> struct IEventHandler
{
    IEventHandler()          = default;
    virtual ~IEventHandler() = default;

    virtual void HandleEvent(TEventArgs const& args) = 0;
};

template <typename TImpl, typename TEventArgs> struct EventHandler : public IEventHandler<TEventArgs>
{
    EventHandler()           = default;
    ~EventHandler() override = default;

    virtual void HandleEvent(TEventArgs const& args) override { static_cast<TImpl*>(this)->OnEvent(args); }
};

template <typename T> struct _Transform;
template <typename... TInterfaceEventStructs> struct _Transform<std::tuple<TInterfaceEventStructs...>>
{
    using Type = std::tuple<IEventHandler<TInterfaceEventStructs>*...>;
};

template <ConceptInterface TInterface> struct InterfaceEventHandlers
{
    using Handlers = typename _Transform<typename Stencil::InterfaceTraits<TInterface>::EventStructs>::Type;
    Handlers handlers;
};

template <typename TImpl, typename TInterfaceEventStructs> struct _InterfaceEventHandlers;

template <typename TImpl, typename... TInterfaceEventStructs>
struct _InterfaceEventHandlers<TImpl, std::tuple<TInterfaceEventStructs...>> : EventHandler<TImpl, TInterfaceEventStructs>...
{};

template <typename TImpl, ConceptInterface TInterface>
struct InterfaceEventHandlerT : _InterfaceEventHandlers<TImpl, typename Stencil::InterfaceTraits<TInterface>::EventStructs>,
                                InterfaceEventHandlers<TInterface>
{
    InterfaceEventHandlerT()
    {
        std::apply([&](auto& handler) { handler = this; }, InterfaceEventHandlers<TInterface>::handlers);
    }
};

}    // namespace impl::Interface

template <typename T> struct InterfaceT
{
    template <typename TArgStruct> void RaiseEvent(TArgStruct const& args)
    {
        auto handlers = reinterpret_cast<impl::Interface::InterfaceEventHandlers<T>*>(handlerptr);
        std::get<impl::Interface::IEventHandler<TArgStruct>*>(handlers->handlers)->HandleEvent(args);
    }

    template <typename THandler> void SetHandler(THandler* ptr)
    {
        handlerptr = static_cast<impl::Interface::InterfaceEventHandlers<T>*>(ptr);
    }

    void* handlerptr{nullptr};
};

}    // namespace Stencil
