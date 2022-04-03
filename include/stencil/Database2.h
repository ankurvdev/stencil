#pragma once
#include "CommonMacros.h"

#include <assert.h>
#include <bitset>
#include <filesystem>
#include <fstream>
#include <map>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <span>
#include <string>
#include <unordered_map>
#include <vector>

#define TODO_OBJREF 0

namespace Database2
{

template <typename> struct is_tuple : std::false_type
{};

template <typename... T> struct is_tuple<std::tuple<T...>> : std::true_type
{};

template <typename T, typename TTup, size_t I = 0> constexpr size_t tuple_index_of()
{
    if constexpr (std::tuple_size<TTup>::value == I) { static_assert(std::is_same_v<T, T>, "Tuple Out of range"); }
    else
    {
        if constexpr (std::is_same_v<std::tuple_element_t<I, TTup>, T>) { return I; }
        else
        {
            return tuple_index_of<T, TTup, I + 1>();
        }
    }
}

struct shared_lock : std::unique_lock<std::shared_mutex>
{
    shared_lock(std::shared_mutex& mutex) : std::unique_lock<std::shared_mutex>(mutex) {}
    CLASS_DELETE_COPY_DEFAULT_MOVE(shared_lock);
};

struct exclusive_lock : shared_lock
{
    exclusive_lock(std::shared_mutex& mutex) : shared_lock(mutex) {}
    CLASS_DELETE_COPY_DEFAULT_MOVE(exclusive_lock);
};

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

template <typename TDb> struct DatabaseT;
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

    constexpr static ObjTypeId TypeId() { return TDb::template TypeId<TObj>(); }
    constexpr static size_t    RecordSize() { return sizeof(TObj); }
    constexpr bool             IsEncrypted() { return false; }
    constexpr static size_t    StructMemberCount() { return 0; }    // Unavailable / not-needed
};

template <typename TDb, typename TObj> struct ObjTraits : FixedSizeObjTraits<TDb, TObj>
{};

template <typename T> constexpr void VerifyTypeTraits()
{
    static_assert(T::ObjTypeId() > 0);
    static_assert(T::IsEncrypted() || (!T::IsEncrypted()), "Should define whether the object is encrypted");
    constexpr auto ownership = T::Ownership();

    if constexpr (T::RecordSize() == 0) { static_assert(T::ObjTypeId() > 0xff, "Blob Traits should have typeids > 255"); }
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
    void SetTypeId(ObjTypeId typeId, uint32_t pageRecDataSize)
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
    ObjTypeId      _typeId          = 0;
    uint32_t       _pageRecDataSize = 0;

    Flags                 _flags{};
    std::unique_ptr<Page> _page{};
};

static constexpr size_t GetSlotUInt32s(size_t count)
{
    return (((count - 1) | 0x1f) + 1) / 32;
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
    template <typename TLock> bool Full(TLock const& /*guardscope*/) { return _page._availableSlot >= SlotCount; }

    SlotObj Allocate([[maybe_unused]] exclusive_lock const& guardscope)
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

    uint8_t Release(exclusive_lock const& /*guardscope*/, Ref::SlotIndex slot)
    {
        auto& rec = _records->at(slot);
        std::fill(rec.begin(), rec.end(), uint8_t{0});
        _page.MarkSlotFree(slot);
        return 0;
    }

    SlotView Get(shared_lock const& /*guardscope*/, Ref::SlotIndex slot) const
    {
        assert(ValidSlot(slot));
        auto& rec = _records->at(slot);
        return SlotView{slot, rec};
    }

    SlotObj Get(exclusive_lock const& /*guardscope*/, Ref::SlotIndex slot) const
    {
        assert(ValidSlot(slot));
        auto& rec = _records->at(slot);
        return SlotObj{slot, rec};
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

// In memory transformation for temporary computation
template <size_t RecordSize> struct PageForSharedRecord
{

    static constexpr size_t SlotCount = GetSlotCapacityForSharedRec(RecordSize);

    constexpr PageForSharedRecord(PageRuntime& page) : _page(page)
    {
        static_assert((GetSlotCapacityForSharedRec(RecordSize) * RecordSize) + GetSlotCapacityForSharedRec(RecordSize)
                      <= Page::PageDataSize);

        _pageIndex = page._pageIndex;
        // auto& buffer = page->_page.buffer;
        _refCounts = reinterpret_cast<decltype(_refCounts)>(page.RawData().data());
        _records   = reinterpret_cast<decltype(_records)>(page.RawData().data() + sizeof(*_refCounts));
        static_assert(sizeof(*_records) == SlotCount * RecordSize);
        static_assert(sizeof(*_records) + sizeof(*_refCounts) < Page::PageSizeInBytes);
        while (_page._availableSlot < GetSlotCount() && ValidSlot(_page._availableSlot)) ++_page._availableSlot;
    }

    CLASS_DELETE_COPY_AND_MOVE(PageForSharedRecord);

    size_t  GetSlotCount() const { return SlotCount; }
    bool    ValidSlot(size_t slot) const { return _refCounts->at(slot) > 0; }
    uint8_t GetRefCount(Ref::SlotIndex slot) const { return _refCounts->at(slot); }
    bool    Full(shared_lock const& /* guardscope */) { return _page._availableSlot >= _refCounts->size(); }

    SlotObj Allocate([[maybe_unused]] exclusive_lock const& guardscope)
    {
        assert(!Full(guardscope));
        assert(_refCounts[_page._availableSlot] == 0u);
        Ref::SlotIndex slot = _page._availableSlot;
        ++_page._availableSlot;
        _refCounts[_page._availableSlot]++;
        std::array<uint8_t, RecordSize>& rec = _records->at(slot);
        std::fill(rec.begin(), rec.end(), 0u);
        _page.MarkDirty();
        return SlotObj{slot, {rec.size(), rec.data()}};
    }

    uint8_t Release(exclusive_lock const& /*guardscope*/, Ref::SlotIndex slot)
    {
        assert(_refCounts->at(slot) > 0u);
        _refCounts->at(slot)--;
        if (_refCounts->at(slot) == 0u)
        {
            std::array<uint8_t, RecordSize>& rec = _records->at(slot);
            std::fill(rec.begin(), rec.end(), uint8_t{0u});
            _page._availableSlot = std::min(slot, _page._availableSlot);
        }
        return _refCounts->at(slot);
    }

    SlotView Get(shared_lock const& /*guardscope*/, Ref::SlotIndex slot) const
    {
        assert(_refCounts->at(slot) > 0);
        auto& rec = _records->at(slot);
        return SlotView{slot, rec};
    }

    SlotObj Get(exclusive_lock const& /*guardscope*/, Ref::SlotIndex slot) const
    {
        assert(_refCounts->at(slot) > 0);
        auto& rec = _records->at(slot);
        return SlotObj{slot, rec};
    }

    PageRuntime& _page;
    uint32_t     _pageIndex = 0;

    std::array<uint8_t, SlotCount>* _refCounts = nullptr;

    std::array<std::array<uint8_t, RecordSize>, SlotCount>* _records = nullptr;
};

template <> struct PageForRecord<0>
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
        size_t SlotsWithoutSlotTracking = (Page::PageDataSize - 2) / AlignedRecordSize;
        size_t SlotTrackingCost         = AlignToWord(sizeof(uint32_t) * GetSlotUInt32s(SlotsWithoutSlotTracking));
        size_t SlotsWithSlotTracking    = (Page::PageDataSize - 2 - SlotTrackingCost) / AlignedRecordSize;
        return SlotsWithSlotTracking;
    }

    void _SetRecordSize(uint16_t recordSize)
    {
        _recordSize = recordSize;
        _slots      = reinterpret_cast<decltype(_slots)>(_page.RawData().data() + sizeof(uint16_t));
        _records    = reinterpret_cast<decltype(_records)>(_page.RawData().data() + sizeof(uint16_t)
                                                        + (GetSlotUInt32s(GetSlotCapacity(recordSize)) * 4));
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
    bool   ValidSlot(size_t slot) const { return (*(_slots + (slot / 32)) & (0x1 << (slot % 32))) != 0; }
    void   _FillSlot(size_t slot) { *(_slots + (slot / 32)) |= 0x1 << slot % 32; }

    template <typename TLock> bool Full(TLock const& /*guardscope*/) { return _page._availableSlot >= GetSlotCount(); }

    SlotObj Get(exclusive_lock const& /*guardscope*/, Ref::SlotIndex slot) const
    {
        assert(ValidSlot(slot));
        auto rec = _records + (slot * _recordSize);
        assert(static_cast<size_t>((rec + _recordSize) - _page.RawData().data()) <= Page::PageDataSize);
        return SlotObj{slot, {rec, _recordSize}};
    }

    SlotView Get(shared_lock const& /*guardscope*/, Ref::SlotIndex slot) const
    {
        assert(ValidSlot(slot));
        auto rec = _records + (slot * _recordSize);
        assert(static_cast<size_t>((rec + _recordSize) - _page.RawData().data()) <= Page::PageDataSize);
        return SlotView{slot, {rec, _recordSize}};
    }

    SlotObj Allocate([[maybe_unused]] exclusive_lock const& guardscope)
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

    uint8_t Release(exclusive_lock const& /*guardscope*/, Ref::SlotIndex slot)
    {
        auto ptr = _records + (slot * _recordSize);
        std::fill(ptr, ptr + _recordSize, uint8_t{0});
        _page.MarkSlotFree(slot);
        return 0;
    }

    PageRuntime& _page;
    uint16_t     _recordSize{0};
    uint32_t*    _slots   = nullptr;
    uint8_t*     _records = nullptr;
};

template <> struct PageForSharedRecord<0> : public PageForRecord<0>
{
    PageForSharedRecord(PageRuntime& page) : PageForRecord(page) {}
    CLASS_DELETE_COPY_AND_MOVE(PageForSharedRecord);
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
        ObjTypeId typeId{0};
        uint32_t  pageRecDataSize{0};
    };

    static constexpr size_t EntryCount = (Page::PageDataSize - sizeof(Header)) / sizeof(Entry);

    CLASS_DELETE_COPY_AND_MOVE(JournalPage);

    Ref::PageIndex GetEntryCount() const { return EntryCount; }

    Entry& GetJournalEntry(Ref::PageIndex entryIndex) { return _page.Get<Entry>(sizeof(Header))[entryIndex]; }

    bool Full(Ref::PageIndex pageIndex) const { return pageIndex > EntryCount + _StartPageIndex(); }

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

    auto LockForRead() { return shared_lock(_mutex); }
    auto LockForEdit() { return exclusive_lock(_mutex); }

    public:    // Methods
    Ref::PageIndex GetPageCount() const { return static_cast<Ref::PageIndex>(_pageRuntimeStates.size()); }
    ObjTypeId      GetPageObjTypeId(Ref::PageIndex pageIndex) const { return _pageRuntimeStates[pageIndex]._typeId; }
    uint32_t       GetPageDataSize(Ref::PageIndex pageIndex) const { return _pageRuntimeStates[pageIndex]._pageRecDataSize; }

    PageRuntime& LoadPage(Ref::PageIndex pageIndex)
    {
        auto& pageRT = _pageRuntimeStates[pageIndex];
        if (pageRT.Loaded()) { return pageRT; }
        pageRT.Load(_serdes);
        return pageRT;
    }

    PageRuntime& CreateNewPage(ObjTypeId objTypeId, uint32_t pageRecDataSize)
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
    void _RecordJournalEntry(Ref::PageIndex pageIndex, ObjTypeId objTypeId, uint32_t pageRecDataSize)
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
            else
            {
                assert(_pageRuntimeStates[_journalPageIndex]._typeId == 0);
            }
            _RecordJournalEntry(pageIndex, objTypeId, pageRecDataSize);
        }
        else
        {
            journal.RecordJournalEntry(pageIndex, {objTypeId, pageRecDataSize});
        }
    }

    private:    // Members
    std::shared_mutex        _mutex;
    impl::SerDes             _serdes;
    Ref::PageIndex           _journalPageIndex = 1;
    std::vector<PageRuntime> _pageRuntimeStates;
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
        it._lock    = lock;
        it._db      = &db;
        it._current = impl::Ref(0, 0);
        it._MoveToValidSlot();
        return it;
    }

    static auto End() { return Iterator(); }
    static auto Range(Database& db) { return Range(Begin(db), End()); }

    bool      operator==(Iterator const& rhs) const { return _lock == rhs._lock && _db == rhs._db && _current == rhs._current; }
    bool      operator!=(Iterator const& rhs) const { return !(*this == rhs); }
    Iterator& operator++()
    {
        _current.slot++;
        _MoveToValidSlot();
        return *this;
    }

    auto operator*() { return this->Get(); }

    auto Get()
    {
        if (_db == nullptr) { throw std::runtime_error("Reached the end of iteration"); }
        return this->_db->Get(*this->_lock, this->_current);
    }

    void _MoveToValidSlot()
    {
        if (_db == nullptr) { return; }

        auto& pi      = _current.page;
        auto& si      = _current.slot;
        auto& pagemgr = _db->_pagemgr;

        for (; pi < pagemgr->GetPageCount(); pi++, si = 0)
        {
            if (pagemgr->GetPageObjTypeId(pi) == 0) continue;
            if (pagemgr->GetPageObjTypeId(pi) != Traits::TypeId()) { continue; }

            PageForRecord<Traits::RecordSize()> page = pagemgr->LoadPage(pi);
            for (; si != page.GetSlotCount(); ++si)
            {
                if (page.ValidSlot(si)) { return; }
            }
        }
        *this = End();
    }

    Iterator(shared_lock& lock, Database& db) : _db(db), _lock(&lock) {}
    Iterator() = default;

    TLock*    _lock = nullptr;
    Database* _db   = nullptr;
    Ref       _current{impl::Ref::Invalid()};
};

template <typename TDb, typename TObj, typename TLock> struct RefAndObjIterator : public Iterator<TDb, TObj, TLock>
{
    auto operator*() { return std::make_tuple(this->_current, this->Get()); }
};

}    // namespace Database2::impl

namespace Database2
{
template <typename TDb, typename TObj> struct Ref : impl::Ref
{
    Ref() = default;
    Ref(impl::Ref const& val) : impl::Ref(val) {}
};

struct ChildRefMarker
{};

template <typename TObj> struct ChildRef : ChildRefMarker
{
    using Obj = TObj;
    template <typename TLock, typename TDb> auto Get(TLock const& lock, TDb& db) const
    {
        if (ref == impl::Ref::Invalid())
        {
            decltype(db.Get(lock, static_cast<Ref<TDb, TObj>>(ref))) obj;
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
        RangeForViewT(rlock& lock, DatabaseT<TDb>& db) :
            _lock(&lock),
            _begin{impl::RefAndObjIterator<TDb, TObj, rlock>::Begin(&lock, db)},
            _end{impl::RefAndObjIterator<TDb, TObj, rlock>::End()}
        {}

        CLASS_DELETE_COPY_DEFAULT_MOVE(RangeForViewT);

        rlock* _lock{};

        impl::RefAndObjIterator<TDb, TObj, rlock> _begin;
        impl::RefAndObjIterator<TDb, TObj, rlock> _end;

        auto begin() const { return _begin; }
        auto end() const { return _end; }
    };

    template <typename TObj> struct RangeForEditT
    {
        RangeForEditT(wlock& lock, DatabaseT<TDb>& db) :
            _lock(&lock),
            _begin{impl::RefAndObjIterator<TDb, TObj, wlock>::Begin(&lock, db)},
            _end{impl::RefAndObjIterator<TDb, TObj, wlock>::End()}
        {}

        CLASS_DELETE_COPY_DEFAULT_MOVE(RangeForEditT);

        wlock* _lock{};

        impl::RefAndObjIterator<TDb, TObj, wlock> _begin;
        impl::RefAndObjIterator<TDb, TObj, wlock> _end;

        auto begin() const { return _begin; }
        auto end() const { return _end; }
    };

    template <typename TObj> static constexpr ObjTypeId TypeId() { return static_cast<ObjTypeId>(typeid(TObj).hash_code() + 1); }

    public:    // Constructor Destructor
    static constexpr struct InMemoryType
    {
    } InMemory{};

    DatabaseT()  = default;
    ~DatabaseT() = default;
    CLASS_DEFAULT_COPY_AND_MOVE(DatabaseT);

    DatabaseT(InMemoryType) { Init(); }
    DatabaseT(std::filesystem::path const& path) : _pagemgr(std::make_shared<impl::PageManager>(path)) {}
    DatabaseT(std::ofstream&& stream) : _pagemgr(std::make_shared<impl::PageManager>(stream)) {}
    DatabaseT(std::ifstream&& stream) : _pagemgr(std::make_shared<impl::PageManager>(stream)) {}

    public:    // Methods
    auto LockForRead() { return _pagemgr->LockForRead(); }
    auto LockForEdit() { return _pagemgr->LockForEdit(); }

    template <size_t TRecordSize> impl::PageRuntime& _FindOrCreatePage(wlock const& lock, ObjTypeId typeId, uint32_t recDataSize)
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

    template <size_t TRecordSize> std::tuple<impl::Ref, impl::SlotObj> _Allocate(wlock const& lock, ObjTypeId typeId, uint32_t recDataSize)
    {
        auto page = _FindOrCreatePage<TRecordSize>(lock, typeId, recDataSize).template As<impl::PageForRecord<TRecordSize>>();
        auto slot = page.Allocate(lock);
        return std::make_tuple(impl::Ref(page.PageIndex(), slot.index), slot);
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
        if constexpr ((N + 1) < Count) { _FillParent<N + 1, Count, TObj>(lock, obj, std::forward<TArgs>(args)...); }
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

    template <typename TObj, typename... TArgs> RefAndEditT<TObj> Create(wlock const& lock, TArgs&&... args)
    {
        if constexpr (Traits<TObj>::RecordSize() == 0)
        {
            size_t datasize = Traits<TObj>::GetDataSize(std::forward<TArgs>(args)...);
            if (datasize == 0)
            {
                EditT<TObj> obj;
                return RefAndEditT<TObj>(RefT<TObj>{}, obj);
            }
            auto recsize       = _bit_ceil(static_cast<uint32_t>(datasize));
            auto [ref, buffer] = _Allocate<0>(lock, Traits<TObj>::TypeId(), recsize);
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
                throw std::logic_error("TODO_OBJREF");
            }
            else
            {
                auto editptr = new (buffer.data.data()) WireT<TObj>{recsize, std::forward<TArgs>(args)...};
                return RefAndEditT<TObj>(RefT<TObj>{ref}, *editptr);
            }
        }
        else
        {
            auto [ref, slotobj]
                = _Allocate<Traits<TObj>::RecordSize()>(lock, Traits<TObj>::TypeId(), static_cast<uint32_t>(Traits<TObj>::RecordSize()));
            void* buffer = slotobj.data.data();
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

    template <typename TObj> SnapT<TObj> Get(RefT<TObj> const& id) { return Get(LockForRead(), id); }

    template <typename TObj> ViewT<TObj> Get(rlock const& lock, RefT<TObj> const& id)
    {
        assert(id.Valid());
        assert(id.page < _pagemgr->GetPageCount());
        impl::PageForRecord<Traits<TObj>::RecordSize()> page(_pagemgr->LoadPage(id.page));

        auto slot   = page.Get(lock, id.slot);
        auto objptr = reinterpret_cast<WireT<TObj> const*>(slot.data.data());
        return *objptr;
    }

    template <typename TObj> EditT<TObj> Get(wlock const& lock, RefT<TObj> const& id)
    {
        assert(id.Valid());
        assert(id.page < _pagemgr->GetPageCount());
        impl::PageForRecord<Traits<TObj>::RecordSize()> page(_pagemgr->LoadPage(id.page));

        auto slot = page.Get(lock, id.slot);
        page._page.MarkDirty();
        EditT<TObj> editobj(*reinterpret_cast<WireT<TObj>*>(slot.data.data()));
        page._page.MarkDirty();    // TODO : test
        return editobj;
    }

    template <typename TObj> RangeForViewT<TObj> Objects(rlock& lock) { return RangeForViewT<TObj>(lock, *this); }
    template <typename TObj> RangeForEditT<TObj> Objects(wlock& lock) { return RangeForEditT<TObj>(lock, *this); }

    template <typename TObj> void _ReleaseChildRefs(wlock const& /*lock*/, impl::SlotObj /*slotobj*/)
    {
        throw std::logic_error("TODO_OBJREF");
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
            impl::PageForSharedRecord<Traits<TObj>::RecordSize()> page(_pagemgr->LoadPage(id.page));

            auto refcount = page.Release(lock, id.slot);
            if (refcount == 0) { _ReleaseChildRefs<TObj>(lock, page.Get(lock, id.slot)); }
        }
        else
        {
            impl::PageForRecord<Traits<TObj>::RecordSize()> page(_pagemgr->LoadPage(id.page));
            page.Release(lock, id.slot);
        }
    }

    void Init(std::filesystem::path const& path) { _pagemgr->Init(path); }
    void Init() { _pagemgr->Init(); }

    void Flush(exclusive_lock const& /*guardscope*/) { _pagemgr->Flush(); }

    private:
    // std::shared_mutex _mutex;
    std::shared_ptr<impl::PageManager> _pagemgr = std::make_shared<impl::PageManager>();

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
            std::copy(str.begin(), str.end(), reinterpret_cast<TChar*>(ptr));
        }

        operator EditType() { return std::basic_string_view<TChar>(reinterpret_cast<TChar*>(ptr), size / sizeof(TChar)); }
        operator ViewType() const { return std::basic_string_view<TChar>(reinterpret_cast<TChar const*>(ptr), size / sizeof(TChar)); }
    };

    static ViewType View(WireType const& obj)
    {
        return std::basic_string_view<TChar>(reinterpret_cast<TChar*>(obj.ptr), obj.size / sizeof(TChar));
    }

    static EditType Edit(WireType const& obj)
    {
        return std::basic_string_view<TChar>(reinterpret_cast<TChar*>(obj.ptr), obj.size / sizeof(TChar));
    }

    static size_t           GetDataSize(ViewType const& str) { return str.empty() ? 0 : (str.size() * sizeof(TChar) + sizeof(uint16_t)); }
    constexpr static size_t StructMemberCount() { return 0; }    // Unavailable / not-needed
    static WireType         Create(WireType const& /* str */) { return {}; }
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

#define DEFINESTRING(str)                                                                                                    \
    using str = str##Traits::WireType;                                                                                       \
    template <typename TDb> struct ObjTraits<TDb, str> : str##Traits                                                         \
    {                                                                                                                        \
        static WireType Create(TDb& /* db */, exclusive_lock const& /* lock */, ViewType const& str) { return Create(str); } \
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
    OwnerT()              = default;
    ~OwnerT()             = default;
    OwnerT(const OwnerT&) = default;
    OwnerT& operator=(OwnerT const&) = default;
    OwnerT(OwnerT&&) { throw std::logic_error("Not Impl"); }
    OwnerT& operator=(OwnerT&&) { throw std::logic_error("Not Impl"); }
};

template <typename TDb, typename TObj> struct ObjectT
{};

template <typename TObj> struct EncryptedT
{};
}    // namespace Database2

template <typename T> struct Stencil::TypeTraits<Database2::ChildRef<T>>
{
    using Categories = std::tuple<Stencil::Category::Primitive>;
};
