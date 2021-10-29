# Build System

# Codegen
* Rename
    - relationship, relationshipdef => linkedattribute attributelink
    - Stencil namespace
    - DataBase -> Repository / Journal
* TypeDecl
	• Reflection.Base - OnChanged Event Handling
	• Reflection.Database - Encryption for Identity and unittests
Reflection.Database - Database Merging with OnChanged event handling with more Unittests

# Runtime

* SerDes + Writer Reader
    - JsonSerDes
    - CLISerDes
    - FastSerDes (reinterpret_cast/memcpy)
    - FileWriter, MemWriter
    - FileReader, MemReader
* [base] 
    - Get rid of timestamp
    - Handler Interfaces should not be templated virtual classes. Simplify SubComponent and isolate
    - Rename Union -> Variant
    - Move string out of Value. Rename to Fixed64BitValue
* [database]
    - Encrypted string support Encryption support with openssl integration
    - Add openssl to list of external linking projects and Migrate libssh2 to use openssl
    - Add support for multiple lifetime objects (saved, session, temp) [not doing]
	- Primary Key caching
	- Pooled Fixed size Blob types
	- Ref counted objects
	- Constexpr Member-Object-Ref iterator for objects
	- Merge database
* [logging]
	- Logging and exception cleanup
* [shared_string]
    - Ability to compile with shared_wstring internally using winrt::hstring

# Tests
## Codegen-Tests

## Runtime-Tests
