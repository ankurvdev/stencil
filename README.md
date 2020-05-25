# Stencil

![Build status](https://dev.azure.com/ankurverma0037/ankurverma/_apis/build/status/ankurverma85.stencil)

A code generation tool that is inspired by [MVVM](https://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93viewmodel) Data-Binding techniques often utilized in GUI programming for binding IDL representations of structs and interfaces to built-in or easy to write custom templates for boiler plate code generation providing lighting fast implementations without subjecting the developers to [RSI Injuries](https://en.wikipedia.org/wiki/Repetitive_strain_injury)

Currently support IDL representations 

* Custom [Thrift](https://en.wikipedia.org/wiki/Apache_Thrift)-like IDL 

Built-in templates for 

* JSON import/export 
* Command line parsing 
* Table Based disk read/write of records represented in IDL 
* C++ based REST HTTP Web-service (JSON) based on IDL interface definition. 
* MySQL records and tables (TODO)

## Examples (JSON, Command Line)
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
Here's the C++ Code to use for Json serialization/deserialization and cli args parsing
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

