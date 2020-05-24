#include "Test_Database.h"
#include <catch2/catch.hpp>
#include <iterator>
using namespace std::string_literals;

TEST_CASE("CodeGen::Database2::Encryption")
{
}

TEST_CASE("CodeGen::Database2::MergeDatabase")
{
}

TEST_CASE("CodeGen::Database2::SaveAndLoad")
{
    auto filepath = "SaveAndLoad.bin"s;
    struct Data
    {
        shared_wstring name;
        shared_wstring uri;
    } data[] = {{L"TestHost1", L"ssh://aadyaverma.com:41422"}, {L"TestHost2", L"ssh://192.168.99.1:41422"}};

    std::filesystem::remove(filepath);

    // Create New Database and Create Objects
    {
        UserData::UserData::Data database;
        database.Init(&database, filepath);
        std::vector<Database2::Ref<UserData::UserData::Data, UserData::RemoteHost::Data>> objs;

        size_t i = 0;

        auto lock = database.LockForEdit();
        for (i = 0; i < std::size(data); i++)
        {
            auto [id1, obj1] = database.Create<UserData::Identity::Data>(lock, data[i].name, data[i].uri, L"", L"", L"");
            auto [id2, obj2] = database.Create<UserData::RemoteHost::Data>(lock, data[i].name, data[i].uri, obj1.id);
            REQUIRE(id1.Valid());
            REQUIRE(id2.Valid());
            auto name = obj2.name().Get(lock, database);
            auto uri  = obj2.uri().Get(lock, database);
            REQUIRE(name == data[i].name);
            REQUIRE(uri == data[i].uri);
        }

        // Count objects in the same session
        size_t j = 0;
        for (auto const& [ref, obj] : database.Objects<UserData::RemoteHost::Data>(lock))
        {
            REQUIRE(!obj.identity().Empty());
            auto name = obj.name().Get(lock, database);
            auto uri  = obj.uri().Get(lock, database);
            REQUIRE(name == data[j].name);
            REQUIRE(uri == data[j].uri);
            ++j;
        }
        REQUIRE(i == j);
    }

    // Count objects in a new session
    {
        UserData::UserData::Data database;
        database.Init(&database, filepath);

        {
            auto   lock = database.LockForEdit();
            size_t j    = 0;
            for (auto const& [ref, obj] : database.Objects<UserData::RemoteHost::Data>(lock))
            {
                REQUIRE(!obj.identity().Empty());
                auto name = obj.name().Get(lock, database);
                auto uri  = obj.uri().Get(lock, database);
                REQUIRE(name == data[j].name);
                REQUIRE(uri == data[j].uri);
                ++j;
            }
            REQUIRE(j == std::size(data));
        }

        {
            // Create More objects
            auto lock = database.LockForEdit();
            for (size_t i = 0; i < std::size(data); i++)
            {
                auto [id1, obj1] = database.Create<UserData::Identity::Data>(lock, data[i].name, data[i].uri, L"", L"", L"");
                auto [id2, obj2] = database.Create<UserData::RemoteHost::Data>(lock, data[i].name, data[i].uri, obj1.id);
                REQUIRE(id1.Valid());
                REQUIRE(id2.Valid());
                auto name = obj2.name().Get(lock, database);
                auto uri  = obj2.uri().Get(lock, database);
                REQUIRE(name == data[i].name);
                REQUIRE(uri == data[i].uri);
            }
        }
        {
            auto lock = database.LockForRead();
            // Count of objects should be double now
            size_t j = 0;
            for (auto const& [ref, obj] : database.Objects<UserData::RemoteHost::Data>(lock))
            {
                auto name = obj.name().Get(lock, database);
                auto uri  = obj.uri().Get(lock, database);
                REQUIRE(name == data[j % std::size(data)].name);
                REQUIRE(uri == data[j % std::size(data)].uri);
                ++j;
            }
            REQUIRE(j == 2 * std::size(data));
        }
    }
}
