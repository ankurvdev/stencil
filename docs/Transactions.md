# Overview

Transactions can be used for the following scenarios, mainly to deal with sparse changes to object models.

1. Object Model listeners
2. Generate Events / Send RPC messages with delta changes to Object models
3. Optimized Record and Replay Object model changes with sparse delta
4. Logging Object model changes
5. Suppress / Avoid non-change edits

In general the use-cases involve the following steps

1. Start Transaction
2. Make changes to objects and sub-objects
    * Change values
    * Add remove list values
    * Edit sub-structs
3. End Transaction
4. Notify listeners with transaction data

with the following characteristics

* Listeners notified at the end of the transaction
    - Notification generated only if the transaction actually changes object model
    - No notification for empty or non-altering edits (assigning with same values)
* Notification involves sending TransactionRecordData and Object Model itself with a read-lock
* Allow Serialization/Deserialization of TransactionRecordData (TransactionRecordSerDes)
    - Binary Serialization for Trace recording
    - Text Serialization for logging/debugging
* TransactionRecordData can be replayed by deserializing Text based TransactionRecordData and applying it to an Object model.
* Apply TransactionRecordData notifies listeners as well.

Dependencies
* Visitor (Apply TransactionRecordData and Replay, Serialization of TransactionRecordData)
* JsonSerDes
* BinarySerDes

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

## Delta Change listeners
```c++
NotifyListeners(txn, foo);
```

## Data Recording
```c++
std::ofstream f("record.bin", ios::binary| ios::app);
f << timestamp << BinaryTransactionSerDes::Writer(txn, foo);
```

## Replay

```c++
std::ifstream f("record.bin", ios::binary);
while(f.good())
{
    f >> BinaryTransactionSerDes::Applicator(foo);
}
```

## Delta Logging

```c++
fmt::print("{}", StringTransactionSerDes::Deserialize(txn, foo);
```

## Change Commands

```c++
auto txn = StringTransactionSerDes::Apply(foo, "child1.val1 = 1;child1.val2 = \"newvalue\";child1.list1:add[0] = {val: 1}; child1.list1:add[0]= {val: 2};val3 = 2");
```

# Specializing for Custom types

```C++

struct MyType{}

template <> struct Transaction<MyType> : TransactionT<MyType>
{

// lambda is a auto lambda
template <typename TLambda>
void Visit(size_t index, TLamda&& lambda); // Invoke lambda(Tranction<SubType>) based on given index
template <typename TLambda>
void Visit(std::string_view const& name, TLambda&& lambda); // Invoke  lambda(Tranction<SubType>) based on given name


};


```