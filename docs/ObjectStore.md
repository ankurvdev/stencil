# Database

## Spec Keywords/Features

- Transactional
- Load Pages on Demand
- In memory + on-disk
-

### Types

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

- Dictionaries
  - Blob : List of Items. Each item is a key value pair struct ?

### Ownership/Lifetime

- Self  : Trivial Fixed Size Records
  - Regular CRUD
- Shared : shared_ptr, nested trivial fixed size records/blobs
- Unique : blob in a nested struct
  - Not allowed to delete without owner object

### Create
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
