# Database

## Design Problems

- typeid => set for tuple-index of types
- view-type =>
- shared_ptr =>

### uint32_t:typeid vs tuple-index

Try to use tuple-index. CRUD for complex types need capabilities which are a superset of this

- typeid: may not be cross-plat. but we dont care about crossplat consistency
- typeid: there are maybe just hundred types at most. uint8_t should be more than enough
- tuple-index: there are hidden types, how to figure out the complete set

### CRUD for complex types

- Trait Support
  - `ConceptRefType<T>` : To figure out if a type translates to a ref
  - `Record<T>`: Transformed Type for storage in a record as either blob or Fixed.
  - `Record<T>`: With read/write lock used to access sub-objects.
  
- Create : Transform each nested type into a Ref<>. Recurisvely create ref objects. Return toplevel ref<> and ViewT.
  - dict - Visit-All. Create Key, Create Value => Create Pair<Ref<K>, Ref<V>> => Create List<Ref<Pair>>
  - struct - Visit-All. Create Refs => Create Fixed-Record
  - list - Visit-All. Create<Ref<T>> => Create List<Ref<T>>
  - shared-ptr - FindOrCreate<Ref<T>>

- Read : Read Toplevel and fetch on-demand. ViewT for these types exposes helpers
- Edit : ViewT with edit-lock
- Delete : DecrementRefCount for toplevel and proceed recursively. Decrement equates to free for non-shared types

### shared_ptr support

- 2 independent Create<> provide take the same shared_ptr object.
- How to dedup and match against an already created ?
- We need some runtime context. (Runtime weak_ptr cache to match up ?)
- Do we auto-dedup using search ?

## Spec Keywords/Features

- Transactional
- Load Pages on Demand
- In memory + on-disk

### Types

Broadly speaking there are 2 categories of Types

- Serialized-Types: Can be memory mapped directly
  - Fixed-Size - Compile time size available
  - Blobs - Length available at runtime or managed

- Complex Types: Data spread across multiple types
  - CRUD

Cases to handle:

Type        | Example                                      | Record Types
------------|----------------------------------------------|---------------------------------------
dict        | `unordered_map<K, V>`                        | K, V , Blob(List) Pair: Ref<K>, ref<V>      |
nested      | `struct Type { std::unique_ptr<NestedType> }`| Type, NestedType
lists       | `vector<K>`                                  | K, Blob(List) : `Ref<K>`
polymorphic | `struct Type1: Type{}; struct Type2: Type{}` | Type1, Type2, Type
shared_ptr  |                                              |

How do we handle repeated reads ?

- Serializaed Types
-

What do READ + UPDATE + DEL look like for various type?
Type        | API
------------|-----
fixed-size  |
dict        | Create<unordered_map<>> | Accept const&
dict        | Read() Create everytime ?, R
dict        | Udpdat

#### Type Id assignment

We need to assign a unique Id for each record type

Observations:
CRUD APIs only available for top-level types.

If we want to do Inference from top-level list of types

- Recursively Aggregate sub-types

- Type to Id Mapping
  - How do we collect and assign ids for all types ?
  - What about nested types (dictionary, lists of objects or shared/unique ptr subobjects)

- Polymorphism : Same type but different grouping
  - Support for creating inherited sub-types which result in unique grouping/ids

- Trivial Fixed Size Records (Direct structs)
  - Simply Memcpy
  - How do we know if a struct is trivially copyable ?

- Blobs: No need for separate string types. Everything is a polymorphic blob
  - Simply memcpy
  - Subgrouped by blob-size ?

- Nested : Records (Refs, Unique/Shared)
  - Ref for nested type
  - On demand loading ? Do we need avatars ?
  - Recursively create subobjects and then mempy for itself.

- Lists of Fixed Size Records
  - Blob: (List of Refs) for nested types
  - On demand loading ?
  - Recursively create subobjects and then create blob for itself.

- Dictionaries (unordered_map<K,V>)
  - Blob : List of Items. Each item is a key value pair struct ?

### Ownership/Lifetime

- Self  : Trivial Fixed Size Records
  - Regular CRUD
- Shared : shared_ptr, nested trivial fixed size records/blobs
- Unique : blob in a nested struct
  - Not allowed to delete without owner object

### Create

How to construct a nested object recursively ?

- Constructors
  - Ignore for now
  - Fully constructed object as input for copying (Wire/Snap ?)

### Read

- ?? Object to ID mapping (auto)
  - Additional strain. Should we load pages on-demand ?
- Avatars
  - Trivial (Same View/Edit/Wire)
  - With Blob Refs (Shared/Unique)
  - Shared Fixed Refs (Is there a use-case for Unique FixexSize Refs)
  - View : Auto-dereference (on-demand ?) decryption?
  - Edit : Auto-recreate ?
  - Snap : Fully dereferenced and decrypted
  - Wire : Serialized , Fully dereferenced and decrypted with size available

- Undo / Redo ?
- Merging Of 2 DBs
  - Preserve modification history (at page level ?)
- Page-encryption / decryption

- Search / Filter
  - Only Query By ID

# Reference Links

<https://www.remi-coulom.fr/joedb/vectors.html>
Apache arrow and parquet
Rocksdb
<https://en.m.wikipedia.org/wiki/Wide-column_store>

Nosql

lmdb |Version: 0.9.29
LMDB is an extraordinarily fast, memory-efficient database

berkeleydb |Version: 4.8.30
BDB - A high-performance embedded database for key/value data.

hdf5 |Version: 1.12.2
HDF5 is a data model, library, and file format for storing and managing data