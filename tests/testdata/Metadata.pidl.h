#pragma once
#include <DataModel.h>
namespace Metadata
{
namespace MapPoint
{
struct Data;
}
namespace GeographicalArea
{
struct Data;
}
namespace DigitalAssetInfo
{
struct Data;
}
typedef shared_tree<shared_string>              Keyword;
typedef shared_tree<shared_string>              Entity;
typedef std::vector<MapPoint::Data>             AreaPolygon;
typedef shared_tree<GeographicalArea::Data>     GeographicalLocation;
typedef std::vector<shared_tree<shared_string>> Keywords;
typedef std::vector<shared_tree<shared_string>> Entities;
namespace MapPoint
{

struct Data : public ReflectionBase::ObjMarker
{
    /*template <typename...TArgs> Data(TArgs&& ... args)
    {
        ReflectionBase::Construct<Data>(this, std::forward<TArgs>(args)...);
    }*/

    enum class FieldIndex
    {
        Invalid,
        latitude,
        longitude
    };

    static constexpr std::string_view FieldAttributeValue(FieldIndex index, const std::string_view& key)
    {
        switch (index)
        {
        case FieldIndex::latitude: return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::longitude: return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::Invalid: break;

        default: break;
        }
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    private:
    int64_t _latitude = {};

    public:
    int64_t&       latitude() { return _latitude; }
    const int64_t& latitude() const { return _latitude; }
    void           latitude(int64_t&& val) { _latitude = std::move(val); }

    int64_t& get_latitude()
    {
        return _latitude;
        ;
    }
    void set_latitude(int64_t&& val) { _latitude = std::move(val); }

    private:
    int64_t _longitude = {};

    public:
    int64_t&       longitude() { return _longitude; }
    const int64_t& longitude() const { return _longitude; }
    void           longitude(int64_t&& val) { _longitude = std::move(val); }

    int64_t& get_longitude()
    {
        return _longitude;
        ;
    }
    void set_longitude(int64_t&& val) { _longitude = std::move(val); }
};

}    // namespace MapPoint
namespace GeographicalArea
{

struct Data : public ReflectionBase::ObjMarker
{
    /*template <typename...TArgs> Data(TArgs&& ... args)
    {
        ReflectionBase::Construct<Data>(this, std::forward<TArgs>(args)...);
    }*/

    enum class FieldIndex
    {
        Invalid,
        type,
        name,
        areaPolygon
    };

    static constexpr std::string_view FieldAttributeValue(FieldIndex index, const std::string_view& key)
    {
        switch (index)
        {
        case FieldIndex::type: return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::name: return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::areaPolygon: return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::Invalid: break;

        default: break;
        }
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    private:
    shared_string _type = {};

    public:
    shared_string&       type() { return _type; }
    const shared_string& type() const { return _type; }
    void                 type(shared_string&& val) { _type = std::move(val); }

    shared_string& get_type()
    {
        return _type;
        ;
    }
    void set_type(shared_string&& val) { _type = std::move(val); }

    private:
    shared_string _name = {};

    public:
    shared_string&       name() { return _name; }
    const shared_string& name() const { return _name; }
    void                 name(shared_string&& val) { _name = std::move(val); }

    shared_string& get_name()
    {
        return _name;
        ;
    }
    void set_name(shared_string&& val) { _name = std::move(val); }

    private:
    std::vector<MapPoint::Data> _areaPolygon = {};

    public:
    std::vector<MapPoint::Data>&       areaPolygon() { return _areaPolygon; }
    const std::vector<MapPoint::Data>& areaPolygon() const { return _areaPolygon; }
    void                               areaPolygon(std::vector<MapPoint::Data>&& val) { _areaPolygon = std::move(val); }

    std::vector<MapPoint::Data>& get_areaPolygon()
    {
        return _areaPolygon;
        ;
    }
    void set_areaPolygon(std::vector<MapPoint::Data>&& val) { _areaPolygon = std::move(val); }
};

}    // namespace GeographicalArea
namespace DigitalAssetInfo
{

struct Data : public ReflectionBase::ObjMarker
{
    /*template <typename...TArgs> Data(TArgs&& ... args)
    {
        ReflectionBase::Construct<Data>(this, std::forward<TArgs>(args)...);
    }*/

    enum class FieldIndex
    {
        Invalid,
        id,
        keywords,
        location,
        md5sum,
        thumbnailBlob,
        fileUrl
    };

    static constexpr std::string_view FieldAttributeValue(FieldIndex index, const std::string_view& key)
    {
        switch (index)
        {
        case FieldIndex::id: return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::keywords: return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::location: return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::md5sum: return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::thumbnailBlob: return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::fileUrl: return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::Invalid: break;

        default: break;
        }
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    private:
    int32_t _id = {};

    public:
    int32_t&       id() { return _id; }
    const int32_t& id() const { return _id; }
    void           id(int32_t&& val) { _id = std::move(val); }

    int32_t& get_id()
    {
        return _id;
        ;
    }
    void set_id(int32_t&& val) { _id = std::move(val); }

    private:
    std::vector<shared_tree<shared_string>> _keywords = {};

    public:
    std::vector<shared_tree<shared_string>>&       keywords() { return _keywords; }
    const std::vector<shared_tree<shared_string>>& keywords() const { return _keywords; }
    void                                           keywords(std::vector<shared_tree<shared_string>>&& val) { _keywords = std::move(val); }

    std::vector<shared_tree<shared_string>>& get_keywords()
    {
        return _keywords;
        ;
    }
    void set_keywords(std::vector<shared_tree<shared_string>>&& val) { _keywords = std::move(val); }

    private:
    shared_tree<GeographicalArea::Data> _location = {};

    public:
    shared_tree<GeographicalArea::Data>&       location() { return _location; }
    const shared_tree<GeographicalArea::Data>& location() const { return _location; }
    void                                       location(shared_tree<GeographicalArea::Data>&& val) { _location = std::move(val); }

    shared_tree<GeographicalArea::Data>& get_location()
    {
        return _location;
        ;
    }
    void set_location(shared_tree<GeographicalArea::Data>&& val) { _location = std::move(val); }

    private:
    int64_t _md5sum = {};

    public:
    int64_t&       md5sum() { return _md5sum; }
    const int64_t& md5sum() const { return _md5sum; }
    void           md5sum(int64_t&& val) { _md5sum = std::move(val); }

    int64_t& get_md5sum()
    {
        return _md5sum;
        ;
    }
    void set_md5sum(int64_t&& val) { _md5sum = std::move(val); }

    private:
    shared_string _thumbnailBlob = {};

    public:
    shared_string&       thumbnailBlob() { return _thumbnailBlob; }
    const shared_string& thumbnailBlob() const { return _thumbnailBlob; }
    void                 thumbnailBlob(shared_string&& val) { _thumbnailBlob = std::move(val); }

    shared_string& get_thumbnailBlob()
    {
        return _thumbnailBlob;
        ;
    }
    void set_thumbnailBlob(shared_string&& val) { _thumbnailBlob = std::move(val); }

    private:
    shared_string _fileUrl = {};

    public:
    shared_string&       fileUrl() { return _fileUrl; }
    const shared_string& fileUrl() const { return _fileUrl; }
    void                 fileUrl(shared_string&& val) { _fileUrl = std::move(val); }

    shared_string& get_fileUrl()
    {
        return _fileUrl;
        ;
    }
    void set_fileUrl(shared_string&& val) { _fileUrl = std::move(val); }
};

}    // namespace DigitalAssetInfo
typedef shared_tree<shared_string>              Keyword;
typedef shared_tree<shared_string>              Entity;
typedef std::vector<MapPoint::Data>             AreaPolygon;
typedef shared_tree<GeographicalArea::Data>     GeographicalLocation;
typedef std::vector<shared_tree<shared_string>> Keywords;
typedef std::vector<shared_tree<shared_string>> Entities;
}    // namespace Metadata

template <> struct ReflectionBase::TypeTraits<Metadata::MapPoint::Data&>
{
    struct Traits_latitude
    {
        using TOwner = Metadata::MapPoint::Data;

        static constexpr std::string_view Name() { return "latitude"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_latitude; }
        static constexpr auto TPropertySetter() { return &TOwner::set_latitude; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::latitude, key);
        }

        static const ::ReflectionBase::Flags Flags() { return ::ReflectionBase::Flags{::ReflectionBase::Flag::Max}; }
    };
    struct Traits_longitude
    {
        using TOwner = Metadata::MapPoint::Data;

        static constexpr std::string_view Name() { return "longitude"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_longitude; }
        static constexpr auto TPropertySetter() { return &TOwner::set_longitude; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::longitude, key);
        }

        static const ::ReflectionBase::Flags Flags() { return ::ReflectionBase::Flags{::ReflectionBase::Flag::Max}; }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "MapPoint"; }
    static constexpr std::string_view AttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

    using Handler = ::ReflectionServices::ReflectedStructHandler<Metadata::MapPoint::Data, Traits_latitude, Traits_longitude>;
};
template <> struct ReflectionBase::TypeTraits<Metadata::GeographicalArea::Data&>
{
    struct Traits_type
    {
        using TOwner = Metadata::GeographicalArea::Data;

        static constexpr std::string_view Name() { return "type"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_type; }
        static constexpr auto TPropertySetter() { return &TOwner::set_type; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::type, key);
        }

        static const ::ReflectionBase::Flags Flags() { return ::ReflectionBase::Flags{::ReflectionBase::Flag::Max}; }
    };
    struct Traits_name
    {
        using TOwner = Metadata::GeographicalArea::Data;

        static constexpr std::string_view Name() { return "name"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_name; }
        static constexpr auto TPropertySetter() { return &TOwner::set_name; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::name, key);
        }

        static const ::ReflectionBase::Flags Flags() { return ::ReflectionBase::Flags{::ReflectionBase::Flag::Max}; }
    };
    struct Traits_areaPolygon
    {
        using TOwner = Metadata::GeographicalArea::Data;

        static constexpr std::string_view Name() { return "areaPolygon"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_areaPolygon; }
        static constexpr auto TPropertySetter() { return &TOwner::set_areaPolygon; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::areaPolygon, key);
        }

        static const ::ReflectionBase::Flags Flags() { return ::ReflectionBase::Flags{::ReflectionBase::Flag::Max}; }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "GeographicalArea"; }
    static constexpr std::string_view AttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

    using Handler
        = ::ReflectionServices::ReflectedStructHandler<Metadata::GeographicalArea::Data, Traits_type, Traits_name, Traits_areaPolygon>;
};
template <> struct ReflectionBase::TypeTraits<Metadata::DigitalAssetInfo::Data&>
{
    struct Traits_id
    {
        using TOwner = Metadata::DigitalAssetInfo::Data;

        static constexpr std::string_view Name() { return "id"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_id; }
        static constexpr auto TPropertySetter() { return &TOwner::set_id; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::id, key);
        }

        static const ::ReflectionBase::Flags Flags() { return ::ReflectionBase::Flags{::ReflectionBase::Flag::Max}; }
    };
    struct Traits_keywords
    {
        using TOwner = Metadata::DigitalAssetInfo::Data;

        static constexpr std::string_view Name() { return "keywords"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_keywords; }
        static constexpr auto TPropertySetter() { return &TOwner::set_keywords; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::keywords, key);
        }

        static const ::ReflectionBase::Flags Flags() { return ::ReflectionBase::Flags{::ReflectionBase::Flag::Max}; }
    };
    struct Traits_location
    {
        using TOwner = Metadata::DigitalAssetInfo::Data;

        static constexpr std::string_view Name() { return "location"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_location; }
        static constexpr auto TPropertySetter() { return &TOwner::set_location; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::location, key);
        }

        static const ::ReflectionBase::Flags Flags() { return ::ReflectionBase::Flags{::ReflectionBase::Flag::Max}; }
    };
    struct Traits_md5sum
    {
        using TOwner = Metadata::DigitalAssetInfo::Data;

        static constexpr std::string_view Name() { return "md5sum"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_md5sum; }
        static constexpr auto TPropertySetter() { return &TOwner::set_md5sum; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::md5sum, key);
        }

        static const ::ReflectionBase::Flags Flags() { return ::ReflectionBase::Flags{::ReflectionBase::Flag::Max}; }
    };
    struct Traits_thumbnailBlob
    {
        using TOwner = Metadata::DigitalAssetInfo::Data;

        static constexpr std::string_view Name() { return "thumbnailBlob"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_thumbnailBlob; }
        static constexpr auto TPropertySetter() { return &TOwner::set_thumbnailBlob; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::thumbnailBlob, key);
        }

        static const ::ReflectionBase::Flags Flags() { return ::ReflectionBase::Flags{::ReflectionBase::Flag::Max}; }
    };
    struct Traits_fileUrl
    {
        using TOwner = Metadata::DigitalAssetInfo::Data;

        static constexpr std::string_view Name() { return "fileUrl"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_fileUrl; }
        static constexpr auto TPropertySetter() { return &TOwner::set_fileUrl; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::fileUrl, key);
        }

        static const ::ReflectionBase::Flags Flags() { return ::ReflectionBase::Flags{::ReflectionBase::Flag::Max}; }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "DigitalAssetInfo"; }
    static constexpr std::string_view AttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

    using Handler = ::ReflectionServices::ReflectedStructHandler<Metadata::DigitalAssetInfo::Data,
                                                                 Traits_id,
                                                                 Traits_keywords,
                                                                 Traits_location,
                                                                 Traits_md5sum,
                                                                 Traits_thumbnailBlob,
                                                                 Traits_fileUrl>;
};
