Spec Keywords/Features
* object-store using struct/union syntax
  - Same Object Types but multiple grouping
  - No need for multiple string types and blobs. Everything can be a blob


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
  - Preserve modification history (at page level ?)

* Page-encryption / decryption

* Search / Filter
  * Only Query By ID
  * 
* Object-Store Backends 
  - MySQL
  - JournalledTabled

* Object Constructors
* CRUD
  - ViewT (In-memory view)
  - EditT (In memory editing)
  - WireT (Serialization)
* Custom Layering/Limitations on Object construction
* Backends
    - MySQL
    - Tables

