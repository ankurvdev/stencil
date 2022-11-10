#include "TestUtils.h"
#include <Catalog.pidl.h>

#include <iostream>

static void add_local_content(Database2::exclusive_lock&    lock,
                              Catalog::DataStore&           datastore,
                              Stencil::Ref<Catalog::Source> content_src,
                              std::filesystem::path const&  path)
{
    auto [ref1, src] = datastore.Create<Catalog::ContentInfo>(lock);
    src.added        = Stencil::Timestamp::clock::now();
    src.folder       = {};
    src.modified     = src.added;
    src.path         = shared_string(path.string());
    src.size         = std::filesystem::file_size(path);
    src.source       = content_src;
    src.status_info  = "";
    src.type         = 'l';
}

static void add_remote_content(Database2::exclusive_lock&    lock,
                               Catalog::DataStore&           datastore,
                               Stencil::Ref<Catalog::Source> content_src,
                               std::string_view const&       url)
{
    auto [ref1, src] = datastore.Create<Catalog::ContentInfo>(lock);
    src.added        = Stencil::Timestamp::clock::now();
    src.folder       = {};
    src.modified     = src.added;
    src.path         = shared_string(url);
    src.size         = 0;
    src.source       = content_src;
    src.status_info  = "";
    src.type         = 'r';
}

static void
scan_source(Database2::exclusive_lock& lock, Catalog::DataStore& datastore, Stencil::Ref<Catalog::Source> const& ref, Catalog::Source& src)
{
    src.last_checked = Stencil::Timestamp::clock::now();
    auto spath       = std::filesystem::path(src.path.str());
    if (src.type == 'l')
    {
        for (auto item : std::filesystem::recursive_directory_iterator(spath))
        {
            if (!item.is_regular_file()) continue;
            add_local_content(lock, datastore, ref, item);
        }
    }
    if (src.type == 'r')
    {
        add_remote_content(lock, datastore, ref, "http://foo.com/1.txt");
        add_remote_content(lock, datastore, ref, "http://foo.com/2.txt");
        add_remote_content(lock, datastore, ref, "http://foo.com/3.txt");
    }
}

TEST_CASE("CodeGen::Catalog::Simplecase", "[Database]")
{
    auto                  srcdir      = std::filesystem::temp_directory_path() / "test_stencil_catalog";
    std::filesystem::path testfiles[] = {srcdir / "test1.txt", srcdir / "test2.txt"};

    if (!std::filesystem::is_directory(srcdir)) { std::filesystem::create_directories(srcdir); }
    for (auto f : testfiles)
    {
        if (!std::filesystem::is_regular_file(f))
        {
            std::ofstream of(f, std::ios::binary | std::ios::trunc);
            of << "a";
        }
    }

    Catalog::DataStore datastore;
    datastore.Init();
    {
        auto lock = datastore.LockForEdit();

        auto [ref1, src]    = datastore.Create<Catalog::Source>(lock);
        src.added           = Stencil::Timestamp::clock::now();
        src.frequencyInSecs = 0;
        src.last_checked    = Stencil::Timestamp();
        src.path            = shared_string(srcdir.string());
        src.status_info     = "";
        src.type            = 'l';
    }
    {
        auto lock = datastore.LockForEdit();

        auto [ref1, src]    = datastore.Create<Catalog::Source>(lock);
        src.added           = Stencil::Timestamp::clock::now();
        src.frequencyInSecs = 0;
        src.last_checked    = Stencil::Timestamp();
        src.path            = shared_string("http://foo.com/");
        src.status_info     = "";
        src.type            = 'r';
    }
    {
        auto lock = datastore.LockForRead();

        std::vector<std::string> lines;

        for (auto [ref, obj] : datastore.Objects<Catalog::Source>(lock))
        {
            auto copy  = obj;
            copy.added = Stencil::Timestamp{};
            if (copy.type == 'l')
                copy.path = shared_string(std::filesystem::relative(std::filesystem::path(copy.path.str()), srcdir).string());
            lines.push_back(Stencil::Json::Stringify(copy));
        }
        CheckOutputAgainstStrResource(lines, "Sources");
    }
    {
        auto lock = datastore.LockForRead();
        for (auto [ref, obj] : datastore.Objects<Catalog::ContentInfo>(lock)) { FAIL("Found Content , Expected Empty"); }
    }
    {
        auto lock = datastore.LockForEdit();
        for (auto [ref, obj] : datastore.Objects<Catalog::Source>(lock)) { scan_source(lock, datastore, ref, obj); }
    }
    {
        auto lock = datastore.LockForRead();

        std::vector<std::string> lines;
        for (auto [ref, obj] : datastore.Objects<Catalog::ContentInfo>(lock))
        {
            auto copy     = obj;
            copy.added    = Stencil::Timestamp{};
            copy.modified = Stencil::Timestamp{};
            if (copy.type == 'l')
                copy.path = shared_string(std::filesystem::relative(std::filesystem::path(copy.path.str()), srcdir).string());
            lines.push_back(Stencil::Json::Stringify(copy));
        }
        std::sort(lines.begin(), lines.end());
        CheckOutputAgainstStrResource(lines, "ContentInfo");
    }
}
