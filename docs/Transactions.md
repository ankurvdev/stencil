# Transactions

## Overview

Transactions can be used for the following scenarios, mainly to deal with sparse changes to object models.

1. Object Model listeners
2. Generate Events / Send RPC messages with delta changes to Object models
3. Optimized Record and Replay Object model changes with sparse delta
4. Logging Object model changes
5. Suppress / Avoid non-change edits

In general the use-cases involve the following steps

1. Start Transaction
2. Make changes to objects and sub-objects
   - Change values
   - Add remove list values
   - Edit sub-structs
3. End Transaction
4. Notify listeners with transaction data

with the following characteristics

- Listeners notified at the end of the transaction
  - Notification generated only if the transaction actually changes object model
  - No notification for empty or non-altering edits (assigning with same values)
- Notification involves sending TransactionRecordData and Object Model itself with a read-lock
- Allow Serialization/Deserialization of TransactionRecordData (TransactionRecordSerDes)
  - Binary Serialization for Trace recording
  - Text Serialization for logging/debugging
- TransactionRecordData can be replayed by deserializing Text based TransactionRecordData and applying it to an Object
  model.
- Apply TransactionRecordData notifies listeners as well.

Dependencies

- Visitor (Apply TransactionRecordData and Replay, Serialization of TransactionRecordData)
- JsonSerDes
- BinarySerDes

Use-cases

```c++
struct SubStruct
{
    int     val1;
    string  val2;
}

struct ListObject
{
    int val
}

struct Foo
{
    SubStruct        child1;
    list<ListObject> list1;
    int              val3;
}
```

Generating a transaction

```c++
Foo foo;
{
    auto txn = Transaction<Foo>(foo);
    txn.child1().set_val1(1);
    txn.child1().set_val2("newvalue");
    txn.add_list1({val = 1});
    txn.add_list1({val = 2});
    txn.set_val3(2);
}
```

## Dev Notes

### Multiple levels of recordings

- Tagging: previous state lost. New state marked as changed
- History: previous state available. Undo support.
- Visitor: ability to walk /visit changes.
- Streaming: no visitation after changes
  - What's the use case ?

### Features

1. Recursive (Nesting). For codegen support (structs)
2. Primitives need to be empty. Struct members tagged via bitmask ( Parent support required )
3. Editing objects via visitors (serialization/deserialization) support
4. Detect no-changes for primitive values. (Recursively)
    - Testcase : deserializing twice into same obj should produce empty txn on the second attempt.
    - What to do for lists
5. Serializing /deserializing into patches via protocol ( json, string, binary)

### Question

#### `Transaction<T, TOwner> + Transaction<...>::State`

- `Constructor`: 
  - `Transaction(TOwner& owner, State& state, Accessor fn)`
  - `Transaction(State& state, Accessor fn)`

- `TObj& Obj()` : fn(owner, state) -> TObj&
  - `vector` : State contains index which can be used to dereference into the object
  - `unordered_map` : State contains key
  - `primitives` : State is empty
  - `struct`:

##### How to access object from the Transaction

A Transaction doesnt directly have access to the object.
Reason: The object could be a managed object and so parents could move pointer around for no apparent reason (containers, vector, unordered_map)
So Access to the object is granted by an accessorfn provided by the parent
Within a Transaction AccessorFn can be called by providing an Owner object and will provide access to the object
Vector/List: AccessorFn is uniform so some obj-state must be provided

The AccessorFn will need some identifier to identify the exact object


- Vectors/List
  - This will provide access to the vector or the list (Container)
  - How does a sub-transaction get access to the contained value.
    - 

##### Overview
- Pros
  - Simplifies ser des. Assists with visitor pattern.
  - Establishes parent relationship. Provides up navigability which is important for Feature(2)
  - Ability to get unstable ptr objects lazily
  - Makes feature (1)(2) simpler
  - Fork persistent state into another object for nesting
  - Change txn struct to be temp var.
  - Solves issues with unstable container memberptr  (vector)

- Cons
  - Additional parent ptr overhead
    - Makes the transaction object itself a temporary object.
  - Requires separation of state for feature (2)

#### `Visitor<Transaction<...>>`

- Makes feature (5) simpler

Mutator support

VisitChanges

### FAQ

- Multi-threading and locking
  - Caller enforced. Caller ensures editing happens in a single thread
- Should we use thread local to maintain state around the sink ?
- The Ostream should be able to control output and formatting
  