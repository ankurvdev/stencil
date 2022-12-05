#pragma once
#include "Interfaces.pidl.h"
#include "Objects.pidl.h"

struct ObjectsTester
{
    size_t              _counter{0};
    size_t              _list_counter{0};
    uint8_t             create_uint8() { return static_cast<uint8_t>(++_counter); }
    uint16_t            create_uint16() { return static_cast<uint16_t>(++_counter); }
    int16_t             create_int16() { return static_cast<int16_t>(++_counter); }
    uint32_t            create_uint32() { return static_cast<uint32_t>(++_counter); }
    int32_t             create_int32() { return static_cast<int32_t>(++_counter); }
    uint64_t            create_uint64() { return static_cast<uint64_t>(++_counter); }
    int64_t             create_int64() { return static_cast<int64_t>(++_counter); }
    std::array<char, 8> create_array_char_8() { return {}; }

    shared_string create_string() { return shared_string(fmt::format("str{}", static_cast<uint32_t>(++_counter))); }
    double        create_double()
    {
        size_t count1 = ++_counter;
        size_t count2 = ++_counter;
        return static_cast<double>(count1 * 100) + (static_cast<double>(count2) / 100.0);
    }
    bool create_bool() { return !!(++_counter); }

    auto create_timestamp() { return Stencil::Timestamp{} + std::chrono::seconds{++_counter}; }

    auto create_simple_object1()
    {
        Objects::SimpleObject1 obj{};
        obj.val1         = create_int32();
        obj.val2         = create_uint32();
        obj.val3         = create_uint8();
        obj.val4         = create_string();
        obj.val5         = create_double();
        obj.lastmodified = create_timestamp();
        return obj;
    }

    auto create_simple_object2()
    {
        Objects::SimpleObject2 obj{};
        obj.val1         = create_bool();
        obj.val2         = create_double();
        obj.val3         = create_array_char_8();
        obj.val4         = create_timestamp();
        obj.val5         = create_uint64();
        obj.val6         = create_int64();
        obj.val7         = create_uint16();
        obj.val8         = create_int16();
        obj.lastmodified = create_timestamp();
        return obj;
    }

    auto create_list_object()
    {
        Objects::ListObject obj{};
        obj.obj1         = create_simple_object1();
        obj.value        = create_uint32();
        obj.lastmodified = create_timestamp();
        return obj;
    }

    auto create_list()
    {
        Objects::List obj{};
        for (size_t i = (++_list_counter + 1); i > 0; i--) { obj.listobj.push_back(create_list_object()); }
        return obj;
    }

    auto create_dict()
    {
        Objects::DictObject obj{};
        for (size_t i = (++_list_counter + 1); i > 0; i--)
        {

            obj.dictobj[create_uint32()]  = create_simple_object1();
            obj.dictval[create_string()]  = create_timestamp();
            obj.dictdict[create_uint32()] = {{create_uint32(), create_simple_object1()}, {create_uint32(), create_simple_object1()}};
        }
        return obj;
    }

    auto create_nested_object()
    {
        Objects::NestedObject obj{};
        obj.obj1         = create_simple_object1();
        obj.obj2         = create_simple_object2();
        obj.obj3         = create_list_object();
        obj.list1        = create_list();
        obj.dict1        = create_dict();
        obj.lastmodified = create_timestamp();
        return obj;
    }
};
