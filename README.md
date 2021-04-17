# Overview

![Build status](https://dev.azure.com/ankurverma0037/ankurverma/_apis/build/status/ankurverma85.stencil)

A code generation tool that is inspired by [Model-Template-View](https://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93viewmodel) architectural patterns that involve Data-Binding.
The tool in its bare-bones form can take as input: 
1. An IDL representation of interfaces and structs (Model)
2. Native Type and container mappings (Template)
3. Templates for binding the Data-Structs and API-Interfaces (Template)

And generate code (View) thats based on replication and substitution of bindable expression in the templates (data-binding)


To allow for a more practical usage a set of built-in templates are provided along with a runtime (C++20 header-only) that relies on code-generated compile time reflection to enable certain practical real-world usages.

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

THe built-in templates generate compile time reflection of structs and interface that along with a header-only runtime make possible the following core usage scenarios 

* JSON Data-Serialization/Deserialization
* Command Line Args parsing for CLI Tools with auto help generation
* Data Storage : MySQL (Broken currently)
* Data Storage : Records + PageTables (FixedSize, Blobs)
* Web-Service : C++ based REST HTTP Web-service (JSON) based on IDL interface definition. 

Along with certain supplemental functionalities
* Data Recording and replay along with delta-binary patching of objects.
* Property Change Event notifcations and listeners

The built-in functionalities follow the listed design principles: 

* Pay for what you use. So if you want simply JSON Parsing functionality to simply populate your struct members based on inputs json string. That is as lean as it can be, with no hit from other core or supplemental features
* Dont care about compile-times. 
* Header only to allow for easier build integration and portability
* The templates generate highly reusable compile-time reflection metadata that the header-only runtime uses for providing the requested functionality by integration with the popular corresponding framework (JSON -> nlohmann-json, Web-Service -> cpp-httplib) 
* One liner integrations for most common scenarios.
* C++20 . Dont care about older compilers or platforms where C++20 isnt available
* Cross platform Runtime header generation: Tested for Windows, Linux, RaspberryPi's, Android, with Clang, GCC, MSVC
* No compiler warnings with Wall, WEverything, Wextra pedantic pedantic-errors etc on either 32 bit or 64 bit platforms.

# Using Built-in templates

The following code usage examples demonstrate how to use the builtin templates for each use-case

## Serialization / Deserialization

JSON Input and Output
Command Line Arg parsing and help generation

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
  std::unique_ptr<Foo> foocliobj = CommandLineArgsReader::Parse<Foo>(argc, argv);
  // {"s": "some-string", "l": ["val1", "val2"], "i": 3444 }
  std::string foojsonstr = Json::Stringify(foo);
  auto foojsonobj = Json::Parse<Foo>(foojsonstr);
  return 0;
}

```

# Data Storage

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

# REST based Web-Service(JSON)
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
