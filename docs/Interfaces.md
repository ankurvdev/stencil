# Interfaces

## Overview

Practical Scenarios

* RPC Function calls
* Web-Services
  * Object Store (CRUD)
  * Events (SSE)
  * REST Apis (JSON)
* Language Bindings and API projections for Python PHP etc

## Supported Member Types

### Events

```IDL
interface Foo {
    // ...

    event Bar(int arg1, string arg2);
    
    // ...
}
```

Implementation in C++ would have be inheriting from a virtual base class consisting of the following projected function available

```C++
    void RaiseEvent_Bar(int arg1, std::string const& arg2)
```

that can be used to notify clients of the event (along with args)

Client will use event handler registration / de-registration apis to recieve these notifications

For the Web-Service Projection framework the following Javascript snippet can be used to register for the event

```javascript

sse = new EventSource(serverprefix + '/foo/eventsource')

sse.addEventListener('bar', function (e: any) {
        console.log(e.data)
        console.log(JSON.parse(e.data).arg1)
        console.log(JSON.parse(e.data).arg2)
});

```

Other language / framework projections will have similiar 

### Object Data Store

```IDL
interface Foo {
    // ...

    objectstore DataObject dataobject;
    
    // ...
}
```

The Object store exposes CRUD (Create, Read, Update, Delete) apis for a given object through the interface.

The interface implementors can use the C++ projections of the object-store CRUD apis to make the same interactions that are made available to projection frameworks

```C++ 

auto id = this->Create_dataobject();

// Use Transactions to update objects 
auto& txn = this->Edit_dataobject(id);
txn.set_arg1(0);
// ...

this->Delete_dataobject(id);

```

```javascript

xhr.open('GET', serverprefix + '/foo/dataobject/create?arg1=0') // Create new object
xhr.open('GET', serverprefix + '/foo/dataobject/get?ids=id_12345') // Fetch multiple objects in a single request
xhr.open('GET', serverprefix + '/foo/dataobject/id_12345') // Fetch single object
xhr.open('GET', serverprefix + '/foo/dataobject/id_12345?arg1=1') // Edit and update and return single object
xhr.open('GET', serverprefix + '/foo/dataobject/delete?ids=id_12345') // Delete the objects
xhr.open('GET', serverprefix + '/foo/dataobject/all?arg1=1') // Get all objects

```

### Functions

```IDL
interface Foo {
    // ...

    int AddNumbers(int arg1, int arg2);
    
    // ...
}
```

The interface implementors must provide a override function that is invoked when the api invocation is requested by the client.

For the WebService framework projection, the client can use the snippet

```javascript

xhr.open('GET', serverprefix + '/foo/addnumbers?arg1=0&arg2=1')

```

