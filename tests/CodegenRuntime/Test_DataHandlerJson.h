#include <stencil/json_parse.h>

#include <assert.h>
#include <iostream>
#include <sstream>
#include <string>

struct TestStruct2
{

    shared_string& get_field1() { return _field1; }
    void           set_field1(shared_string&& value) { _field1 = value; }

    shared_string _field1;
};

template <> struct Stencil::TypeTraits<TestStruct2&>
{
    struct Traits_field1
    {
        using TOwner     = TestStruct2;
        using TFieldType = shared_string;

        static constexpr std::string_view Name() { return "field1"; }
//        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TestStruct2::get_field1; }
        static constexpr auto TPropertySetter() { return &TestStruct2::set_field1; }
    };

    static const std::string_view     AttributeValue(const std::string_view& /*key*/) { return ""; }
    static constexpr DataType         Type() { return DataType::Iterable; }
    static constexpr std::string_view Name() { return "TestStruct2"; }
    // using Handler = typename ::Stencil::ReflectedStructHandler<TestStruct2, Traits_field1>;
};

struct TestStruct1
{
    TestStruct1() {}

    shared_string& get_field1() { return _field1; }
    void           set_field1(shared_string&& value) { _field1 = value; }

    bool& get_field2() { return _field2; }
    void  set_field2(bool&& value) { _field2 = value; }

    short& get_field3() { return _field3; }
    void   set_field3(short&& value) { _field3 = value; }

    std::vector<short>& get_field4() { return _field4; }
    void                set_field4(std::vector<short>&& value) { _field4 = std::move(value); }

    std::shared_ptr<TestStruct2>& get_field5() { return _field5; }
    void                          set_field5(std::shared_ptr<TestStruct2>&& value) { _field5 = std::move(value); }

    shared_string                _field1;
    bool                         _field2{};
    short                        _field3{};
    std::vector<short>           _field4;
    std::shared_ptr<TestStruct2> _field5;
};

template <> struct Stencil::TypeTraits<TestStruct1&>
{

    struct Traits_field1
    {
        using TOwner     = TestStruct1;
        using TFieldType = shared_string;

        // static const ::ReflectionBase::Flags Flags() { return {}; }

        static constexpr std::string_view Name() { return "field1"; }

        static constexpr auto TPropertyGetter() { return &TestStruct1::get_field1; }
        static constexpr auto TPropertySetter() { return &TestStruct1::set_field1; }
        // static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
    };
    struct Traits_field2
    {
        using TOwner     = TestStruct1;
        using TFieldType = bool;

        // static const ::ReflectionBase::Flags Flags() { return {}; }

        static constexpr std::string_view Name() { return "field2"; }

        static constexpr auto TPropertyGetter() { return &TestStruct1::get_field2; }
        static constexpr auto TPropertySetter() { return &TestStruct1::set_field2; }
        // static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
    };
    struct Traits_field3
    {
        using TOwner     = TestStruct1;
        using TFieldType = short;

        // static const ::ReflectionBase::Flags Flags() { return {}; }

        static constexpr std::string_view Name() { return "field3"; }
        // static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

        static constexpr auto TPropertyGetter() { return &TestStruct1::get_field3; }
        static constexpr auto TPropertySetter() { return &TestStruct1::set_field3; }
    };
    struct Traits_field4
    {
        using TOwner     = TestStruct1;
        using TFieldType = std::vector<short>;

        // static const ::ReflectionBase::Flags Flags() { return {}; }

        static constexpr std::string_view Name() { return "field4"; }
        // static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

        static constexpr auto TPropertyGetter() { return &TestStruct1::get_field4; }
        static constexpr auto TPropertySetter() { return &TestStruct1::set_field4; }
    };
    struct Traits_field5
    {
        using TOwner     = TestStruct1;
        using TFieldType = std::shared_ptr<TestStruct2>;

        // static const ::ReflectionBase::Flags Flags() { return {}; }

        static constexpr std::string_view Name() { return "field5"; }
        // static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

        static constexpr auto TPropertyGetter() { return &TestStruct1::get_field5; }
        static constexpr auto TPropertySetter() { return &TestStruct1::set_field5; }
    };

    // static constexpr auto     TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
    static constexpr DataType         Type() { return DataType::Iterable; }
    static constexpr std::string_view Name() { return "TestStruct1"; }

    //  using Handler = typename ::ReflectionServices::
    //     ReflectedStructHandler<TestStruct1, Traits_field1, Traits_field2, Traits_field3, Traits_field4, Traits_field5>;
};
