# Interfaces

## Design Notes

### Virtual functions vs Linked Functions

Pros (Virtual)
* Easier context. Avoid use of singletons or thread local context managers.
* separate static and virtual. So more choice.

Pros (Linked)
* make the class non instantiable
* how do brokers get access to the implementors 
* static doesn't make sense. Everything is static.



## Overview

Practical Scenarios

* RPC Function calls
* Web-Services
* BLE services
* MQTT
* Language Bindings and API projections for Python PHP etc

## Components

### Events

```IDL
interface Foo {
    // ...
    event Bar(int arg1, string arg2);
    // ...
}
```

Translates to

```C++
struct Foo {
    void RaiseEvent_Bar(int arg1, std::string const& arg2)
}
```

### Functions

```IDL
interface Foo {
    // ...
    static int DoSomething();
    int DoSomethingContextual(int arg1, int arg2);
    // ...
}
```
Translates to

```c++
struct Foo {
    static int DoSomething();
    virtual int AddNumbers(int arg1, int arg2);
}
```

If an interface contain non static functions, the generated class must be implemented before it can be used.



### ~~~Object Data Store~~~
```IDL
interface Foo {
    // ...
    objectstore DataObject dataobject;
    // ...
}
```
Adding object data Store to the interface doesn't add value.
Object Data Store can always be added to the services exposing the interface.

## Brokers

### Web
```c++
struct Svc : public Foo, WebSvc<Foo, DataStore>, BLESvc<Foo, DataStore>
```
