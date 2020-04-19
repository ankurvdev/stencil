#pragma once

#include <Common.h>
#include <DataModel.h>

#include <assert.h>
#include <bitset>
#include <filesystem>
#include <fstream>
#include <map>
#include <shared_mutex>
#include <string>
#include <vector>

namespace Database2
{
enum class CompareResult
{
    Equal,
    Greater,
    Lesser
};

enum class Encryption
{
    Yes,
    No
};

enum class Type
{
    Blob,
    Object
};

enum class Ownership
{
    Self,
    Unique,
    Shared
};

typedef uint32_t ObjTypeId;

template <typename T> struct DatabaseT;
template <typename TDb> struct PageLoaderTraits;
template <typename TDb, typename TObj> struct ObjTraits;
template <typename TDb, typename TObj> struct Ref;
template <typename TDb, typename TOwner, typename TObj> struct RefContainer;

template <typename TDb, typename TObj> struct FixedSizeObjTraits
{
    using WireType = TObj;
    using SnapType = TObj;
    using ViewType = TObj const&;
    using EditType = TObj&;

    constexpr static ObjTypeId TypeId() { return TObj::TypeId(); }
    constexpr static size_t    RecordSize() { return sizeof(TObj); }
    constexpr bool             IsEncrypted() { return false; }
    constexpr static size_t    StructMemberCount() { return 0; }    // Unavailable / not-needed

    // using RefType = struct Ref<TDb, TObj>;
    // static const TObj&   GetView(const RecordBuffer& buffer);
    // static CompareResult Compare(TDb& owner, const TObj& lhs, const TObj& rhs)

    // template <typename... TArgs> static std::tuple<RefType, TObj> Create(TOwner& owner, TArgs&&... args);

    // static void Release();
};

template <typename TDb, typename TObj> struct ObjTraits : FixedSizeObjTraits<TDb, TObj>
{
};

template <typename T> constexpr void VerifyTypeTraits()
{
    static_assert(T::ObjTypeId() > 0);
    static_assert(T::IsEncrypted() || (!T::IsEncrypted()), "Should define whether the object is encrypted");
    constexpr auto ownership = T::Ownership();

    if constexpr (T::RecordSize() == 0)
    {
        static_assert(T::ObjTypeId() > 0xff, "Blob Traits should have typeids > 255");
    }
    else
    {
        static_assert(T::RecordSize() < 0xff, "Record size should be less than 255 uint8_ts");
        static_assert(T::ObjTypeId() < 0xff, "Object Traits should have typeids < 255");
    }

    //    static_assert(std::is_trivially_default_constructible<T::WireType>::value, "WireType should be copy
    //    constructible"); static_assert(std::is_trivially_default_constructible<T::ViewType>::value, "ViewType should
    //    be default constructible");
}
}    // namespace Database2

namespace Database2::impl
{
using namespace Database2;

struct Ref
{
    typedef uint16_t PageIndex;
    typedef uint16_t SlotIndex;
    PageIndex        page{0};
    SlotIndex        slot{0};

    bool operator==(Ref const& rhs) const { return page == rhs.page && slot == rhs.slot; }

    constexpr Ref() = default;
    constexpr Ref(PageIndex pageIn, SlotIndex slotIn) : page(pageIn), slot(slotIn) {}

    Ref(Ref const& val) = default;
    Ref& operator=(Ref const& val) = default;

    static Ref Invalid() { return Ref{}; }
    bool       Valid() const { return page >= 2 && slot < 1000; }
};

// Keep this always at 8192 uint8_ts to optimize memory usage
struct Page
{
    constexpr static size_t PageSizeInBytes = 8192;

    struct Header
    {
        ObjTypeId typeId = 0;
    } header{};

    constexpr static size_t PageDataSize         = PageSizeInBytes - sizeof(Header);
    uint8_t                 buffer[PageDataSize] = {};
};

struct SerDes
{
    struct Header
    {
        uint8_t  magicCode[64] = {};
        uint64_t lastModified  = 0;
        uint64_t creationDate  = 0;
    } header;

    void Attach(std::filesystem::path const& path)
    {
        if (std::filesystem::exists(path))
        {
            auto fstatus = std::filesystem::status(path);
            if (fstatus.type() != std::filesystem::file_type::regular)
            {
                throw ::Logging::TODOCreateException("File is not a regular file");
            }
            if ((fstatus.permissions() & std::filesystem::perms::owner_write) == std::filesystem::perms::none)
            {
                AttachStream(std::ifstream(path));
            }
            else
            {
                AttachStream(std::fstream(path, std::fstream::binary | std::fstream::in | std::fstream::out));
            }
        }
        else
        {
            AttachStream(std::fstream(path, std::fstream::binary | std::fstream::in | std::fstream::out | std::fstream::trunc));
        }
    }

    void AttachStream(std::fstream&& stream)
    {
        assert(!stream.fail());
        _iostream = std::move(stream);
        _AttachStream((std::istream*)&_iostream);
        _AttachStream((std::ostream*)&_iostream);
    }

    void AttachStream(std::ifstream&& stream)
    {
        _istream = std::move(stream);
        _AttachStream(&_istream);
    }

    private:
    void _AttachStream(std::ostream* stream)
    {
        _flushto = stream;
        _EnsureHeader(_curheader, *stream);
        assert(GetInputPageCount() >= 2);
    }

    void _AttachStream(std::istream* stream)
    {
        _readFrom = stream;
        assert(_loadedPages.size() == 0);
        _loadedPages.clear();
    }

    public:
    ~SerDes()
    {
        if (_flushto == nullptr) return;
        _flushto->flush();
    }
    void InitInMemory()
    {
        _loadedPages[0] = std::make_unique<Page>();
        _loadedPages[1] = std::make_unique<Page>();
    }

    void LoadInMemory(std::istream& istream)
    {
        assert(_readFrom == nullptr);
        _readFrom = nullptr;

        // Size of istream should either be 0 or 2 pages + header
        _readFrom->seekg(0, std::ios_base::end);
        auto offset     = _readFrom->tellg();
        _loadedPages[0] = std::make_unique<Page>();
        _loadedPages[1] = std::make_unique<Page>();
        if (offset == 0)
        {
            return;
        }

        _ReadHeader(_curheader, istream);
        _ReadPage(*_loadedPages[0], 0, istream);
        _ReadPage(*_loadedPages[1], 1, istream);
    }

    void ReadPage(Page& page, uint32_t index)
    {
        if (_loadedPages.size() == 0)
        {
            if (_readFrom == nullptr) throw ::Logging::TODOCreateException("No Input file attached");
            _ReadPage(page, index, *_readFrom);
        }
        else
        {
            if (_readFrom != nullptr) throw ::Logging::TODOCreateException("Invalid State");
            page = *_loadedPages[index];
        }
    }

    void WritePage(Page const& page, uint32_t index)
    {
        if (_flushto == nullptr) return;
        _WritePage(page, index, *_flushto);
    }

    void Close()
    {
        if (_flushto == nullptr) return;
        _WriteHeader(_curheader, *_flushto);
    }

    uint32_t GetInputPageCount()
    {
        if (_loadedPages.size() == 0)
        {
            if (_readFrom == nullptr) throw ::Logging::TODOCreateException("No Input file attached");
            _readFrom->seekg(0, std::ios_base::end);
            auto offset = _readFrom->tellg();
            return GetPageIndexFromOffset(offset);
        }
        else
        {
            if (_readFrom != nullptr) throw ::Logging::TODOCreateException("Invalid State");
            return (uint32_t)_loadedPages.size();
        }
    }

    private:
    static const size_t PageStreamOffset(size_t page) { return (page * Page::PageSizeInBytes) + sizeof(Header); }

    static const uint32_t GetPageIndexFromOffset(std::streamoff offset)
    {
        assert((offset - sizeof(Header)) % Page::PageSizeInBytes == 0);
        return static_cast<uint32_t>((offset - sizeof(Header)) / Page::PageSizeInBytes);
    }

    static void _EnsureHeader(Header const& header, std::ostream& stream)
    {
        assert(!stream.fail());
        stream.seekp(0, std::ios_base::end);
        assert(!stream.fail());
        auto offset = stream.tellp();
        assert(!stream.fail());
        if (offset == 0)
        {
            _WriteHeader(header, stream);
            _WritePage(Page{/*headerPage*/}, 0, stream);
            _WritePage(Page{/*journalPage*/}, 1, stream);
        }
        assert(!stream.fail());
    }

    static void _WriteHeader(Header const& header, std::ostream& stream)
    {
        assert(!stream.fail());
        stream.seekp(0, std::ios_base::beg);
        stream.write((const char*)&header, sizeof(header));
        assert(!stream.fail());
    }

    static void _ReadHeader(Header& header, std::istream& stream)
    {
        assert(!stream.fail());
        stream.seekg(0, std::ios_base::beg);
        stream.read((char*)&header, sizeof(header));
        assert(!stream.fail());
    }

    static void _ReadPage(Page& page, uint32_t index, std::istream& stream)
    {
        assert(!stream.fail());
        std::streampos offsetreq = PageStreamOffset(index);
        stream.seekg(offsetreq + (std::streampos)Page::PageSizeInBytes, std::ios_base::beg);
        auto offsetcur = stream.tellg();
        if (offsetcur != (offsetreq + (std::streampos)Page::PageSizeInBytes))
        {
            throw ::Logging::TODOCreateException("Invalid Page Ref");
        }
        stream.seekg(offsetreq, std::ios_base::beg);
        assert(!stream.fail());
        offsetcur = stream.tellg();
        assert(!stream.fail());
        if (offsetcur != (std::streampos)offsetreq) throw ::Logging::TODOCreateException("Invalid Page Ref");

        stream.read((char*)&page, Page::PageSizeInBytes);
        assert(!stream.fail());
    }

    static void _WritePage(Page const& page, uint32_t index, std::ostream& stream)
    {
        assert(!stream.fail());
        std::streamoff offsetreq = PageStreamOffset(index);
        stream.seekp(offsetreq, std::ios_base::beg);
        auto offsetcur = stream.tellp();
        assert(offsetcur >= 0);
        assert((offsetcur <= offsetreq) && (((offsetreq - offsetcur)) % Page::PageSizeInBytes == 0));
        uint8_t zerobuffer[Page::PageSizeInBytes] = {};
        assert(zerobuffer[0] == 0 && zerobuffer[Page::PageSizeInBytes - 1] == 0);
        while (offsetcur < offsetreq)
        {
            stream.write((const char*)zerobuffer, Page::PageSizeInBytes);
            offsetcur = stream.tellp();
        }
        stream.write((const char*)&page, sizeof(Page));
        assert(!stream.fail());
    }

    std::unordered_map<uint32_t, std::unique_ptr<Page>> _loadedPages;    // for in-memory

    Header        _fileheader;
    Header        _curheader;
    std::istream* _readFrom = nullptr;
    std::ostream* _flushto  = nullptr;
    std::ifstream _istream;
    std::ofstream _ostream;
    std::fstream  _iostream;
};

// All non-seriazable and tracking objects go here
struct PageRuntime
{
    using exclusive_lock = ::exclusive_lock;
    using shared_lock    = ::shared_lock;
    enum class Flag
    {
        Dirty,
        COUNT
    };
    using Flags = std::bitset<(size_t)Flag::COUNT>;

    Ref::SlotIndex m_availableSlot = 0;
    Ref::PageIndex m_pageIndex     = 0;
    Flags          m_flags{};
    Page           m_page;

    void MarkDirty() { m_flags.set((size_t)Flag::Dirty); }
    void Flush(SerDes& serdes)
    {
        if (!m_flags.test((size_t)Flag::Dirty)) return;
        serdes.WritePage(m_page, m_pageIndex);
        m_flags.reset((size_t)Flag::Dirty);
    }

    PageRuntime(ObjTypeId id) { m_page.header.typeId = id; }
};

struct PageForRecordInterface
{
    virtual ~PageForRecordInterface() = default;
    struct ArrayView
    {
        size_t   size{0};
        uint8_t* buffer{nullptr};
    };

    struct SlotObj
    {
        Ref::SlotIndex index{0};
        ArrayView      data{};
    };

    virtual size_t  GetSlotCount() const                                           = 0;
    virtual bool    ValidSlot(size_t index) const                                  = 0;
    virtual SlotObj Get(shared_lock const& guardscope, Ref::SlotIndex slot) const  = 0;
    virtual uint8_t Release(exclusive_lock const& guardscope, Ref::SlotIndex slot) = 0;
};

// In memory transformation for temporary computation
template <size_t RecordSize> struct PageForRecord : public PageForRecordInterface
{
    static constexpr auto GetSlotCapacity()
    {
        // n slots need n/8 uint8_ts.
        // n * s + n/8 + 4 = 8192
        // n = ((8192 - 4) * 8) / (8s + 1);
        // s = 1    : 8192 = 7278 * 1 + 910 + 4 (0 uint8_ts wasted)
        // s = 4    : 8192 = 1984 * 4 + 248 + 4 (4 uint8_ts wasted)
        // s = 128  :
        // s = 1024 : 8192 = 7 * 1024 + 1 + 4   (1019 uint8_ts wasted)
        constexpr size_t ApproxSlotCount = ((Page::PageSizeInBytes - sizeof(Page::Header)) * 8) / (8 * RecordSize + 1);
        constexpr size_t AlignmentCost   = sizeof(std::bitset<ApproxSlotCount>) - ((ApproxSlotCount) / 8);
        constexpr size_t SlotCount       = ((Page::PageSizeInBytes - sizeof(Page::Header) - AlignmentCost) * 8) / (8 * RecordSize + 1);
        constexpr size_t SlotMapSize     = sizeof(std::bitset<SlotCount>);
        static_assert(sizeof(Page) == Page::PageSizeInBytes);
        static_assert(((SlotCount * RecordSize) + SlotMapSize + sizeof(Page::Header)) < Page::PageSizeInBytes);
        return SlotCount;
    }

    using shared_lock                 = PageRuntime::shared_lock;
    using exclusive_lock              = PageRuntime::exclusive_lock;
    static constexpr size_t SlotCount = GetSlotCapacity();

    PageForRecord() {}
    PageForRecord(PageRuntime* page) : m_page(page)
    {
        m_pageIndex = page->m_pageIndex;
        m_slots     = (decltype(m_slots))(page->m_page.buffer);
        m_records   = (decltype(m_records))(page->m_page.buffer + sizeof(*m_slots));
        static_assert(sizeof(*m_records) == SlotCount * RecordSize);
        static_assert(sizeof(*m_records) + sizeof(*m_slots) < Page::PageSizeInBytes);
        while (m_page->m_availableSlot < m_slots->size() && m_slots->test(m_page->m_availableSlot)) ++m_page->m_availableSlot;
        // TODO unit test
    }

    size_t GetSlotCount() const override { return SlotCount; }
    bool   ValidSlot(size_t index) const override { return m_slots->test(index); }
    bool   Full(shared_lock const& /*guardscope*/) { return m_page->m_availableSlot >= m_slots->size(); }

    SlotObj Allocate([[maybe_unused]] exclusive_lock const& guardscope)
    {
        assert(!Full(guardscope.shared()));
        assert(!m_slots->test(m_page->m_availableSlot));
        Ref::SlotIndex slot = m_page->m_availableSlot;
        ++m_page->m_availableSlot;
        m_slots->set(slot);
        auto& rec = m_records->at(slot);
        std::fill(rec.begin(), rec.end(), (uint8_t)0);
        m_page->MarkDirty();
        return SlotObj{slot, {rec.size(), rec.data()}};
    }

    uint8_t Release(exclusive_lock const& /*guardscope*/, Ref::SlotIndex slot) override
    {
        auto& rec = m_records->at(slot);
        std::fill(rec.begin(), rec.end(), (uint8_t)0);
        m_page->m_availableSlot = std::min(slot, m_page->m_availableSlot);
        m_page->MarkDirty();
        return 0;
    }

    SlotObj Get(shared_lock const& /*guardscope*/, Ref::SlotIndex slot) const override
    {
        assert(m_slots->test(slot));
        auto& rec = m_records->at(slot);
        return SlotObj{slot, {rec.size(), rec.data()}};
    }

    PageRuntime*            m_page{nullptr};
    Ref::PageIndex          m_pageIndex = 0;
    std::bitset<SlotCount>* m_slots     = nullptr;
    // Warning.. using bitset make this non portable across 32 bit and 64 bit
    std::array<std::array<uint8_t, RecordSize>, SlotCount>* m_records = nullptr;
};

// In memory transformation for temporary computation
template <size_t RecordSize> struct PageForSharedRecord : public PageForRecordInterface
{
    static constexpr auto GetSlotCapacity()
    {
        // n slots need n uint8_ts.
        // n * s + n + 4 = 8192
        // n = ((8192 - 4) / (s + 1);
        // s = 1    : 8192 = 7278 * 1 + 910 + 4 (0 uint8_ts wasted)
        // s = 4    : 8192 = 1984 * 4 + 248 + 4 (4 uint8_ts wasted)
        // s = 128  :
        // s = 1024 : 8192 = 7 * 1024 + 1 + 4   (1019 uint8_ts wasted)
        constexpr Ref::SlotIndex slotcount   = (Page::PageSizeInBytes - sizeof(Page::Header)) / (RecordSize + 1);
        constexpr Ref::SlotIndex slotmapsize = slotcount;
        static_assert(sizeof(Page) == Page::PageSizeInBytes);
        static_assert(((slotcount * RecordSize) + slotmapsize + sizeof(Page::Header)) <= Page::PageSizeInBytes);
        return slotcount;
    }

    using shared_lock                 = PageRuntime::shared_lock;
    using exclusive_lock              = PageRuntime::exclusive_lock;
    static constexpr size_t SlotCount = GetSlotCapacity();

    PageForSharedRecord() {}
    constexpr PageForSharedRecord(PageRuntime* page) : m_page(page)
    {
        m_pageIndex  = page->m_pageIndex;
        //auto& buffer = page->m_page.buffer;
        m_refCounts  = (decltype(m_refCounts))(page->m_page.buffer);
        m_records    = (decltype(m_records))(page->m_page.buffer + sizeof(*m_refCounts));
        static_assert(sizeof(*m_records) == SlotCount * RecordSize);
        static_assert(sizeof(*m_records) + sizeof(*m_refCounts) < Page::PageSizeInBytes);
    }

    size_t  GetSlotCount() const override { return SlotCount; }
    bool    ValidSlot(size_t slot) const override { return m_refCounts->at(slot) > 0; }
    uint8_t GetRefCount(Ref::SlotIndex slot) const { return m_refCounts->at(slot); }
    bool    Full(shared_lock const& guardscope) { return m_page->m_availableSlot >= m_refCounts->size(); }

    SlotObj Allocate(exclusive_lock const& guardscope)
    {
        assert(!Full(guardscope.shared()));
        assert(m_refCounts[m_page->m_availableSlot] == 0u);
        Ref::SlotIndex slot = m_page->m_availableSlot;
        ++m_page->m_availableSlot;
        m_refCounts[m_page->m_availableSlot]++;
        std::array<uint8_t, RecordSize>& rec = m_records->at(slot);
        std::fill(rec.begin(), rec.end(), 0u);
        m_page->MarkDirty();
        return SlotObj{slot, {rec.size(), rec.data()}};
    }

    uint8_t Release(exclusive_lock const& /*guardscope*/, Ref::SlotIndex slot)
    {
        assert(m_refCounts->at(slot) > 0u);
        m_refCounts->at(slot)--;
        if (m_refCounts->at(slot) == 0u)
        {
            std::array<uint8_t, RecordSize>& rec = m_records->at(slot);
            std::fill(rec.begin(), rec.end(), uint8_t{0u});
            m_page->m_availableSlot = std::min(slot, m_page->m_availableSlot);
        }
        return m_refCounts->at(slot);
    }

    SlotObj Get(shared_lock const& /*guardscope*/, Ref::SlotIndex slot) const override
    {
        assert(m_refCounts->at(slot) > 0);
        auto& rec = m_records->at(slot);
        return SlotObj{slot, {rec.size(), rec.data()}};
    }

    PageRuntime*                                            m_page{nullptr};
    uint32_t                                                m_pageIndex = 0;
    std::array<uint8_t, SlotCount>*                         m_refCounts = nullptr;
    std::array<std::array<uint8_t, RecordSize>, SlotCount>* m_records   = nullptr;
};

template <> struct PageForRecord<0> : public PageForRecordInterface
{
    size_t  GetSlotCount() const override { return impl->GetSlotCount(); }
    bool    ValidSlot(size_t index) const override { return impl->ValidSlot(index); }
    SlotObj Get(shared_lock const& guardscope, Ref::SlotIndex slot) const override { return impl->Get(guardscope, slot); }

    PageForRecord(PageRuntime* page) : m_page(page)
    {
        auto typeId = page->m_page.header.typeId;
        assert(typeId > 0xff);
        auto logRecordSize = typeId & 0xff;
        assert(logRecordSize < 12);
        m_recordSize = (uint64_t)1 << logRecordSize;
        switch (logRecordSize)
        {
        default:
        case 0x0:
        case 0x1: throw Logging::TODOCreateException("Blob data size outside range");
        case 0x2: impl = std::make_unique<PageForRecord<(1 << 0x2)>>(m_page); break;
        case 0x3: impl = std::make_unique<PageForRecord<(1 << 0x3)>>(m_page); break;
        case 0x4: impl = std::make_unique<PageForRecord<(1 << 0x4)>>(m_page); break;
        case 0x5: impl = std::make_unique<PageForRecord<(1 << 0x5)>>(m_page); break;
        case 0x6: impl = std::make_unique<PageForRecord<(1 << 0x6)>>(m_page); break;
        case 0x7: impl = std::make_unique<PageForRecord<(1 << 0x7)>>(m_page); break;
        case 0x8: impl = std::make_unique<PageForRecord<(1 << 0x8)>>(m_page); break;
        case 0x9: impl = std::make_unique<PageForRecord<(1 << 0x9)>>(m_page); break;
        case 0xa: impl = std::make_unique<PageForRecord<(1 << 0xa)>>(m_page); break;
        case 0xb: impl = std::make_unique<PageForRecord<(1 << 0xb)>>(m_page); break;
        }
    }

    uint8_t Release(exclusive_lock const& guardscope, Ref::SlotIndex slot) override { return impl->Release(guardscope, slot); }

    std::unique_ptr<PageForRecordInterface> impl;
    PageRuntime*                            m_page;
    size_t                                  m_recordSize = 0;
};

template <> struct PageForSharedRecord<0> : public PageForRecord<0>
{
    PageForSharedRecord(PageRuntime* page) : PageForRecord(page) {}
};

struct JournalPage
{
    struct Header
    {
        Ref::PageIndex startPageIndex{0};
        Ref::PageIndex nextJournalPage{0};
    };

    static constexpr size_t EntryCount = (Page::PageDataSize - sizeof(Header)) / sizeof(ObjTypeId);

    struct Entry
    {
        Ref::PageIndex pageIndex;
        ObjTypeId      typeId;
    };

    JournalPage(PageRuntime* page) : _page(page)
    {
        if (_page == nullptr) return;
        _header  = reinterpret_cast<Header*>(page->m_page.buffer);
        _typeIds = reinterpret_cast<ObjTypeId*>(page->m_page.buffer + sizeof(Header));
    }

    Ref::PageIndex GetEntryCount() const { return EntryCount; }
    Entry          GetJournalEntry(Ref::PageIndex index) const
    {
        return {(Ref::PageIndex)(_header->startPageIndex + index), _GetRef((Ref::PageIndex)(_header->startPageIndex + index))};
    }

    Ref::PageIndex GetNextJornalPage() const { return _header->nextJournalPage; }

    ObjTypeId& _GetRef(Ref::PageIndex pageIndex) const
    {
        assert(pageIndex >= _header->startPageIndex);
        assert(pageIndex < _header->startPageIndex + EntryCount);
        return _typeIds[pageIndex - _header->startPageIndex];
    }
    void RecordJournalEntry(Entry const& entry)
    {
        assert(_GetRef(entry.pageIndex) == 0);
        _GetRef(entry.pageIndex) = entry.typeId;
        _page->MarkDirty();
    }

    void InitializeEmptyJournal(Ref::PageIndex startPageIndex)
    {
        std::fill(std::begin(_page->m_page.buffer), std::end(_page->m_page.buffer), (uint8_t)0);
        _header->startPageIndex = startPageIndex;
        _page->MarkDirty();
    }

    Header*      _header{};
    ObjTypeId*   _typeIds{};
    PageRuntime* _page = nullptr;
};

struct HeaderPage
{
    HeaderPage(PageRuntime* page) : _page(page) {}
    PageRuntime* _page;
};

struct PageManager
{
    public:    // Constructor, destructor
    PageManager() = default;

    PageManager(std::filesystem::path const& path)
    {
        _serdes.Attach(path);
        _Initialize();
    }

    template <typename TStream> PageManager(TStream&& stream)
    {
        _serdes.AttachStream(std::move(stream));
        _Initialize();
    }
    ~PageManager() { Flush(); }

    void Init(std::filesystem::path const& path)
    {
        _serdes.Attach(path);
        _Initialize();
    }

    void Init()
    {
        _serdes.InitInMemory();
        _Initialize();
    }

    void Flush()
    {
        for (auto& page : _pages)
        {
            if (page == nullptr) continue;
            page->Flush(_serdes);
        }
    }

    public:    // Methods
    Ref::PageIndex GetPageCount() const { return (Ref::PageIndex)_pages.size(); }
    ObjTypeId      GetPageObjTypeId(Ref::PageIndex pageIndex) const { return _pageTypes[pageIndex]; }

    impl::PageRuntime* LoadPage(ObjTypeId id, Ref::PageIndex pageIndex)
    {
        auto& page = _pages[pageIndex];
        if (page != nullptr)
        {
            return page.get();
        }
        page              = std::make_unique<impl::PageRuntime>(id);
        page->m_pageIndex = pageIndex;
        _serdes.ReadPage(page->m_page, pageIndex);
        return page.get();
    }

    impl::PageRuntime* CreateNewPage(ObjTypeId objTypeId)
    {
        auto ptr         = new impl::PageRuntime(objTypeId);
        ptr->m_pageIndex = (Ref::PageIndex)_pages.size();
        assert(_pages.size() == _pageTypes.size());
        _pages.push_back(std::unique_ptr<impl::PageRuntime>(ptr));
        _pageTypes.push_back(objTypeId);
        _serdes.WritePage(ptr->m_page, ptr->m_pageIndex);
        _RecordJournalEntry(ptr->m_pageIndex, objTypeId);
        return ptr;
    }

    private:    // Methods
    void _Initialize()
    {
        auto pageCount = _serdes.GetInputPageCount();
        assert(pageCount >= 2);
        _pages.resize(pageCount);    // atleast one header page and one journal page
        _pageTypes.resize(pageCount, 0);
        _header = LoadPage(0, 0);
        _LoadJournal();
    }

    void _RecordJournalEntry(Ref::PageIndex pageIndex, ObjTypeId objTypeId)
    {
        // TODO check if journal is full
        _journal.RecordJournalEntry({pageIndex, objTypeId});
    }

    void _LoadJournal()
    {
        Ref::PageIndex i = 1;
        while (i != 0)
        {
            _journal = JournalPage(LoadPage(0, i));
            for (Ref::PageIndex j = 0; j < _journal.GetEntryCount(); j++)
            {
                auto entry = _journal.GetJournalEntry(j);
                if (entry.typeId == 0)
                {
                    continue;
                }
                assert(_pageTypes[entry.pageIndex] == 0);
                _pageTypes[entry.pageIndex] = entry.typeId;
            }
            i = _journal.GetNextJornalPage();
        }
    }

    private:    // Members
    HeaderPage   _header{nullptr};
    JournalPage  _journal{nullptr};
    impl::SerDes _serdes;

    std::vector<ObjTypeId>                          _pageTypes;
    std::vector<std::unique_ptr<impl::PageRuntime>> _pages;
};

template <typename TDb, typename TObj, typename TLock> struct Iterator
{
    public:
    using Database = Database2::DatabaseT<TDb>;
    using Ref      = Database2::Ref<TDb, TObj>;
    using Traits   = Database2::ObjTraits<TDb, TObj>;
    struct Range
    {
        Iterator begin;
        Iterator end;
    };

    static auto Begin(TLock* lock, Database& db)
    {
        Iterator it;
        it.m_lock    = lock;
        it.m_db      = &db;
        it.m_current = impl::Ref(0, 0);
        it._MoveToValidSlot();
        return it;
    }

    static auto End() { return Iterator(); }
    static auto Range(Database& db) { return Range(Begin(db), End()); }

    bool      operator==(Iterator const& rhs) const { return m_lock == rhs.m_lock && m_db == rhs.m_db && m_current == rhs.m_current; }
    bool      operator!=(Iterator const& rhs) const { return !(*this == rhs); }
    Iterator& operator++()
    {
        m_current.slot++;
        _MoveToValidSlot();
        return *this;
    }

    auto operator*() { return this->Get(); }

    auto Get()
    {
        if (m_db == nullptr)
        {
            throw Logging::TODOCreateException("Reached the end of iteration");
        }
        return this->m_db->Get(*this->m_lock, this->m_current);
    }

    void _MoveToValidSlot()
    {
        if (m_db == nullptr)
        {
            return;
        }

        auto& pi      = m_current.page;
        auto& si      = m_current.slot;
        auto& pagemgr = m_db->_pagemgr;

        for (; pi < pagemgr.GetPageCount(); pi++, si = 0)
        {
            if (pagemgr.GetPageObjTypeId(pi) == 0) continue;
            if constexpr (Traits::RecordSize() == 0)
            {
                if ((pagemgr.GetPageObjTypeId(pi) & ~(0xff)) != Traits::TypeId())
                {
                    continue;
                }
            }
            else
            {
                if (pagemgr.GetPageObjTypeId(pi) != Traits::TypeId())
                {
                    continue;
                }
            }

            PageForRecord<Traits::RecordSize()> page = pagemgr.LoadPage(Traits::TypeId(), pi);
            for (; si != page.GetSlotCount(); ++si)
            {
                if (page.ValidSlot(si))
                {
                    return;
                }
            }
        }
        *this = End();
    }

    Iterator(shared_lock& lock, Database& db) : m_db(db), m_lock(&lock) {}
    Iterator() = default;

    TLock*    m_lock = nullptr;
    Database* m_db   = nullptr;
    Ref       m_current{impl::Ref::Invalid()};
};

template <typename TDb, typename TObj, typename TLock> struct RefAndObjIterator : public Iterator<TDb, TObj, TLock>
{
    auto operator*() { return std::make_tuple(this->m_current, this->Get()); }
};

}    // namespace Database2::impl

namespace Database2
{
template <typename TDb, typename TObj> struct Ref : impl::Ref
{
    Ref() = default;
    Ref(impl::Ref const& val) : impl::Ref(val) {}
};
#if 0
struct RefOwnerMarker
{
};

template <typename TDb, typename TOwner, typename... TObjs> struct RefOwner : RefOwnerMarker
{
    //   template <typename... TArgs> RefOwner(TArgs&&... args) : TData{std::forward<TArgs>(args)...} {}
    template <typename TObj> using Traits = ObjTraits<TDb, TObj>;
    template <typename TObj> using WireT  = typename Traits<TObj>::WireType;    // Snapshot of the serilized buffer (no lock)
    template <typename TObj> using SnapT  = typename Traits<TObj>::SnapType;    // Snapshot of the serilized buffer (no lock)
    template <typename TObj> using ViewT  = typename Traits<TObj>::ViewType;    // Use the serialized buffer (with read lock)
    template <typename TObj> using EditT  = typename Traits<TObj>::EditType;    // Edit the serialized buffer (with write lock)

    template <size_t N> using Nth = typename std::tuple_element<N, std::tuple<TObjs...>>::type;

    template <size_t N> struct ChildRef
    {
        auto Get(shared_lock const& lock, TDb& db, TOwner& obj) { return db.template Get<Nth<N>>(lock, obj.ref[N]); }
    };

    static constexpr size_t GetChildRefCount() { return sizeof...(TObjs); }

    template <size_t N> Ref<TDb, Nth<N>> GetRef() { return ref[N]; }
    template <size_t N> void             SetRef(impl::Ref r) { ref[N] = r; }
    impl::Ref                            ref[sizeof...(TObjs)];
};
#endif
struct ChildRefMarker
{
};
template <typename TObj> struct ChildRef : ChildRefMarker
{
    using Obj = TObj;
    template <typename TLock, typename TDb> auto Get(TLock const& lock, TDb& db) const
    {
        if (ref == impl::Ref::Invalid())
        {
            decltype(db.Get(lock, (Ref<TDb, TObj>)ref)) obj;
            return obj;
        }
        return db.Get(lock, Ref<TDb, TObj>{ref});
    }
    template <typename TLock, typename TDb> void Release(TLock const& lock, TDb& db)
    {
        if (ref.Valid())
        {
            db.Delete(lock, Ref<TDb, TObj>{ref});
            ref = impl::Ref{0, 0};
        }
    }

    template <typename TLock, typename TDb, typename... TArgs> void Reset(TLock const& lock, TDb& db, TArgs&&... args)
    {
        Release(lock, db);
        ref = std::get<0>(db.template Create<Obj>(lock, std::forward<TArgs>(args)...));
    }
    impl::Ref ref{0, 0};
};
template <typename TDb> struct DatabaseT
{
    public:    // Internal type declarations
    using rlock = shared_lock;
    using wlock = exclusive_lock;

    template <typename TObj> using RefT        = Ref<TDb, TObj>;
    template <typename TObj> using Traits      = ObjTraits<TDb, TObj>;
    template <typename TObj> using WireT       = typename Traits<TObj>::WireType;    // Snapshot of the serilized buffer (no lock)
    template <typename TObj> using SnapT       = typename Traits<TObj>::SnapType;    // Snapshot of the serilized buffer (no lock)
    template <typename TObj> using ViewT       = typename Traits<TObj>::ViewType;    // Use the serialized buffer (with read lock)
    template <typename TObj> using EditT       = typename Traits<TObj>::EditType;    // Edit the serialized buffer (with write lock)
    template <typename TObj> using RefAndViewT = std::tuple<RefT<TObj>, ViewT<TObj>>;
    template <typename TObj> using RefAndEditT = std::tuple<RefT<TObj>, EditT<TObj>>;
    template <typename TObj> struct RangeForViewT
    {
        rlock& m_lock;

        impl::RefAndObjIterator<TDb, TObj, rlock> m_begin;
        impl::RefAndObjIterator<TDb, TObj, rlock> m_end;

        auto begin() const { return m_begin; }
        auto end() const { return m_end; }
    };
    template <typename TObj> struct RangeForEditT
    {
        wlock& m_lock;

        impl::RefAndObjIterator<TDb, TObj, wlock> m_begin;
        impl::RefAndObjIterator<TDb, TObj, wlock> m_end;

        auto begin() const { return m_begin; }
        auto end() const { return m_end; }
    };

    template <typename TObj> constexpr ObjTypeId TypeId() { return Traits<TObj>::TypeId(); }

    public:    // Constructor Destructor
    DatabaseT() {}
    DatabaseT(TDb* ptr) { Init(ptr); }
    DatabaseT(TDb* ptr, std::filesystem::path const& path) : _ptr(ptr), _pagemgr(path){};
    DatabaseT(TDb* ptr, std::ofstream&& stream) : _ptr(ptr), _pagemgr(stream){};
    DatabaseT(TDb* ptr, std::ifstream&& stream) : _ptr(ptr), _pagemgr(stream){};

    public:    // Methods
    rlock LockForRead() { return shared_lock(_mutex); }
    wlock LockForEdit() { return exclusive_lock(_mutex); }

    template <size_t TRecordSize> auto _Allocate(wlock const& lock, ObjTypeId typeId)
    {
        impl::PageForRecord<TRecordSize> page;
        assert(_pagemgr.GetPageCount() > 1);
        for (impl::Ref::PageIndex i = _pagemgr.GetPageCount() - (uint8_t)1; i > 0; i--)
        {
            if (_pagemgr.GetPageObjTypeId(i) != typeId) continue;
            page = _pagemgr.LoadPage(typeId, i);
            if (!page.Full(lock.shared())) break;
        }

        if (page.m_pageIndex == 0 || page.Full(lock.shared()))
        {
            page = _pagemgr.CreateNewPage(typeId);
        }

        auto slot = page.Allocate(lock);

        return std::make_tuple(impl::Ref(page.m_pageIndex, slot.index), (void*)slot.data.buffer);
    }

    template <size_t LogN> auto _AllocateForDataSize(size_t dataSize, wlock const& lock, ObjTypeId typeId)
    {
        constexpr size_t RecordSize = (((size_t)2u) << (LogN - 1));
        if (dataSize > RecordSize)
        {
            assert(dataSize <= RecordSize * 2);
            return std::tuple_cat(_Allocate<RecordSize * 2>(lock, typeId + LogN + 1), std::tie(RecordSize));
        }
        else
        {
            if constexpr (LogN > 0x2)
            {
                return _AllocateForDataSize<LogN - 1>(dataSize, lock, typeId);
            }
            else
            {
                throw "TODO";
            }
        }
    }

    template <size_t N, typename TObj, typename TArg> void _FillParent_N([[maybe_unused]] wlock const& lock, WireT<TObj>& obj, TArg&& arg)
    {
        using Nth = std::remove_reference_t<decltype(Traits<TObj>::template StructMember<N>(obj))>;
        if constexpr (std::is_base_of<ChildRefMarker, Nth>::value)
        {
            auto& ptr = Traits<TObj>::template StructMember<N>(obj);
            if constexpr (is_tuple<TArg>::value)
            {
                std::apply([&](auto... args) { ptr.ref = std::get<0>(Create<typename Nth::Obj>(lock, args...)); }, std::forward<TArg>(arg));
            }
            else
            {
                ptr.ref = std::get<0>(Create<typename Nth::Obj>(lock, std::forward<TArg>(arg)));
            }
        }
        else
        {
            auto& ptr = Traits<TObj>::template StructMember<N>(obj);
            ptr       = std::forward<TArg>(arg);
            //            std::swap(*Traits<TObj>::StructMember<N>(obj), arg);
        }
    }

    template <size_t N, size_t Count, typename TObj, typename TArg, typename... TArgs>
    void _FillParent(wlock const& lock, WireT<TObj>& obj, TArg&& arg, TArgs&&... args)
    {
        _FillParent_N<N, TObj>(lock, obj, std::forward<TArg>(arg));
        if constexpr ((N + 1) < Count)
        {
            _FillParent<N + 1, Count, TObj>(lock, obj, std::forward<TArgs>(args)...);
        }
    }

    template <typename TObj, typename... TArgs> RefAndEditT<TObj> Create(wlock const& lock, TArgs&&... args)
    {
        if constexpr (Traits<TObj>::RecordSize() == 0)
        {
            auto typeId   = TypeId<TObj>();
            auto datasize = Traits<TObj>::GetDataSize(std::forward<TArgs>(args)...);
            if (datasize == 0)
            {
                EditT<TObj> obj;
                return RefAndEditT<TObj>(RefT<TObj>{}, obj);
            }
            auto [ref, buffer, recsize] = _AllocateForDataSize<0xb>(datasize, lock, typeId);
            if constexpr (Traits<TObj>::StructMemberCount() > 0)
            {
                // auto wireobj = new (buffer) WireT<TObj>();
                // static_assert(sizeof...(args) == Traits<TObj>::StructMemberCount());
                //_FillParent<sizeof...(args)>(lock, *wireobj, std::forward<TArgs>(args)...);
                // return RefAndEditT<TObj>(RefT<TObj>{ref}, *wireobj);

                // The first arg is for base
                // Next N args are for N child objects
                // The rest of the args are for the child objects.
                // TODO figure out if thats possible
                TODO("TODO_OBJREF");
            }
            else
            {
                auto editptr = new (buffer) WireT<TObj>{recsize, std::forward<TArgs>(args)...};
                return RefAndEditT<TObj>(RefT<TObj>{ref}, *editptr);
            }
        }
        else
        {
            auto [ref, buffer] = _Allocate<Traits<TObj>::RecordSize()>(lock, TypeId<TObj>());
            if constexpr (Traits<TObj>::StructMemberCount() > 0)
            {
                //                auto wireobj = new (buffer) WireT<TObj>{};

                // static_assert(Traits<TObj>::StructMemberCount() == sizeof...(args));
                // auto wireobj = new (buffer) Traits<TObj>::WireT{};
                auto wireobj = new (buffer) WireT<TObj>();
                static_assert(sizeof...(args) == Traits<TObj>::StructMemberCount());
                _FillParent<0, sizeof...(args), TObj>(lock, *wireobj, std::forward<TArgs>(args)...);
                return RefAndEditT<TObj>(RefT<TObj>{ref}, *wireobj);

                // return lambda(std::forward<TArgs>(args)...);
            }
            else
            {
                auto editptr = new (buffer) WireT<TObj>{std::forward<TArgs>(args)...};
                return RefAndEditT<TObj>(RefT<TObj>{ref}, *editptr);
            }
        }
    }

    template <typename TObj> SnapT<TObj> Get(RefT<TObj> const& id) { return Get(LockForRead(), id); };

    template <typename TObj> ViewT<TObj> Get(rlock const& lock, RefT<TObj> const& id)
    {
        assert(id.Valid());
        assert(id.page < _pagemgr.GetPageCount());
        impl::PageForRecord<Traits<TObj>::RecordSize()> page(_pagemgr.LoadPage(TypeId<TObj>(), id.page));

        auto slot   = page.Get(lock, id.slot);
        auto objptr = reinterpret_cast<WireT<TObj>*>(slot.data.buffer);
        return *objptr;
    }

    template <typename TObj> EditT<TObj> Get(wlock const& lock, RefT<TObj> const& id)
    {
        assert(id.Valid());
        assert(id.page < _pagemgr.GetPageCount());
        impl::PageForRecord<Traits<TObj>::RecordSize()> page(_pagemgr.LoadPage(TypeId<TObj>(), id.page));

        auto slot = page.Get(lock.shared(), id.slot);
        page.m_page->MarkDirty();
        EditT<TObj> editobj(*reinterpret_cast<WireT<TObj>*>(slot.data.buffer));
        page.m_page->MarkDirty();    // TODO : test
        return editobj;
    }

    template <typename TObj> RangeForViewT<TObj> Objects(rlock& lock)
    {
        return RangeForViewT<TObj>{
            lock, impl::RefAndObjIterator<TDb, TObj, rlock>::Begin(&lock, *this), impl::RefAndObjIterator<TDb, TObj, rlock>::End()};
    }

    template <typename TObj> RangeForEditT<TObj> Objects(wlock& lock)
    {
        return RangeForEditT<TObj>{
            lock, impl::RefAndObjIterator<TDb, TObj, wlock>::Begin(&lock, *this), impl::RefAndObjIterator<TDb, TObj, wlock>::End()};
    }

    template <typename TObj> void _ReleaseChildRefs(wlock const& /*lock*/, impl::PageForRecordInterface::SlotObj /*slotobj*/)
    {
        TODO("TODO_OBJREF");
#if TODO_OBJREF
        if constexpr (std::is_base_of<RefOwnerMarker, TObj>::value)
        {
            using WireT = typename TObj::WireT;
            auto& obj   = *reinterpret_cast<WireT*>(slotobj.data.buffer);
            static_assert(TObj::RefObjCount() < 8);
            if constexpr (TObj::RefObjCount() > 0) Delete(lock, obj.template GetRef<0>());
            if constexpr (TObj::RefObjCount() > 1) Delete(lock, obj.template GetRef<1>());
            if constexpr (TObj::RefObjCount() > 2) Delete(lock, obj.template GetRef<2>());
            if constexpr (TObj::RefObjCount() > 3) Delete(lock, obj.template GetRef<3>());
            if constexpr (TObj::RefObjCount() > 4) Delete(lock, obj.template GetRef<4>());
            if constexpr (TObj::RefObjCount() > 5) Delete(lock, obj.template GetRef<5>());
            if constexpr (TObj::RefObjCount() > 6) Delete(lock, obj.template GetRef<6>());
            if constexpr (TObj::RefObjCount() > 7) Delete(lock, obj.template GetRef<7>());
        }
#endif
    }

    template <typename TObj> void Delete(wlock const& lock, RefT<TObj> const& id)
    {
        // If ownership == shared, Decrease ref count
        // If ownership == unique or ref count == 0
        //      Iterate over all sub-objects.
        //      Release all the sub-data-objects
        //      Release the slot
        //      Mark the page as dirty
        if (Traits<TObj>::GetOwnership() == Ownership::Shared)
        {
            impl::PageForSharedRecord<Traits<TObj>::RecordSize()> page(_pagemgr.LoadPage(TypeId<TObj>(), id.page));

            auto refcount = page.Release(lock, id.slot);
            if (refcount == 0)
            {
                _ReleaseChildRefs<TObj>(lock, page.Get(lock.shared(), id.slot));
            }
        }
        else
        {
            impl::PageForRecord<Traits<TObj>::RecordSize()> page(_pagemgr.LoadPage(TypeId<TObj>(), id.page));
            page.Release(lock, id.slot);
        }
    }

    ~DatabaseT() {}

    void Init(TDb* thisptr, std::filesystem::path const& path)
    {
        assert(thisptr != nullptr && _ptr == nullptr);
        _ptr = thisptr;
        _pagemgr.Init(path);
    }
    void Init(TDb* thisptr)
    {
        assert(thisptr != nullptr && _ptr == nullptr);
        _ptr = thisptr;
        _pagemgr.Init();
    }
    void Flush(exclusive_lock const& /*guardscope*/) { _pagemgr.Flush(); }

    private:
    TDb*              _ptr{nullptr};
    std::shared_mutex _mutex;
    impl::PageManager _pagemgr;

    // friends
    template <typename _TDb, typename TObj, typename TLock> friend struct impl::Iterator;

};    // namespace Database2

template <ObjTypeId TId, Ownership TOwnership, Encryption TEncrypted, typename TChar> struct TStringTraits
{
    static constexpr size_t     RecordSize() { return 0; }
    static constexpr ObjTypeId  TypeId() { return TId; }
    static constexpr Ownership  GetOwnership() { return TOwnership; }
    static constexpr Encryption Encrypted() { return TEncrypted; }
    using ViewType = std::basic_string_view<TChar>;
    using EditType = std::basic_string_view<TChar>;

    struct WireType
    {
        uint16_t size = 0;
        uint8_t  ptr[1];
        WireType(size_t /*bufsize*/, std::basic_string_view<TChar> const& str)
        {
            size = static_cast<uint16_t>(str.size() * sizeof(TChar));
            std::copy(str.begin(), str.end(), (TChar*)ptr);
        }

        operator EditType() { return std::basic_string_view<TChar>((TChar*)ptr, size / sizeof(TChar)); }
        operator ViewType() const { return std::basic_string_view<TChar>((TChar*)ptr, size / sizeof(TChar)); }
    };

    static ViewType View(WireType const& obj) { return std::basic_string_view<TChar>((TChar*)obj.ptr, obj.size / sizeof(TChar)); }

    static EditType Edit(WireType const& obj) { return std::basic_string_view<TChar>((TChar*)obj.ptr, obj.size / sizeof(TChar)); }

    static size_t           GetDataSize(ViewType const& str) { return str.empty() ? 0 : (str.size() * sizeof(TChar) + sizeof(uint16_t)); }
    constexpr static size_t StructMemberCount() { return 0; }    // Unavailable / not-needed
    static WireType         Create(WireType const& str) { return {}; }
};

using ByteStringTraits                = TStringTraits<0x200, Ownership::Self, Encryption::No, char>;
using ByteSharedStringTraits          = TStringTraits<0x300, Ownership::Shared, Encryption::No, char>;
using ByteUniqueStringTraits          = TStringTraits<0x400, Ownership::Unique, Encryption::No, char>;
using ByteEncryptedStringTraits       = TStringTraits<0x500, Ownership::Self, Encryption::Yes, char>;
using ByteEncryptedSharedStringTraits = TStringTraits<0x600, Ownership::Shared, Encryption::Yes, char>;
using ByteEncryptedUniqueStringTraits = TStringTraits<0x700, Ownership::Unique, Encryption::Yes, char>;

using WideStringTraits                = TStringTraits<0x800, Ownership::Self, Encryption::No, wchar_t>;
using WideSharedStringTraits          = TStringTraits<0x900, Ownership::Shared, Encryption::No, wchar_t>;
using WideUniqueStringTraits          = TStringTraits<0xA00, Ownership::Unique, Encryption::No, wchar_t>;
using WideEncryptedStringTraits       = TStringTraits<0xB00, Ownership::Self, Encryption::Yes, wchar_t>;
using WideEncryptedSharedStringTraits = TStringTraits<0xC00, Ownership::Shared, Encryption::Yes, wchar_t>;
using WideEncryptedUniqueStringTraits = TStringTraits<0xD00, Ownership::Unique, Encryption::Yes, wchar_t>;

#define DEFINESTRING(str)                                                                                        \
    using str = str##Traits::WireType;                                                                           \
    template <typename TDb> struct ObjTraits<TDb, str> : str##Traits                                             \
    {                                                                                                            \
        static WireType Create(TDb& db, exclusive_lock const& lock, ViewType const& str) { return Create(str); } \
    };
DEFINESTRING(ByteString)
DEFINESTRING(ByteSharedString)
DEFINESTRING(ByteUniqueString)
DEFINESTRING(ByteEncryptedString)
DEFINESTRING(ByteEncryptedSharedString)
DEFINESTRING(ByteEncryptedUniqueString)
DEFINESTRING(WideString)
DEFINESTRING(WideSharedString)
DEFINESTRING(WideUniqueString)
DEFINESTRING(WideEncryptedString)
DEFINESTRING(WideEncryptedSharedString)
DEFINESTRING(WideEncryptedUniqueString)

}    // namespace Database2

namespace Database2
{

template <typename TDb, typename TObj> struct OwnerT : public virtual DatabaseT<TDb>
{
};

template <typename TDb, typename TObj> struct ObjectT
{
};

template <typename TObj> struct EncryptedT
{
};
}    // namespace Database2
