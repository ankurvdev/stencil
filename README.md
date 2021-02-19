- [Overview](#overview)
- [Using Built-in Templates](#using-built-in-templates)
	- [JSON](#json-cli-args-parsing)
	- [Command Line Args Parsing](#json-cli-args-parsing)
	- [Data Storage : MySQL](#data-storage)
	- [JSON Web-Service](#rest-based-web-servicejson)
- [Writing Custom Templates](#writing-custom-templates)
	- [Code Templates](#code-templates)
	- [Native Types](#native-types)

# Overview

![Build status](https://dev.azure.com/ankurverma0037/ankurverma/_apis/build/status/ankurverma85.stencil)

A code generation tool that is inspired by [MVVM](https://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93viewmodel) Data-Binding techniques often utilized in GUI programming for binding IDL representations of structs and interfaces to built-in or easy to write custom templates for boiler plate code generation providing lighting fast implementations without subjecting the developers to [RSI Injuries](https://en.wikipedia.org/wiki/Repetitive_strain_injury)

Currently support IDL representations 

* Custom [Thrift](https://en.wikipedia.org/wiki/Apache_Thrift)-like IDL 

Built-in templates for

* JSON Data-Serialization/Deserialization
* Command line Args parsing 
* Data Storage : MySQL
* Data Storage : Custom (Pagified) storage of records
* Web-Service : C++ based REST HTTP Web-service (JSON) based on IDL interface definition. 

It also supports writing your own custom templates that can be binded with the IDL representation to generate any kind of custom code (not limited to c++). Combined with custom native-types specifications this can be used to generate code in any language with only configuration and templates and without the need for changing the utility. 
For more details see Custom Templates below


# Using Built-in templates

The following code usage examples demonstrate how to use the builtin templates for each use-case

## JSON, CLI args parsing
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
#include <foo.pidl.h>

#include <DataHandlerJson.h>
#include <CommandLineArgsReader.h>

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

## Code Templates

## Native Types
