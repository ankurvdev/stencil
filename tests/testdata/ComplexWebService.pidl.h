#pragma once
#include <stencil/stencil.h>

// SECTION START: DECLARATIONS
#if true

namespace ComplexWebService
{
struct MapPoint;
struct GeographicalArea;
struct DigitalAssetInfo;
struct KeywordS;
struct Content;
struct Search;
struct Store;
}    // namespace ComplexWebService

template <> struct Stencil::TypeTraits<ComplexWebService::MapPoint>;
template <> struct Stencil::TypeTraits<ComplexWebService::GeographicalArea>;
template <> struct Stencil::TypeTraits<ComplexWebService::DigitalAssetInfo>;
template <> struct Stencil::TypeTraits<ComplexWebService::KeywordS>;
template <> struct Stencil::TypeTraits<ComplexWebService::Content>;
template <> struct Stencil::TypeTraits<ComplexWebService::Search>;
template <> struct Stencil::TypeTraits<ComplexWebService::Store>;
#endif
// SECTION END: DECLARATIONS

// SECTION START: Definitions
#if true
namespace ComplexWebService
{
typedef shared_tree<shared_string> Keyword;
typedef shared_tree<shared_string> Entity;
typedef std::vector<::ComplexWebService::MapPoint> AreaPolygon;
typedef shared_tree<::ComplexWebService::GeographicalArea> GeographicalLocation;
typedef std::vector<shared_tree<shared_string>> Keywords;
typedef std::vector<shared_tree<shared_string>> Entities;
struct MapPoint :
    public Stencil::StructT<MapPoint>
{
    int64_t latitude = {};
    int64_t longitude = {};
};
struct GeographicalArea :
    public Stencil::StructT<GeographicalArea>
{
    shared_string type = {};
    shared_string name = {};
    std::vector<::ComplexWebService::MapPoint> areaPolygon = {};
};
struct DigitalAssetInfo :
    public Stencil::StructT<DigitalAssetInfo>
{
    int32_t id = {};
    std::vector<shared_tree<shared_string>> keywords = {};
    shared_tree<::ComplexWebService::GeographicalArea> location = {};
    int64_t md5sum = {};
    shared_string thumbnailBlob = {};
    shared_string fileUrl = {};
};
struct KeywordS : public Stencil::InterfaceT<KeywordS>
{
    public:
    struct Dummy
    {};
    struct ObjectStore;

    struct ObjectStore : Dummy
    {};

    KeywordS()          = default;
    virtual ~KeywordS() = default;
    DELETE_COPY_AND_MOVE(KeywordS);
    virtual shared_tree<shared_string> Find(
        shared_tree<shared_string> const& root
,        shared_string const& query
,        shared_string const& type
        )
        = 0;
    virtual shared_tree<shared_string> Create(
        shared_string const& word
,        shared_tree<shared_string> const& parent
        )
        = 0;
    virtual int32_t Delete(
        )
        = 0;
    virtual int32_t Modify(
        shared_string const& word
        )
        = 0;
    virtual int32_t SetParent(
        shared_tree<shared_string> const& parent
        )
        = 0;
    struct Args_Find
    {
        shared_tree<shared_string> arg_root{};
        shared_string arg_query{};
        shared_string arg_type{};
    };

    shared_tree<shared_string> Find(Args_Find const& args)
    {
        return this->Find(
            args.root
,            args.query
,            args.type
        );
    }

    shared_tree<shared_string> Find(Args_Find&& args)
    {
        return this->Find(
            std::move(args.root)
,            std::move(args.query)
,            std::move(args.type)
        );
    }

    struct Args_Create
    {
        shared_string arg_word{};
        shared_tree<shared_string> arg_parent{};
    };

    shared_tree<shared_string> Create(Args_Create const& args)
    {
        return this->Create(
            args.word
,            args.parent
        );
    }

    shared_tree<shared_string> Create(Args_Create&& args)
    {
        return this->Create(
            std::move(args.word)
,            std::move(args.parent)
        );
    }

    struct Args_Delete
    {
    };

    int32_t Delete(Args_Delete const& args)
    {
        return this->Delete(
        );
    }

    int32_t Delete(Args_Delete&& args)
    {
        return this->Delete(
        );
    }

    struct Args_Modify
    {
        shared_string arg_word{};
    };

    int32_t Modify(Args_Modify const& args)
    {
        return this->Modify(
            args.word
        );
    }

    int32_t Modify(Args_Modify&& args)
    {
        return this->Modify(
            std::move(args.word)
        );
    }

    struct Args_SetParent
    {
        shared_tree<shared_string> arg_parent{};
    };

    int32_t SetParent(Args_SetParent const& args)
    {
        return this->SetParent(
            args.parent
        );
    }

    int32_t SetParent(Args_SetParent&& args)
    {
        return this->SetParent(
            std::move(args.parent)
        );
    }

    ObjectStore objects;
};
struct Content : public Stencil::InterfaceT<Content>
{
    public:
    struct Dummy
    {};
    struct ObjectStore;

    struct ObjectStore : Dummy
    {};

    Content()          = default;
    virtual ~Content() = default;
    DELETE_COPY_AND_MOVE(Content);
    virtual int32_t AddKeyword(
        shared_tree<shared_string> const& keyword
        )
        = 0;
    virtual int32_t RemoveKeyword(
        shared_tree<shared_string> const& keyword
        )
        = 0;
    struct Args_AddKeyword
    {
        shared_tree<shared_string> arg_keyword{};
    };

    int32_t AddKeyword(Args_AddKeyword const& args)
    {
        return this->AddKeyword(
            args.keyword
        );
    }

    int32_t AddKeyword(Args_AddKeyword&& args)
    {
        return this->AddKeyword(
            std::move(args.keyword)
        );
    }

    struct Args_RemoveKeyword
    {
        shared_tree<shared_string> arg_keyword{};
    };

    int32_t RemoveKeyword(Args_RemoveKeyword const& args)
    {
        return this->RemoveKeyword(
            args.keyword
        );
    }

    int32_t RemoveKeyword(Args_RemoveKeyword&& args)
    {
        return this->RemoveKeyword(
            std::move(args.keyword)
        );
    }

    ObjectStore objects;
};
struct Search : public Stencil::InterfaceT<Search>
{
    public:
    struct Dummy
    {};
    struct ObjectStore;

    struct ObjectStore : Dummy
    {};

    Search()          = default;
    virtual ~Search() = default;
    DELETE_COPY_AND_MOVE(Search);
    virtual std::vector<long> SearchByKeywords(
        std::vector<shared_tree<shared_string>> const& keywords
        )
        = 0;
    virtual std::vector<long> SearchByDate(
        int64_t const& startDate
,        int64_t const& endDate
        )
        = 0;
    struct Args_SearchByKeywords
    {
        std::vector<shared_tree<shared_string>> arg_keywords{};
    };

    std::vector<long> SearchByKeywords(Args_SearchByKeywords const& args)
    {
        return this->SearchByKeywords(
            args.keywords
        );
    }

    std::vector<long> SearchByKeywords(Args_SearchByKeywords&& args)
    {
        return this->SearchByKeywords(
            std::move(args.keywords)
        );
    }

    struct Args_SearchByDate
    {
        int64_t arg_startDate{};
        int64_t arg_endDate{};
    };

    std::vector<long> SearchByDate(Args_SearchByDate const& args)
    {
        return this->SearchByDate(
            args.startDate
,            args.endDate
        );
    }

    std::vector<long> SearchByDate(Args_SearchByDate&& args)
    {
        return this->SearchByDate(
            std::move(args.startDate)
,            std::move(args.endDate)
        );
    }

    ObjectStore objects;
};
struct Store : public Stencil::InterfaceT<Store>
{
    public:
    struct Dummy
    {};
    struct ObjectStore;

    struct ObjectStore : Dummy
    {};

    Store()          = default;
    virtual ~Store() = default;
    DELETE_COPY_AND_MOVE(Store);
    virtual ::ComplexWebService::DigitalAssetInfo Upload(
        shared_string const& file
        )
        = 0;
    virtual shared_string Download(
        long const& content
        )
        = 0;
    struct Args_Upload
    {
        shared_string arg_file{};
    };

    ::ComplexWebService::DigitalAssetInfo Upload(Args_Upload const& args)
    {
        return this->Upload(
            args.file
        );
    }

    ::ComplexWebService::DigitalAssetInfo Upload(Args_Upload&& args)
    {
        return this->Upload(
            std::move(args.file)
        );
    }

    struct Args_Download
    {
        long arg_content{};
    };

    shared_string Download(Args_Download const& args)
    {
        return this->Download(
            args.content
        );
    }

    shared_string Download(Args_Download&& args)
    {
        return this->Download(
            std::move(args.content)
        );
    }

    ObjectStore objects;
};
}    // namespace ComplexWebService
#endif

// SECTION END: Definitions

// SECTION START: Template specializations
#if true

// SECTION:

template <> struct Stencil::TypeTraits<ComplexWebService::MapPoint>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<ComplexWebService::MapPoint>
{
    enum class Fields
    {
        Invalid,
        Field_latitude
,        Field_longitude
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<ComplexWebService::MapPoint>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<ComplexWebService::MapPoint>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "latitude"
,        "longitude"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<ComplexWebService::MapPoint>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<ComplexWebService::MapPoint>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<ComplexWebService::MapPoint> : Stencil::TransactionT<ComplexWebService::MapPoint>
{
    using TData  = ComplexWebService::MapPoint;
    using Fields = Stencil::TypeTraitsForIndexable<ComplexWebService::MapPoint>::Fields;

    Transaction<int64_t> _subtracker_latitude;
    Transaction<int64_t> _subtracker_longitude;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<ComplexWebService::MapPoint>(ptr)
        ,
        _subtracker_latitude(Obj().latitude)
        ,
        _subtracker_longitude(Obj().longitude)
    {}

    auto& latitude()
    {
        MarkFieldEdited_(Fields::Field_latitude);
        return _subtracker_latitude;
    }
    auto& longitude()
    {
        MarkFieldEdited_(Fields::Field_longitude);
        return _subtracker_longitude;
    }
    void set_latitude(int64_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_latitude, Obj().latitude, val);
        Obj().latitude = std::move(val);
    }

    void set_longitude(int64_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_longitude, Obj().longitude, val);
        Obj().longitude = std::move(val);
    }

    template <typename TLambda> auto Visit(Fields index, TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_latitude: return lambda("latitude", latitude()); return;
        case Fields::Field_longitude: return lambda("longitude", longitude()); return;
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "latitude") { return lambda(Fields::Field_latitude, latitude()); }
        if (fieldName == "longitude") { return lambda(Fields::Field_longitude, longitude()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("latitude", Fields::Field_latitude, latitude(), Obj().latitude);
        lambda("longitude", Fields::Field_longitude, longitude(), Obj().longitude);
    }

    void Flush()
    {
        latitude().Flush();

        if (IsFieldEdited(Fields::Field_latitude))
        {
            if (!latitude().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_latitude));
        }

        longitude().Flush();

        if (IsFieldEdited(Fields::Field_longitude))
        {
            if (!longitude().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_longitude));
        }

        Stencil::TransactionT<ComplexWebService::MapPoint>::Flush_();
    }
};

template <> struct Stencil::Visitor<ComplexWebService::MapPoint> : Stencil::VisitorT<ComplexWebService::MapPoint>
{
    using TData  = ComplexWebService::MapPoint;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey(T& obj, Fields field, TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_latitude: return lambda(obj.latitude);
        case Fields::Field_longitude: return lambda(obj.longitude);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies(T& obj, TLambda&& lambda)
    {
        lambda(Fields::Field_latitude, obj.latitude);
        lambda(Fields::Field_longitude, obj.longitude);
    }
};

template <> struct Stencil::TypeTraits<ComplexWebService::GeographicalArea>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<ComplexWebService::GeographicalArea>
{
    enum class Fields
    {
        Invalid,
        Field_type
,        Field_name
,        Field_areaPolygon
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<ComplexWebService::GeographicalArea>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<ComplexWebService::GeographicalArea>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "type"
,        "name"
,        "areaPolygon"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<ComplexWebService::GeographicalArea>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<ComplexWebService::GeographicalArea>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<ComplexWebService::GeographicalArea> : Stencil::TransactionT<ComplexWebService::GeographicalArea>
{
    using TData  = ComplexWebService::GeographicalArea;
    using Fields = Stencil::TypeTraitsForIndexable<ComplexWebService::GeographicalArea>::Fields;

    Transaction<shared_string> _subtracker_type;
    Transaction<shared_string> _subtracker_name;
    Transaction<std::vector<::ComplexWebService::MapPoint>> _subtracker_areaPolygon;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<ComplexWebService::GeographicalArea>(ptr)
        ,
        _subtracker_type(Obj().type)
        ,
        _subtracker_name(Obj().name)
        ,
        _subtracker_areaPolygon(Obj().areaPolygon)
    {}

    auto& type()
    {
        MarkFieldEdited_(Fields::Field_type);
        return _subtracker_type;
    }
    auto& name()
    {
        MarkFieldEdited_(Fields::Field_name);
        return _subtracker_name;
    }
    auto& areaPolygon()
    {
        MarkFieldEdited_(Fields::Field_areaPolygon);
        return _subtracker_areaPolygon;
    }
    void set_type(shared_string&& val)
    {
        MarkFieldAssigned_(Fields::Field_type, Obj().type, val);
        Obj().type = std::move(val);
    }

    void set_name(shared_string&& val)
    {
        MarkFieldAssigned_(Fields::Field_name, Obj().name, val);
        Obj().name = std::move(val);
    }

    void set_areaPolygon(std::vector<::ComplexWebService::MapPoint>&& val)
    {
        MarkFieldAssigned_(Fields::Field_areaPolygon, Obj().areaPolygon, val);
        Obj().areaPolygon = std::move(val);
    }

    template <typename TLambda> auto Visit(Fields index, TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_type: return lambda("type", type()); return;
        case Fields::Field_name: return lambda("name", name()); return;
        case Fields::Field_areaPolygon: return lambda("areaPolygon", areaPolygon()); return;
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "type") { return lambda(Fields::Field_type, type()); }
        if (fieldName == "name") { return lambda(Fields::Field_name, name()); }
        if (fieldName == "areaPolygon") { return lambda(Fields::Field_areaPolygon, areaPolygon()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("type", Fields::Field_type, type(), Obj().type);
        lambda("name", Fields::Field_name, name(), Obj().name);
        lambda("areaPolygon", Fields::Field_areaPolygon, areaPolygon(), Obj().areaPolygon);
    }

    void Flush()
    {
        type().Flush();

        if (IsFieldEdited(Fields::Field_type))
        {
            if (!type().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_type));
        }

        name().Flush();

        if (IsFieldEdited(Fields::Field_name))
        {
            if (!name().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_name));
        }

        areaPolygon().Flush();

        if (IsFieldEdited(Fields::Field_areaPolygon))
        {
            if (!areaPolygon().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_areaPolygon));
        }

        Stencil::TransactionT<ComplexWebService::GeographicalArea>::Flush_();
    }
};

template <> struct Stencil::Visitor<ComplexWebService::GeographicalArea> : Stencil::VisitorT<ComplexWebService::GeographicalArea>
{
    using TData  = ComplexWebService::GeographicalArea;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey(T& obj, Fields field, TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_type: return lambda(obj.type);
        case Fields::Field_name: return lambda(obj.name);
        case Fields::Field_areaPolygon: return lambda(obj.areaPolygon);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies(T& obj, TLambda&& lambda)
    {
        lambda(Fields::Field_type, obj.type);
        lambda(Fields::Field_name, obj.name);
        lambda(Fields::Field_areaPolygon, obj.areaPolygon);
    }
};

template <> struct Stencil::TypeTraits<ComplexWebService::DigitalAssetInfo>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<ComplexWebService::DigitalAssetInfo>
{
    enum class Fields
    {
        Invalid,
        Field_id
,        Field_keywords
,        Field_location
,        Field_md5sum
,        Field_thumbnailBlob
,        Field_fileUrl
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<ComplexWebService::DigitalAssetInfo>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<ComplexWebService::DigitalAssetInfo>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "id"
,        "keywords"
,        "location"
,        "md5sum"
,        "thumbnailBlob"
,        "fileUrl"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<ComplexWebService::DigitalAssetInfo>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<ComplexWebService::DigitalAssetInfo>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<ComplexWebService::DigitalAssetInfo> : Stencil::TransactionT<ComplexWebService::DigitalAssetInfo>
{
    using TData  = ComplexWebService::DigitalAssetInfo;
    using Fields = Stencil::TypeTraitsForIndexable<ComplexWebService::DigitalAssetInfo>::Fields;

    Transaction<int32_t> _subtracker_id;
    Transaction<std::vector<shared_tree<shared_string>>> _subtracker_keywords;
    Transaction<shared_tree<::ComplexWebService::GeographicalArea>> _subtracker_location;
    Transaction<int64_t> _subtracker_md5sum;
    Transaction<shared_string> _subtracker_thumbnailBlob;
    Transaction<shared_string> _subtracker_fileUrl;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<ComplexWebService::DigitalAssetInfo>(ptr)
        ,
        _subtracker_id(Obj().id)
        ,
        _subtracker_keywords(Obj().keywords)
        ,
        _subtracker_location(Obj().location)
        ,
        _subtracker_md5sum(Obj().md5sum)
        ,
        _subtracker_thumbnailBlob(Obj().thumbnailBlob)
        ,
        _subtracker_fileUrl(Obj().fileUrl)
    {}

    auto& id()
    {
        MarkFieldEdited_(Fields::Field_id);
        return _subtracker_id;
    }
    auto& keywords()
    {
        MarkFieldEdited_(Fields::Field_keywords);
        return _subtracker_keywords;
    }
    auto& location()
    {
        MarkFieldEdited_(Fields::Field_location);
        return _subtracker_location;
    }
    auto& md5sum()
    {
        MarkFieldEdited_(Fields::Field_md5sum);
        return _subtracker_md5sum;
    }
    auto& thumbnailBlob()
    {
        MarkFieldEdited_(Fields::Field_thumbnailBlob);
        return _subtracker_thumbnailBlob;
    }
    auto& fileUrl()
    {
        MarkFieldEdited_(Fields::Field_fileUrl);
        return _subtracker_fileUrl;
    }
    void set_id(int32_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_id, Obj().id, val);
        Obj().id = std::move(val);
    }

    void set_keywords(std::vector<shared_tree<shared_string>>&& val)
    {
        MarkFieldAssigned_(Fields::Field_keywords, Obj().keywords, val);
        Obj().keywords = std::move(val);
    }

    void set_location(shared_tree<::ComplexWebService::GeographicalArea>&& val)
    {
        MarkFieldAssigned_(Fields::Field_location, Obj().location, val);
        Obj().location = std::move(val);
    }

    void set_md5sum(int64_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_md5sum, Obj().md5sum, val);
        Obj().md5sum = std::move(val);
    }

    void set_thumbnailBlob(shared_string&& val)
    {
        MarkFieldAssigned_(Fields::Field_thumbnailBlob, Obj().thumbnailBlob, val);
        Obj().thumbnailBlob = std::move(val);
    }

    void set_fileUrl(shared_string&& val)
    {
        MarkFieldAssigned_(Fields::Field_fileUrl, Obj().fileUrl, val);
        Obj().fileUrl = std::move(val);
    }

    template <typename TLambda> auto Visit(Fields index, TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_id: return lambda("id", id()); return;
        case Fields::Field_keywords: return lambda("keywords", keywords()); return;
        case Fields::Field_location: return lambda("location", location()); return;
        case Fields::Field_md5sum: return lambda("md5sum", md5sum()); return;
        case Fields::Field_thumbnailBlob: return lambda("thumbnailBlob", thumbnailBlob()); return;
        case Fields::Field_fileUrl: return lambda("fileUrl", fileUrl()); return;
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "id") { return lambda(Fields::Field_id, id()); }
        if (fieldName == "keywords") { return lambda(Fields::Field_keywords, keywords()); }
        if (fieldName == "location") { return lambda(Fields::Field_location, location()); }
        if (fieldName == "md5sum") { return lambda(Fields::Field_md5sum, md5sum()); }
        if (fieldName == "thumbnailBlob") { return lambda(Fields::Field_thumbnailBlob, thumbnailBlob()); }
        if (fieldName == "fileUrl") { return lambda(Fields::Field_fileUrl, fileUrl()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("id", Fields::Field_id, id(), Obj().id);
        lambda("keywords", Fields::Field_keywords, keywords(), Obj().keywords);
        lambda("location", Fields::Field_location, location(), Obj().location);
        lambda("md5sum", Fields::Field_md5sum, md5sum(), Obj().md5sum);
        lambda("thumbnailBlob", Fields::Field_thumbnailBlob, thumbnailBlob(), Obj().thumbnailBlob);
        lambda("fileUrl", Fields::Field_fileUrl, fileUrl(), Obj().fileUrl);
    }

    void Flush()
    {
        id().Flush();

        if (IsFieldEdited(Fields::Field_id))
        {
            if (!id().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_id));
        }

        keywords().Flush();

        if (IsFieldEdited(Fields::Field_keywords))
        {
            if (!keywords().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_keywords));
        }

        location().Flush();

        if (IsFieldEdited(Fields::Field_location))
        {
            if (!location().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_location));
        }

        md5sum().Flush();

        if (IsFieldEdited(Fields::Field_md5sum))
        {
            if (!md5sum().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_md5sum));
        }

        thumbnailBlob().Flush();

        if (IsFieldEdited(Fields::Field_thumbnailBlob))
        {
            if (!thumbnailBlob().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_thumbnailBlob));
        }

        fileUrl().Flush();

        if (IsFieldEdited(Fields::Field_fileUrl))
        {
            if (!fileUrl().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_fileUrl));
        }

        Stencil::TransactionT<ComplexWebService::DigitalAssetInfo>::Flush_();
    }
};

template <> struct Stencil::Visitor<ComplexWebService::DigitalAssetInfo> : Stencil::VisitorT<ComplexWebService::DigitalAssetInfo>
{
    using TData  = ComplexWebService::DigitalAssetInfo;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey(T& obj, Fields field, TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_id: return lambda(obj.id);
        case Fields::Field_keywords: return lambda(obj.keywords);
        case Fields::Field_location: return lambda(obj.location);
        case Fields::Field_md5sum: return lambda(obj.md5sum);
        case Fields::Field_thumbnailBlob: return lambda(obj.thumbnailBlob);
        case Fields::Field_fileUrl: return lambda(obj.fileUrl);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies(T& obj, TLambda&& lambda)
    {
        lambda(Fields::Field_id, obj.id);
        lambda(Fields::Field_keywords, obj.keywords);
        lambda(Fields::Field_location, obj.location);
        lambda(Fields::Field_md5sum, obj.md5sum);
        lambda(Fields::Field_thumbnailBlob, obj.thumbnailBlob);
        lambda(Fields::Field_fileUrl, obj.fileUrl);
    }
};

template <> struct Stencil::TypeTraits<ComplexWebService::KeywordS_Find_Args>
{
    struct Traits_arg_root
    {
        using TOwner     = ComplexWebService::KeywordS_Find_Args;
        using TFieldType = shared_tree<shared_string>;

        static constexpr std::string_view Name() { return "root"; }
        static const ::Stencil::Flags     Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TOwner::get_arg_root; }
        static constexpr auto TPropertySetter() { return &TOwner::set_arg_root; }
    };
    struct Traits_arg_query
    {
        using TOwner     = ComplexWebService::KeywordS_Find_Args;
        using TFieldType = shared_string;

        static constexpr std::string_view Name() { return "query"; }
        static const ::Stencil::Flags     Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TOwner::get_arg_query; }
        static constexpr auto TPropertySetter() { return &TOwner::set_arg_query; }
    };
    struct Traits_arg_type
    {
        using TOwner     = ComplexWebService::KeywordS_Find_Args;
        using TFieldType = shared_string;

        static constexpr std::string_view Name() { return "type"; }
        static const ::Stencil::Flags     Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TOwner::get_arg_type; }
        static constexpr auto TPropertySetter() { return &TOwner::set_arg_type; }
    };
    static constexpr ::Stencil::DataType Type() { return ::Stencil::DataType::Object; }
    static constexpr std::string_view    Name() { return "Find"; }
    static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

    using ThisType = ComplexWebService::KeywordS_Find_Args;

    using Handler = ::ReflectionServices::ReflectedStructHandler<ComplexWebService::KeywordS_Find_Args
                                                                     Traits_arg_root
,                                                                     Traits_arg_query
,                                                                     Traits_arg_type
                                                                 >;
};

template <>
Stencil::Comparator<ComplexWebService::KeywordS_Find_Args,
                    ComplexWebService::KeywordS_Find_Args>{
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2){
        return true
               && Stencil::AreEqual(obj1.get_carg_root(), obj2.get_carg_root())
               && Stencil::AreEqual(obj1.get_carg_query(), obj2.get_carg_query())
               && Stencil::AreEqual(obj1.get_carg_type(), obj2.get_carg_type())
        ;
}
}
;
template <> struct Stencil::TypeTraits<ComplexWebService::KeywordS_Create_Args>
{
    struct Traits_arg_word
    {
        using TOwner     = ComplexWebService::KeywordS_Create_Args;
        using TFieldType = shared_string;

        static constexpr std::string_view Name() { return "word"; }
        static const ::Stencil::Flags     Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TOwner::get_arg_word; }
        static constexpr auto TPropertySetter() { return &TOwner::set_arg_word; }
    };
    struct Traits_arg_parent
    {
        using TOwner     = ComplexWebService::KeywordS_Create_Args;
        using TFieldType = shared_tree<shared_string>;

        static constexpr std::string_view Name() { return "parent"; }
        static const ::Stencil::Flags     Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TOwner::get_arg_parent; }
        static constexpr auto TPropertySetter() { return &TOwner::set_arg_parent; }
    };
    static constexpr ::Stencil::DataType Type() { return ::Stencil::DataType::Object; }
    static constexpr std::string_view    Name() { return "Create"; }
    static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

    using ThisType = ComplexWebService::KeywordS_Create_Args;

    using Handler = ::ReflectionServices::ReflectedStructHandler<ComplexWebService::KeywordS_Create_Args
                                                                     Traits_arg_word
,                                                                     Traits_arg_parent
                                                                 >;
};

template <>
Stencil::Comparator<ComplexWebService::KeywordS_Create_Args,
                    ComplexWebService::KeywordS_Create_Args>{
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2){
        return true
               && Stencil::AreEqual(obj1.get_carg_word(), obj2.get_carg_word())
               && Stencil::AreEqual(obj1.get_carg_parent(), obj2.get_carg_parent())
        ;
}
}
;
template <> struct Stencil::TypeTraits<ComplexWebService::KeywordS_Delete_Args>
{
    static constexpr ::Stencil::DataType Type() { return ::Stencil::DataType::Object; }
    static constexpr std::string_view    Name() { return "Delete"; }
    static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

    using ThisType = ComplexWebService::KeywordS_Delete_Args;

    using Handler = ::ReflectionServices::ReflectedStructHandler<ComplexWebService::KeywordS_Delete_Args
                                                                 >;
};

template <>
Stencil::Comparator<ComplexWebService::KeywordS_Delete_Args,
                    ComplexWebService::KeywordS_Delete_Args>{
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2){
        return true
        ;
}
}
;
template <> struct Stencil::TypeTraits<ComplexWebService::KeywordS_Modify_Args>
{
    struct Traits_arg_word
    {
        using TOwner     = ComplexWebService::KeywordS_Modify_Args;
        using TFieldType = shared_string;

        static constexpr std::string_view Name() { return "word"; }
        static const ::Stencil::Flags     Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TOwner::get_arg_word; }
        static constexpr auto TPropertySetter() { return &TOwner::set_arg_word; }
    };
    static constexpr ::Stencil::DataType Type() { return ::Stencil::DataType::Object; }
    static constexpr std::string_view    Name() { return "Modify"; }
    static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

    using ThisType = ComplexWebService::KeywordS_Modify_Args;

    using Handler = ::ReflectionServices::ReflectedStructHandler<ComplexWebService::KeywordS_Modify_Args
                                                                     Traits_arg_word
                                                                 >;
};

template <>
Stencil::Comparator<ComplexWebService::KeywordS_Modify_Args,
                    ComplexWebService::KeywordS_Modify_Args>{
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2){
        return true
               && Stencil::AreEqual(obj1.get_carg_word(), obj2.get_carg_word())
        ;
}
}
;
template <> struct Stencil::TypeTraits<ComplexWebService::KeywordS_SetParent_Args>
{
    struct Traits_arg_parent
    {
        using TOwner     = ComplexWebService::KeywordS_SetParent_Args;
        using TFieldType = shared_tree<shared_string>;

        static constexpr std::string_view Name() { return "parent"; }
        static const ::Stencil::Flags     Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TOwner::get_arg_parent; }
        static constexpr auto TPropertySetter() { return &TOwner::set_arg_parent; }
    };
    static constexpr ::Stencil::DataType Type() { return ::Stencil::DataType::Object; }
    static constexpr std::string_view    Name() { return "SetParent"; }
    static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

    using ThisType = ComplexWebService::KeywordS_SetParent_Args;

    using Handler = ::ReflectionServices::ReflectedStructHandler<ComplexWebService::KeywordS_SetParent_Args
                                                                     Traits_arg_parent
                                                                 >;
};

template <>
Stencil::Comparator<ComplexWebService::KeywordS_SetParent_Args,
                    ComplexWebService::KeywordS_SetParent_Args>{
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2){
        return true
               && Stencil::AreEqual(obj1.get_carg_parent(), obj2.get_carg_parent())
        ;
}
}
;
template <> struct Stencil::InterfaceTraits<ComplexWebService::KeywordS>
{
    struct ApiTraits_Find
    {
        using TOwner = ComplexWebService::KeywordS;
        static const ::Stencil::Flags     Flags() { return {}; }
        static constexpr std::string_view Name() { return "Find"; }
        static constexpr bool             Static = false;
    };
    struct ApiTraits_Create
    {
        using TOwner = ComplexWebService::KeywordS;
        static const ::Stencil::Flags     Flags() { return {}; }
        static constexpr std::string_view Name() { return "Create"; }
        static constexpr bool             Static = false;
    };
    struct ApiTraits_Delete
    {
        using TOwner = ComplexWebService::KeywordS;
        static const ::Stencil::Flags     Flags() { return {}; }
        static constexpr std::string_view Name() { return "Delete"; }
        static constexpr bool             Static = false;
    };
    struct ApiTraits_Modify
    {
        using TOwner = ComplexWebService::KeywordS;
        static const ::Stencil::Flags     Flags() { return {}; }
        static constexpr std::string_view Name() { return "Modify"; }
        static constexpr bool             Static = false;
    };
    struct ApiTraits_SetParent
    {
        using TOwner = ComplexWebService::KeywordS;
        static const ::Stencil::Flags     Flags() { return {}; }
        static constexpr std::string_view Name() { return "SetParent"; }
        static constexpr bool             Static = false;
    };
    using Apis = std::tuple<
        ApiTraits_Find
,        ApiTraits_Create
,        ApiTraits_Delete
,        ApiTraits_Modify
,        ApiTraits_SetParent
        >;

    using Objects = std::tuple<
        >;
};

template <> struct Stencil::InterfaceApiTraits<Stencil::InterfaceTraits<ComplexWebService::KeywordS>::ApiTraits_Find>
{
    using ArgsStruct = ComplexWebService::KeywordS_Find_Args;
    static constexpr bool             IsStatic() { return false; }
    static constexpr std::string_view Name() { return "Find"; }

    static auto Invoke(ArgsStruct& args)
    {
        return args.instance->Find(
            args.get_arg_root()
,            args.get_arg_query()
,            args.get_arg_type()
        );
    }
};

template <> struct Stencil::InterfaceApiTraits<Stencil::InterfaceTraits<ComplexWebService::KeywordS>::ApiTraits_Create>
{
    using ArgsStruct = ComplexWebService::KeywordS_Create_Args;
    static constexpr bool             IsStatic() { return false; }
    static constexpr std::string_view Name() { return "Create"; }

    static auto Invoke(ArgsStruct& args)
    {
        return args.instance->Create(
            args.get_arg_word()
,            args.get_arg_parent()
        );
    }
};

template <> struct Stencil::InterfaceApiTraits<Stencil::InterfaceTraits<ComplexWebService::KeywordS>::ApiTraits_Delete>
{
    using ArgsStruct = ComplexWebService::KeywordS_Delete_Args;
    static constexpr bool             IsStatic() { return false; }
    static constexpr std::string_view Name() { return "Delete"; }

    static auto Invoke(ArgsStruct& args)
    {
        return args.instance->Delete(
        );
    }
};

template <> struct Stencil::InterfaceApiTraits<Stencil::InterfaceTraits<ComplexWebService::KeywordS>::ApiTraits_Modify>
{
    using ArgsStruct = ComplexWebService::KeywordS_Modify_Args;
    static constexpr bool             IsStatic() { return false; }
    static constexpr std::string_view Name() { return "Modify"; }

    static auto Invoke(ArgsStruct& args)
    {
        return args.instance->Modify(
            args.get_arg_word()
        );
    }
};

template <> struct Stencil::InterfaceApiTraits<Stencil::InterfaceTraits<ComplexWebService::KeywordS>::ApiTraits_SetParent>
{
    using ArgsStruct = ComplexWebService::KeywordS_SetParent_Args;
    static constexpr bool             IsStatic() { return false; }
    static constexpr std::string_view Name() { return "SetParent"; }

    static auto Invoke(ArgsStruct& args)
    {
        return args.instance->SetParent(
            args.get_arg_parent()
        );
    }
};

#if ((defined STENCIL_USING_WEBSERVICE) and (STENCIL_USING_WEBSERVICE > 0))
template <> struct Stencil::WebServiceHandlerTraits<ComplexWebService::KeywordS>
{
    static constexpr const std::string_view Url() { return std::string_view("KeywordS"); }
    // Name to api enum
    // Name to funcptr mapping
    // Name to argstruct mapping
    // Invoke api for each arg struct
    static void HandleRequest(ComplexWebService::KeywordS& obj,
                              httplib::Request const&               req,
                              httplib::Response&                    res,
                              std::string_view const&               path);
};
#endif

template <> struct Stencil::TypeTraits<ComplexWebService::Content_AddKeyword_Args>
{
    struct Traits_arg_keyword
    {
        using TOwner     = ComplexWebService::Content_AddKeyword_Args;
        using TFieldType = shared_tree<shared_string>;

        static constexpr std::string_view Name() { return "keyword"; }
        static const ::Stencil::Flags     Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TOwner::get_arg_keyword; }
        static constexpr auto TPropertySetter() { return &TOwner::set_arg_keyword; }
    };
    static constexpr ::Stencil::DataType Type() { return ::Stencil::DataType::Object; }
    static constexpr std::string_view    Name() { return "AddKeyword"; }
    static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

    using ThisType = ComplexWebService::Content_AddKeyword_Args;

    using Handler = ::ReflectionServices::ReflectedStructHandler<ComplexWebService::Content_AddKeyword_Args
                                                                     Traits_arg_keyword
                                                                 >;
};

template <>
Stencil::Comparator<ComplexWebService::Content_AddKeyword_Args,
                    ComplexWebService::Content_AddKeyword_Args>{
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2){
        return true
               && Stencil::AreEqual(obj1.get_carg_keyword(), obj2.get_carg_keyword())
        ;
}
}
;
template <> struct Stencil::TypeTraits<ComplexWebService::Content_RemoveKeyword_Args>
{
    struct Traits_arg_keyword
    {
        using TOwner     = ComplexWebService::Content_RemoveKeyword_Args;
        using TFieldType = shared_tree<shared_string>;

        static constexpr std::string_view Name() { return "keyword"; }
        static const ::Stencil::Flags     Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TOwner::get_arg_keyword; }
        static constexpr auto TPropertySetter() { return &TOwner::set_arg_keyword; }
    };
    static constexpr ::Stencil::DataType Type() { return ::Stencil::DataType::Object; }
    static constexpr std::string_view    Name() { return "RemoveKeyword"; }
    static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

    using ThisType = ComplexWebService::Content_RemoveKeyword_Args;

    using Handler = ::ReflectionServices::ReflectedStructHandler<ComplexWebService::Content_RemoveKeyword_Args
                                                                     Traits_arg_keyword
                                                                 >;
};

template <>
Stencil::Comparator<ComplexWebService::Content_RemoveKeyword_Args,
                    ComplexWebService::Content_RemoveKeyword_Args>{
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2){
        return true
               && Stencil::AreEqual(obj1.get_carg_keyword(), obj2.get_carg_keyword())
        ;
}
}
;
template <> struct Stencil::InterfaceTraits<ComplexWebService::Content>
{
    struct ApiTraits_AddKeyword
    {
        using TOwner = ComplexWebService::Content;
        static const ::Stencil::Flags     Flags() { return {}; }
        static constexpr std::string_view Name() { return "AddKeyword"; }
        static constexpr bool             Static = false;
    };
    struct ApiTraits_RemoveKeyword
    {
        using TOwner = ComplexWebService::Content;
        static const ::Stencil::Flags     Flags() { return {}; }
        static constexpr std::string_view Name() { return "RemoveKeyword"; }
        static constexpr bool             Static = false;
    };
    using Apis = std::tuple<
        ApiTraits_AddKeyword
,        ApiTraits_RemoveKeyword
        >;

    using Objects = std::tuple<
        >;
};

template <> struct Stencil::InterfaceApiTraits<Stencil::InterfaceTraits<ComplexWebService::Content>::ApiTraits_AddKeyword>
{
    using ArgsStruct = ComplexWebService::Content_AddKeyword_Args;
    static constexpr bool             IsStatic() { return false; }
    static constexpr std::string_view Name() { return "AddKeyword"; }

    static auto Invoke(ArgsStruct& args)
    {
        return args.instance->AddKeyword(
            args.get_arg_keyword()
        );
    }
};

template <> struct Stencil::InterfaceApiTraits<Stencil::InterfaceTraits<ComplexWebService::Content>::ApiTraits_RemoveKeyword>
{
    using ArgsStruct = ComplexWebService::Content_RemoveKeyword_Args;
    static constexpr bool             IsStatic() { return false; }
    static constexpr std::string_view Name() { return "RemoveKeyword"; }

    static auto Invoke(ArgsStruct& args)
    {
        return args.instance->RemoveKeyword(
            args.get_arg_keyword()
        );
    }
};

#if ((defined STENCIL_USING_WEBSERVICE) and (STENCIL_USING_WEBSERVICE > 0))
template <> struct Stencil::WebServiceHandlerTraits<ComplexWebService::Content>
{
    static constexpr const std::string_view Url() { return std::string_view("Content"); }
    // Name to api enum
    // Name to funcptr mapping
    // Name to argstruct mapping
    // Invoke api for each arg struct
    static void HandleRequest(ComplexWebService::Content& obj,
                              httplib::Request const&               req,
                              httplib::Response&                    res,
                              std::string_view const&               path);
};
#endif

template <> struct Stencil::TypeTraits<ComplexWebService::Search_SearchByKeywords_Args>
{
    struct Traits_arg_keywords
    {
        using TOwner     = ComplexWebService::Search_SearchByKeywords_Args;
        using TFieldType = std::vector<shared_tree<shared_string>>;

        static constexpr std::string_view Name() { return "keywords"; }
        static const ::Stencil::Flags     Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TOwner::get_arg_keywords; }
        static constexpr auto TPropertySetter() { return &TOwner::set_arg_keywords; }
    };
    static constexpr ::Stencil::DataType Type() { return ::Stencil::DataType::Object; }
    static constexpr std::string_view    Name() { return "SearchByKeywords"; }
    static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

    using ThisType = ComplexWebService::Search_SearchByKeywords_Args;

    using Handler = ::ReflectionServices::ReflectedStructHandler<ComplexWebService::Search_SearchByKeywords_Args
                                                                     Traits_arg_keywords
                                                                 >;
};

template <>
Stencil::Comparator<ComplexWebService::Search_SearchByKeywords_Args,
                    ComplexWebService::Search_SearchByKeywords_Args>{
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2){
        return true
               && Stencil::AreEqual(obj1.get_carg_keywords(), obj2.get_carg_keywords())
        ;
}
}
;
template <> struct Stencil::TypeTraits<ComplexWebService::Search_SearchByDate_Args>
{
    struct Traits_arg_startDate
    {
        using TOwner     = ComplexWebService::Search_SearchByDate_Args;
        using TFieldType = int64_t;

        static constexpr std::string_view Name() { return "startDate"; }
        static const ::Stencil::Flags     Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TOwner::get_arg_startDate; }
        static constexpr auto TPropertySetter() { return &TOwner::set_arg_startDate; }
    };
    struct Traits_arg_endDate
    {
        using TOwner     = ComplexWebService::Search_SearchByDate_Args;
        using TFieldType = int64_t;

        static constexpr std::string_view Name() { return "endDate"; }
        static const ::Stencil::Flags     Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TOwner::get_arg_endDate; }
        static constexpr auto TPropertySetter() { return &TOwner::set_arg_endDate; }
    };
    static constexpr ::Stencil::DataType Type() { return ::Stencil::DataType::Object; }
    static constexpr std::string_view    Name() { return "SearchByDate"; }
    static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

    using ThisType = ComplexWebService::Search_SearchByDate_Args;

    using Handler = ::ReflectionServices::ReflectedStructHandler<ComplexWebService::Search_SearchByDate_Args
                                                                     Traits_arg_startDate
,                                                                     Traits_arg_endDate
                                                                 >;
};

template <>
Stencil::Comparator<ComplexWebService::Search_SearchByDate_Args,
                    ComplexWebService::Search_SearchByDate_Args>{
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2){
        return true
               && Stencil::AreEqual(obj1.get_carg_startDate(), obj2.get_carg_startDate())
               && Stencil::AreEqual(obj1.get_carg_endDate(), obj2.get_carg_endDate())
        ;
}
}
;
template <> struct Stencil::InterfaceTraits<ComplexWebService::Search>
{
    struct ApiTraits_SearchByKeywords
    {
        using TOwner = ComplexWebService::Search;
        static const ::Stencil::Flags     Flags() { return {}; }
        static constexpr std::string_view Name() { return "SearchByKeywords"; }
        static constexpr bool             Static = false;
    };
    struct ApiTraits_SearchByDate
    {
        using TOwner = ComplexWebService::Search;
        static const ::Stencil::Flags     Flags() { return {}; }
        static constexpr std::string_view Name() { return "SearchByDate"; }
        static constexpr bool             Static = false;
    };
    using Apis = std::tuple<
        ApiTraits_SearchByKeywords
,        ApiTraits_SearchByDate
        >;

    using Objects = std::tuple<
        >;
};

template <> struct Stencil::InterfaceApiTraits<Stencil::InterfaceTraits<ComplexWebService::Search>::ApiTraits_SearchByKeywords>
{
    using ArgsStruct = ComplexWebService::Search_SearchByKeywords_Args;
    static constexpr bool             IsStatic() { return false; }
    static constexpr std::string_view Name() { return "SearchByKeywords"; }

    static auto Invoke(ArgsStruct& args)
    {
        return args.instance->SearchByKeywords(
            args.get_arg_keywords()
        );
    }
};

template <> struct Stencil::InterfaceApiTraits<Stencil::InterfaceTraits<ComplexWebService::Search>::ApiTraits_SearchByDate>
{
    using ArgsStruct = ComplexWebService::Search_SearchByDate_Args;
    static constexpr bool             IsStatic() { return false; }
    static constexpr std::string_view Name() { return "SearchByDate"; }

    static auto Invoke(ArgsStruct& args)
    {
        return args.instance->SearchByDate(
            args.get_arg_startDate()
,            args.get_arg_endDate()
        );
    }
};

#if ((defined STENCIL_USING_WEBSERVICE) and (STENCIL_USING_WEBSERVICE > 0))
template <> struct Stencil::WebServiceHandlerTraits<ComplexWebService::Search>
{
    static constexpr const std::string_view Url() { return std::string_view("Search"); }
    // Name to api enum
    // Name to funcptr mapping
    // Name to argstruct mapping
    // Invoke api for each arg struct
    static void HandleRequest(ComplexWebService::Search& obj,
                              httplib::Request const&               req,
                              httplib::Response&                    res,
                              std::string_view const&               path);
};
#endif

template <> struct Stencil::TypeTraits<ComplexWebService::Store_Upload_Args>
{
    struct Traits_arg_file
    {
        using TOwner     = ComplexWebService::Store_Upload_Args;
        using TFieldType = shared_string;

        static constexpr std::string_view Name() { return "file"; }
        static const ::Stencil::Flags     Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TOwner::get_arg_file; }
        static constexpr auto TPropertySetter() { return &TOwner::set_arg_file; }
    };
    static constexpr ::Stencil::DataType Type() { return ::Stencil::DataType::Object; }
    static constexpr std::string_view    Name() { return "Upload"; }
    static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

    using ThisType = ComplexWebService::Store_Upload_Args;

    using Handler = ::ReflectionServices::ReflectedStructHandler<ComplexWebService::Store_Upload_Args
                                                                     Traits_arg_file
                                                                 >;
};

template <>
Stencil::Comparator<ComplexWebService::Store_Upload_Args,
                    ComplexWebService::Store_Upload_Args>{
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2){
        return true
               && Stencil::AreEqual(obj1.get_carg_file(), obj2.get_carg_file())
        ;
}
}
;
template <> struct Stencil::TypeTraits<ComplexWebService::Store_Download_Args>
{
    struct Traits_arg_content
    {
        using TOwner     = ComplexWebService::Store_Download_Args;
        using TFieldType = long;

        static constexpr std::string_view Name() { return "content"; }
        static const ::Stencil::Flags     Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TOwner::get_arg_content; }
        static constexpr auto TPropertySetter() { return &TOwner::set_arg_content; }
    };
    static constexpr ::Stencil::DataType Type() { return ::Stencil::DataType::Object; }
    static constexpr std::string_view    Name() { return "Download"; }
    static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

    using ThisType = ComplexWebService::Store_Download_Args;

    using Handler = ::ReflectionServices::ReflectedStructHandler<ComplexWebService::Store_Download_Args
                                                                     Traits_arg_content
                                                                 >;
};

template <>
Stencil::Comparator<ComplexWebService::Store_Download_Args,
                    ComplexWebService::Store_Download_Args>{
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2){
        return true
               && Stencil::AreEqual(obj1.get_carg_content(), obj2.get_carg_content())
        ;
}
}
;
template <> struct Stencil::InterfaceTraits<ComplexWebService::Store>
{
    struct ApiTraits_Upload
    {
        using TOwner = ComplexWebService::Store;
        static const ::Stencil::Flags     Flags() { return {}; }
        static constexpr std::string_view Name() { return "Upload"; }
        static constexpr bool             Static = false;
    };
    struct ApiTraits_Download
    {
        using TOwner = ComplexWebService::Store;
        static const ::Stencil::Flags     Flags() { return {}; }
        static constexpr std::string_view Name() { return "Download"; }
        static constexpr bool             Static = false;
    };
    using Apis = std::tuple<
        ApiTraits_Upload
,        ApiTraits_Download
        >;

    using Objects = std::tuple<
        >;
};

template <> struct Stencil::InterfaceApiTraits<Stencil::InterfaceTraits<ComplexWebService::Store>::ApiTraits_Upload>
{
    using ArgsStruct = ComplexWebService::Store_Upload_Args;
    static constexpr bool             IsStatic() { return false; }
    static constexpr std::string_view Name() { return "Upload"; }

    static auto Invoke(ArgsStruct& args)
    {
        return args.instance->Upload(
            args.get_arg_file()
        );
    }
};

template <> struct Stencil::InterfaceApiTraits<Stencil::InterfaceTraits<ComplexWebService::Store>::ApiTraits_Download>
{
    using ArgsStruct = ComplexWebService::Store_Download_Args;
    static constexpr bool             IsStatic() { return false; }
    static constexpr std::string_view Name() { return "Download"; }

    static auto Invoke(ArgsStruct& args)
    {
        return args.instance->Download(
            args.get_arg_content()
        );
    }
};

#if ((defined STENCIL_USING_WEBSERVICE) and (STENCIL_USING_WEBSERVICE > 0))
template <> struct Stencil::WebServiceHandlerTraits<ComplexWebService::Store>
{
    static constexpr const std::string_view Url() { return std::string_view("Store"); }
    // Name to api enum
    // Name to funcptr mapping
    // Name to argstruct mapping
    // Invoke api for each arg struct
    static void HandleRequest(ComplexWebService::Store& obj,
                              httplib::Request const&               req,
                              httplib::Response&                    res,
                              std::string_view const&               path);
};
#endif

#endif
// SECTION END: Template specializations

// SECTION START: Inline Function Definitions
#if true

#endif
// SECTION END: Inline Function Definitions
