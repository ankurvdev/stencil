#pragma once
#include "CommonMacros.h"

#include "optionalprops.h"
#include "ref.h"
#include "shared_tree.h"
#include "timestamped.h"
#include "typetraits.h"

#include <cassert>
#include <filesystem>
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

struct RefKeyType
{};    // Special Type: for use when when visiting recordview to visit ref<> of self

template <typename T> using Ref = Stencil::Ref<T>;

template <typename T> constexpr bool IsRef         = false;
template <typename T> constexpr bool IsRef<Ref<T>> = true;

template <typename T>
concept ConceptRef = IsRef<T>;

template <typename T, typename TTup, size_t I = 0> constexpr size_t TupleIndexOf()
{
    if constexpr (std::tuple_size_v<TTup> == I)
    {
        static_assert(!std::is_same_v<T, T>, "Tuple Out of range");
        return I;
    }
    else
    {
        if constexpr (std::is_same_v<std::tuple_element_t<I, TTup>, T>) { return I; }
        else
        {
            return TupleIndexOf<T, TTup, I + 1>();
        }
    }
}

template <typename... Tuples> using tuple_cat_t = decltype(std::tuple_cat(std::declval<Tuples>()...));

template <ConceptRecord T, typename TDb>
static constexpr uint16_t TypeId = static_cast<uint16_t>(TupleIndexOf<T, typename TDb::RecordTypes>());

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
    using PageIndex = uint16_t;
    using SlotIndex = uint16_t;
    PageIndex page{0};
    SlotIndex slot{0};

    bool operator==(Ref const& rhs) const { return page == rhs.page && slot == rhs.slot; }

    constexpr Ref() = default;
    constexpr Ref(PageIndex pageIn, SlotIndex slotIn) : page(pageIn), slot(slotIn) {}

    template <ConceptRecord T>
    constexpr explicit Ref(Stencil::Database::Ref<T> const& ref) :
        page(static_cast<uint16_t>(ref.id >> 16)), slot(static_cast<uint16_t>(ref.id & 0xffff))
    {}

    Ref(Ref const& val)            = default;
    Ref& operator=(Ref const& val) = default;

    template <ConceptRecord T> explicit operator Stencil::Database::Ref<T>() const
    { return Stencil::Database::Ref<T>{(uint32_t{page} << 16) | uint32_t{slot}}; }

    static Ref         Invalid() { return Ref{}; }
    [[nodiscard]] bool Valid() const { return page >= 2 && slot < 1000; }
    Ref&               IncrementSlot() LFTBND
    {
        slot++;
        return *this;
    }

    static auto FromUInt(uint32_t value)
    {
        auto slot = static_cast<uint16_t>(value & 0xff);
        auto page = static_cast<uint16_t>(value >> 16);
        return Ref(page, slot);
    }
};

struct SlotObj
{
    Ref::SlotIndex     index{0};
    std::span<uint8_t> data;
};

struct SlotView
{
    Ref::SlotIndex           index{0};
    std::span<uint8_t const> data;
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
        assert(_loadedPages.empty());
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
        if (_loadedPages.empty())
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
        if (_loadedPages.empty())
        {
            if (_readFrom == nullptr) throw std::runtime_error("No Input file attached");
            _readFrom->seekg(0, std::ios_base::end);
            auto offset = _readFrom->tellg();
            return GetPageIndexFromOffset_(offset);
        }

        if (_readFrom != nullptr) throw std::runtime_error("Invalid State");
        return static_cast<uint32_t>(_loadedPages.size());
    }

    private:
    static std::streamoff PageStreamOffset_(size_t page)
    { return static_cast<std::streamoff>((page * Page::PageSizeInBytes) + sizeof(Header)); }

    static uint32_t GetPageIndexFromOffset_(std::streamoff offset)
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
        stream.write(reinterpret_cast<char const*>(&header), sizeof(header));
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
        std::streamoff offsetreq{PageStreamOffset_(index)};
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
    SUPPRESS_WARNINGS_START
    SUPPRESS_CLANG_WARNING("-Wunsafe-buffer-usage")

    static void _WritePage(Page const& page, uint32_t index, std::ostream& stream)
    {
        assert(!stream.fail());
        std::streamoff offsetreq{PageStreamOffset_(index)};
        stream.seekp(offsetreq, std::ios_base::beg);
        auto offsetcur = stream.tellp();
        assert(offsetcur >= 0);
        assert((offsetcur <= offsetreq) && ((static_cast<size_t>(offsetreq - offsetcur) % Page::PageSizeInBytes) == 0));
        uint8_t zerobuffer[Page::PageSizeInBytes] = {};
        assert(zerobuffer[0] == 0 && zerobuffer[Page::PageSizeInBytes - 1] == 0);
        while (offsetcur < offsetreq)
        {
            stream.write(reinterpret_cast<char const*>(zerobuffer), Page::PageSizeInBytes);
            offsetcur = stream.tellp();
        }
        stream.write(reinterpret_cast<char const*>(&page), sizeof(Page));
        assert(!stream.fail());
    }
    SUPPRESS_WARNINGS_END
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
    void SetTypeId(uint32_t typeIdIn, uint32_t pageRecDataSizeIn)
    {
        typeId          = typeIdIn;
        pageRecDataSize = pageRecDataSizeIn;
    }

    [[nodiscard]] bool Loaded() const { return page != nullptr; }
    void               Load(SerDes& serdes)
    {
        page = std::make_unique<Page>();
        serdes.ReadPage(*page, pageIndex);
    }

    void InitPage() { page = std::make_unique<Page>(); }
    void WriteTo(SerDes& serdes) const { serdes.WritePage(*page, pageIndex); }
    void MarkSlotFree(Ref::SlotIndex slot)
    {
        availableSlot = std::min(slot, availableSlot);
        MarkDirty();
    }

    void MarkDirty() { flags.set(static_cast<size_t>(Flag::Dirty)); }
    void Flush(SerDes& serdes)
    {
        if (!flags.test(static_cast<size_t>(Flag::Dirty))) return;
        serdes.WritePage(*page, pageIndex);
        flags.reset(static_cast<size_t>(Flag::Dirty));
    }
    PageRuntime() = default;
    PageRuntime(Ref::PageIndex pageIndexIn) : pageIndex(pageIndexIn) {}    // NOLINT
    CLASS_DELETE_COPY_DEFAULT_MOVE(PageRuntime);

    std::span<uint8_t>                     RawData() LFTBND { return page->buffer; }
    [[nodiscard]] std::span<uint8_t const> RawData() const LFTBND { return page->buffer; }
    SUPPRESS_WARNINGS_START                SUPPRESS_CLANG_WARNING("-Wunsafe-buffer-usage")

        template <typename T>
        std::span<T> Get(size_t offset = 0)
    {
        auto ptr = reinterpret_cast<T*>(page->buffer + offset);
        return std::span<T>(ptr, std::size(page->buffer) - offset);
    }
    SUPPRESS_WARNINGS_END
    template <typename T> [[nodiscard]] std::span<T const> Get(size_t offset = 0) const
    {
        auto ptr = reinterpret_cast<T*>(page->buffer + offset);
        return std::span<T>(ptr, std::size(page->buffer) - offset);
    }

    template <typename TPage> TPage As() LFTBND { return TPage(*this); }

    // private:

    enum class Flag : size_t
    {
        Dirty = 0,
        COUNT = 1,
    };

    using Flags = std::bitset<1>;

    Ref::SlotIndex availableSlot   = 0;
    Ref::PageIndex pageIndex       = 0;
    uint32_t       typeId          = 0;
    uint32_t       pageRecDataSize = 0;

    Flags                 flags;
    std::unique_ptr<Page> page;
};

static constexpr size_t GetSlotUInt32s(size_t count)
{ return (((count - 1) | (32 - 1)) + 1) / 32; }
static constexpr size_t GetSlotUInt8s(size_t count)
{ return (((count - 1) | (8 - 1)) + 1) / 8; }

static constexpr size_t AlignToWord(size_t s)
{ return (((s - 1) | (sizeof(void*) - 1)) + 1); }

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
        size_t alignedRecordSize        = AlignToWord(recordSizeInBytes);
        size_t slotsWithoutSlotTracking = Page::PageDataSize / alignedRecordSize;
        size_t slotTrackingCost         = AlignToWord(sizeof(uint32_t) * GetSlotUInt32s(slotsWithoutSlotTracking));
        size_t slotsWithSlotTracking    = (Page::PageDataSize - slotTrackingCost) / alignedRecordSize;
        return slotsWithSlotTracking;
    }

    static constexpr size_t SlotCount = GetSlotCapacity(RecordSize);

    PageForRecord(PageRuntime& pageIn LFTBND) : page(pageIn)    // NOLINT
    {
        SUPPRESS_WARNINGS_START
        SUPPRESS_CLANG_WARNING("-Wunsafe-buffer-usage")
        static_assert((sizeof(*slots) + sizeof(*records)) <= Page::PageSizeInBytes);

        slots   = reinterpret_cast<decltype(slots)>(page.RawData().data());
        records = reinterpret_cast<decltype(records)>(page.RawData().data() + sizeof(*slots));

        static_assert(sizeof(*records) == SlotCount * RecordSize);
        static_assert(sizeof(*records) + sizeof(*slots) <= Page::PageSizeInBytes);

        while (page.availableSlot < GetSlotCount() && ValidSlot(page.availableSlot)) ++page.availableSlot;
        // TODO unit test
        SUPPRESS_WARNINGS_END
    }

    CLASS_DELETE_COPY_AND_MOVE(PageForRecord);

    [[nodiscard]] Ref::PageIndex PageIndex() const { return page.pageIndex; }

    [[nodiscard]] size_t GetSlotCount() const { return SlotCount; }
    [[nodiscard]] bool   ValidSlot(size_t index) const { return (slots->at(index / 32) & (0x1 << (index % 32))) > 0; }
    void                 _FillSlot(size_t index)
    {
        auto mask = slots->at(index / 32);
        mask |= 0x1 << index % 32;
        slots->at(index / 32) = mask;
    }
    void _ClearSlot(size_t index)
    {
        auto mask = slots->at(index / 32);
        mask &= ~(uint32_t{0x1} << index % 32);
        slots->at(index / 32) = mask;
    }
    template <typename TLock> bool Full(TLock const& /*guardscope*/) { return page.availableSlot >= SlotCount; }

    SlotObj Allocate([[maybe_unused]] RWLock const& guardscope)
    {
        assert(!Full(guardscope));
        assert(!ValidSlot(page.availableSlot));

        Ref::SlotIndex slot = page.availableSlot;
        ++page.availableSlot;
        _FillSlot(slot);
        auto& rec = records->at(slot);
        std::fill(rec.begin(), rec.end(), uint8_t{0});
        page.MarkDirty();
        return SlotObj{slot, rec};
    }

    uint8_t Release(RWLock const& /*guardscope*/, Ref::SlotIndex slot)
    {
        auto& rec = records->at(slot);
        std::fill(rec.begin(), rec.end(), uint8_t{0});
        _ClearSlot(slot);
        page.MarkSlotFree(slot);
        return 0;
    }

    [[nodiscard]] SlotView Get(ROLock const& /*guardscope*/, Ref::SlotIndex slot) const
    {
        assert(ValidSlot(slot));
        auto& rec = records->at(slot);
        return SlotView{slot, rec};
    }

    SlotObj Edit(ROLock const& /*guardscope*/, Ref::SlotIndex slot)
    {
        assert(ValidSlot(slot));
        auto& rec = records->at(slot);
        return SlotObj{slot, rec};
    }

    PageRuntime&                                     page;
    std::array<uint32_t, GetSlotUInt32s(SlotCount)>* slots = nullptr;
    // Warning.. using bitset make this non portable across 32 bit and 64 bit
    std::array<std::array<uint8_t, RecordSize>, SlotCount>* records = nullptr;
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
        size_t alignedRecordSize        = recordSizeInBytes > sizeof(size_t) ? AlignToWord(recordSizeInBytes) : recordSizeInBytes;
        size_t slotsWithoutSlotTracking = (Page::PageDataSize - 2) / alignedRecordSize;
        size_t slotTrackingCost         = AlignToWord(2 + (sizeof(uint8_t) * GetSlotUInt8s(slotsWithoutSlotTracking)));
        size_t slotsWithSlotTracking    = (Page::PageDataSize - slotTrackingCost) / alignedRecordSize;
        return slotsWithSlotTracking;
    }
    SUPPRESS_WARNINGS_START
    SUPPRESS_CLANG_WARNING("-Wunsafe-buffer-usage")

    void _SetRecordSize(uint16_t recordSizeIn)
    {
        // TODO: Fix for both 32 bit and 64 bit
        // static_assert(GetSlotCapacity(1) == 7160);
        // static_assert(GetSlotCapacity(8184) == 1);
        recordSize = recordSizeIn;
        slots      = reinterpret_cast<decltype(slots)>(page.RawData().data() + sizeof(uint16_t));
        records
            = reinterpret_cast<decltype(records)>(page.RawData().data() + sizeof(uint16_t) + GetSlotUInt8s(GetSlotCapacity(recordSize)));
        while (page.availableSlot < GetSlotCount() && ValidSlot(page.availableSlot)) ++page.availableSlot;
    }

    void SetRecordSize(uint32_t recordSizeIn)
    {
        auto* recordSizePtr = reinterpret_cast<uint16_t*>(page.RawData().data());
        assert(*recordSizePtr == 0);
        *recordSizePtr = static_cast<uint16_t>(recordSizeIn);
        _SetRecordSize(static_cast<uint16_t>(recordSizeIn));
        page.MarkDirty();
    }
    [[nodiscard]] auto   PageIndex() const { return page.pageIndex; }
    [[nodiscard]] auto   GetPageDataSize() const { return recordSize; }
    [[nodiscard]] size_t GetSlotCount() const { return GetSlotCapacity(recordSize); }
    [[nodiscard]] bool   ValidSlot(size_t slot) const { return (*(slots + (slot / 8)) & (0x1 << (slot % 8))) != 0; }
    void                 _FillSlot(size_t slot) const { *(slots + (slot / 8)) |= 0x1 << slot % 8; }

    template <typename TLock> bool Full(TLock const& /*guardscope*/) { return page.availableSlot >= GetSlotCount(); }

    [[nodiscard]] SlotObj Get(ROLock const& /*guardscope*/, Ref::SlotIndex slot) const
    {
        assert(ValidSlot(slot));
        auto* rec = records + (slot * recordSize);
        assert(std::cmp_less_equal((rec + recordSize) - page.RawData().data(), Page::PageDataSize));
        return SlotObj{.index = slot, .data = {rec, recordSize}};
    }

    SlotObj Allocate([[maybe_unused]] RWLock const& guardscope)
    {
        assert(!Full(guardscope));
        assert(!ValidSlot(page.availableSlot));

        Ref::SlotIndex slot = page.availableSlot;
        ++page.availableSlot;
        _FillSlot(slot);
        auto slotObj = Get(guardscope, slot);
        std::fill(slotObj.data.begin(), slotObj.data.end(), uint8_t{0});
        page.MarkDirty();
        return slotObj;
    }

    explicit PageForRecord(PageRuntime& pageIn LFTBND) : page(pageIn)
    {
        recordSize = *reinterpret_cast<uint16_t*>(page.RawData().data());
        if (recordSize != 0) { _SetRecordSize(recordSize); }
    }

    CLASS_DELETE_COPY_AND_MOVE(PageForRecord);

    uint8_t Release(RWLock const& /*guardscope*/, Ref::SlotIndex slot)
    {
        auto* ptr = records + (slot * recordSize);
        std::fill(ptr, ptr + recordSize, uint8_t{0});
        page.MarkSlotFree(slot);
        return 0;
    }
    SUPPRESS_WARNINGS_END
    PageRuntime& page;
    uint16_t     recordSize{0};
    uint8_t*     slots   = nullptr;
    uint8_t*     records = nullptr;
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

    explicit JournalPage(PageRuntime& page LFTBND) : _page(page) {}
    CLASS_DELETE_COPY_AND_MOVE(JournalPage);

    [[nodiscard]] static Ref::PageIndex GetEntryCount() { return EntryCount; }

    Entry& GetJournalEntry(Ref::PageIndex entryIndex) { return _page.Get<Entry>(sizeof(Header))[entryIndex]; }

    [[nodiscard]] bool Full(Ref::PageIndex pageIndex) const { return pageIndex >= EntryCount + _StartPageIndex(); }

    [[nodiscard]] Ref::PageIndex GetNextJornalPage() const { return _page.Get<Header>()[0].nextJournalPage; }

    void SetNextJornalPage(Ref::PageIndex pageIndex) const
    {
        _page.Get<Header>()[0].nextJournalPage = pageIndex;
        _page.MarkDirty();
    }

    void RecordJournalEntry(Ref::PageIndex pageIndex, Entry const& entry)
    {
        assert(pageIndex >= _StartPageIndex());
        assert(pageIndex < _StartPageIndex() + EntryCount);
        auto entryIndex             = static_cast<Ref::PageIndex>(pageIndex - _StartPageIndex());
        GetJournalEntry(entryIndex) = entry;
        _page.MarkDirty();
    }

    void InitializeEmptyJournal(Ref::PageIndex startPageIndex)
    {
        _page.Get<Header>()[0].startPageIndex = startPageIndex;
        _page.MarkDirty();
    }

    [[nodiscard]] Ref::PageIndex _StartPageIndex() const { return _page.Get<Header>()[0].startPageIndex; }

    private:
    PageRuntime& _page;
    friend struct PageRuntime;
};

struct HeaderPage
{
    CLASS_DELETE_COPY_AND_MOVE(HeaderPage);

    private:
    explicit HeaderPage(PageRuntime& /*unused*/) {}

    friend struct PageRuntime;
};

struct PageManager
{
    public:    // Constructor, destructor
    PageManager() = default;
    CLASS_DELETE_COPY_AND_MOVE(PageManager);

    explicit PageManager(std::filesystem::path const& path)
    {
        _serdes.Attach(path);
        _Initialize();
    }

    template <typename TStream> explicit PageManager(TStream&& stream)
    {
        _serdes.AttachStream(std::forward<TStream>(stream));
        _Initialize();
    }
    ~PageManager() { Flush(); }

    void Init(std::filesystem::path const& path)
    {
        _serdes.Attach(path);
        _Initialize();
    }

    void Init(std::ifstream&& istrm)
    {
        _serdes.AttachStream(std::move(istrm));
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
    uint32_t       GetPageObjTypeId(Ref::PageIndex pageIndex) const { return _pageRuntimeStates[pageIndex].typeId; }
    uint32_t       GetPageDataSize(Ref::PageIndex pageIndex) const { return _pageRuntimeStates[pageIndex].pageRecDataSize; }

    PageRuntime& LoadPage(Ref::PageIndex pageIndex) LFTBND
    {
        auto& pageRT = _pageRuntimeStates[pageIndex];
        if (pageRT.Loaded()) { return pageRT; }
        pageRT.Load(_serdes);
        return pageRT;
    }

    PageRuntime& CreateNewPage(uint32_t objTypeId, uint32_t pageRecDataSize) LFTBND
    {
        auto pageIndex = static_cast<impl::Ref::PageIndex>(_pageRuntimeStates.size());
        _pageRuntimeStates.emplace_back(pageIndex);
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
        _pageRuntimeStates[1].pageIndex = 1;

        while (curJournalPage != 0)
        {
            auto journal = LoadPage(curJournalPage).As<JournalPage>();
            for (Ref::PageIndex j = 0; j < Stencil::Database::impl::JournalPage::GetEntryCount(); j++)
            {
                auto entry = journal.GetJournalEntry(j);
                if (entry.typeId == 0) { continue; }
                _pageRuntimeStates[j].pageIndex = j;
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
                journal.SetNextJornalPage(page.pageIndex);
                _journalPageIndex = page.pageIndex;
            }
            else
            {
                assert(_pageRuntimeStates[_journalPageIndex].typeId == 0);
            }
            _RecordJournalEntry(pageIndex, objTypeId, pageRecDataSize);
        }
        else
        {
            journal.RecordJournalEntry(pageIndex, {.typeId = objTypeId, .pageRecDataSize = pageRecDataSize});
        }
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

    static auto begin(TLock* lock, TDb* db)    // NOLINT
    {
        Iterator it;
        it.lock    = lock;
        it.db      = db;
        it.current = impl::Ref(0, 0);
        it._MoveToValidSlot();
        SUPPRESS_WARNINGS_START
        SUPPRESS_CLANG_WARNING("-Wnrvo")
        return it;
        SUPPRESS_WARNINGS_END
    }

    static auto end() { return Iterator(); }    // NOLINT
    static auto Range(TDb* db) { return Range(begin(db), end()); }

    bool      operator==(Iterator const& rhs) const { return lock == rhs.lock && db == rhs.db && current == rhs.current; }
    bool      operator!=(Iterator const& rhs) const { return !(*this == rhs); }
    Iterator& operator++() LFTBND
    {
        current = impl::Ref(current).IncrementSlot();
        _MoveToValidSlot();
        return *this;
    }

    // auto operator*() { return this->Get(); }

    auto& Get()
    {
        if (db == nullptr) { throw std::runtime_error("Reached the end of iteration"); }
        return this->db->Get(*this->lock, Stencil::Database::Ref<T>(this->current));
    }

    void _MoveToValidSlot()
    {
        if (db == nullptr) { return; }

        auto dbId = impl::Ref(current);

        auto& pi      = dbId.page;
        auto& si      = dbId.slot;
        auto& pagemgr = db->_pagemgr;

        for (; pi < pagemgr->GetPageCount(); pi++, si = 0)
        {
            if (pagemgr->GetPageObjTypeId(pi) == 0) continue;
            if (pagemgr->GetPageObjTypeId(pi) != Stencil::Database::TypeId<T, TDb>) { continue; }

            PageForRecord<RecordSize> page = pagemgr->LoadPage(pi);
            for (; si != page.GetSlotCount(); ++si)
            {
                if (page.ValidSlot(si))
                {
                    current = dbId;
                    return;
                }
            }
        }

        *this = end();
    }

    std::tuple<Stencil::Database::Ref<T>, Stencil::Database::Record<T> const&> operator*()
    { return {static_cast<Stencil::Database::Ref<T>>(this->current), this->Get()}; }

    Iterator(TLock& lockIn, TDb& dbIn) : db(&dbIn), lock(&lockIn) {}
    Iterator() = default;

    TLock* lock = nullptr;
    TDb*   db   = nullptr;
    Ref    current{Ref::Invalid()};
};

template <ConceptRecord T, typename TDb, typename TLock> struct RangeForView
{
    using IteratorType = Iterator<T, TDb, TLock>;

    RangeForView(TLock& lock, TDb& db) : beginIt{IteratorType::begin(&lock, &db)} {}

    CLASS_DELETE_COPY_DEFAULT_MOVE(RangeForView);

    IteratorType beginIt;
    IteratorType endIt = IteratorType::end();

    auto begin() { return beginIt; }    // NOLINT
    auto end() { return endIt; }        // NOLINT
};

template <typename T, typename TDb> void WriteToBuffer(TDb& db, RWLock const& lock, T const& obj, Stencil::Database::Record<T>& rec)
{ RecordTraits<T>::WriteToBuffer(db, lock, obj, rec); }

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
    SUPPRESS_WARNINGS_START
    SUPPRESS_CLANG_WARNING("-Wunsafe-buffer-usage")
    uint8_t*                     _GetDataPtr() LFTBND { return reinterpret_cast<uint8_t*>(this) + sizeof(Blob); }
    [[nodiscard]] uint8_t const* _GetDataPtr() const LFTBND { return reinterpret_cast<uint8_t const*>(this) + sizeof(Blob); }
    SUPPRESS_WARNINGS_END
    public:
    template <typename T> [[nodiscard]] size_t   Count() const { return static_cast<size_t>(blobSize) / sizeof(T); }
    template <typename T> [[nodiscard]] T const* Data() const LFTBND { return reinterpret_cast<T const*>(_GetDataPtr()); }
    template <typename T> T*                     Data() LFTBND { return reinterpret_cast<T*>(_GetDataPtr()); }

    template <typename T> std::span<T>                     AsSpan() { return std::span<T>(Data<T>(), Count<T>()); }
    template <typename T> [[nodiscard]] std::span<T const> AsSpan() const { return std::span<T const>(Data<T>(), Count<T>()); }
};

template <typename T, typename TRec> auto AsBlob(Record<TRec> /*unused*/)
{}

}    // namespace Stencil::Database::impl
namespace Stencil::Database    // Class/Inferface
{

template <typename T> static constexpr T BitCeil(T v) noexcept
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
        static constexpr auto RecordSize = static_cast<uint32_t>(RecordTraits<T>::Size());

        impl::PageForRecord<RecordSize> page(_pagemgr->LoadPage(dbId.page));

        auto slot = page.Get(lock, dbId.slot);
        auto rec  = reinterpret_cast<Record<T> const*>(slot.data.data());
        return *rec;
    }

    template <ConceptRecord T> Record<T>& Edit(RWLock const& lock, Ref<T> const& ref)
    {
        impl::Ref dbId{ref};
        // assert(ref.id.Valid());
        assert(dbId.page != 0);
        assert(dbId.page < _pagemgr->GetPageCount());
        static constexpr auto RecordSize = static_cast<uint32_t>(RecordTraits<T>::Size());

        impl::PageForRecord<RecordSize> page(_pagemgr->LoadPage(dbId.page));

        auto slot = page.Edit(lock, dbId.slot);
        page._page.MarkDirty();
        auto rec = reinterpret_cast<Record<T>*>(slot.data.data());
        return *rec;
    }
    template <ConceptRecord T> auto Items(ROLock& lock) { return impl::RangeForView<T, ThisT, ROLock>(lock, *this); }
    template <ConceptRecord T> void Delete(RWLock const& lock, Ref<T> const& ref)
    {
        auto& rec = Get(lock, ref);

        static constexpr auto RecordSize = static_cast<uint32_t>(RecordTraits<T>::Size());
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
            size_t recsize = datasize + sizeof(impl::Blob);
            if (recsize > impl::PageForRecord<0>::MaxRecordSize) { throw std::logic_error("Large Blobs not yet implemented"); }
            recsize = BitCeil(recsize);
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
            static constexpr auto RecordSize = static_cast<uint32_t>(FixedSizeRecordTraits<T>::GetDataSize());
            auto [ref, slotobj]              = _Allocate<RecordSize>(lock, TypeId<T, ThisT>, RecordSize);
            assert(impl::Ref{ref}.page < _pagemgr->GetPageCount());
            auto rec = reinterpret_cast<Record<T>*>(slotobj.data.data());
            FixedSizeRecordTraits<T>::WriteToBuffer(obj, *rec);
            return RefAndRecord<T>(ref, *rec);
        }
        else if constexpr (ConceptComplex<T>)
        {
            static constexpr auto RecordSize = static_cast<uint32_t>(RecordTraits<T>::Size());
            if constexpr (RecordSize == 0)
            {
                size_t datasize = RecordTraits<T>::GetDataSize(obj);
                if (datasize == 0)
                {
                    // Objects can contain empty lists
                    // throw std::logic_error("Empty Blobs not allowed");
                    // Record<T> obj;
                    // return RefAndRecord<T>({}, {});
                }
                auto recsize = datasize + sizeof(impl::Blob);
                if (recsize > impl::PageForRecord<0>::MaxRecordSize) { throw std::logic_error("Large Blobs not yet implemented"); }

                recsize = BitCeil(recsize);
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
        else
        {
            throw std::logic_error("Unknown Type");
        }
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
    void Init(std::ifstream&& ifstrm) { _pagemgr->Init(std::move(ifstrm)); }

    void Flush(RWLock const& /*guardscope*/) { _pagemgr->Flush(); }

    private:
    // std::shared_mutex _mutex;
    std::shared_ptr<impl::PageManager> _pagemgr = std::make_shared<impl::PageManager>();

    // friends
    template <ConceptRecord T, typename TDb, typename TLock> friend struct impl::Iterator;

    void Init_(std::string_view const& fname);
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

    [[nodiscard]] std::string_view Get() const { return {Data<char const>(), blobSize}; }
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
    auto               Items() { return AsSpan<MapItem<K, V>>(); }
    [[nodiscard]] auto Items() const { return AsSpan<MapItem<K, V>>(); }
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
    auto               Items() { return AsSpan<Ref<T>>(); }
    [[nodiscard]] auto Items() const { return AsSpan<Ref<T>>(); }
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
    using RecordTypes = RecordTraits<T>::RecordTypes;
    using ObjectType  = std::unique_ptr<T>;
    template <typename TDb>
    static void WriteToBuffer(TDb& /*db*/, RWLock const& /*lock*/, ObjectType const& /*obj*/, Record<ObjectType>& /*rec*/)
    { throw std::logic_error("unique_ptr<T> for database not implemented"); }
};

template <typename T> struct RecordTraits<shared_stringT<T>>
{
    using RecordTypes = std::tuple<shared_stringT<T>>;
    static constexpr size_t Size() { return 0; }
    static uint32_t         GetDataSize(shared_stringT<T> const& obj) { return static_cast<uint32_t>(obj.size() * sizeof(T)); }

    template <typename TDb>
    static void WriteToBuffer(TDb& /*db*/, RWLock const& /*lock*/, shared_stringT<T> const& obj, Record<shared_stringT<T>>& rec)
    {
        static_assert(sizeof(Record<shared_stringT<T>>) == sizeof(impl::Blob));
        auto spn = rec.template AsSpan<T>();
        std::copy(obj.begin(), obj.end(), spn.begin());
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
    [[nodiscard]] T const& Get() const LFTBND { return data; }

    T data;
};

}    // namespace Stencil::Database
namespace Stencil::Database
{

template <typename T, typename TDb> struct RecordView;
template <typename T, typename TDb> struct RecordEdit;

template <typename T, typename TDb> struct RecordView
{

    RecordView(TDb& dbIn LFTBND, ROLock& lockIn LFTBND, Ref<T> const& idIn LFTBND, Record<T> const& recIn LFTBND) :
        db(dbIn), lock(lockIn), id(idIn), rec(recIn)
    {}
    ~RecordView() = default;
    CLASS_DELETE_COPY_AND_MOVE(RecordView);

    TDb&             db;
    ROLock&          lock;
    Ref<T> const&    id;
    Record<T> const& rec;
};

template <typename T> struct RecordViewTraits;
template <typename T, typename TDb> struct RecordViewTraits<RecordView<T, TDb>>
{
    using Db         = TDb;
    using Type       = T;
    using RecordType = Record<T>;
    using NestType   = RecordNest<T>;
};

template <typename T, typename TDb>
RecordView<T, TDb> CreateRecordView(TDb& db LFTBND, ROLock& lock LFTBND, Ref<T> const& id LFTBND, Record<T> const& rec LFTBND)
{ return RecordView<T, TDb>(db, lock, id, rec); }

template <typename T> static constexpr bool               IsRecordView                     = false;
template <typename T, typename TDb> static constexpr bool IsRecordView<RecordView<T, TDb>> = true;
template <typename T>
concept ConceptRecordView = IsRecordView<T>;

template <typename T>
concept ConceptTrivialRecordView = ConceptRecordView<T> && ConceptTrivial<typename RecordViewTraits<T>::Type>;

template <typename T>
concept ConceptComplexRecordView = ConceptRecordView<T> && ConceptComplex<typename RecordViewTraits<T>::Type>;

template <typename T>
concept ConceptFixedSizeRecordView = ConceptRecordView<T> && ConceptFixedSize<typename RecordViewTraits<T>::Type>;

}    // namespace Stencil::Database

template <Stencil::Database::ConceptRecordView T>
struct Stencil::TypeTraits<T> : Stencil::TypeTraits<typename Stencil::Database::RecordViewTraits<T>::Type>
{};

template <Stencil::Database::ConceptRecordView T> struct Stencil::Visitor<T>
{
    template <typename T1, typename TLambda> static void VisitAll([[maybe_unused]] T1& obj, [[maybe_unused]] TLambda&& lambda)
    {
        using RecType = Stencil::Database::RecordViewTraits<T>::RecordType;
        using Type    = Stencil::Database::RecordViewTraits<T>::Type;
        if constexpr (Stencil::ConceptPreferPrimitive<Type>) {}
        else if constexpr (Stencil::ConceptPreferIndexable<Type>)
        {
            lambda(Stencil::Database::RefKeyType{}, obj.id);    // Visit the ref<> of itself as a special field
            Stencil::Visitor<RecType>::VisitAll(obj.rec, [&](auto key, auto& subobj) {
                if constexpr (Stencil::Database::IsRef<std::remove_cvref_t<decltype(subobj)>>)
                {
                    auto& vrec  = obj.db.Get(obj.lock, subobj);
                    auto  vrecv = Stencil::Database::CreateRecordView(obj.db, obj.lock, subobj, vrec);
                    if constexpr (Stencil::Database::IsRef<std::remove_cvref_t<decltype(key)>>)
                    {
                        auto& krec  = obj.db.Get(obj.lock, key);
                        auto  krecv = Stencil::Database::CreateRecordView(obj.db, obj.lock, key, krec);
                        lambda(krecv, vrecv);
                    }
                    else
                    {
                        lambda(key, vrecv);
                    }
                }
                else
                {
                    lambda(key, subobj.Get());
                }
            });
        }
        else if constexpr (Stencil::ConceptPreferIterable<Type>)
        {
            Stencil::Visitor<RecType>::VisitAll(obj.rec, [&](auto k, auto& subobj) {
                if constexpr (Stencil::Database::IsRef<std::remove_cvref_t<decltype(subobj)>>)
                {
                    auto& vrec     = obj.db.Get(obj.lock, subobj);
                    auto  itemrecv = Stencil::Database::CreateRecordView(obj.db, obj.lock, subobj, vrec);
                    lambda(k, itemrecv);
                }
                else
                {
                    lambda(k, subobj.get());
                }
            });
        }
        else
        {
            throw std::logic_error("Unknown Type");
        }
    }
};

template <Stencil::ConceptProtocol TProt> struct Stencil::SerDes<Stencil::Database::RefKeyType, TProt>
{
    template <typename Context> static auto Write(Context& ctx, Stencil::Database::RefKeyType const& /* obj */)
    { SerDes<std::string_view, TProt>::Write(ctx, "__id"); }
    template <typename Context> static auto Read(Stencil::Database::RefKeyType& /* obj */, Context& /* ctx */);    // Undefined
};

template <Stencil::ConceptIndexable T, typename TDb> struct Stencil::TypeTraits<Stencil::Database::RecordView<T, TDb>>
{
    using Categories = Stencil::TypeTraits<T>::Categories;
};

template <Stencil::ConceptIndexable T, typename TDb> struct Stencil::TypeTraitsForIndexable<Stencil::Database::RecordView<T, TDb>>
{
    using Key = Stencil::TypeTraitsForIndexable<T>::Key;
};

template <Stencil::Database::ConceptTrivialRecordView T, Stencil::ConceptProtocol TProt> struct Stencil::SerDes<T, TProt>
{
    using Type       = Stencil::Database::RecordViewTraits<T>::Type;
    using RecordType = Stencil::Database::RecordViewTraits<T>::RecordType;

    template <typename Context> static auto Write(Context& ctx, T const& obj)
    {
        auto val = obj.rec.Get();
        SerDes<decltype(val), TProt>::Write(ctx, val);
    }
    template <typename Context> static auto Read(T& obj, Context& ctx) { SerDes<Type, TProt>::Read(obj._rec.Get(), ctx); }
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

template <typename T> struct Stencil::Database::RecordTraits<Stencil::OptionalPropsT<T>>
{
    using RecordTypes = std::tuple<T>;
    static constexpr size_t Size() { return sizeof(T); }

    template <typename TDb>
    static void
    WriteToBuffer(TDb& /*db*/, RWLock const& /*lock*/, Stencil::OptionalPropsT<T> const& obj, Record<Stencil::OptionalPropsT<T>>& rec)
    { rec.fieldtracker = obj.fieldtracker; }
};

template <typename T> struct Stencil::Database::RecordTraits<Stencil::TimestampedT<T>>
{
    using RecordTypes = std::tuple<T>;
    static constexpr size_t Size() { return sizeof(T); }

    template <typename TDb>
    static void
    WriteToBuffer(TDb& /*db*/, RWLock const& /*lock*/, Stencil::TimestampedT<T> const& obj, Record<Stencil::TimestampedT<T>>& rec)
    { rec.lastmodified = obj.lastmodified; }
};

template <typename T> struct Stencil::Database::RecordTraits<Stencil::RefMap<T>>
{
    using RecordTypes = std::tuple<T>;
    static constexpr size_t Size() { return 0; }

    template <typename TDb>
    static void
    WriteToBuffer(TDb& /*db*/, RWLock const& /*lock*/, Stencil::RefMap<T> const& /* obj */, Record<Stencil::RefMap<T>>& /* rec */)
    { throw std::logic_error("Not implemented"); }
};

template <typename T> struct Stencil::Database::RecordTraits<SharedTree<T>>
{
    using RecordTypes = std::tuple<T>;
    static constexpr size_t Size() { return 0; }

    template <typename TDb>
    static void WriteToBuffer(TDb& /*db*/, RWLock const& /*lock*/, SharedTree<T> const& /* obj */, Record<SharedTree<T>>& /* rec */)
    { throw std::logic_error("Not implemented"); }
};
