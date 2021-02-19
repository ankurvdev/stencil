#pragma once
#include <stencil/stencil.h>
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
        Stencil::ObservablePropsT<Data>::OnChangeRequested(*this, FieldIndex::latitude, _latitude, val);
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
        Stencil::ObservablePropsT<Data>::OnChangeRequested(*this, FieldIndex::longitude, _longitude, val);
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
        Stencil::ObservablePropsT<Data>::OnChangeRequested(*this, FieldIndex::type, _type, val);
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
        Stencil::ObservablePropsT<Data>::OnChangeRequested(*this, FieldIndex::name, _name, val);
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
        Stencil::ObservablePropsT<Data>::OnChangeRequested(*this, FieldIndex::areaPolygon, _areaPolygon, val);
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
        Stencil::ObservablePropsT<Data>::OnChangeRequested(*this, FieldIndex::id, _id, val);
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
        Stencil::ObservablePropsT<Data>::OnChangeRequested(*this, FieldIndex::keywords, _keywords, val);
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
        Stencil::ObservablePropsT<Data>::OnChangeRequested(*this, FieldIndex::location, _location, val);
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
        Stencil::ObservablePropsT<Data>::OnChangeRequested(*this, FieldIndex::md5sum, _md5sum, val);
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
        Stencil::ObservablePropsT<Data>::OnChangeRequested(*this, FieldIndex::thumbnailBlob, _thumbnailBlob, val);
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
        Stencil::ObservablePropsT<Data>::OnChangeRequested(*this, FieldIndex::fileUrl, _fileUrl, val);
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

template <> struct ReflectionBase::TypeTraits<Metadata::MapPoint::Data&>
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
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
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

template <typename T> struct Stencil::DeltaTracker<T, std::enable_if_t<std::is_same_v<T, Metadata::MapPoint::Data>>>
{
    using TData = T;

    // TODO : Tentative: We hate pointers
    TData const* const _ptr;
    // TODO : Better way to unify creation interface
    bool _changed = false;

    DELETE_COPY_AND_MOVE(DeltaTracker);

    DeltaTracker(TData const* ptr, bool changed) : _ptr(ptr), _changed(changed)
    {
        // TODO: Tentative
        static_assert(std::is_base_of<Stencil::ObservablePropsT<TData>, TData>::value);
    }

    static constexpr auto Type() { return ReflectionBase::TypeTraits<TData&>::Type(); }

    size_t NumFields() const { return TData::FieldCount(); }
    bool   IsChanged() const { return _ptr->_changetracker.any(); }

    uint8_t MutatorIndex() const;
    bool    OnlyHasDefaultMutator() const;

    bool IsFieldChanged(typename TData::FieldIndex index) const { return _ptr->_changetracker.test(static_cast<size_t>(index)); }

    size_t CountFieldsChanged() const { return _ptr->_changetracker.count(); }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::latitude:
            lambda(DeltaTracker<int64_t>(&_ptr->latitude(), IsFieldChanged(TData::FieldIndex::latitude)));
            return;
        case TData::FieldIndex::longitude:
            lambda(DeltaTracker<int64_t>(&_ptr->longitude(), IsFieldChanged(TData::FieldIndex::longitude)));
            return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }
};

template <> struct ReflectionBase::TypeTraits<Metadata::GeographicalArea::Data&>
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
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
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

template <typename T> struct Stencil::DeltaTracker<T, std::enable_if_t<std::is_same_v<T, Metadata::GeographicalArea::Data>>>
{
    using TData = T;

    // TODO : Tentative: We hate pointers
    TData const* const _ptr;
    // TODO : Better way to unify creation interface
    bool _changed = false;

    DELETE_COPY_AND_MOVE(DeltaTracker);

    DeltaTracker(TData const* ptr, bool changed) : _ptr(ptr), _changed(changed)
    {
        // TODO: Tentative
        static_assert(std::is_base_of<Stencil::ObservablePropsT<TData>, TData>::value);
    }

    static constexpr auto Type() { return ReflectionBase::TypeTraits<TData&>::Type(); }

    size_t NumFields() const { return TData::FieldCount(); }
    bool   IsChanged() const { return _ptr->_changetracker.any(); }

    uint8_t MutatorIndex() const;
    bool    OnlyHasDefaultMutator() const;

    bool IsFieldChanged(typename TData::FieldIndex index) const { return _ptr->_changetracker.test(static_cast<size_t>(index)); }

    size_t CountFieldsChanged() const { return _ptr->_changetracker.count(); }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::type:
            lambda(DeltaTracker<shared_string>(&_ptr->type(), IsFieldChanged(TData::FieldIndex::type)));
            return;
        case TData::FieldIndex::name:
            lambda(DeltaTracker<shared_string>(&_ptr->name(), IsFieldChanged(TData::FieldIndex::name)));
            return;
        case TData::FieldIndex::areaPolygon:
            lambda(DeltaTracker<std::vector<::Metadata::MapPoint::Data>>(&_ptr->areaPolygon(), IsFieldChanged(TData::FieldIndex::areaPolygon)));
            return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }
};

template <> struct ReflectionBase::TypeTraits<Metadata::DigitalAssetInfo::Data&>
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
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
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

template <typename T> struct Stencil::DeltaTracker<T, std::enable_if_t<std::is_same_v<T, Metadata::DigitalAssetInfo::Data>>>
{
    using TData = T;

    // TODO : Tentative: We hate pointers
    TData const* const _ptr;
    // TODO : Better way to unify creation interface
    bool _changed = false;

    DELETE_COPY_AND_MOVE(DeltaTracker);

    DeltaTracker(TData const* ptr, bool changed) : _ptr(ptr), _changed(changed)
    {
        // TODO: Tentative
        static_assert(std::is_base_of<Stencil::ObservablePropsT<TData>, TData>::value);
    }

    static constexpr auto Type() { return ReflectionBase::TypeTraits<TData&>::Type(); }

    size_t NumFields() const { return TData::FieldCount(); }
    bool   IsChanged() const { return _ptr->_changetracker.any(); }

    uint8_t MutatorIndex() const;
    bool    OnlyHasDefaultMutator() const;

    bool IsFieldChanged(typename TData::FieldIndex index) const { return _ptr->_changetracker.test(static_cast<size_t>(index)); }

    size_t CountFieldsChanged() const { return _ptr->_changetracker.count(); }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::id:
            lambda(DeltaTracker<int32_t>(&_ptr->id(), IsFieldChanged(TData::FieldIndex::id)));
            return;
        case TData::FieldIndex::keywords:
            lambda(DeltaTracker<std::vector<shared_tree<shared_string>>>(&_ptr->keywords(), IsFieldChanged(TData::FieldIndex::keywords)));
            return;
        case TData::FieldIndex::location:
            lambda(DeltaTracker<shared_tree<::Metadata::GeographicalArea::Data>>(&_ptr->location(), IsFieldChanged(TData::FieldIndex::location)));
            return;
        case TData::FieldIndex::md5sum:
            lambda(DeltaTracker<int64_t>(&_ptr->md5sum(), IsFieldChanged(TData::FieldIndex::md5sum)));
            return;
        case TData::FieldIndex::thumbnailBlob:
            lambda(DeltaTracker<shared_string>(&_ptr->thumbnailBlob(), IsFieldChanged(TData::FieldIndex::thumbnailBlob)));
            return;
        case TData::FieldIndex::fileUrl:
            lambda(DeltaTracker<shared_string>(&_ptr->fileUrl(), IsFieldChanged(TData::FieldIndex::fileUrl)));
            return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }
};

