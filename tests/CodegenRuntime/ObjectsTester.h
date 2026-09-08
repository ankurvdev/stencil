#pragma once
#include "Interfaces.pidl.h"
#include "Objects.pidl.h"

struct ObjectsTester
{
    size_t              _counter{0};
    size_t              _list_counter{0};
    uint8_t             CreateUint8() { return static_cast<uint8_t>(++_counter); }
    uint16_t            create_uint16() { return static_cast<uint16_t>(++_counter); }
    int16_t             create_int16() { return static_cast<int16_t>(++_counter); }
    uint32_t            CreateUint32() { return static_cast<uint32_t>(++_counter); }
    int32_t             CreateInt32() { return static_cast<int32_t>(++_counter); }
    uint64_t            create_uint64() { return static_cast<uint64_t>(++_counter); }
    int64_t             create_int64() { return static_cast<int64_t>(++_counter); }
    std::array<char, 8> create_array_char_8() { return {}; }

    shared_string CreateString() { return shared_string(fmt::format("str{}", static_cast<uint32_t>(++_counter))); }
    double        CreateDouble()
    {
        size_t count1 = ++_counter;
        size_t count2 = ++_counter;
        return static_cast<double>(count1 * 100) + (static_cast<double>(count2) / 100.0);
    }
    bool create_bool() { return !!(++_counter); }

    auto CreateTimestamp() { return Stencil::Timestamp{} + std::chrono::seconds{++_counter}; }

    auto CreateSimpleObject1()
    {
        Objects::SimpleObject1 obj{};
        obj.val1         = CreateInt32();
        obj.val2         = CreateUint32();
        obj.val3         = CreateUint8();
        obj.val4         = CreateString();
        obj.val5         = CreateDouble();
        obj.lastmodified = CreateTimestamp();
        return obj;
    }

    auto CreateSimpleObject2()
    {
        Objects::SimpleObject2 obj{};
        obj.val1         = create_bool();
        obj.val2         = CreateDouble();
        obj.val3         = create_array_char_8();
        obj.val4         = CreateTimestamp();
        obj.val5         = create_uint64();
        obj.val6         = create_int64();
        obj.val7         = create_uint16();
        obj.val8         = create_int16();
        obj.lastmodified = CreateTimestamp();
        return obj;
    }

    auto CreateList_object()
    {
        Objects::ListObject obj{};
        obj.obj1         = CreateSimpleObject1();
        obj.value        = CreateUint32();
        obj.lastmodified = CreateTimestamp();
        return obj;
    }

    auto incr_list()
    {
        ++_list_counter;
        _list_counter = _list_counter & 0xf;
        return _list_counter + 1;
    }

    auto CreateList()
    {
        Objects::List obj{};
        for (size_t i = incr_list(); i > 0; i--) { obj.listobj.push_back(CreateList_object()); }
        return obj;
    }

    auto CreateDict()
    {
        Objects::DictObject obj{};
        for (size_t i = incr_list(); i > 0; i--)
        {

            obj.dictobj[CreateUint32()]  = CreateSimpleObject1();
            obj.dictval[CreateString()]  = CreateTimestamp();
            obj.dictdict[CreateUint32()] = {{CreateUint32(), CreateSimpleObject1()}, {CreateUint32(), CreateSimpleObject1()}};
        }
        return obj;
    }

    auto CreateNestedObject()
    {
        Objects::NestedObject obj{};
        obj.obj1         = CreateSimpleObject1();
        obj.obj2         = CreateSimpleObject2();
        obj.obj3         = CreateList_object();
        obj.list1        = CreateList();
        obj.dict1        = CreateDict();
        obj.lastmodified = CreateTimestamp();
        return obj;
    }
};
