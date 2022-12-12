#pragma once
#include "CommonMacros.h"

#include <concepts>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <tuple>
#include <unordered_map>

namespace Stencil::Database    // Type/Trait Declarations
{

template <typename T> struct RecordTraits;

template <typename T> struct FixedSizeRecordTraits;
template <typename T> struct BlobRecordTraits;
template <typename T> struct ComplexRecordTraits;

template <typename T>
concept ConceptRecord = requires { typename RecordTraits<T>::RecordTypes; };

template <typename T>
concept ConceptComplex = ConceptRecord<T> && std::tuple_size_v<typename RecordTraits<T>::RecordTypes> > 1;

template <typename T>
concept ConceptTrivial = ConceptRecord<T> && !(ConceptComplex<T>);

template <typename T>
concept ConceptBlob = ConceptRecord<T> && ConceptTrivial<T> && RecordTraits<T>::Size() == 0;

template <typename T>
concept ConceptFixedSize = ConceptRecord<T> && ConceptTrivial<T> && RecordTraits<T>::Size() > 0;

template <ConceptRecord T> struct Ref
{
    uint32_t id{0};
};

template <typename T> constexpr bool IsRef         = false;
template <typename T> constexpr bool IsRef<Ref<T>> = true;

template <typename T>
concept ConceptRef = IsRef<T>;

template <typename T, typename TTup, size_t I = 0> constexpr size_t tuple_index_of()
{
    if constexpr (std::tuple_size<TTup>::value == I)
    {
        static_assert(!std::is_same_v<T, T>, "Tuple Out of range");
        return I;
    }
    else
    {
        if constexpr (std::is_same_v<std::tuple_element_t<I, TTup>, T>) { return I; }
        else { return tuple_index_of<T, TTup, I + 1>(); }
    }
}

template <typename... Tuples> using tuple_cat_t = decltype(std::tuple_cat(std::declval<Tuples>()...));

template <ConceptRecord T, typename TDb>
static constexpr uint16_t TypeId = static_cast<uint16_t>(tuple_index_of<T, typename TDb::RecordTypes>());

template <typename T> struct Record;
template <typename T> struct RecordT
{};

template <typename T> struct RecordNest
{
    using Type = T;
};

template <ConceptRecord T> struct RecordNest<T>
{
    using Type = Record<T>;
};

template <ConceptRecord T> using RefAndRecord = std::tuple<Ref<T>, Record<T>>;

using ROLock = std::unique_lock<std::shared_mutex>;
using RWLock = std::unique_lock<std::shared_mutex>;

template <typename T>
concept ConceptLock = std::is_same_v<T, ROLock> || std::is_same_v<T, RWLock>;

template <typename T> struct List
{};

template <ConceptFixedSize T> struct FixedSizeRecordTraits<T>
{
    static constexpr uint32_t GetDataSize() { return sizeof(T); }
    static void               WriteToBuffer(T const& obj, Record<T>& rec)
    {
        static_assert(sizeof(T) == sizeof(Record<T>), "For Fixed sized records Object and Record should be identical");
        rec = *reinterpret_cast<Record<T> const*>(&obj);
    }
};

}    // namespace Stencil::Database

namespace Stencil::Database::impl
{

struct Ref
{
    typedef uint16_t PageIndex;
    typedef uint16_t SlotIndex;
    PageIndex        page{0};
    SlotIndex        slot{0};

    bool operator==(Ref const& rhs) const { return page == rhs.page && slot == rhs.slot; }

    constexpr Ref() = default;
    constexpr Ref(PageIndex pageIn, SlotIndex slotIn) : page(pageIn), slot(slotIn) {}

    template <ConceptRecord T>
    constexpr Ref(Stencil::Database::Ref<T> const& ref) :
        page(static_cast<uint16_t>(ref.id >> 16)), slot(static_cast<uint16_t>(ref.id & 0xffff))
    {}

    Ref(Ref const& val)            = default;
    Ref& operator=(Ref const& val) = default;

    template <ConceptRecord T> operator Stencil::Database::Ref<T>() const
    {
        return Stencil::Database::Ref<T>{(uint32_t{page} << 16) | uint32_t{slot}};
    }

    static Ref Invalid() { return Ref{}; }
    bool       Valid() const { return page >= 2 && slot < 1000; }
    Ref&       IncrementSlot()
    {
        slot++;
        return *this;
    }

    static auto FromUInt(uint32_t value)
    {
        SlotIndex slot = static_cast<uint16_t>(value & 0xff);
        PageIndex page = static_cast<uint16_t>(value >> 16);
        return Ref(page, slot);
    }
};

struct SlotObj
{
    Ref::SlotIndex     index{0};
    std::span<uint8_t> data{};
};

struct SlotView
{
    Ref::SlotIndex           index{0};
    std::span<uint8_t const> data{};
};

// Keep this always at 8192 uint8_ts to optimize memory usage
struct Page
{
    constexpr static size_t PageSizeInBytes      = 8192;
    constexpr static size_t PageDataSize         = PageSizeInBytes;
    uint8_t                 buffer[PageDataSize] = {};
};

static_assert(sizeof(Page) == Page::PageSizeInBytes);

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
            if (fstatus.type() != std::filesystem::file_type::regular) { throw std::runtime_error("File is not a regular file"); }
            if ((fstatus.permissions() & std::filesystem::perms::owner_write) == std::filesystem::perms::none)
            {
                AttachStream(std::ifstream(path));
            }
            else { AttachStream(std::fstream(path, std::fstream::binary | std::fstream::in | std::fstream::out)); }
        }
        else { AttachStream(std::fstream(path, std::fstream::binary | std::fstream::in | std::fstream::out | std::fstream::trunc)); }
    }

    void AttachStream(std::fstream&& stream)
    {
        assert(!stream.fail());
        _iostream = std::move(stream);
        _AttachStream(static_cast<std::istream*>(&_iostream));
        _AttachStream(static_cast<std::ostream*>(&_iostream));
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
    SerDes() = default;
    CLASS_DELETE_COPY_AND_MOVE(SerDes);

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
        if (offset == 0) { return; }

        _ReadHeader(_curheader, istream);
        _ReadPage(*_loadedPages[0], 0, istream);
        _ReadPage(*_loadedPages[1], 1, istream);
    }

    void ReadPage(Page& page, uint32_t index)
    {
        if (_loadedPages.size() == 0)
        {
            if (_readFrom == nullptr) throw std::runtime_error("No Input file attached");
            _ReadPage(page, index, *_readFrom);
        }
        else
        {
            if (_readFrom != nullptr) throw std::runtime_error("Invalid State");
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
            if (_readFrom == nullptr) throw std::runtime_error("No Input file attached");
            _readFrom->seekg(0, std::ios_base::end);
            auto offset = _readFrom->tellg();
            return GetPageIndexFromOffset(offset);
        }
        else
        {
            if (_readFrom != nullptr) throw std::runtime_error("Invalid State");
            return static_cast<uint32_t>(_loadedPages.size());
        }
    }

    private:
    static std::streamoff PageStreamOffset(size_t page) { return std::streamoff(page * Page::PageSizeInBytes + sizeof(Header)); }

    static uint32_t GetPageIndexFromOffset(std::streamoff offset)
    {
        assert((static_cast<uint32_t>(offset) - sizeof(Header)) % Page::PageSizeInBytes == 0);
        return static_cast<uint32_t>((static_cast<uint32_t>(offset) - sizeof(Header)) / Page::PageSizeInBytes);
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
        stream.write(reinterpret_cast<const char*>(&header), sizeof(header));
        assert(!stream.fail());
    }

    static void _ReadHeader(Header& header, std::istream& stream)
    {
        assert(!stream.fail());
        stream.seekg(0, std::ios_base::beg);
        stream.read(reinterpret_cast<char*>(&header), sizeof(header));
        assert(!stream.fail());
    }

    static void _ReadPage(Page& page, uint32_t index, std::istream& stream)
    {
        assert(!stream.fail());
        std::streamoff offsetreq{PageStreamOffset(index)};
        stream.seekg(offsetreq + static_cast<std::streampos>(Page::PageSizeInBytes), std::ios_base::beg);
        auto offsetcur = stream.tellg();
        if (offsetcur != (offsetreq + static_cast<std::streampos>(Page::PageSizeInBytes))) { throw std::runtime_error("Invalid Page Ref"); }
        stream.seekg(offsetreq, std::ios_base::beg);
        assert(!stream.fail());
        offsetcur = stream.tellg();
        assert(!stream.fail());
        if (offsetcur != static_cast<std::streampos>(offsetreq)) throw std::runtime_error("Invalid Page Ref");

        stream.read(reinterpret_cast<char*>(&page), Page::PageSizeInBytes);
        assert(!stream.fail());
    }

    static void _WritePage(Page const& page, uint32_t index, std::ostream& stream)
    {
        assert(!stream.fail());
        std::streamoff offsetreq{PageStreamOffset(index)};
        stream.seekp(offsetreq, std::ios_base::beg);
        auto offsetcur = stream.tellp();
        assert(offsetcur >= 0);
        assert((offsetcur <= offsetreq) && ((static_cast<size_t>(offsetreq - offsetcur) % Page::PageSizeInBytes) == 0));
        uint8_t zerobuffer[Page::PageSizeInBytes] = {};
        assert(zerobuffer[0] == 0 && zerobuffer[Page::PageSizeInBytes - 1] == 0);
        while (offsetcur < offsetreq)
        {
            stream.write(reinterpret_cast<const char*>(zerobuffer), Page::PageSizeInBytes);
            offsetcur = stream.tellp();
        }
        stream.write(reinterpret_cast<const char*>(&page), sizeof(Page));
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
    public:
    void SetTypeId(uint32_t typeId, uint32_t pageRecDataSize)
    {
        _typeId          = typeId;
        _pageRecDataSize = pageRecDataSize;
    }

    bool Loaded() { return _page != nullptr; }
    void Load(SerDes& serdes)
    {
        _page.reset(new Page());
        serdes.ReadPage(*_page, _pageIndex);
    }

    void InitPage() { _page.reset(new Page()); }
    void WriteTo(SerDes& serdes) { serdes.WritePage(*_page, _pageIndex); }
    void MarkSlotFree(Ref::SlotIndex slot)
    {
        _availableSlot = std::min(slot, _availableSlot);
        MarkDirty();
    }

    void MarkDirty() { _flags.set(static_cast<size_t>(Flag::Dirty)); }
    void Flush(SerDes& serdes)
    {
        if (!_flags.test(static_cast<size_t>(Flag::Dirty))) return;
        serdes.WritePage(*_page, _pageIndex);
        _flags.reset(static_cast<size_t>(Flag::Dirty));
    }
    PageRuntime() = default;
    PageRuntime(Ref::PageIndex pageIndex) { _pageIndex = pageIndex; }
    CLASS_DELETE_COPY_DEFAULT_MOVE(PageRuntime);

    std::span<uint8_t>       RawData() { return _page->buffer; }
    std::span<uint8_t const> RawData() const { return _page->buffer; }

    template <typename T> std::span<T> Get(size_t offset = 0)
    {
        auto ptr = reinterpret_cast<T*>(_page->buffer + offset);
        return std::span<T>(ptr, std::size(_page->buffer) - offset);
    }

    template <typename T> std::span<T const> Get(size_t offset = 0) const
    {
        auto ptr = reinterpret_cast<T*>(_page->buffer + offset);
        return std::span<T>(ptr, std::size(_page->buffer) - offset);
    }

    template <typename TPage> TPage As() { return TPage(*this); }

    // private:

    enum class Flag : size_t
    {
        Dirty = 0,
        COUNT = 1
    };

    using Flags = std::bitset<1>;

    Ref::SlotIndex _availableSlot   = 0;
    Ref::PageIndex _pageIndex       = 0;
    uint32_t       _typeId          = 0;
    uint32_t       _pageRecDataSize = 0;

    Flags                 _flags{};
    std::unique_ptr<Page> _page{};
};

static constexpr size_t GetSlotUInt32s(size_t count)
{
    return (((count - 1) | (32 - 1)) + 1) / 32;
}
static constexpr size_t GetSlotUInt8s(size_t count)
{
    return (((count - 1) | (8 - 1)) + 1) / 8;
}

static constexpr size_t AlignToWord(size_t s)
{
    return (((s - 1) | (sizeof(void*) - 1)) + 1);
}

// In memory transformation for temporary computation
template <size_t RecordSize> struct PageForRecord
{

    static constexpr size_t GetSlotCapacity(size_t recordSizeInBytes)
    {
        // n slots need n/8 uint8_ts.
        // n * s + n/8 + 4 = 8192
        // n = ((8192 - 4) * 8) / (8s + 1);
        // s = 1    : 8192 = 7278 * 1 + 910 + 4 (0 uint8_ts wasted)
        // s = 4    : 8192 = 1984 * 4 + 248 + 4 (4 uint8_ts wasted)
        // s = 128  :
        // s = 1024 : 8192 = 7 * 1024 + 1 + 4   (1019 uint8_ts wasted)
        size_t AlignedRecordSize        = AlignToWord(recordSizeInBytes);
        size_t SlotsWithoutSlotTracking = Page::PageDataSize / AlignedRecordSize;
        size_t SlotTrackingCost         = AlignToWord(sizeof(uint32_t) * GetSlotUInt32s(SlotsWithoutSlotTracking));
        size_t SlotsWithSlotTracking    = (Page::PageDataSize - SlotTrackingCost) / AlignedRecordSize;
        return SlotsWithSlotTracking;
    }

    static constexpr size_t SlotCount = GetSlotCapacity(RecordSize);

    PageForRecord(PageRuntime& page) : _page(page)
    {

        static_assert((sizeof(*_slots) + sizeof(*_records)) <= Page::PageSizeInBytes);

        _slots   = reinterpret_cast<decltype(_slots)>(page.RawData().data());
        _records = reinterpret_cast<decltype(_records)>(page.RawData().data() + sizeof(*_slots));

        static_assert(sizeof(*_records) == SlotCount * RecordSize);
        static_assert(sizeof(*_records) + sizeof(*_slots) <= Page::PageSizeInBytes);

        while (_page._availableSlot < GetSlotCount() && ValidSlot(_page._availableSlot)) ++_page._availableSlot;
        // TODO unit test
    }

    CLASS_DELETE_COPY_AND_MOVE(PageForRecord);

    Ref::PageIndex PageIndex() const { return _page._pageIndex; }

    size_t GetSlotCount() const { return SlotCount; }
    bool   ValidSlot(size_t index) const { return (_slots->at(index / 32) & (0x1 << (index % 32))) > 0; }
    void   _FillSlot(size_t index)
    {
        auto mask = _slots->at(index / 32);
        mask |= 0x1 << index % 32;
        _slots->at(index / 32) = mask;
    }
    void _ClearSlot(size_t index)
    {
        auto mask = _slots->at(index / 32);
        mask &= ~(0x1 << index % 32);
        _slots->at(index / 32) = mask;
    }
    template <typename TLock> bool Full(TLock const& /*guardscope*/) { return _page._availableSlot >= SlotCount; }

    SlotObj Allocate([[maybe_unused]] RWLock const& guardscope)
    {
        assert(!Full(guardscope));
        assert(!ValidSlot(_page._availableSlot));

        Ref::SlotIndex slot = _page._availableSlot;
        ++_page._availableSlot;
        _FillSlot(slot);
        auto& rec = _records->at(slot);
        std::fill(rec.begin(), rec.end(), uint8_t{0});
        _page.MarkDirty();
        return SlotObj{slot, rec};
    }

    uint8_t Release(RWLock const& /*guardscope*/, Ref::SlotIndex slot)
    {
        auto& rec = _records->at(slot);
        std::fill(rec.begin(), rec.end(), uint8_t{0});
        _ClearSlot(slot);
        _page.MarkSlotFree(slot);
        return 0;
    }

    SlotView Get(ROLock const& /*guardscope*/, Ref::SlotIndex slot) const
    {
        assert(ValidSlot(slot));
        auto& rec = _records->at(slot);
        return SlotView{slot, rec};
    }

    PageRuntime&                                     _page;
    std::array<uint32_t, GetSlotUInt32s(SlotCount)>* _slots = nullptr;
    // Warning.. using bitset make this non portable across 32 bit and 64 bit
    std::array<std::array<uint8_t, RecordSize>, SlotCount>* _records = nullptr;
};

static constexpr size_t GetSlotCapacityForSharedRec(size_t recordSize)
{
    // n slots need n uint8_ts.
    // n * s + n + 4 = 8192
    // n = ((8192 - 4) / (s + 1);
    // s = 1    : 8192 = 7278 * 1 + 910 + 4 (0 uint8_ts wasted)
    // s = 4    : 8192 = 1984 * 4 + 248 + 4 (4 uint8_ts wasted)
    // s = 128  :
    // s = 1024 : 8192 = 7 * 1024 + 1 + 4   (1019 uint8_ts wasted)
    return Page::PageDataSize / (recordSize + 1);
    static_assert(sizeof(Page) == Page::PageSizeInBytes);
}

template <> struct PageForRecord<0>
{
    static constexpr size_t MaxRecordSize = Page::PageDataSize - AlignToWord(2 + 1);

    static constexpr size_t GetSlotCapacity(size_t recordSizeInBytes)
    {
        // n slots need n/8 uint8_ts.
        // n * s + n/8 + 4 = 8192
        // n = ((8192 - 4) * 8) / (8s + 1);
        // s = 1    : 8192 = 7278 * 1 + 910 + 4 (0 uint8_ts wasted)
        // s = 4    : 8192 = 1984 * 4 + 248 + 4 (4 uint8_ts wasted)
        // s = 128  :
        // s = 1024 : 8192 = 7 * 1024 + 1 + 4   (1019 uint8_ts wasted)
        size_t AlignedRecordSize        = recordSizeInBytes > sizeof(size_t) ? AlignToWord(recordSizeInBytes) : recordSizeInBytes;
        size_t SlotsWithoutSlotTracking = (Page::PageDataSize - 2) / AlignedRecordSize;
        size_t SlotTrackingCost         = AlignToWord(2 + sizeof(uint8_t) * GetSlotUInt8s(SlotsWithoutSlotTracking));
        size_t SlotsWithSlotTracking    = (Page::PageDataSize - SlotTrackingCost) / AlignedRecordSize;
        return SlotsWithSlotTracking;
    }

    void _SetRecordSize(uint16_t recordSize)
    {
        static_assert(GetSlotCapacity(1) == 7160);
        static_assert(GetSlotCapacity(8184) == 1);
        _recordSize = recordSize;
        _slots      = reinterpret_cast<decltype(_slots)>(_page.RawData().data() + sizeof(uint16_t));
        _records    = reinterpret_cast<decltype(_records)>(_page.RawData().data() + sizeof(uint16_t)
                                                        + (GetSlotUInt8s(GetSlotCapacity(recordSize))));
        while (_page._availableSlot < GetSlotCount() && ValidSlot(_page._availableSlot)) ++_page._availableSlot;
    }

    void SetRecordSize(uint32_t recordSize)
    {
        auto recordSizePtr = reinterpret_cast<uint16_t*>(_page.RawData().data());
        assert(*recordSizePtr == 0);
        *recordSizePtr = static_cast<uint16_t>(recordSize);
        _SetRecordSize(static_cast<uint16_t>(recordSize));
        _page.MarkDirty();
    }
    auto   PageIndex() const { return _page._pageIndex; }
    auto   GetPageDataSize() const { return _recordSize; }
    size_t GetSlotCount() const { return GetSlotCapacity(_recordSize); }
    bool   ValidSlot(size_t slot) const { return (*(_slots + (slot / 8)) & (0x1 << (slot % 8))) != 0; }
    void   _FillSlot(size_t slot) { *(_slots + (slot / 8)) |= 0x1 << slot % 8; }

    template <typename TLock> bool Full(TLock const& /*guardscope*/) { return _page._availableSlot >= GetSlotCount(); }

    SlotObj Get(ROLock const& /*guardscope*/, Ref::SlotIndex slot) const
    {
        assert(ValidSlot(slot));
        auto rec = _records + (slot * _recordSize);
        assert(static_cast<size_t>((rec + _recordSize) - _page.RawData().data()) <= Page::PageDataSize);
        return SlotObj{slot, {rec, _recordSize}};
    }

    SlotObj Allocate([[maybe_unused]] RWLock const& guardscope)
    {
        assert(!Full(guardscope));
        assert(!ValidSlot(_page._availableSlot));

        Ref::SlotIndex slot = _page._availableSlot;
        ++_page._availableSlot;
        _FillSlot(slot);
        auto slotObj = Get(guardscope, slot);
        std::fill(slotObj.data.begin(), slotObj.data.end(), uint8_t{0});
        _page.MarkDirty();
        return slotObj;
    }

    PageForRecord(PageRuntime& page) : _page(page)
    {
        _recordSize = *reinterpret_cast<uint16_t*>(page.RawData().data());
        if (_recordSize != 0) { _SetRecordSize(_recordSize); }
    }

    CLASS_DELETE_COPY_AND_MOVE(PageForRecord);

    uint8_t Release(RWLock const& /*guardscope*/, Ref::SlotIndex slot)
    {
        auto ptr = _records + (slot * _recordSize);
        std::fill(ptr, ptr + _recordSize, uint8_t{0});
        _page.MarkSlotFree(slot);
        return 0;
    }

    PageRuntime& _page;
    uint16_t     _recordSize{0};
    uint8_t*     _slots   = nullptr;
    uint8_t*     _records = nullptr;
};

struct JournalPage
{
    struct Header
    {
        Ref::PageIndex startPageIndex{0};
        Ref::PageIndex nextJournalPage{0};
    };

    struct Entry
    {
        uint32_t typeId{0};
        uint32_t pageRecDataSize{0};
    };

    static constexpr size_t EntryCount = (Page::PageDataSize - sizeof(Header)) / sizeof(Entry);

    CLASS_DELETE_COPY_AND_MOVE(JournalPage);

    Ref::PageIndex GetEntryCount() const { return EntryCount; }

    Entry& GetJournalEntry(Ref::PageIndex entryIndex) { return _page.Get<Entry>(sizeof(Header))[entryIndex]; }

    bool Full(Ref::PageIndex pageIndex) const { return pageIndex >= EntryCount + _StartPageIndex(); }

    Ref::PageIndex GetNextJornalPage() const { return _page.Get<Header>()[0].nextJournalPage; }

    void SetNextJornalPage(Ref::PageIndex pageIndex) const
    {
        _page.Get<Header>()[0].nextJournalPage = pageIndex;
        _page.MarkDirty();
    }

    void RecordJournalEntry(Ref::PageIndex pageIndex, Entry const& entry)
    {
        assert(pageIndex >= _StartPageIndex());
        assert(pageIndex < _StartPageIndex() + EntryCount);
        Ref::PageIndex entryIndex   = static_cast<Ref::PageIndex>(pageIndex - _StartPageIndex());
        GetJournalEntry(entryIndex) = entry;
        _page.MarkDirty();
    }

    void InitializeEmptyJournal(Ref::PageIndex startPageIndex)
    {
        _page.Get<Header>()[0].startPageIndex = startPageIndex;
        _page.MarkDirty();
    }

    Ref::PageIndex _StartPageIndex() const { return _page.Get<Header>()[0].startPageIndex; }

    private:
    JournalPage(PageRuntime& page) : _page(page) {}

    PageRuntime& _page;
    friend struct PageRuntime;
};

struct HeaderPage
{
    CLASS_DELETE_COPY_AND_MOVE(HeaderPage);

    private:
    HeaderPage(PageRuntime&) {}

    friend struct PageRuntime;
};

struct PageManager
{
    public:    // Constructor, destructor
    PageManager() = default;
    CLASS_DELETE_COPY_AND_MOVE(PageManager);

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
        for (auto& pageRT : _pageRuntimeStates)
        {
            if (!pageRT.Loaded()) continue;
            pageRT.Flush(_serdes);
        }
    }

    auto LockForRead() { return ROLock(_mutex); }
    auto LockForEdit() { return RWLock(_mutex); }

    public:    // Methods
    Ref::PageIndex GetPageCount() const { return static_cast<Ref::PageIndex>(_pageRuntimeStates.size()); }
    uint32_t       GetPageObjTypeId(Ref::PageIndex pageIndex) const { return _pageRuntimeStates[pageIndex]._typeId; }
    uint32_t       GetPageDataSize(Ref::PageIndex pageIndex) const { return _pageRuntimeStates[pageIndex]._pageRecDataSize; }

    PageRuntime& LoadPage(Ref::PageIndex pageIndex)
    {
        auto& pageRT = _pageRuntimeStates[pageIndex];
        if (pageRT.Loaded()) { return pageRT; }
        pageRT.Load(_serdes);
        return pageRT;
    }

    PageRuntime& CreateNewPage(uint32_t objTypeId, uint32_t pageRecDataSize)
    {
        auto pageIndex = static_cast<impl::Ref::PageIndex>(_pageRuntimeStates.size());
        _pageRuntimeStates.push_back(PageRuntime{pageIndex});
        auto& pageRT = _pageRuntimeStates.back();
        pageRT.InitPage();
        pageRT.WriteTo(_serdes);
        pageRT.SetTypeId(objTypeId, pageRecDataSize);
        if (objTypeId != 0) { _RecordJournalEntry(pageIndex, objTypeId, pageRecDataSize); }
        return pageRT;
    }

    private:    // Methods
    void _Initialize()
    {
        auto pageCount = _serdes.GetInputPageCount();
        assert(pageCount >= 2);
        _pageRuntimeStates.resize(pageCount);    // atleast one header page and one journal page

        Ref::PageIndex curJournalPage = 1;
        _pageRuntimeStates[1].SetTypeId(0, 0);
        _pageRuntimeStates[1]._pageIndex = 1;

        while (curJournalPage != 0)
        {
            auto journal = LoadPage(curJournalPage).As<JournalPage>();
            for (Ref::PageIndex j = 0; j < journal.GetEntryCount(); j++)
            {
                auto entry = journal.GetJournalEntry(j);
                if (entry.typeId == 0) { continue; }
                _pageRuntimeStates[j]._pageIndex = j;
                _pageRuntimeStates[j].SetTypeId(entry.typeId, entry.pageRecDataSize);
            }
            curJournalPage = journal.GetNextJornalPage();
        }
    }

    /// <summary>
    /// Record a journal entry
    /// </summary>
    /// <param name="pageIndex"></param>
    /// <param name="objTypeId"></param>
    void _RecordJournalEntry(Ref::PageIndex pageIndex, uint32_t objTypeId, uint32_t pageRecDataSize)
    {
        auto journal = LoadPage(_journalPageIndex).As<JournalPage>();
        if (journal.Full(pageIndex))
        {
            _journalPageIndex = journal.GetNextJornalPage();
            if (_journalPageIndex == 0)
            {
                auto& page       = CreateNewPage(0, pageRecDataSize);
                auto  newJournal = page.As<JournalPage>();
                newJournal.InitializeEmptyJournal(pageIndex);
                journal.SetNextJornalPage(page._pageIndex);
                _journalPageIndex = page._pageIndex;
            }
            else { assert(_pageRuntimeStates[_journalPageIndex]._typeId == 0); }
            _RecordJournalEntry(pageIndex, objTypeId, pageRecDataSize);
        }
        else { journal.RecordJournalEntry(pageIndex, {objTypeId, pageRecDataSize}); }
    }

    private:    // Members
    std::shared_mutex        _mutex;
    impl::SerDes             _serdes;
    Ref::PageIndex           _journalPageIndex = 1;
    std::vector<PageRuntime> _pageRuntimeStates;
};

template <ConceptRecord T, typename TDb, typename TLock> struct Iterator
{
    static constexpr size_t RecordSize = Stencil::Database::RecordTraits<T>::Size();

    public:
    struct Range
    {
        Iterator begin;
        Iterator end;
    };

    static auto Begin(TLock* lock, TDb* db)
    {
        Iterator it;
        it._lock    = lock;
        it._db      = db;
        it._current = impl::Ref(0, 0);
        it._MoveToValidSlot();
        return it;
    }

    static auto End() { return Iterator(); }
    static auto Range(TDb* db) { return Range(Begin(db), End()); }

    bool      operator==(Iterator const& rhs) const { return _lock == rhs._lock && _db == rhs._db && _current == rhs._current; }
    bool      operator!=(Iterator const& rhs) const { return !(*this == rhs); }
    Iterator& operator++()
    {
        _current = impl::Ref(_current).IncrementSlot();
        _MoveToValidSlot();
        return *this;
    }

    // auto operator*() { return this->Get(); }

    auto& Get()
    {
        if (_db == nullptr) { throw std::runtime_error("Reached the end of iteration"); }
        return this->_db->Get(*this->_lock, Stencil::Database::Ref<T>(this->_current));
    }

    void _MoveToValidSlot()
    {
        if (_db == nullptr) { return; }

        auto dbId = impl::Ref(_current);

        auto& pi      = dbId.page;
        auto& si      = dbId.slot;
        auto& pagemgr = _db->_pagemgr;

        for (; pi < pagemgr->GetPageCount(); pi++, si = 0)
        {
            if (pagemgr->GetPageObjTypeId(pi) == 0) continue;
            if (pagemgr->GetPageObjTypeId(pi) != Stencil::Database::TypeId<T, TDb>) { continue; }

            PageForRecord<RecordSize> page = pagemgr->LoadPage(pi);
            for (; si != page.GetSlotCount(); ++si)
            {
                if (page.ValidSlot(si))
                {
                    _current = dbId;
                    return;
                }
            }
        }

        *this = End();
    }

    std::tuple<Stencil::Database::Ref<T>, Stencil::Database::Record<T> const&> operator*()
    {
        // TODO("TODO2");
        return {static_cast<Stencil::Database::Ref<T>>(this->_current), this->Get()};
    }

    Iterator(TLock& lock, TDb& db) : _db(db), _lock(&lock) {}
    Iterator() = default;

    TLock* _lock = nullptr;
    TDb*   _db   = nullptr;
    Ref    _current{Ref::Invalid()};
};

template <ConceptRecord T, typename TDb, typename TLock> struct RangeForView
{
    using IteratorType = Iterator<T, TDb, TLock>;

    RangeForView(TLock& lock, TDb& db) : _begin{IteratorType::Begin(&lock, &db)} {}

    CLASS_DELETE_COPY_DEFAULT_MOVE(RangeForView);

    IteratorType _begin;
    IteratorType _end = IteratorType::End();

    auto begin() { return _begin; }
    auto end() { return _end; }
};

template <typename T, typename TDb> void WriteToBuffer(TDb& db, RWLock const& lock, T const& obj, Stencil::Database::Record<T>& rec)
{
    RecordTraits<T>::WriteToBuffer(db, lock, obj, rec);
}

template <typename T, typename TDb> void WriteToBuffer(TDb& db, RWLock const& lock, T const& obj, Stencil::Database::Ref<T>& rec)
{
    auto [ref, nrec] = db.Create(lock, obj);
    rec              = ref;
}

struct Blob
{
    Blob()  = default;
    ~Blob() = default;
    CLASS_DEFAULT_COPY_AND_MOVE(Blob);

    uint32_t blobSize{};

    private:
    uint8_t*       _GetDataPtr() { return reinterpret_cast<uint8_t*>(this) + sizeof(Blob); }
    uint8_t const* _GetDataPtr() const { return reinterpret_cast<uint8_t const*>(this) + sizeof(Blob); }

    public:
    template <typename T> size_t   Count() const { return static_cast<size_t>(blobSize) / sizeof(T); }
    template <typename T> T const* Data() const { return reinterpret_cast<T const*>(_GetDataPtr()); }
    template <typename T> T*       Data() { return reinterpret_cast<T*>(_GetDataPtr()); }

    template <typename T> std::span<T>       AsSpan() { return std::span<T>(Data<T>(), Count<T>()); }
    template <typename T> std::span<T const> AsSpan() const { return std::span<T const>(Data<T>(), Count<T>()); }
};

template <typename T, typename TRec> auto AsBlob(Record<TRec>)
{}

}    // namespace Stencil::Database::impl
namespace Stencil::Database    // Class/Inferface
{

constexpr size_t _bit_ceil(size_t v) noexcept
{
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}

constexpr uint32_t _bit_ceil(uint32_t v) noexcept
{
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}

template <ConceptRecord... Ts> struct Database
{
    using ThisT       = Database<Ts...>;
    using RecordTypes = tuple_cat_t<typename RecordTraits<Ts>::RecordTypes...>;
    Database()        = default;
    ~Database()       = default;

    CLASS_DELETE_COPY_AND_MOVE(Database);

    template <ConceptRecord T> Record<T> const& Get(ROLock const& lock, Ref<T> const& ref)
    {
        impl::Ref dbId{ref};
        // assert(ref.id.Valid());
        assert(dbId.page != 0);
        assert(dbId.page < _pagemgr->GetPageCount());
        static constexpr uint32_t RecordSize = static_cast<uint32_t>(RecordTraits<T>::Size());

        impl::PageForRecord<RecordSize> page(_pagemgr->LoadPage(dbId.page));

        auto slot = page.Get(lock, dbId.slot);
        page._page.MarkDirty();
        auto rec = reinterpret_cast<Record<T> const*>(slot.data.data());
        return *rec;
    }
    template <ConceptRecord T> auto Items(ROLock& lock) { return impl::RangeForView<T, ThisT, ROLock>(lock, *this); }
    template <ConceptRecord T> void Delete(RWLock const& lock, Ref<T> const& ref)
    {
        auto& rec = Get(lock, ref);

        static constexpr uint32_t RecordSize = static_cast<uint32_t>(RecordTraits<T>::Size());
        if constexpr (ConceptComplex<T>)
        {
            Stencil::Visitor<Record<T>>::VisitAll(rec, [&](auto k, auto& v) {
                if constexpr (IsRef<std::remove_cvref_t<decltype(k)>>) { Delete(lock, k); }
                if constexpr (IsRef<std::remove_cvref_t<decltype(v)>>) { Delete(lock, v); }
            });
        }

        impl::Ref dbId{ref};
        {
            impl::PageForRecord<RecordSize> page(_pagemgr->LoadPage(dbId.page));
            page.Release(lock, dbId.slot);
        }
    }

    template <ConceptRecord T> RefAndRecord<T> Create([[maybe_unused]] RWLock const& lock, [[maybe_unused]] T const& obj)
    {
        if constexpr (ConceptBlob<T>)
        {
            size_t datasize = RecordTraits<T>::GetDataSize(obj);
            if (datasize == 0)
            {
                throw std::logic_error("Empty Blobs not allowed");
                // Record<T> obj;
                // return RefAndEditT<TObj>(RefT<TObj>{}, obj);
            }
            auto recsize = datasize + sizeof(impl::Blob);
            if (recsize > impl::PageForRecord<0>::MaxRecordSize) { throw std::logic_error("Large Blobs not yet implemented"); }
            recsize = _bit_ceil(recsize);
            recsize = std::min(impl::PageForRecord<0>::MaxRecordSize, recsize);

            auto [ref, slotobj] = _Allocate<0>(lock, TypeId<T, ThisT>, static_cast<uint32_t>(recsize));
            auto rec            = reinterpret_cast<Record<T>*>(slotobj.data.data());
            rec->blobSize       = static_cast<uint32_t>(datasize);
            RecordTraits<T>::WriteToBuffer(*this, lock, obj, *rec);
            // assert(ref.id.Valid());
            assert(impl::Ref{ref}.page < _pagemgr->GetPageCount());
            return RefAndRecord<T>(ref, *rec);
        }
        else if constexpr (ConceptFixedSize<T>)
        {
            static constexpr uint32_t RecordSize = static_cast<uint32_t>(FixedSizeRecordTraits<T>::GetDataSize());
            auto [ref, slotobj]                  = _Allocate<RecordSize>(lock, TypeId<T, ThisT>, RecordSize);
            assert(impl::Ref{ref}.page < _pagemgr->GetPageCount());
            auto rec = reinterpret_cast<Record<T>*>(slotobj.data.data());
            FixedSizeRecordTraits<T>::WriteToBuffer(obj, *rec);
            return RefAndRecord<T>(ref, *rec);
        }
        else if constexpr (ConceptComplex<T>)
        {
            static constexpr uint32_t RecordSize = static_cast<uint32_t>(RecordTraits<T>::Size());
            if constexpr (RecordSize == 0)
            {
                size_t datasize = RecordTraits<T>::GetDataSize(obj);
                if (datasize == 0)
                {
                    throw std::logic_error("Empty Blobs not allowed");
                    // Record<T> obj;
                    // return RefAndEditT<TObj>(RefT<TObj>{}, obj);
                }
                auto recsize = datasize + sizeof(impl::Blob);
                if (recsize > impl::PageForRecord<0>::MaxRecordSize) { throw std::logic_error("Large Blobs not yet implemented"); }

                recsize = _bit_ceil(recsize);
                recsize = std::min(impl::PageForRecord<0>::MaxRecordSize, recsize);

                auto [ref, slotobj] = _Allocate<0>(lock, TypeId<T, ThisT>, static_cast<uint32_t>(recsize));
                assert(impl::Ref{ref}.page < _pagemgr->GetPageCount());

                auto rec      = reinterpret_cast<Record<T>*>(slotobj.data.data());
                rec->blobSize = static_cast<uint32_t>(datasize);
                RecordTraits<T>::WriteToBuffer(*this, lock, obj, *rec);
                return RefAndRecord<T>(ref, *rec);
            }
            else
            {
                auto [ref, slotobj] = _Allocate<RecordSize>(lock, TypeId<T, ThisT>, RecordSize);
                assert(impl::Ref{ref}.page < _pagemgr->GetPageCount());
                auto rec = reinterpret_cast<Record<T>*>(slotobj.data.data());
                RecordTraits<T>::WriteToBuffer(*this, lock, obj, *rec);
                return RefAndRecord<T>(ref, *rec);
            }
        }
        else { throw std::logic_error("Unknown Type"); }
    }

    public:    // Methods
    auto LockForRead() { return _pagemgr->LockForRead(); }
    auto LockForEdit() { return _pagemgr->LockForEdit(); }

    template <size_t TRecordSize> impl::PageRuntime& _FindOrCreatePage(RWLock const& lock, uint16_t typeId, uint32_t recDataSize)
    {
        assert(_pagemgr->GetPageCount() > 1);
        for (impl::Ref::PageIndex i = _pagemgr->GetPageCount() - 1u; i > 0; i--)
        {
            if (_pagemgr->GetPageObjTypeId(i) != typeId) { continue; }
            if (_pagemgr->GetPageDataSize(i) != recDataSize) { continue; }
            auto& page    = _pagemgr->LoadPage(i);
            auto  pageRec = page.As<impl::PageForRecord<TRecordSize>>();
            if (!pageRec.Full(lock)) { return page; }
        }

        auto& pageRT = _pagemgr->CreateNewPage(typeId, recDataSize);
        if constexpr (TRecordSize == 0) { pageRT.As<impl::PageForRecord<0>>().SetRecordSize(recDataSize); }
        return pageRT;
    }

    template <size_t TRecordSize> std::tuple<impl::Ref, impl::SlotObj> _Allocate(RWLock const& lock, uint16_t typeId, uint32_t recDataSize)
    {
        auto page = _FindOrCreatePage<TRecordSize>(lock, typeId, recDataSize).template As<impl::PageForRecord<TRecordSize>>();
        auto slot = page.Allocate(lock);
        return std::make_tuple(impl::Ref(page.PageIndex(), slot.index), slot);
    }

    void Init(std::filesystem::path const& path) { _pagemgr->Init(path); }
    void Init() { _pagemgr->Init(); }

    void Flush(RWLock const& /*guardscope*/) { _pagemgr->Flush(); }

    private:
    // std::shared_mutex _mutex;
    std::shared_ptr<impl::PageManager> _pagemgr = std::make_shared<impl::PageManager>();

    // friends
    template <ConceptRecord T, typename _TDb, typename TLock> friend struct impl::Iterator;

    void Init(std::string_view const& fname);
};
}    // namespace Stencil::Database

// Specializations
namespace Stencil::Database
{

template <> struct Record<shared_string> : impl::Blob
{
    Record()  = default;
    ~Record() = default;
    CLASS_DEFAULT_COPY_AND_MOVE(Record);

    std::string_view get() const { return std::string_view(Data<char const>(), blobSize); }
};

template <ConceptRecord K, ConceptRecord V> struct MapItem
{
    Ref<K> k;
    Ref<V> v;
};

template <ConceptRecord K, ConceptRecord V> struct Record<std::unordered_map<K, V>> : impl::Blob
{
    Record()  = default;
    ~Record() = default;
    CLASS_DEFAULT_COPY_AND_MOVE(Record);
    auto Items() { return AsSpan<MapItem<K, V>>(); }
    auto Items() const { return AsSpan<MapItem<K, V>>(); }
};

template <ConceptRecord K, ConceptRecord V> struct RecordTraits<std::unordered_map<K, V>>
{

    using RecordTypes = tuple_cat_t<typename RecordTraits<K>::RecordTypes,
                                    typename RecordTraits<V>::RecordTypes,
                                    std::tuple<List<MapItem<K, V>>, std::unordered_map<K, V>>>;

    using ObjectType = std::unordered_map<K, V>;
    template <typename TDb> static void WriteToBuffer(TDb& db, RWLock const& lock, ObjectType const& obj, Record<ObjectType>& rec)
    {
        static_assert(sizeof(Record<ObjectType>) == sizeof(impl::Blob));
        auto it = rec.Items().begin();
        for (auto const& [k, v] : obj)
        {
            auto kref = db.Create(lock, k);
            auto vref = db.Create(lock, v);
            *it       = MapItem<K, V>{std::get<0>(kref), std::get<0>(vref)};
            ++it;
        }
    }

    static constexpr size_t Size() { return 0; }
    static size_t           GetDataSize(ObjectType const& obj) { return sizeof(MapItem<K, V>) * obj.size(); }
};

template <ConceptRecord T> struct Record<std::vector<T>> : impl::Blob
{
    Record()  = default;
    ~Record() = default;
    CLASS_DEFAULT_COPY_AND_MOVE(Record);
    auto Items() { return AsSpan<Ref<T>>(); }
    auto Items() const { return AsSpan<Ref<T>>(); }
};

template <ConceptRecord T> struct RecordTraits<std::vector<T>>
{
    using RecordTypes = tuple_cat_t<typename RecordTraits<T>::RecordTypes, std::tuple<List<Ref<T>>, std::vector<T>>>;
    using ObjectType  = std::vector<T>;
    template <typename TDb> static void WriteToBuffer(TDb& db, RWLock const& lock, ObjectType const& obj, Record<ObjectType>& rec)
    {
        static_assert(sizeof(Record<ObjectType>) == sizeof(impl::Blob));

        auto it = rec.template AsSpan<Ref<T>>().begin();
        for (auto const& v : obj)
        {
            auto vref = db.Create(lock, v);
            *it       = std::get<0>(vref);
            ++it;
        }
    }

    static constexpr size_t Size() { return 0; }
    static size_t           GetDataSize(ObjectType const& obj) { return sizeof(Ref<T>) * obj.size(); }
};

template <ConceptRecord T> struct RecordTraits<std::unique_ptr<T>>
{
    using RecordTypes = typename RecordTraits<T>::RecordTypes;
    using ObjectType  = std::unique_ptr<T>;
    template <typename TDb>
    static void WriteToBuffer(TDb& /*db*/, RWLock const& /*lock*/, ObjectType const& /*obj*/, Record<ObjectType>& /*rec*/)
    {
        TODO("TODO2");
    }
};

template <> struct RecordTraits<shared_string>
{
    using RecordTypes = std::tuple<shared_string>;
    static constexpr size_t Size() { return 0; }
    static uint32_t         GetDataSize(shared_string const& obj) { return static_cast<uint32_t>(obj.size()); }

    template <typename TDb>
    static void WriteToBuffer(TDb& /*db*/, RWLock const& /*lock*/, shared_string const& obj, Record<shared_string>& rec)
    {
        static_assert(sizeof(Record<shared_string>) == sizeof(impl::Blob));
        auto spn = rec.template AsSpan<char>();
        std::copy(obj.begin(), obj.end(), spn.begin());
    }
};

template <> struct RecordTraits<shared_wstring>
{
    using RecordTypes = std::tuple<shared_wstring>;
    static constexpr size_t Size() { return 0; }

    using ObjectType = shared_wstring;
    template <typename TDb>
    static void WriteToBuffer(TDb& /*db*/, RWLock const& /*lock*/, ObjectType const& /*obj*/, Record<ObjectType>& /*rec*/)
    {
        TODO("TODO2");
    }
};

template <Stencil::ConceptPrimitive T> struct RecordTraits<T>
{
    using RecordTypes = std::tuple<T>;
    static constexpr size_t Size() { return sizeof(T); }

    template <typename TDb> static void WriteToBuffer(TDb& /*db*/, RWLock const& /*lock*/, T const& obj, Record<T>& rec) { rec.data = obj; }
};

template <ConceptBlob T> struct RecordNest<T>
{
    using Type = Ref<T>;
};

template <ConceptComplex T> struct RecordNest<T>
{
    using Type = Ref<T>;
};

template <ConceptRecord T> struct RecordNest<std::unique_ptr<T>>
{
    using Type = Ref<T>;
};

template <ConceptRecord T> struct RecordNest<std::shared_ptr<T>>
{
    using Type = Ref<T>;
};

template <ConceptFixedSize T> struct Record<T>
{
    T const& get() const { return data; }

    T data;
};

// template <ConceptRecord T> struct Record<std::unique_ptr<T>> = delete;    // No need to store unique_ptr in the database
// template <ConceptRecord T> struct Record<std::shared_ptr<T>> = delete;    // No need to store shared_ptr in the database

}    // namespace Stencil::Database
namespace Stencil::Database
{

template <typename T, typename TDb> struct RecordView;
template <typename T, typename TDb> struct RecordEdit;

template <typename T, typename TDb> struct RecordView
{
    RecordView(TDb& db, ROLock& lock, Record<T> const& rec) : _db(db), _lock(lock), _rec(rec) {}
    ~RecordView() = default;
    CLASS_DELETE_COPY_AND_MOVE(RecordView);

    TDb&             _db;
    ROLock&          _lock;
    Record<T> const& _rec;
};

#if 0
template <typename T, typename TDb> struct RecordView<T, TDb, RWLock>
{
    RecordView(TDb& db, RWLock& lock, Record<T>& rec) : _db(db), _lock(lock), _rec(rec) {}
    ~RecordView() = default;
    CLASS_DELETE_COPY_AND_MOVE(RecordView);

    TDb&       _db;
    RWLock&    _lock;
    Record<T>& _rec;
};
#endif

template <typename T> struct RecordViewTraits;
template <typename T, typename TDb> struct RecordViewTraits<RecordView<T, TDb>>
{
    using Db         = TDb;
    using Type       = T;
    using RecordType = Record<T>;
    using NestType   = RecordNest<T>;
};

template <typename T, typename TDb> RecordView<T, TDb> CreateRecordView(TDb& db, ROLock& lock, Record<T> const& rec)
{
    return RecordView<T, TDb>(db, lock, rec);
}

// template <typename T, typename TDb> RecordView<T, TDb> CreateRecordView(TDb& db, RWLock& lock, Record<T>& rec)
//{
//     return RecordView<T, TDb>(db, lock, rec);
// }

template <typename T> static constexpr bool               IsRecordView                     = false;
template <typename T, typename TDb> static constexpr bool IsRecordView<RecordView<T, TDb>> = true;
template <typename T>
concept ConceptRecordView = IsRecordView<T>;

template <typename T>
concept ConceptTrivialRecordView = ConceptRecordView<T> && ConceptTrivial<typename RecordViewTraits<T>::Type>;

}    // namespace Stencil::Database

template <Stencil::Database::ConceptRecordView T>
struct Stencil::TypeTraits<T> : Stencil::TypeTraits<typename Stencil::Database::RecordViewTraits<T>::Type>
{};

template <Stencil::Database::ConceptRecordView T> struct Stencil::Visitor<T>
{
    template <typename T1, typename TLambda> static void VisitAll([[maybe_unused]] T1& obj, [[maybe_unused]] TLambda&& lambda)
    {
        using RecType = typename Stencil::Database::RecordViewTraits<T>::RecordType;
        using Type    = typename Stencil::Database::RecordViewTraits<T>::Type;
        if constexpr (Stencil::ConceptPreferPrimitive<Type>) {}
        else if constexpr (Stencil::ConceptPreferIndexable<Type>)
        {
            Stencil::Visitor<RecType>::VisitAll(obj._rec, [&](auto key, auto& subobj) {
                if constexpr (Stencil::Database::IsRef<std::remove_cvref_t<decltype(subobj)>>)
                {
                    auto& vrec  = obj._db.Get(obj._lock, subobj);
                    auto  vrecv = Stencil::Database::CreateRecordView(obj._db, obj._lock, vrec);
                    if constexpr (Stencil::Database::IsRef<std::remove_cvref_t<decltype(key)>>)
                    {
                        auto& krec  = obj._db.Get(obj._lock, key);
                        auto  krecv = Stencil::Database::CreateRecordView(obj._db, obj._lock, krec);
                        lambda(krecv, vrecv);
                    }
                    else { lambda(key, vrecv); }
                }
                else { lambda(key, subobj.get()); }
            });
        }
        else if constexpr (Stencil::ConceptPreferIterable<Type>)
        {
            Stencil::Visitor<RecType>::VisitAll(obj._rec, [&](auto k, auto& subobj) {
                if constexpr (Stencil::Database::IsRef<std::remove_cvref_t<decltype(subobj)>>)
                {
                    auto& vrec     = obj._db.Get(obj._lock, subobj);
                    auto  itemrecv = Stencil::Database::CreateRecordView(obj._db, obj._lock, vrec);
                    lambda(k, itemrecv);
                }
                else { lambda(k, subobj.get()); }
            });
        }
        else { throw std::logic_error("Unknown Type"); }
    }
};

template <Stencil::Database::ConceptTrivialRecordView T, typename TProt> struct Stencil::SerDes<T, TProt>
{
    using Type       = typename Stencil::Database::RecordViewTraits<T>::Type;
    using RecordType = typename Stencil::Database::RecordViewTraits<T>::RecordType;

    template <typename Context> static auto Write(Context& ctx, T const& obj)
    {
        auto val = obj._rec.get();
        SerDes<decltype(val), TProt>::Write(ctx, val);
    }
    template <typename Context> static auto Read(T& obj, Context& ctx) { SerDes<Type, TProt>::Read(obj._rec.get(), ctx); }
};

template <typename K, typename V> struct Stencil::Visitor<Stencil::Database::Record<std::unordered_map<K, V>>>
{
    using TObj = Stencil::Database::Record<std::unordered_map<K, V>>;
    template <typename T, typename TLambda> static void VisitAll([[maybe_unused]] T& obj, [[maybe_unused]] TLambda&& lambda)
    {
        for (auto item : obj.Items()) { lambda(item.k, item.v); }
    }
};

template <typename T> struct Stencil::Visitor<Stencil::Database::Record<std::vector<T>>>
{
    using TObj = Stencil::Database::Record<std::vector<T>>;
    template <typename T1, typename TLambda> static void VisitAll([[maybe_unused]] T1& obj, [[maybe_unused]] TLambda&& lambda)
    {
        size_t index = 0;
        for (auto item : obj.Items()) { lambda(index++, item); }
    }
};

template <typename T> struct Stencil::Database::Record<Stencil::OptionalPropsT<T>> : Stencil::OptionalPropsT<T>
{};

template <typename T> struct Stencil::Database::Record<Stencil::TimestampedT<T>> : Stencil::TimestampedT<T>
{};

template <typename T>
struct Stencil::TypeTraits<Stencil::Database::Record<Stencil::OptionalPropsT<T>>> : Stencil::TypeTraits<Stencil::OptionalPropsT<T>>
{};

template <typename T>
struct Stencil::TypeTraits<Stencil::Database::Record<Stencil::TimestampedT<T>>> : Stencil::TypeTraits<Stencil::TimestampedT<T>>
{};