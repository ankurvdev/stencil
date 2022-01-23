# Build System

# Runtime

* SerDes + Writer Reader
    - JsonSerDes
    - CLISerDes
    - FastSerDes (reinterpret_cast/memcpy)
    - FileWriter, MemWriter
    - FileReader, MemReader
* [base] 
    - Get rid of timestamp
 [database]
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
