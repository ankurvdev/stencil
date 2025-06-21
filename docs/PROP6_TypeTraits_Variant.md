Current Core Types are

1. Indexable
2. Iterable
3. Primitives

Should Variant be considered a core-type ?

Justification:

Although its possible to emulate it with indexable of indexable such as

````{
    type: <enum>
    value : {

    }
}```
There are inherent restrictions are to what the value can be.

Json Data formats dont recognize variants as core-types
C++ recognizes variants / unions are core-types

Pros:
1. Variant has certain restrictions on

Cons:
Overhead in dealing with an additional core-type
````
