#pragma once
#include <stencil/stencil.h>

// SECTION START: DECLARATIONS
#if true

namespace Metadata
{
struct MapPoint;
struct GeographicalArea;
struct DigitalAssetInfo;
}    // namespace Metadata

template <> struct Stencil::TypeTraits<Metadata::MapPoint>;
template <> struct Stencil::TypeTraits<Metadata::GeographicalArea>;
template <> struct Stencil::TypeTraits<Metadata::DigitalAssetInfo>;
#endif
// SECTION END: DECLARATIONS

// SECTION START: Definitions
#if true
namespace Metadata
{
typedef shared_tree<shared_string> Keyword;
typedef shared_tree<shared_string> Entity;
typedef std::vector<::Metadata::MapPoint> AreaPolygon;
typedef shared_tree<::Metadata::GeographicalArea> GeographicalLocation;
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
    std::vector<::Metadata::MapPoint> areaPolygon = {};
};
struct DigitalAssetInfo :
    public Stencil::StructT<DigitalAssetInfo>
{
    int32_t id = {};
    std::vector<shared_tree<shared_string>> keywords = {};
    shared_tree<::Metadata::GeographicalArea> location = {};
    int64_t md5sum = {};
    shared_string thumbnailBlob = {};
    shared_string fileUrl = {};
};
}    // namespace Metadata
#endif

// SECTION END: Definitions

// SECTION START: Template specializations
#if true

// SECTION:

template <> struct Stencil::TypeTraits<Metadata::MapPoint>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<Metadata::MapPoint>
{
    enum class Fields
    {
        Invalid,
        Field_latitude
,        Field_longitude
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<Metadata::MapPoint>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<Metadata::MapPoint>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "latitude"
,        "longitude"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<Metadata::MapPoint>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<Metadata::MapPoint>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<Metadata::MapPoint> : Stencil::TransactionT<Metadata::MapPoint>
{
    using TData  = Metadata::MapPoint;
    using Fields = Stencil::TypeTraitsForIndexable<Metadata::MapPoint>::Fields;

    Transaction<int64_t> _subtracker_latitude;
    Transaction<int64_t> _subtracker_longitude;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<Metadata::MapPoint>(ptr)
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

        Stencil::TransactionT<Metadata::MapPoint>::Flush_();
    }
};

template <> struct Stencil::Visitor<Metadata::MapPoint> : Stencil::VisitorT<Metadata::MapPoint>
{
    using TData  = Metadata::MapPoint;
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

template <> struct Stencil::TypeTraits<Metadata::GeographicalArea>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<Metadata::GeographicalArea>
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

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<Metadata::GeographicalArea>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<Metadata::GeographicalArea>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "type"
,        "name"
,        "areaPolygon"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<Metadata::GeographicalArea>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<Metadata::GeographicalArea>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<Metadata::GeographicalArea> : Stencil::TransactionT<Metadata::GeographicalArea>
{
    using TData  = Metadata::GeographicalArea;
    using Fields = Stencil::TypeTraitsForIndexable<Metadata::GeographicalArea>::Fields;

    Transaction<shared_string> _subtracker_type;
    Transaction<shared_string> _subtracker_name;
    Transaction<std::vector<::Metadata::MapPoint>> _subtracker_areaPolygon;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<Metadata::GeographicalArea>(ptr)
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

    void set_areaPolygon(std::vector<::Metadata::MapPoint>&& val)
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

        Stencil::TransactionT<Metadata::GeographicalArea>::Flush_();
    }
};

template <> struct Stencil::Visitor<Metadata::GeographicalArea> : Stencil::VisitorT<Metadata::GeographicalArea>
{
    using TData  = Metadata::GeographicalArea;
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

template <> struct Stencil::TypeTraits<Metadata::DigitalAssetInfo>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<Metadata::DigitalAssetInfo>
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

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<Metadata::DigitalAssetInfo>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<Metadata::DigitalAssetInfo>::Fields;

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

    static Stencil::TypeTraitsForIndexable<Metadata::DigitalAssetInfo>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<Metadata::DigitalAssetInfo>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<Metadata::DigitalAssetInfo> : Stencil::TransactionT<Metadata::DigitalAssetInfo>
{
    using TData  = Metadata::DigitalAssetInfo;
    using Fields = Stencil::TypeTraitsForIndexable<Metadata::DigitalAssetInfo>::Fields;

    Transaction<int32_t> _subtracker_id;
    Transaction<std::vector<shared_tree<shared_string>>> _subtracker_keywords;
    Transaction<shared_tree<::Metadata::GeographicalArea>> _subtracker_location;
    Transaction<int64_t> _subtracker_md5sum;
    Transaction<shared_string> _subtracker_thumbnailBlob;
    Transaction<shared_string> _subtracker_fileUrl;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<Metadata::DigitalAssetInfo>(ptr)
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

    void set_location(shared_tree<::Metadata::GeographicalArea>&& val)
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

        Stencil::TransactionT<Metadata::DigitalAssetInfo>::Flush_();
    }
};

template <> struct Stencil::Visitor<Metadata::DigitalAssetInfo> : Stencil::VisitorT<Metadata::DigitalAssetInfo>
{
    using TData  = Metadata::DigitalAssetInfo;
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

#endif
// SECTION END: Template specializations

// SECTION START: Inline Function Definitions
#if true

#endif
// SECTION END: Inline Function Definitions
