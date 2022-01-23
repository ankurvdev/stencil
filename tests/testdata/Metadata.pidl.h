#pragma once
#include <stencil/stencil.h>

// SECTION START: DECLARATIONS
#if true
namespace Metadata::MapPoint
{
struct Data;
}
template <> struct Stencil::TypeTraits<Metadata::MapPoint::Data&>;
namespace Metadata::GeographicalArea
{
struct Data;
}
template <> struct Stencil::TypeTraits<Metadata::GeographicalArea::Data&>;
namespace Metadata::DigitalAssetInfo
{
struct Data;
}
template <> struct Stencil::TypeTraits<Metadata::DigitalAssetInfo::Data&>;
#endif
// SECTION END: DECLARATIONS

// SECTION START: Definitions
#if true
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
typedef shared_tree<shared_string> Keyword;
typedef shared_tree<shared_string> Entity;
typedef std::vector<::Metadata::MapPoint::Data> AreaPolygon;
typedef shared_tree<::Metadata::GeographicalArea::Data> GeographicalLocation;
typedef std::vector<shared_tree<shared_string>> Keywords;
typedef std::vector<shared_tree<shared_string>> Entities;
namespace MapPoint
{

struct Data :
    public ReflectionBase::ObjMarker
{
    /*template <typename...TArgs> Data(TArgs&& ... args)
    {
        ReflectionBase::Construct<Data>(this, std::forward<TArgs>(args)...);
    }*/

    enum class FieldIndex
    {
        Invalid,
        latitude
,        longitude
    };

    static constexpr size_t FieldCount()
    {
        return 0u
               + 1u
               + 1u
            ;
    }

    static constexpr std::string_view FieldAttributeValue(FieldIndex index, const std::string_view& key)
    {
        switch (index)
        {
        case FieldIndex::latitude:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::longitude:
            return ::ReflectionServices::EmptyAttributeValue(key);
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
    void                            latitude(int64_t&& val) { _latitude = std::move(val); }
    int64_t&       get_latitude() { return _latitude; }

    bool isset_latitude() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::latitude); }

    void set_latitude(int64_t&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::latitude, _latitude, val);
        _latitude = std::move(val);
    }
    private:
    int64_t _longitude = {};

    public:
    int64_t&       longitude() { return _longitude; }
    const int64_t& longitude() const { return _longitude; }
    void                            longitude(int64_t&& val) { _longitude = std::move(val); }
    int64_t&       get_longitude() { return _longitude; }

    bool isset_longitude() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::longitude); }

    void set_longitude(int64_t&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::longitude, _longitude, val);
        _longitude = std::move(val);
    }
};

}    // namespace MapPoint
namespace GeographicalArea
{

struct Data :
    public ReflectionBase::ObjMarker
{
    /*template <typename...TArgs> Data(TArgs&& ... args)
    {
        ReflectionBase::Construct<Data>(this, std::forward<TArgs>(args)...);
    }*/

    enum class FieldIndex
    {
        Invalid,
        type
,        name
,        areaPolygon
    };

    static constexpr size_t FieldCount()
    {
        return 0u
               + 1u
               + 1u
               + 1u
            ;
    }

    static constexpr std::string_view FieldAttributeValue(FieldIndex index, const std::string_view& key)
    {
        switch (index)
        {
        case FieldIndex::type:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::name:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::areaPolygon:
            return ::ReflectionServices::EmptyAttributeValue(key);
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
    void                            type(shared_string&& val) { _type = std::move(val); }
    shared_string&       get_type() { return _type; }

    bool isset_type() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::type); }

    void set_type(shared_string&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::type, _type, val);
        _type = std::move(val);
    }
    private:
    shared_string _name = {};

    public:
    shared_string&       name() { return _name; }
    const shared_string& name() const { return _name; }
    void                            name(shared_string&& val) { _name = std::move(val); }
    shared_string&       get_name() { return _name; }

    bool isset_name() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::name); }

    void set_name(shared_string&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::name, _name, val);
        _name = std::move(val);
    }
    private:
    std::vector<::Metadata::MapPoint::Data> _areaPolygon = {};

    public:
    std::vector<::Metadata::MapPoint::Data>&       areaPolygon() { return _areaPolygon; }
    const std::vector<::Metadata::MapPoint::Data>& areaPolygon() const { return _areaPolygon; }
    void                            areaPolygon(std::vector<::Metadata::MapPoint::Data>&& val) { _areaPolygon = std::move(val); }
    std::vector<::Metadata::MapPoint::Data>&       get_areaPolygon() { return _areaPolygon; }

    bool isset_areaPolygon() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::areaPolygon); }

    void set_areaPolygon(std::vector<::Metadata::MapPoint::Data>&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::areaPolygon, _areaPolygon, val);
        _areaPolygon = std::move(val);
    }
};

}    // namespace GeographicalArea
namespace DigitalAssetInfo
{

struct Data :
    public ReflectionBase::ObjMarker
{
    /*template <typename...TArgs> Data(TArgs&& ... args)
    {
        ReflectionBase::Construct<Data>(this, std::forward<TArgs>(args)...);
    }*/

    enum class FieldIndex
    {
        Invalid,
        id
,        keywords
,        location
,        md5sum
,        thumbnailBlob
,        fileUrl
    };

    static constexpr size_t FieldCount()
    {
        return 0u
               + 1u
               + 1u
               + 1u
               + 1u
               + 1u
               + 1u
            ;
    }

    static constexpr std::string_view FieldAttributeValue(FieldIndex index, const std::string_view& key)
    {
        switch (index)
        {
        case FieldIndex::id:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::keywords:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::location:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::md5sum:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::thumbnailBlob:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::fileUrl:
            return ::ReflectionServices::EmptyAttributeValue(key);
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
    void                            id(int32_t&& val) { _id = std::move(val); }
    int32_t&       get_id() { return _id; }

    bool isset_id() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::id); }

    void set_id(int32_t&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::id, _id, val);
        _id = std::move(val);
    }
    private:
    std::vector<shared_tree<shared_string>> _keywords = {};

    public:
    std::vector<shared_tree<shared_string>>&       keywords() { return _keywords; }
    const std::vector<shared_tree<shared_string>>& keywords() const { return _keywords; }
    void                            keywords(std::vector<shared_tree<shared_string>>&& val) { _keywords = std::move(val); }
    std::vector<shared_tree<shared_string>>&       get_keywords() { return _keywords; }

    bool isset_keywords() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::keywords); }

    void set_keywords(std::vector<shared_tree<shared_string>>&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::keywords, _keywords, val);
        _keywords = std::move(val);
    }
    private:
    shared_tree<::Metadata::GeographicalArea::Data> _location = {};

    public:
    shared_tree<::Metadata::GeographicalArea::Data>&       location() { return _location; }
    const shared_tree<::Metadata::GeographicalArea::Data>& location() const { return _location; }
    void                            location(shared_tree<::Metadata::GeographicalArea::Data>&& val) { _location = std::move(val); }
    shared_tree<::Metadata::GeographicalArea::Data>&       get_location() { return _location; }

    bool isset_location() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::location); }

    void set_location(shared_tree<::Metadata::GeographicalArea::Data>&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::location, _location, val);
        _location = std::move(val);
    }
    private:
    int64_t _md5sum = {};

    public:
    int64_t&       md5sum() { return _md5sum; }
    const int64_t& md5sum() const { return _md5sum; }
    void                            md5sum(int64_t&& val) { _md5sum = std::move(val); }
    int64_t&       get_md5sum() { return _md5sum; }

    bool isset_md5sum() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::md5sum); }

    void set_md5sum(int64_t&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::md5sum, _md5sum, val);
        _md5sum = std::move(val);
    }
    private:
    shared_string _thumbnailBlob = {};

    public:
    shared_string&       thumbnailBlob() { return _thumbnailBlob; }
    const shared_string& thumbnailBlob() const { return _thumbnailBlob; }
    void                            thumbnailBlob(shared_string&& val) { _thumbnailBlob = std::move(val); }
    shared_string&       get_thumbnailBlob() { return _thumbnailBlob; }

    bool isset_thumbnailBlob() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::thumbnailBlob); }

    void set_thumbnailBlob(shared_string&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::thumbnailBlob, _thumbnailBlob, val);
        _thumbnailBlob = std::move(val);
    }
    private:
    shared_string _fileUrl = {};

    public:
    shared_string&       fileUrl() { return _fileUrl; }
    const shared_string& fileUrl() const { return _fileUrl; }
    void                            fileUrl(shared_string&& val) { _fileUrl = std::move(val); }
    shared_string&       get_fileUrl() { return _fileUrl; }

    bool isset_fileUrl() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::fileUrl); }

    void set_fileUrl(shared_string&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::fileUrl, _fileUrl, val);
        _fileUrl = std::move(val);
    }
};

}    // namespace DigitalAssetInfo
typedef shared_tree<shared_string> Keyword;
typedef shared_tree<shared_string> Entity;
typedef std::vector<::Metadata::MapPoint::Data> AreaPolygon;
typedef shared_tree<::Metadata::GeographicalArea::Data> GeographicalLocation;
typedef std::vector<shared_tree<shared_string>> Keywords;
typedef std::vector<shared_tree<shared_string>> Entities;
}    // namespace Metadata
#endif
// SECTION END: Definitions

// SECTION START: Template specializations
#if true

// SECTION:
template <> struct Stencil::TypeTraits<Metadata::MapPoint::Data&>
{
    struct Traits_latitude
    {
        using TOwner     = Metadata::MapPoint::Data;
        using TFieldType = int64_t;

        static constexpr std::string_view Name() { return "latitude"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_latitude; }
        static constexpr auto TPropertySetter() { return &TOwner::set_latitude; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::latitude, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_longitude
    {
        using TOwner     = Metadata::MapPoint::Data;
        using TFieldType = int64_t;

        static constexpr std::string_view Name() { return "longitude"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_longitude; }
        static constexpr auto TPropertySetter() { return &TOwner::set_longitude; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::longitude, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    static constexpr ::Stencil::DataType Type() { return ::Stencil::DataType::Object; }
    static constexpr std::string_view           Name() { return "MapPoint"; }
    static constexpr std::string_view           AttributeValue(const std::string_view& key)
    {
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    using ThisType = Metadata::MapPoint::Data;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && ReflectionBase::AreEqual(obj1.latitude(), obj2.latitude())
               && ReflectionBase::AreEqual(obj1.longitude(), obj2.longitude())
            ;
    }

    using Handler = ::ReflectionServices::ReflectedStructHandler<Metadata::MapPoint::Data,
                                                                 Traits_latitude
,                                                                 Traits_longitude
                                                                 >;
};

template <>
struct Stencil::Transaction<Metadata::MapPoint::Data> : Stencil::TransactionT<Metadata::MapPoint::Data>
{
    using TData = Metadata::MapPoint::Data;

    Transaction<int64_t> _subtracker_latitude;
    Transaction<int64_t> _subtracker_longitude;
    DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<Metadata::MapPoint::Data>(ptr)
        ,
        _subtracker_latitude(Obj().latitude())
        ,
        _subtracker_longitude(Obj().longitude())
    {}

    auto& latitude()
    {
        MarkFieldEdited_(TData::FieldIndex::latitude);
        return _subtracker_latitude;
    }
    auto& longitude()
    {
        MarkFieldEdited_(TData::FieldIndex::longitude);
        return _subtracker_longitude;
    }
    void set_latitude(int64_t&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::latitude, Obj().latitude(), val);
        Obj().set_latitude(std::move(val));
    }

    void set_longitude(int64_t&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::longitude, Obj().longitude(), val);
        Obj().set_longitude(std::move(val));
    }

    template <typename TLambda> auto Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::latitude: return lambda("latitude", latitude()); return;
        case TData::FieldIndex::longitude: return lambda("longitude", longitude()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "latitude") { return lambda(TData::FieldIndex::latitude, latitude()); }
        if (fieldName == "longitude") { return lambda(TData::FieldIndex::longitude, longitude()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("latitude", TData::FieldIndex::latitude, latitude(), Obj().latitude());
        lambda("longitude", TData::FieldIndex::longitude, longitude(), Obj().longitude());
    }

    void Flush()
    {
        latitude().Flush();

        if (IsFieldEdited(TData::FieldIndex::latitude))
        {
            if (!latitude().IsChanged()) _edittracker.reset(static_cast<uint8_t>(TData::FieldIndex::latitude));
        }

        longitude().Flush();

        if (IsFieldEdited(TData::FieldIndex::longitude))
        {
            if (!longitude().IsChanged()) _edittracker.reset(static_cast<uint8_t>(TData::FieldIndex::longitude));
        }

        Stencil::TransactionT<Metadata::MapPoint::Data>::Flush_();
    }
};

template <>
struct Stencil::Visitor<Metadata::MapPoint::Data, void> : Stencil::VisitorT<Metadata::MapPoint::Data>
{
    using TData = Metadata::MapPoint::Data;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::latitude: lambda("latitude", _ref.get().latitude()); return;
        case TData::FieldIndex::longitude: lambda("longitude", _ref.get().longitude()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::latitude: lambda("latitude", _ref.get().latitude()); return;
        case TData::FieldIndex::longitude: lambda("longitude", _ref.get().longitude()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        lambda("latitude", _ref.get().latitude());
        lambda("longitude", _ref.get().longitude());
    }

    std::reference_wrapper<TData> _ref;
};

template <>
struct Stencil::Visitor<const Metadata::MapPoint::Data, void>
    : Stencil::VisitorT<const Metadata::MapPoint::Data>
{
    using TData = Metadata::MapPoint::Data const;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::latitude: lambda("latitude", _ref.get().latitude()); return;
        case TData::FieldIndex::longitude: lambda("longitude", _ref.get().longitude()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        lambda("latitude", _ref.get().latitude());
        lambda("longitude", _ref.get().longitude());
    }

    std::reference_wrapper<TData> _ref;
};

template <> struct Stencil::TypeTraits<Metadata::GeographicalArea::Data&>
{
    struct Traits_type
    {
        using TOwner     = Metadata::GeographicalArea::Data;
        using TFieldType = shared_string;

        static constexpr std::string_view Name() { return "type"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_type; }
        static constexpr auto TPropertySetter() { return &TOwner::set_type; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::type, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_name
    {
        using TOwner     = Metadata::GeographicalArea::Data;
        using TFieldType = shared_string;

        static constexpr std::string_view Name() { return "name"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_name; }
        static constexpr auto TPropertySetter() { return &TOwner::set_name; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::name, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_areaPolygon
    {
        using TOwner     = Metadata::GeographicalArea::Data;
        using TFieldType = std::vector<::Metadata::MapPoint::Data>;

        static constexpr std::string_view Name() { return "areaPolygon"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_areaPolygon; }
        static constexpr auto TPropertySetter() { return &TOwner::set_areaPolygon; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::areaPolygon, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    static constexpr ::Stencil::DataType Type() { return ::Stencil::DataType::Object; }
    static constexpr std::string_view           Name() { return "GeographicalArea"; }
    static constexpr std::string_view           AttributeValue(const std::string_view& key)
    {
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    using ThisType = Metadata::GeographicalArea::Data;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && ReflectionBase::AreEqual(obj1.type(), obj2.type())
               && ReflectionBase::AreEqual(obj1.name(), obj2.name())
               && ReflectionBase::AreEqual(obj1.areaPolygon(), obj2.areaPolygon())
            ;
    }

    using Handler = ::ReflectionServices::ReflectedStructHandler<Metadata::GeographicalArea::Data,
                                                                 Traits_type
,                                                                 Traits_name
,                                                                 Traits_areaPolygon
                                                                 >;
};

template <>
struct Stencil::Transaction<Metadata::GeographicalArea::Data> : Stencil::TransactionT<Metadata::GeographicalArea::Data>
{
    using TData = Metadata::GeographicalArea::Data;

    Transaction<shared_string> _subtracker_type;
    Transaction<shared_string> _subtracker_name;
    Transaction<std::vector<::Metadata::MapPoint::Data>> _subtracker_areaPolygon;
    DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<Metadata::GeographicalArea::Data>(ptr)
        ,
        _subtracker_type(Obj().type())
        ,
        _subtracker_name(Obj().name())
        ,
        _subtracker_areaPolygon(Obj().areaPolygon())
    {}

    auto& type()
    {
        MarkFieldEdited_(TData::FieldIndex::type);
        return _subtracker_type;
    }
    auto& name()
    {
        MarkFieldEdited_(TData::FieldIndex::name);
        return _subtracker_name;
    }
    auto& areaPolygon()
    {
        MarkFieldEdited_(TData::FieldIndex::areaPolygon);
        return _subtracker_areaPolygon;
    }
    void set_type(shared_string&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::type, Obj().type(), val);
        Obj().set_type(std::move(val));
    }

    void set_name(shared_string&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::name, Obj().name(), val);
        Obj().set_name(std::move(val));
    }

    void set_areaPolygon(std::vector<::Metadata::MapPoint::Data>&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::areaPolygon, Obj().areaPolygon(), val);
        Obj().set_areaPolygon(std::move(val));
    }

    template <typename TLambda> auto Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::type: return lambda("type", type()); return;
        case TData::FieldIndex::name: return lambda("name", name()); return;
        case TData::FieldIndex::areaPolygon: return lambda("areaPolygon", areaPolygon()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "type") { return lambda(TData::FieldIndex::type, type()); }
        if (fieldName == "name") { return lambda(TData::FieldIndex::name, name()); }
        if (fieldName == "areaPolygon") { return lambda(TData::FieldIndex::areaPolygon, areaPolygon()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("type", TData::FieldIndex::type, type(), Obj().type());
        lambda("name", TData::FieldIndex::name, name(), Obj().name());
        lambda("areaPolygon", TData::FieldIndex::areaPolygon, areaPolygon(), Obj().areaPolygon());
    }

    void Flush()
    {
        type().Flush();

        if (IsFieldEdited(TData::FieldIndex::type))
        {
            if (!type().IsChanged()) _edittracker.reset(static_cast<uint8_t>(TData::FieldIndex::type));
        }

        name().Flush();

        if (IsFieldEdited(TData::FieldIndex::name))
        {
            if (!name().IsChanged()) _edittracker.reset(static_cast<uint8_t>(TData::FieldIndex::name));
        }

        areaPolygon().Flush();

        if (IsFieldEdited(TData::FieldIndex::areaPolygon))
        {
            if (!areaPolygon().IsChanged()) _edittracker.reset(static_cast<uint8_t>(TData::FieldIndex::areaPolygon));
        }

        Stencil::TransactionT<Metadata::GeographicalArea::Data>::Flush_();
    }
};

template <>
struct Stencil::Visitor<Metadata::GeographicalArea::Data, void> : Stencil::VisitorT<Metadata::GeographicalArea::Data>
{
    using TData = Metadata::GeographicalArea::Data;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::type: lambda("type", _ref.get().type()); return;
        case TData::FieldIndex::name: lambda("name", _ref.get().name()); return;
        case TData::FieldIndex::areaPolygon: lambda("areaPolygon", _ref.get().areaPolygon()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::type: lambda("type", _ref.get().type()); return;
        case TData::FieldIndex::name: lambda("name", _ref.get().name()); return;
        case TData::FieldIndex::areaPolygon: lambda("areaPolygon", _ref.get().areaPolygon()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        lambda("type", _ref.get().type());
        lambda("name", _ref.get().name());
        lambda("areaPolygon", _ref.get().areaPolygon());
    }

    std::reference_wrapper<TData> _ref;
};

template <>
struct Stencil::Visitor<const Metadata::GeographicalArea::Data, void>
    : Stencil::VisitorT<const Metadata::GeographicalArea::Data>
{
    using TData = Metadata::GeographicalArea::Data const;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::type: lambda("type", _ref.get().type()); return;
        case TData::FieldIndex::name: lambda("name", _ref.get().name()); return;
        case TData::FieldIndex::areaPolygon: lambda("areaPolygon", _ref.get().areaPolygon()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        lambda("type", _ref.get().type());
        lambda("name", _ref.get().name());
        lambda("areaPolygon", _ref.get().areaPolygon());
    }

    std::reference_wrapper<TData> _ref;
};

template <> struct Stencil::TypeTraits<Metadata::DigitalAssetInfo::Data&>
{
    struct Traits_id
    {
        using TOwner     = Metadata::DigitalAssetInfo::Data;
        using TFieldType = int32_t;

        static constexpr std::string_view Name() { return "id"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_id; }
        static constexpr auto TPropertySetter() { return &TOwner::set_id; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::id, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_keywords
    {
        using TOwner     = Metadata::DigitalAssetInfo::Data;
        using TFieldType = std::vector<shared_tree<shared_string>>;

        static constexpr std::string_view Name() { return "keywords"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_keywords; }
        static constexpr auto TPropertySetter() { return &TOwner::set_keywords; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::keywords, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_location
    {
        using TOwner     = Metadata::DigitalAssetInfo::Data;
        using TFieldType = shared_tree<::Metadata::GeographicalArea::Data>;

        static constexpr std::string_view Name() { return "location"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_location; }
        static constexpr auto TPropertySetter() { return &TOwner::set_location; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::location, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_md5sum
    {
        using TOwner     = Metadata::DigitalAssetInfo::Data;
        using TFieldType = int64_t;

        static constexpr std::string_view Name() { return "md5sum"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_md5sum; }
        static constexpr auto TPropertySetter() { return &TOwner::set_md5sum; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::md5sum, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_thumbnailBlob
    {
        using TOwner     = Metadata::DigitalAssetInfo::Data;
        using TFieldType = shared_string;

        static constexpr std::string_view Name() { return "thumbnailBlob"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_thumbnailBlob; }
        static constexpr auto TPropertySetter() { return &TOwner::set_thumbnailBlob; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::thumbnailBlob, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_fileUrl
    {
        using TOwner     = Metadata::DigitalAssetInfo::Data;
        using TFieldType = shared_string;

        static constexpr std::string_view Name() { return "fileUrl"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_fileUrl; }
        static constexpr auto TPropertySetter() { return &TOwner::set_fileUrl; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::fileUrl, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    static constexpr ::Stencil::DataType Type() { return ::Stencil::DataType::Object; }
    static constexpr std::string_view           Name() { return "DigitalAssetInfo"; }
    static constexpr std::string_view           AttributeValue(const std::string_view& key)
    {
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    using ThisType = Metadata::DigitalAssetInfo::Data;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && ReflectionBase::AreEqual(obj1.id(), obj2.id())
               && ReflectionBase::AreEqual(obj1.keywords(), obj2.keywords())
               && ReflectionBase::AreEqual(obj1.location(), obj2.location())
               && ReflectionBase::AreEqual(obj1.md5sum(), obj2.md5sum())
               && ReflectionBase::AreEqual(obj1.thumbnailBlob(), obj2.thumbnailBlob())
               && ReflectionBase::AreEqual(obj1.fileUrl(), obj2.fileUrl())
            ;
    }

    using Handler = ::ReflectionServices::ReflectedStructHandler<Metadata::DigitalAssetInfo::Data,
                                                                 Traits_id
,                                                                 Traits_keywords
,                                                                 Traits_location
,                                                                 Traits_md5sum
,                                                                 Traits_thumbnailBlob
,                                                                 Traits_fileUrl
                                                                 >;
};

template <>
struct Stencil::Transaction<Metadata::DigitalAssetInfo::Data> : Stencil::TransactionT<Metadata::DigitalAssetInfo::Data>
{
    using TData = Metadata::DigitalAssetInfo::Data;

    Transaction<int32_t> _subtracker_id;
    Transaction<std::vector<shared_tree<shared_string>>> _subtracker_keywords;
    Transaction<shared_tree<::Metadata::GeographicalArea::Data>> _subtracker_location;
    Transaction<int64_t> _subtracker_md5sum;
    Transaction<shared_string> _subtracker_thumbnailBlob;
    Transaction<shared_string> _subtracker_fileUrl;
    DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<Metadata::DigitalAssetInfo::Data>(ptr)
        ,
        _subtracker_id(Obj().id())
        ,
        _subtracker_keywords(Obj().keywords())
        ,
        _subtracker_location(Obj().location())
        ,
        _subtracker_md5sum(Obj().md5sum())
        ,
        _subtracker_thumbnailBlob(Obj().thumbnailBlob())
        ,
        _subtracker_fileUrl(Obj().fileUrl())
    {}

    auto& id()
    {
        MarkFieldEdited_(TData::FieldIndex::id);
        return _subtracker_id;
    }
    auto& keywords()
    {
        MarkFieldEdited_(TData::FieldIndex::keywords);
        return _subtracker_keywords;
    }
    auto& location()
    {
        MarkFieldEdited_(TData::FieldIndex::location);
        return _subtracker_location;
    }
    auto& md5sum()
    {
        MarkFieldEdited_(TData::FieldIndex::md5sum);
        return _subtracker_md5sum;
    }
    auto& thumbnailBlob()
    {
        MarkFieldEdited_(TData::FieldIndex::thumbnailBlob);
        return _subtracker_thumbnailBlob;
    }
    auto& fileUrl()
    {
        MarkFieldEdited_(TData::FieldIndex::fileUrl);
        return _subtracker_fileUrl;
    }
    void set_id(int32_t&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::id, Obj().id(), val);
        Obj().set_id(std::move(val));
    }

    void set_keywords(std::vector<shared_tree<shared_string>>&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::keywords, Obj().keywords(), val);
        Obj().set_keywords(std::move(val));
    }

    void set_location(shared_tree<::Metadata::GeographicalArea::Data>&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::location, Obj().location(), val);
        Obj().set_location(std::move(val));
    }

    void set_md5sum(int64_t&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::md5sum, Obj().md5sum(), val);
        Obj().set_md5sum(std::move(val));
    }

    void set_thumbnailBlob(shared_string&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::thumbnailBlob, Obj().thumbnailBlob(), val);
        Obj().set_thumbnailBlob(std::move(val));
    }

    void set_fileUrl(shared_string&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::fileUrl, Obj().fileUrl(), val);
        Obj().set_fileUrl(std::move(val));
    }

    template <typename TLambda> auto Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::id: return lambda("id", id()); return;
        case TData::FieldIndex::keywords: return lambda("keywords", keywords()); return;
        case TData::FieldIndex::location: return lambda("location", location()); return;
        case TData::FieldIndex::md5sum: return lambda("md5sum", md5sum()); return;
        case TData::FieldIndex::thumbnailBlob: return lambda("thumbnailBlob", thumbnailBlob()); return;
        case TData::FieldIndex::fileUrl: return lambda("fileUrl", fileUrl()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "id") { return lambda(TData::FieldIndex::id, id()); }
        if (fieldName == "keywords") { return lambda(TData::FieldIndex::keywords, keywords()); }
        if (fieldName == "location") { return lambda(TData::FieldIndex::location, location()); }
        if (fieldName == "md5sum") { return lambda(TData::FieldIndex::md5sum, md5sum()); }
        if (fieldName == "thumbnailBlob") { return lambda(TData::FieldIndex::thumbnailBlob, thumbnailBlob()); }
        if (fieldName == "fileUrl") { return lambda(TData::FieldIndex::fileUrl, fileUrl()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("id", TData::FieldIndex::id, id(), Obj().id());
        lambda("keywords", TData::FieldIndex::keywords, keywords(), Obj().keywords());
        lambda("location", TData::FieldIndex::location, location(), Obj().location());
        lambda("md5sum", TData::FieldIndex::md5sum, md5sum(), Obj().md5sum());
        lambda("thumbnailBlob", TData::FieldIndex::thumbnailBlob, thumbnailBlob(), Obj().thumbnailBlob());
        lambda("fileUrl", TData::FieldIndex::fileUrl, fileUrl(), Obj().fileUrl());
    }

    void Flush()
    {
        id().Flush();

        if (IsFieldEdited(TData::FieldIndex::id))
        {
            if (!id().IsChanged()) _edittracker.reset(static_cast<uint8_t>(TData::FieldIndex::id));
        }

        keywords().Flush();

        if (IsFieldEdited(TData::FieldIndex::keywords))
        {
            if (!keywords().IsChanged()) _edittracker.reset(static_cast<uint8_t>(TData::FieldIndex::keywords));
        }

        location().Flush();

        if (IsFieldEdited(TData::FieldIndex::location))
        {
            if (!location().IsChanged()) _edittracker.reset(static_cast<uint8_t>(TData::FieldIndex::location));
        }

        md5sum().Flush();

        if (IsFieldEdited(TData::FieldIndex::md5sum))
        {
            if (!md5sum().IsChanged()) _edittracker.reset(static_cast<uint8_t>(TData::FieldIndex::md5sum));
        }

        thumbnailBlob().Flush();

        if (IsFieldEdited(TData::FieldIndex::thumbnailBlob))
        {
            if (!thumbnailBlob().IsChanged()) _edittracker.reset(static_cast<uint8_t>(TData::FieldIndex::thumbnailBlob));
        }

        fileUrl().Flush();

        if (IsFieldEdited(TData::FieldIndex::fileUrl))
        {
            if (!fileUrl().IsChanged()) _edittracker.reset(static_cast<uint8_t>(TData::FieldIndex::fileUrl));
        }

        Stencil::TransactionT<Metadata::DigitalAssetInfo::Data>::Flush_();
    }
};

template <>
struct Stencil::Visitor<Metadata::DigitalAssetInfo::Data, void> : Stencil::VisitorT<Metadata::DigitalAssetInfo::Data>
{
    using TData = Metadata::DigitalAssetInfo::Data;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::id: lambda("id", _ref.get().id()); return;
        case TData::FieldIndex::keywords: lambda("keywords", _ref.get().keywords()); return;
        case TData::FieldIndex::location: lambda("location", _ref.get().location()); return;
        case TData::FieldIndex::md5sum: lambda("md5sum", _ref.get().md5sum()); return;
        case TData::FieldIndex::thumbnailBlob: lambda("thumbnailBlob", _ref.get().thumbnailBlob()); return;
        case TData::FieldIndex::fileUrl: lambda("fileUrl", _ref.get().fileUrl()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::id: lambda("id", _ref.get().id()); return;
        case TData::FieldIndex::keywords: lambda("keywords", _ref.get().keywords()); return;
        case TData::FieldIndex::location: lambda("location", _ref.get().location()); return;
        case TData::FieldIndex::md5sum: lambda("md5sum", _ref.get().md5sum()); return;
        case TData::FieldIndex::thumbnailBlob: lambda("thumbnailBlob", _ref.get().thumbnailBlob()); return;
        case TData::FieldIndex::fileUrl: lambda("fileUrl", _ref.get().fileUrl()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        lambda("id", _ref.get().id());
        lambda("keywords", _ref.get().keywords());
        lambda("location", _ref.get().location());
        lambda("md5sum", _ref.get().md5sum());
        lambda("thumbnailBlob", _ref.get().thumbnailBlob());
        lambda("fileUrl", _ref.get().fileUrl());
    }

    std::reference_wrapper<TData> _ref;
};

template <>
struct Stencil::Visitor<const Metadata::DigitalAssetInfo::Data, void>
    : Stencil::VisitorT<const Metadata::DigitalAssetInfo::Data>
{
    using TData = Metadata::DigitalAssetInfo::Data const;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::id: lambda("id", _ref.get().id()); return;
        case TData::FieldIndex::keywords: lambda("keywords", _ref.get().keywords()); return;
        case TData::FieldIndex::location: lambda("location", _ref.get().location()); return;
        case TData::FieldIndex::md5sum: lambda("md5sum", _ref.get().md5sum()); return;
        case TData::FieldIndex::thumbnailBlob: lambda("thumbnailBlob", _ref.get().thumbnailBlob()); return;
        case TData::FieldIndex::fileUrl: lambda("fileUrl", _ref.get().fileUrl()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        lambda("id", _ref.get().id());
        lambda("keywords", _ref.get().keywords());
        lambda("location", _ref.get().location());
        lambda("md5sum", _ref.get().md5sum());
        lambda("thumbnailBlob", _ref.get().thumbnailBlob());
        lambda("fileUrl", _ref.get().fileUrl());
    }

    std::reference_wrapper<TData> _ref;
};

#endif
// SECTION END: Template specializations

// SECTION START: Inline Function Definitions
#if true

#endif
// SECTION END: Inline Function Definitions
