# Overview

![Build status](https://dev.azure.com/ankurverma0037/ankurverma/_apis/build/status/ankurverma85.stencil)

A code generation tool that is inspired by [Model-Template-View](https://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93viewmodel) architectural patterns that involve Data-Binding.
The tool in its bare-bones form can take as input: 
1. An IDL representation of interfaces and structs (Model)
2. Native Type and container mappings (Type Declarations)
3. Templates for binding the Data-Structs and API-Interfaces (Template)

![](docs/overview.png)

And generate code (View) thats based on replication and substitution of bindable expression in the templates (data-binding)

Includes (optional) built-in templates for some real-world usage scenarios, like json-parsing, tabular-data-storage, command-line-args etc.

# (Optional) Built-In Templates and scenarios

THe built-in templates generate compile time reflection of structs and interface that along with a header-only runtime make possible the following core usage scenarios 

* Serialization / Deserialization 
    - [JSON](../tests/Test_DataHandlerJson.cpp) (using nlohmann-json)
    - [CLI](../tests/Test_CLOpts.cpp) (argc, argv):  Deserialize Command line args based on IDL structs (with auto help generation)
    - Custom Compact Binary Format
* Data Storage CRUD 
    - SQLITE (TODO: Broken currently)
    - PageDB (Custom Paged Tabular Records) (FixedSize, Blobs)
* RPC
    - REST Web-Service : C++ based REST HTTP Web-service (JSON) based on IDL interface definition. (using cpp-httplib)
    - [TODO] Bluetooth BLE Services 
* Object Modelling
    - [Transactions](Transactions.md) : Record and Replay of Delta changes into runtime objects
    - Property change event notifications and listeners

The built-in functionalities follow the design principles: 

* Pay for what you use. No runtime compute/memory impact from unused features
* Dont care about compile-times
* Header only to allow for easier build integration and portability
* The templates generate highly reusable compile-time reflection metadata that the header-only runtime uses for providing the requested functionality by integration with the popular corresponding framework (JSON -> nlohmann-json, Web-Service -> cpp-httplib) 
* One liner integrations for most common scenarios.
* C++20 . Dont care about older compilers or platforms where C++20 isnt available
* Cross platform Runtime header generation: Tested for Windows, Linux, RaspberryPi's, Android, with Clang, GCC, MSVC
* No compiler warnings with Wall, WEverything, Wextra pedantic pedantic-errors etc on either 32 bit or 64 bit platforms.

## Examples (Built-in templates)

The following code usage examples demonstrate how to use the builtin templates for each use-case

### Serialization / Deserialization

* JSON Input and Output
* Command Line Arg parsing and help generation

Consider a simple IDL (foo.pidl)

```
struct Foo 
{
  string       s	(Description = "Some String"),
  list<string> l	(Description = "List of strings"),
  i32          i = 3443	(Description = "Some integer value"),
  bool         b = false	(Description = "A switch")
}
```
C++ Code
```
#include <foo.pidl.h> // Generated : Auto includes the header only runtime
#include <iostream>

int main(int arg, char* argv[])
{
  // Command line: foo -s "some-string" -l val1,val2, -i 3444
  
  // Command Line args parsing
  std::unique_ptr<Foo> obj = CommandLineArgsReader::Parse<Foo>(argc, argv);
  // Struct object foocliobj now has the following data
  assert(obj.s == "some-string")
  assert(obj.l[0] == "val1")
  assert(obj.i == 3444)
  assert(obj.b == false)
  
  // Json representation of the data
  // {"s": "some-string", "l": ["val1", "val2"], "i": 3444 }
  
  // JSON Serialization
  std::string foojsonstr = Json::Stringify(foo);
  
  // JSON Deserialization
  auto foojsonobj = Json::Parse<Foo>(foojsonstr);
  
  return 0;
}

```

### Data Storage CRUD 

IDL example
```

struct UserData
{
    //databaseref<wstring> name;
    timestamp modified;
    timestamp creation;
}

struct Identity
{
    db_encrypted_unique_wstring username;
    db_encrypted_unique_wstring password;
    db_encrypted_unique_wstring privatekey;
    db_encrypted_unique_wstring clientcert;
    db_encrypted_unique_wstring secretcode;
}

struct RemoteHost
{
    db_encrypted_unique_wstring name;
    db_encrypted_unique_wstring uri;
    uuidref<Identity> identity;
}

relationship database owner = UserData : object = RemoteHost, Identity
relationship uuidbasedobject object = RemoteHost, Identity
relationship encryptedobject object = Identity
relationship timestamped object = RemoteHost, Identity, UserData
```
C++ Code
```
#include <UserData.pidl.h>
#include <Database2.h>
int main(int argc, char *argv[])
{
// TODO
}
```

### REST based Web-Service(JSON)
IDL example (Service.pidl)
```

struct Data
{
	i32 result;
}

interface TestInterface
{
	Data AddNumber(i32 num1, i32 num2);
}

```
C++ Code
```
#include <Service.pidl.h>
#include <WebService.h>

class TestInterfaceImpl : public TestInterface
{
    public:
    // Inherited via TestInterface
    virtual Data AddNumber(int32_t num1, int32_tnum2) override { return Data{ num1 + num2}; }

};

template <> struct ReflectionBase::InterfaceActivator<TestInterface>
{
   std::unique_ptr<TestInterface> Activate()
   {
       return new TestInterfaceImpl();
    }
};

int main(int argc, char* argv[])
{
        WebService<TestInterface> svc;
        svc.StartOnPort(8080);
        svc.WaitForStop();
}
```

# Writing Custom Templates
To either support a new code generation langauage or to support additional scenarios, certain custom templates may be written.

## Code Templates

## Native Types

# Philosophy and design principles

The code generation tool in its bare-form tries to stick to the follow design principles
* Language independent codegen, to allow for potentially generating other languages
  - Currently C++ support only (Builtin)
* Modular parsing of input formats into their in-memory representation to allow extensible and varied input format for each input category
  - Currently supported formats and parsers
    - IDL : Custom [Thrift](https://en.wikipedia.org/wiki/Apache_Thrift)-like IDL 
    - Templates : C++ code (Intellisense available) with HTML-Markup in line-comments and distinct variables naming schema for binding declarations. XML as the parsing engine
    - Type-Mappings : YAML
* Model-Template-View 
  - Model : IDL 
  - Template : Code-Template + Type-Mapping
  - View : Generated Code
* Declarative (Binding-only) templates, no loops or algorithmic generation codification in templates
* Allow for complete bypass of all Built-in layers (Templates and Type-mapping) with full-barebone usage of the core engine
  - Templates and Mappings can be loaded from user specified directories at runtime
  - No loadings of builtin templates and mappings
* Provide Built-in template + mapping support for most common scenarios for easier out-of-box experience
* Cross platform C++
* Easy build system integration. (CMake included)
