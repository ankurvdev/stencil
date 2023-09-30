#pragma once
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

template <typename TImpl, typename TInterfaceEventStruct> struct _InterfaceEventHandlers;

template <typename TImpl, typename... TInterfaceEventStructs>
struct _InterfaceEventHandlers<TImpl, std::tuple<TInterfaceEventStructs...>> : public EventHandler<TImpl, TInterfaceEventStructs>...
{};

template <typename TImpl, typename TInterface>
struct InterfaceEventHandlerT : _InterfaceEventHandlers<TImpl, typename Stencil::InterfaceTraits<TInterface>::EventStructs>
{};

template <typename TInterfaceEventStruct> struct _IInterfaceEventHandlers;

template <typename... TInterfaceEventStructs>
struct _IInterfaceEventHandlers<std::tuple<TInterfaceEventStructs...>> : public IEventHandler<TInterfaceEventStructs>...
{};
}    // namespace impl::Interface

template <typename T> struct InterfaceT
{
    template <typename TArgStruct> void RaiseEvent(TArgStruct const& args)
    {
        if (handler != nullptr)
        {
            auto ptr
                = reinterpret_cast<impl::Interface::_IInterfaceEventHandlers<typename Stencil::InterfaceTraits<T>::EventStructs>*>(handler);
            (static_cast<impl::Interface::IEventHandler<TArgStruct>*>(ptr))->HandleEvent(args);
        }
    }

    void* handler{nullptr};
};

}    // namespace Stencil
