# Interfaces

## Design Notes

### Virtual functions vs Linked Functions

Pros (Virtual)
* Easier context. Avoid use of singletons or thread local context managers.
* separate static and virtual. So more choice.
* implementors can declare and contain data-stores.

Pros (Linked)
* make the class non instantiable
* how do brokers get access to the implementors 
* static doesn't make sense. Everything is static.

### Data-stores part of interface?

* Why yes
  * we can generate client side CRUD code (javascript/c++ etc)
  * we can create protocol/network agnostic object sync code

* Why no
  * There's nothing to implement.
  * could the CRUD code be generated regardless of including it in interface ?
  * could the sync code be generated regardless ?

How would the sync code know what endpoint to use ? Manual init ?

If using virtual. The implementors can inherit markers to declare the data-stores they provide.

There should be a single data Store

Data-stores are optional 


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
    virtual int AddNumbers(int arg1, int arg2) =0;
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

Server (handwritten)

```c++
struct Svc :
      public Foo, 
      public WebSvc<Svc, Foo, DataStore>, 
      public BLESvc<Foo, DataStore>

{

};

```

The generated code should include client helpers (js, c++) for easy interfacing.
