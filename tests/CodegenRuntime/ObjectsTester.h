#pragma once
#include "Interfaces.pidl.h"
#include "Objects.pidl.h"

// NOLINTBEGIN(readability-magic-numbers)
struct ObjectsTester
{
    size_t                     counter{0};
    size_t                     listCounter{0};
    uint8_t                    CreateUint8() { return static_cast<uint8_t>(++counter); }
    uint16_t                   CreateUint16() { return static_cast<uint16_t>(++counter); }
    int16_t                    CreateInt16() { return static_cast<int16_t>(++counter); }
    uint32_t                   CreateUint32() { return static_cast<uint32_t>(++counter); }
    int32_t                    CreateInt32() { return static_cast<int32_t>(++counter); }
    uint64_t                   CreateUint64() { return static_cast<uint64_t>(++counter); }
    int64_t                    CreateInt64() { return static_cast<int64_t>(++counter); }
    static std::array<char, 8> CreateArrayChar8() { return {}; }

    shared_string         CreateString() { return shared_string(fmt::format("str{}", static_cast<uint32_t>(++counter))); }
    std::filesystem::path CreateFilePath() { return {fmt::format("file{}", static_cast<uint32_t>(++counter))}; }
    double                CreateDouble()
    {
        size_t count1 = ++counter;
        size_t count2 = ++counter;
        return static_cast<double>(count1 * 100) + (static_cast<double>(count2) / 100.0);
    }
    bool CreateBool() { return !((++counter) == 0u); }

    auto CreateTimestamp() { return Stencil::Timestamp{} + std::chrono::seconds{++counter}; }

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
        obj.val1         = CreateBool();
        obj.val2         = CreateDouble();
        obj.val3         = CreateArrayChar8();
        obj.val4         = CreateTimestamp();
        obj.val5         = CreateUint64();
        obj.val6         = CreateInt64();
        obj.val7         = CreateUint16();
        obj.val8         = CreateInt16();
        obj.lastmodified = CreateTimestamp();
        return obj;
    }

    auto CreateListObject()
    {
        Objects::ListObject obj{};
        obj.obj1         = CreateSimpleObject1();
        obj.value        = CreateUint32();
        obj.lastmodified = CreateTimestamp();
        return obj;
    }

    auto IncrList()
    {
        ++listCounter;
        listCounter = listCounter & 0xfu;
        return listCounter + 1;
    }

    auto CreateList()
    {
        Objects::List obj{};
        for (size_t i = IncrList(); i > 0; i--) { obj.listobj.push_back(CreateListObject()); }
        return obj;
    }

    auto CreateDict()
    {
        Objects::DictObject obj{};
        for (size_t i = IncrList(); i > 0; i--)
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
        obj.obj3         = CreateListObject();
        obj.list1        = CreateList();
        obj.dict1        = CreateDict();
        obj.lastmodified = CreateTimestamp();
        return obj;
    }
};
// NOLINTEND(readability-magic-numbers)
