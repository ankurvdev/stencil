#pragma once
#include <DataModel.h>
namespace ComplexWebService
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
typedef tree<shared_string> Keyword;
typedef tree<shared_string> Entity;
typedef std::vector<MapPoint::Data> AreaPolygon;
typedef tree<GeographicalArea::Data> GeographicalLocation;
typedef std::vector<tree<shared_string>> Keywords;
typedef std::vector<tree<shared_string>> Entities;
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

    static constexpr std::string_view FieldAttributeValue(FieldIndex index, const std::string_view& key)
    {
        switch (index)
        {
        case FieldIndex::latitude:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::longitude:
            return ::ReflectionServices::EmptyAttributeValue(key);
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
    void                            longitude(int64_t&& val) { _longitude = std::move(val); }

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
    void                            name(shared_string&& val) { _name = std::move(val); }

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
    void                            areaPolygon(std::vector<MapPoint::Data>&& val) { _areaPolygon = std::move(val); }

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

    int32_t& get_id()
    {
        return _id;
        ;
    }
    void set_id(int32_t&& val) { _id = std::move(val); }

    private:
    std::vector<tree<shared_string>> _keywords = {};

    public:
    std::vector<tree<shared_string>>&       keywords() { return _keywords; }
    const std::vector<tree<shared_string>>& keywords() const { return _keywords; }
    void                            keywords(std::vector<tree<shared_string>>&& val) { _keywords = std::move(val); }

    std::vector<tree<shared_string>>& get_keywords()
    {
        return _keywords;
        ;
    }
    void set_keywords(std::vector<tree<shared_string>>&& val) { _keywords = std::move(val); }

    private:
    tree<GeographicalArea::Data> _location = {};

    public:
    tree<GeographicalArea::Data>&       location() { return _location; }
    const tree<GeographicalArea::Data>& location() const { return _location; }
    void                            location(tree<GeographicalArea::Data>&& val) { _location = std::move(val); }

    tree<GeographicalArea::Data>& get_location()
    {
        return _location;
        ;
    }
    void set_location(tree<GeographicalArea::Data>&& val) { _location = std::move(val); }

    private:
    int64_t _md5sum = {};

    public:
    int64_t&       md5sum() { return _md5sum; }
    const int64_t& md5sum() const { return _md5sum; }
    void                            md5sum(int64_t&& val) { _md5sum = std::move(val); }

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
    void                            thumbnailBlob(shared_string&& val) { _thumbnailBlob = std::move(val); }

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
    void                            fileUrl(shared_string&& val) { _fileUrl = std::move(val); }

    shared_string& get_fileUrl()
    {
        return _fileUrl;
        ;
    }
    void set_fileUrl(shared_string&& val) { _fileUrl = std::move(val); }

};

}    // namespace DigitalAssetInfo
typedef tree<shared_string> Keyword;
typedef tree<shared_string> Entity;
typedef std::vector<MapPoint::Data> AreaPolygon;
typedef tree<GeographicalArea::Data> GeographicalLocation;
typedef std::vector<tree<shared_string>> Keywords;
typedef std::vector<tree<shared_string>> Entities;
struct KeywordS : public ReflectionBase::Interface<KeywordS>
{
    public:
    KeywordS() : ReflectionBase::Interface<KeywordS>(this) {}
    virtual tree<shared_string> Find(
        tree<shared_string> const& root
,        shared_string const& query
,        shared_string const& type
        )
        = 0;
    virtual tree<shared_string> Create(
        shared_string const& word
,        tree<shared_string> const& parent
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
        tree<shared_string> const& parent
        )
        = 0;
    // static std::unique_ptr<KeywordS> Create();
};

struct KeywordSFactory : public ReflectionBase::InterfaceFactory<KeywordS>
{
    public:
    virtual std::unique_ptr<KeywordS> Activate() = 0;
};

struct KeywordS_Find_Args
{
    KeywordS* instance = nullptr;

    tree<shared_string>  arg_root{};
    tree<shared_string>& get_arg_root() { return arg_root; }
    void                      set_arg_root(tree<shared_string>&& value) { arg_root = std::move(value); }
    shared_string  arg_query{};
    shared_string& get_arg_query() { return arg_query; }
    void                      set_arg_query(shared_string&& value) { arg_query = std::move(value); }
    shared_string  arg_type{};
    shared_string& get_arg_type() { return arg_type; }
    void                      set_arg_type(shared_string&& value) { arg_type = std::move(value); }
};

struct KeywordS_Create_Args
{
    KeywordS* instance = nullptr;

    shared_string  arg_word{};
    shared_string& get_arg_word() { return arg_word; }
    void                      set_arg_word(shared_string&& value) { arg_word = std::move(value); }
    tree<shared_string>  arg_parent{};
    tree<shared_string>& get_arg_parent() { return arg_parent; }
    void                      set_arg_parent(tree<shared_string>&& value) { arg_parent = std::move(value); }
};

struct KeywordS_Delete_Args
{
    KeywordS* instance = nullptr;

};

struct KeywordS_Modify_Args
{
    KeywordS* instance = nullptr;

    shared_string  arg_word{};
    shared_string& get_arg_word() { return arg_word; }
    void                      set_arg_word(shared_string&& value) { arg_word = std::move(value); }
};

struct KeywordS_SetParent_Args
{
    KeywordS* instance = nullptr;

    tree<shared_string>  arg_parent{};
    tree<shared_string>& get_arg_parent() { return arg_parent; }
    void                      set_arg_parent(tree<shared_string>&& value) { arg_parent = std::move(value); }
};

struct Content : public ReflectionBase::Interface<Content>
{
    public:
    Content() : ReflectionBase::Interface<Content>(this) {}
    virtual int32_t AddKeyword(
        tree<shared_string> const& keyword
        )
        = 0;
    virtual int32_t RemoveKeyword(
        tree<shared_string> const& keyword
        )
        = 0;
    // static std::unique_ptr<Content> Create();
};

struct ContentFactory : public ReflectionBase::InterfaceFactory<Content>
{
    public:
    virtual std::unique_ptr<Content> Activate() = 0;
};

struct Content_AddKeyword_Args
{
    Content* instance = nullptr;

    tree<shared_string>  arg_keyword{};
    tree<shared_string>& get_arg_keyword() { return arg_keyword; }
    void                      set_arg_keyword(tree<shared_string>&& value) { arg_keyword = std::move(value); }
};

struct Content_RemoveKeyword_Args
{
    Content* instance = nullptr;

    tree<shared_string>  arg_keyword{};
    tree<shared_string>& get_arg_keyword() { return arg_keyword; }
    void                      set_arg_keyword(tree<shared_string>&& value) { arg_keyword = std::move(value); }
};

struct Search : public ReflectionBase::Interface<Search>
{
    public:
    Search() : ReflectionBase::Interface<Search>(this) {}
    virtual std::vector<long> SearchByKeywords(
        std::vector<tree<shared_string>> const& keywords
        )
        = 0;
    virtual std::vector<long> SearchByDate(
        int64_t const& startDate
,        int64_t const& endDate
        )
        = 0;
    // static std::unique_ptr<Search> Create();
};

struct SearchFactory : public ReflectionBase::InterfaceFactory<Search>
{
    public:
    virtual std::unique_ptr<Search> Activate() = 0;
};

struct Search_SearchByKeywords_Args
{
    Search* instance = nullptr;

    std::vector<tree<shared_string>>  arg_keywords{};
    std::vector<tree<shared_string>>& get_arg_keywords() { return arg_keywords; }
    void                      set_arg_keywords(std::vector<tree<shared_string>>&& value) { arg_keywords = std::move(value); }
};

struct Search_SearchByDate_Args
{
    Search* instance = nullptr;

    int64_t  arg_startDate{};
    int64_t& get_arg_startDate() { return arg_startDate; }
    void                      set_arg_startDate(int64_t&& value) { arg_startDate = std::move(value); }
    int64_t  arg_endDate{};
    int64_t& get_arg_endDate() { return arg_endDate; }
    void                      set_arg_endDate(int64_t&& value) { arg_endDate = std::move(value); }
};

struct Store : public ReflectionBase::Interface<Store>
{
    public:
    Store() : ReflectionBase::Interface<Store>(this) {}
    virtual DigitalAssetInfo::Data Upload(
        shared_string const& file
        )
        = 0;
    virtual shared_string Download(
        long const& content
        )
        = 0;
    // static std::unique_ptr<Store> Create();
};

struct StoreFactory : public ReflectionBase::InterfaceFactory<Store>
{
    public:
    virtual std::unique_ptr<Store> Activate() = 0;
};

struct Store_Upload_Args
{
    Store* instance = nullptr;

    shared_string  arg_file{};
    shared_string& get_arg_file() { return arg_file; }
    void                      set_arg_file(shared_string&& value) { arg_file = std::move(value); }
};

struct Store_Download_Args
{
    Store* instance = nullptr;

    long  arg_content{};
    long& get_arg_content() { return arg_content; }
    void                      set_arg_content(long&& value) { arg_content = std::move(value); }
};

}    // namespace ComplexWebService

template <> struct ReflectionBase::TypeTraits<ComplexWebService::KeywordS_Find_Args&>
{
    struct Traits_arg_root
    {
        using TOwner = ComplexWebService::KeywordS_Find_Args;
        static constexpr std::string_view    Name() { return "root"; }
        static const ::ReflectionBase::Flags Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TOwner::get_arg_root; }
        static constexpr auto TPropertySetter() { return &TOwner::set_arg_root; }
    };
    struct Traits_arg_query
    {
        using TOwner = ComplexWebService::KeywordS_Find_Args;
        static constexpr std::string_view    Name() { return "query"; }
        static const ::ReflectionBase::Flags Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TOwner::get_arg_query; }
        static constexpr auto TPropertySetter() { return &TOwner::set_arg_query; }
    };
    struct Traits_arg_type
    {
        using TOwner = ComplexWebService::KeywordS_Find_Args;
        static constexpr std::string_view    Name() { return "type"; }
        static const ::ReflectionBase::Flags Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TOwner::get_arg_type; }
        static constexpr auto TPropertySetter() { return &TOwner::set_arg_type; }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "Find"; }
    static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

    using Handler = ::ReflectionServices::ReflectedStructHandler<ComplexWebService::KeywordS_Find_Args
                                                                 ,Traits_arg_root
                                                                 ,Traits_arg_query
                                                                 ,Traits_arg_type
                                                                 >;
};
template <> struct ReflectionBase::TypeTraits<ComplexWebService::KeywordS_Create_Args&>
{
    struct Traits_arg_word
    {
        using TOwner = ComplexWebService::KeywordS_Create_Args;
        static constexpr std::string_view    Name() { return "word"; }
        static const ::ReflectionBase::Flags Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TOwner::get_arg_word; }
        static constexpr auto TPropertySetter() { return &TOwner::set_arg_word; }
    };
    struct Traits_arg_parent
    {
        using TOwner = ComplexWebService::KeywordS_Create_Args;
        static constexpr std::string_view    Name() { return "parent"; }
        static const ::ReflectionBase::Flags Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TOwner::get_arg_parent; }
        static constexpr auto TPropertySetter() { return &TOwner::set_arg_parent; }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "Create"; }
    static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

    using Handler = ::ReflectionServices::ReflectedStructHandler<ComplexWebService::KeywordS_Create_Args
                                                                 ,Traits_arg_word
                                                                 ,Traits_arg_parent
                                                                 >;
};
template <> struct ReflectionBase::TypeTraits<ComplexWebService::KeywordS_Delete_Args&>
{
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "Delete"; }
    static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

    using Handler = ::ReflectionServices::ReflectedStructHandler<ComplexWebService::KeywordS_Delete_Args
                                                                 >;
};
template <> struct ReflectionBase::TypeTraits<ComplexWebService::KeywordS_Modify_Args&>
{
    struct Traits_arg_word
    {
        using TOwner = ComplexWebService::KeywordS_Modify_Args;
        static constexpr std::string_view    Name() { return "word"; }
        static const ::ReflectionBase::Flags Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TOwner::get_arg_word; }
        static constexpr auto TPropertySetter() { return &TOwner::set_arg_word; }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "Modify"; }
    static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

    using Handler = ::ReflectionServices::ReflectedStructHandler<ComplexWebService::KeywordS_Modify_Args
                                                                 ,Traits_arg_word
                                                                 >;
};
template <> struct ReflectionBase::TypeTraits<ComplexWebService::KeywordS_SetParent_Args&>
{
    struct Traits_arg_parent
    {
        using TOwner = ComplexWebService::KeywordS_SetParent_Args;
        static constexpr std::string_view    Name() { return "parent"; }
        static const ::ReflectionBase::Flags Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TOwner::get_arg_parent; }
        static constexpr auto TPropertySetter() { return &TOwner::set_arg_parent; }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "SetParent"; }
    static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

    using Handler = ::ReflectionServices::ReflectedStructHandler<ComplexWebService::KeywordS_SetParent_Args
                                                                 ,Traits_arg_parent
                                                                 >;
};
template <> struct ReflectionBase::InterfaceTraits<ComplexWebService::KeywordS>
{
    struct ApiTraits_Find
    {
        using TOwner = ComplexWebService::KeywordS;
        static const ::ReflectionBase::Flags Flags() { return {}; }
        static constexpr std::string_view    Name() { return "Find"; }
        static constexpr bool                Static = false;
    };
    struct ApiTraits_Create
    {
        using TOwner = ComplexWebService::KeywordS;
        static const ::ReflectionBase::Flags Flags() { return {}; }
        static constexpr std::string_view    Name() { return "Create"; }
        static constexpr bool                Static = false;
    };
    struct ApiTraits_Delete
    {
        using TOwner = ComplexWebService::KeywordS;
        static const ::ReflectionBase::Flags Flags() { return {}; }
        static constexpr std::string_view    Name() { return "Delete"; }
        static constexpr bool                Static = false;
    };
    struct ApiTraits_Modify
    {
        using TOwner = ComplexWebService::KeywordS;
        static const ::ReflectionBase::Flags Flags() { return {}; }
        static constexpr std::string_view    Name() { return "Modify"; }
        static constexpr bool                Static = false;
    };
    struct ApiTraits_SetParent
    {
        using TOwner = ComplexWebService::KeywordS;
        static const ::ReflectionBase::Flags Flags() { return {}; }
        static constexpr std::string_view    Name() { return "SetParent"; }
        static constexpr bool                Static = false;
    };
    using Apis = ::ReflectionBase::InterfaceApiPack<
        ApiTraits_Find
,        ApiTraits_Create
,        ApiTraits_Delete
,        ApiTraits_Modify
,        ApiTraits_SetParent
        >;
};

template <>
struct ReflectionBase::InterfaceApiTraits<ReflectionBase::InterfaceTraits<ComplexWebService::KeywordS>::ApiTraits_Find>
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

template <>
struct ReflectionBase::InterfaceApiTraits<ReflectionBase::InterfaceTraits<ComplexWebService::KeywordS>::ApiTraits_Create>
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

template <>
struct ReflectionBase::InterfaceApiTraits<ReflectionBase::InterfaceTraits<ComplexWebService::KeywordS>::ApiTraits_Delete>
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

template <>
struct ReflectionBase::InterfaceApiTraits<ReflectionBase::InterfaceTraits<ComplexWebService::KeywordS>::ApiTraits_Modify>
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

template <>
struct ReflectionBase::InterfaceApiTraits<ReflectionBase::InterfaceTraits<ComplexWebService::KeywordS>::ApiTraits_SetParent>
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

#if (defined STENCIL_USING_WEBSERVICE) and (STENCIL_USING_WEBSERVICE > 0)
template <> struct WebServiceHandlerTraits<ComplexWebService::KeywordS>
{
    static constexpr const std::string_view Url() { return std::string_view("KeywordS"); }
};
#endif

template <> struct ReflectionBase::TypeTraits<ComplexWebService::Content_AddKeyword_Args&>
{
    struct Traits_arg_keyword
    {
        using TOwner = ComplexWebService::Content_AddKeyword_Args;
        static constexpr std::string_view    Name() { return "keyword"; }
        static const ::ReflectionBase::Flags Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TOwner::get_arg_keyword; }
        static constexpr auto TPropertySetter() { return &TOwner::set_arg_keyword; }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "AddKeyword"; }
    static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

    using Handler = ::ReflectionServices::ReflectedStructHandler<ComplexWebService::Content_AddKeyword_Args
                                                                 ,Traits_arg_keyword
                                                                 >;
};
template <> struct ReflectionBase::TypeTraits<ComplexWebService::Content_RemoveKeyword_Args&>
{
    struct Traits_arg_keyword
    {
        using TOwner = ComplexWebService::Content_RemoveKeyword_Args;
        static constexpr std::string_view    Name() { return "keyword"; }
        static const ::ReflectionBase::Flags Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TOwner::get_arg_keyword; }
        static constexpr auto TPropertySetter() { return &TOwner::set_arg_keyword; }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "RemoveKeyword"; }
    static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

    using Handler = ::ReflectionServices::ReflectedStructHandler<ComplexWebService::Content_RemoveKeyword_Args
                                                                 ,Traits_arg_keyword
                                                                 >;
};
template <> struct ReflectionBase::InterfaceTraits<ComplexWebService::Content>
{
    struct ApiTraits_AddKeyword
    {
        using TOwner = ComplexWebService::Content;
        static const ::ReflectionBase::Flags Flags() { return {}; }
        static constexpr std::string_view    Name() { return "AddKeyword"; }
        static constexpr bool                Static = false;
    };
    struct ApiTraits_RemoveKeyword
    {
        using TOwner = ComplexWebService::Content;
        static const ::ReflectionBase::Flags Flags() { return {}; }
        static constexpr std::string_view    Name() { return "RemoveKeyword"; }
        static constexpr bool                Static = false;
    };
    using Apis = ::ReflectionBase::InterfaceApiPack<
        ApiTraits_AddKeyword
,        ApiTraits_RemoveKeyword
        >;
};

template <>
struct ReflectionBase::InterfaceApiTraits<ReflectionBase::InterfaceTraits<ComplexWebService::Content>::ApiTraits_AddKeyword>
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

template <>
struct ReflectionBase::InterfaceApiTraits<ReflectionBase::InterfaceTraits<ComplexWebService::Content>::ApiTraits_RemoveKeyword>
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

#if (defined STENCIL_USING_WEBSERVICE) and (STENCIL_USING_WEBSERVICE > 0)
template <> struct WebServiceHandlerTraits<ComplexWebService::Content>
{
    static constexpr const std::string_view Url() { return std::string_view("Content"); }
};
#endif

template <> struct ReflectionBase::TypeTraits<ComplexWebService::Search_SearchByKeywords_Args&>
{
    struct Traits_arg_keywords
    {
        using TOwner = ComplexWebService::Search_SearchByKeywords_Args;
        static constexpr std::string_view    Name() { return "keywords"; }
        static const ::ReflectionBase::Flags Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TOwner::get_arg_keywords; }
        static constexpr auto TPropertySetter() { return &TOwner::set_arg_keywords; }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "SearchByKeywords"; }
    static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

    using Handler = ::ReflectionServices::ReflectedStructHandler<ComplexWebService::Search_SearchByKeywords_Args
                                                                 ,Traits_arg_keywords
                                                                 >;
};
template <> struct ReflectionBase::TypeTraits<ComplexWebService::Search_SearchByDate_Args&>
{
    struct Traits_arg_startDate
    {
        using TOwner = ComplexWebService::Search_SearchByDate_Args;
        static constexpr std::string_view    Name() { return "startDate"; }
        static const ::ReflectionBase::Flags Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TOwner::get_arg_startDate; }
        static constexpr auto TPropertySetter() { return &TOwner::set_arg_startDate; }
    };
    struct Traits_arg_endDate
    {
        using TOwner = ComplexWebService::Search_SearchByDate_Args;
        static constexpr std::string_view    Name() { return "endDate"; }
        static const ::ReflectionBase::Flags Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TOwner::get_arg_endDate; }
        static constexpr auto TPropertySetter() { return &TOwner::set_arg_endDate; }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "SearchByDate"; }
    static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

    using Handler = ::ReflectionServices::ReflectedStructHandler<ComplexWebService::Search_SearchByDate_Args
                                                                 ,Traits_arg_startDate
                                                                 ,Traits_arg_endDate
                                                                 >;
};
template <> struct ReflectionBase::InterfaceTraits<ComplexWebService::Search>
{
    struct ApiTraits_SearchByKeywords
    {
        using TOwner = ComplexWebService::Search;
        static const ::ReflectionBase::Flags Flags() { return {}; }
        static constexpr std::string_view    Name() { return "SearchByKeywords"; }
        static constexpr bool                Static = false;
    };
    struct ApiTraits_SearchByDate
    {
        using TOwner = ComplexWebService::Search;
        static const ::ReflectionBase::Flags Flags() { return {}; }
        static constexpr std::string_view    Name() { return "SearchByDate"; }
        static constexpr bool                Static = false;
    };
    using Apis = ::ReflectionBase::InterfaceApiPack<
        ApiTraits_SearchByKeywords
,        ApiTraits_SearchByDate
        >;
};

template <>
struct ReflectionBase::InterfaceApiTraits<ReflectionBase::InterfaceTraits<ComplexWebService::Search>::ApiTraits_SearchByKeywords>
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

template <>
struct ReflectionBase::InterfaceApiTraits<ReflectionBase::InterfaceTraits<ComplexWebService::Search>::ApiTraits_SearchByDate>
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

#if (defined STENCIL_USING_WEBSERVICE) and (STENCIL_USING_WEBSERVICE > 0)
template <> struct WebServiceHandlerTraits<ComplexWebService::Search>
{
    static constexpr const std::string_view Url() { return std::string_view("Search"); }
};
#endif

template <> struct ReflectionBase::TypeTraits<ComplexWebService::Store_Upload_Args&>
{
    struct Traits_arg_file
    {
        using TOwner = ComplexWebService::Store_Upload_Args;
        static constexpr std::string_view    Name() { return "file"; }
        static const ::ReflectionBase::Flags Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TOwner::get_arg_file; }
        static constexpr auto TPropertySetter() { return &TOwner::set_arg_file; }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "Upload"; }
    static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

    using Handler = ::ReflectionServices::ReflectedStructHandler<ComplexWebService::Store_Upload_Args
                                                                 ,Traits_arg_file
                                                                 >;
};
template <> struct ReflectionBase::TypeTraits<ComplexWebService::Store_Download_Args&>
{
    struct Traits_arg_content
    {
        using TOwner = ComplexWebService::Store_Download_Args;
        static constexpr std::string_view    Name() { return "content"; }
        static const ::ReflectionBase::Flags Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TOwner::get_arg_content; }
        static constexpr auto TPropertySetter() { return &TOwner::set_arg_content; }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "Download"; }
    static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

    using Handler = ::ReflectionServices::ReflectedStructHandler<ComplexWebService::Store_Download_Args
                                                                 ,Traits_arg_content
                                                                 >;
};
template <> struct ReflectionBase::InterfaceTraits<ComplexWebService::Store>
{
    struct ApiTraits_Upload
    {
        using TOwner = ComplexWebService::Store;
        static const ::ReflectionBase::Flags Flags() { return {}; }
        static constexpr std::string_view    Name() { return "Upload"; }
        static constexpr bool                Static = false;
    };
    struct ApiTraits_Download
    {
        using TOwner = ComplexWebService::Store;
        static const ::ReflectionBase::Flags Flags() { return {}; }
        static constexpr std::string_view    Name() { return "Download"; }
        static constexpr bool                Static = false;
    };
    using Apis = ::ReflectionBase::InterfaceApiPack<
        ApiTraits_Upload
,        ApiTraits_Download
        >;
};

template <>
struct ReflectionBase::InterfaceApiTraits<ReflectionBase::InterfaceTraits<ComplexWebService::Store>::ApiTraits_Upload>
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

template <>
struct ReflectionBase::InterfaceApiTraits<ReflectionBase::InterfaceTraits<ComplexWebService::Store>::ApiTraits_Download>
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

#if (defined STENCIL_USING_WEBSERVICE) and (STENCIL_USING_WEBSERVICE > 0)
template <> struct WebServiceHandlerTraits<ComplexWebService::Store>
{
    static constexpr const std::string_view Url() { return std::string_view("Store"); }
};
#endif

template <> struct ReflectionBase::TypeTraits<ComplexWebService::MapPoint::Data&>
{
    struct Traits_latitude
    {
        using TOwner = ComplexWebService::MapPoint::Data;

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
        using TOwner = ComplexWebService::MapPoint::Data;

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

    using Handler = ::ReflectionServices::ReflectedStructHandler<ComplexWebService::MapPoint::Data,
                                                                 Traits_latitude
,                                                                 Traits_longitude
                                                                 >;
};
template <> struct ReflectionBase::TypeTraits<ComplexWebService::GeographicalArea::Data&>
{
    struct Traits_type
    {
        using TOwner = ComplexWebService::GeographicalArea::Data;

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
        using TOwner = ComplexWebService::GeographicalArea::Data;

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
        using TOwner = ComplexWebService::GeographicalArea::Data;

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

    using Handler = ::ReflectionServices::ReflectedStructHandler<ComplexWebService::GeographicalArea::Data,
                                                                 Traits_type
,                                                                 Traits_name
,                                                                 Traits_areaPolygon
                                                                 >;
};
template <> struct ReflectionBase::TypeTraits<ComplexWebService::DigitalAssetInfo::Data&>
{
    struct Traits_id
    {
        using TOwner = ComplexWebService::DigitalAssetInfo::Data;

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
        using TOwner = ComplexWebService::DigitalAssetInfo::Data;

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
        using TOwner = ComplexWebService::DigitalAssetInfo::Data;

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
        using TOwner = ComplexWebService::DigitalAssetInfo::Data;

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
        using TOwner = ComplexWebService::DigitalAssetInfo::Data;

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
        using TOwner = ComplexWebService::DigitalAssetInfo::Data;

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

    using Handler = ::ReflectionServices::ReflectedStructHandler<ComplexWebService::DigitalAssetInfo::Data,
                                                                 Traits_id
,                                                                 Traits_keywords
,                                                                 Traits_location
,                                                                 Traits_md5sum
,                                                                 Traits_thumbnailBlob
,                                                                 Traits_fileUrl
                                                                 >;
};
