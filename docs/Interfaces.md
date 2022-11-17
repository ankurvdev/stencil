# Interfaces

### Open questions

- How can interface include datastore as merely api-grouping?
- How would implementors choose datastore backend (DatabaseT, MysqlT, JsonStoreT)
- 

### Data-stores part of interface?
Status : Under consideration (Needs prototyping for linking with DatabaseT)

* Do not dictate backend (DatabaseT for eg) or else it's not an interface 
* CRUD
  * Create : No construction ? Start with always empty ?
  * Read : what Id system to use ?
  * Update : Transactions ? Do we just send a new object ? I'd system ?
  * Delete : I'd system ?
  * The interface itself is fairly tied to the backend
  * Does a uint32/64 Id work everywhere ?
 * if interface is templates on the implementor side. How would the client know the implementors 

#### Yes
  * Auto-generated 
    * CRUD interfaces : Less verbose, succinct, standardized
    * Client side CRUD code (javascript/c++ etc)
    * Remote synced mirror

#### No
  * RPC - would need explicit verbose CRUD interfaces
  * Bindings - would need explitict verbose CRUD interface 
  * SSE Sync - 
  * How would a programmer link the interface with DatabaseT ?
  * could the CRUD code be generated regardless of including it in interface ?
  * could the sync code be generated regardless ?
  * How would the sync code know what endpoint to use ? Manual init ?

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



### Object Store
```IDL
interface Foo {
    // ...
    objectstore DataObject dataobject1;
    objectstore Dataobject dataobject2;
    // ...
}
```
Adding object data Store to the interface doesn't add value.
Object Data Store can always be added to the services exposing the interface.

## Handwritten (Implementation)

```c++
struct FooImpl : public Foo<DatabaseT>
{
  using DataStoreT = DatabaseT; // Used by Foo to 
  
  virtual int AddNumbers(int arg1, int arg2) override 
  {
     RaiseEvent_Bar(arg1, "test");
     return arg1 + arg2 + _offset; 
  }
  
  int offset_{-1};
};

// Static functions
int Foo::DoSomething() { return -1; }

// Activators/Creators 
static std::unique_ptr<Foo> Foo::Create()
{
return new FooImpl();
};
static std::shared_ptr<Foo> Foo::Activate()
{
return new FooImpl();
};


```

## Code Generated Scenarios

### Language Bindings 

```Python
foo1 = new IFoo()
foo1.AddNumbers(1,2)
IFoo.DoSomething()
foo1.Bar.subscribe(callback)
foo2 = new IFoo()
```
Might be useful to have objectstore so we can do

```python
foo1.dataobjects.add(new DataObject(....))
foo1.dataobjects.get(id)
foo1.dataobjects.remove(...)
```

### Typescript SSE sync



## Design Notes

### Virtual functions (vs Linked Functions)
Status : Approved.
If you don't like virtual functions, go full static and manage threadlocal context.

#### Interface activation
Programmer provided functions:.
- `unique_ptr<> IInterface::Create` ( caller managed lifetime)
- `shared_ptr<> IInstance::Activate` (singleton)
- usually caller will choose one or another method of activation.

Pros (Virtual)
* Easier context. Avoid use of singletons or thread local context managers.
* separate static and virtual. So more choice.
* implementors can declare and contain data-stores.

Pros (Linked)
* make the class non instantiable
* how do brokers get access to the implementors 
* static doesn't make sense. Everything is static.
