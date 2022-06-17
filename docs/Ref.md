Ref
CRUD/Native Access
    - Shared_ptr
    - Unique_ptr
Serialization/Deserialization
    - UUID-Ref
    - Object-Id Ref


Ref<TDb, TObj> or Ref<TObj> 

    Should we force use of a DataStore ?
    shared_ptr<T> and unique_ptr<T> dont
    Why should we ?

    Is it too limiting ?

    What do we get out of that limitation ?

    Can we have void as TDb


Ref<TObj> : simpler mapping from pidl to 

we dont want objects to have refs that are tied to a datastore.


A Ref means different things in different context

UuidRef<>
ObjIdRef<>


How do we store information in the object ?
We force it to be 32bit
    For uuidref and bigger refs. The info needs to be stored in the object itself and dereferenced from there ?



Decisions

A Ref will always be a 32-bit integer 
    But what about representing pointers  on 64 bit

A Ref will always be serialized/deserialized as an 32 bit integer or <type>_<int> string


TODO: 

How to use and represent pointers for refs  ?

How to enable serialization/deserialization of refs in-place as objects. ?

How to combine shared_ptr<T>, unique_ptr<T> with uuidref<T> , intidref<T>
    Dereferencer (repr to T&) needs a manager
        UUID -> T&
        INTID -> T&
