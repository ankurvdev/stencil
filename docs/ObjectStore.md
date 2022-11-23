# Database

https://www.remi-coulom.fr/joedb/vectors.html
Apache arrow and parquet 
Rocksdb
https://en.m.wikipedia.org/wiki/Wide-column_store

Nosql

lmdb |Version: 0.9.29
LMDB is an extraordinarily fast, memory-efficient database


berkeleydb |Version: 4.8.30
BDB - A high-performance embedded database for key/value data.


hdf5 |Version: 1.12.2
HDF5 is a data model, library, and file format for storing and managing data






## Spec Keywords/Features

* object-store using struct/union syntax
  * Same Object Types but multiple grouping
  * No need for multiple string types and blobs. Everything can be a blob

* Object to ID mapping (auto)
* Objects
  * Trivial (Same View/Edit/Wire)
  * With Blob Refs (Shared/Unique)
  * Shared Fixed Refs (Is there a use-case for Unique FixexSize Refs)
  * View : Auto-dereference (on-demand ?) decryption?
  * Edit : Auto-recreate ?
  * Snap : Fully dereferenced and decrypted
  * Wire : Serialized , Fully dereferenced and decrypted with size available
* Constructors
  * Ignore for now
  * Fully constructed object as input for copying (Wire/Snap ?)
* Undo / Redo ?

* Merging Of 2 DBs
  * Preserve modification history (at page level ?)

* Page-encryption / decryption

* Search / Filter
  * Only Query By ID
  *
* Object-Store Backends
  * MySQL
  * JournalledTabled

* Object Constructors
* CRUD
  * ViewT (In-memory view)
  * EditT (In memory editing)
  * WireT (Serialization)
* Custom Layering/Limitations on Object construction
* Backends
  * MySQL
  * Tables
