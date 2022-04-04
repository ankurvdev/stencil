#pragma once
#include <stencil/stencil.h>

// SECTION START: DECLARATIONS
#if true

namespace Catalog
{
struct ImdbInfo;
struct Directory;
struct File;
struct ScanRequest;
struct Catalog;
}    // namespace Catalog

template <> struct Stencil::TypeTraits<Catalog::ImdbInfo>;
template <> struct Stencil::TypeTraits<Catalog::Directory>;
template <> struct Stencil::TypeTraits<Catalog::File>;
template <> struct Stencil::TypeTraits<Catalog::ScanRequest>;
template <> struct Stencil::TypeTraits<Catalog::Catalog>;
#endif
// SECTION END: DECLARATIONS

// SECTION START: Definitions
#if true
namespace Catalog
{
struct ImdbInfo :
    public ::UuidObjectT<::Catalog::ImdbInfo>,
    public ::Stencil::TimestampedT<::Catalog::ImdbInfo>,
    public Stencil::StructT<ImdbInfo>
{
    uint64_t imdbId = {};
    uint8_t type = {};
    uint8_t ratingM10 = {};
    Stencil::Timestamp firstReleased = {};
    uint64_t votes = {};
    shared_string title = {};
    shared_string certificate = {};
    shared_string plot = {};
    uint16_t season = {};
    uint16_t episode = {};
    uint64_t showId = {};
};
struct Directory :
    public ::UuidObjectT<::Catalog::Directory>,
    public ::Stencil::TimestampedT<::Catalog::Directory>,
    public Stencil::StructT<Directory>
{
    shared_string url = {};
    Stencil::Timestamp created = {};
    Stencil::Timestamp lastchecked = {};
    uint32_t maxFrequencyInMins = {};
};
struct File :
    public ::UuidObjectT<::Catalog::File>,
    public ::Stencil::TimestampedT<::Catalog::File>,
    public Stencil::StructT<File>
{
    ::UuidBasedId<::Catalog::Directory> dirId = ::UuidBasedId<::Catalog::Directory>::Create();
    ::UuidBasedId<::Catalog::ImdbInfo> imdbId = ::UuidBasedId<::Catalog::ImdbInfo>::Create();
    shared_string url = {};
    Stencil::Timestamp created = {};
    uint64_t fsize = {};
    Stencil::Timestamp modified = {};
    Stencil::Timestamp seen = {};
    bool queued = {};
    bool pinned = {};
};
struct ScanRequest :
    public ::UuidObjectT<::Catalog::ScanRequest>,
    public ::Stencil::TimestampedT<::Catalog::ScanRequest>,
    public Stencil::StructT<ScanRequest>
{
    ::UuidBasedId<::Catalog::Directory> dirId = ::UuidBasedId<::Catalog::Directory>::Create();
    Stencil::Timestamp scannedAt = {};
    uint32_t statusCode = {};
    shared_string statusDesc = {};
};
struct Catalog : public Stencil::InterfaceT<Catalog>
{
    public:
    struct Dummy
    {};
    struct ObjectStore;

    struct ObjectStore_onlinedir : ::Catalog::Directory, Database2::ObjectT<ObjectStore, ObjectStore_onlinedir>
    {};

    struct ObjectStore_onlinefile : ::Catalog::File, Database2::ObjectT<ObjectStore, ObjectStore_onlinefile>
    {};

    struct ObjectStore_onlinescan : ::Catalog::ScanRequest, Database2::ObjectT<ObjectStore, ObjectStore_onlinescan>
    {};

    struct ObjectStore_localdir : ::Catalog::Directory, Database2::ObjectT<ObjectStore, ObjectStore_localdir>
    {};

    struct ObjectStore_localfile : ::Catalog::File, Database2::ObjectT<ObjectStore, ObjectStore_localfile>
    {};

    struct ObjectStore_localscan : ::Catalog::ScanRequest, Database2::ObjectT<ObjectStore, ObjectStore_localscan>
    {};

    struct ObjectStore_imdbinfo : ::Catalog::ImdbInfo, Database2::ObjectT<ObjectStore, ObjectStore_imdbinfo>
    {};

    struct ObjectStore : Dummy
        ,
                         public Database2::OwnerT<ObjectStore, ObjectStore_onlinedir>
        ,
                         public Database2::OwnerT<ObjectStore, ObjectStore_onlinefile>
        ,
                         public Database2::OwnerT<ObjectStore, ObjectStore_onlinescan>
        ,
                         public Database2::OwnerT<ObjectStore, ObjectStore_localdir>
        ,
                         public Database2::OwnerT<ObjectStore, ObjectStore_localfile>
        ,
                         public Database2::OwnerT<ObjectStore, ObjectStore_localscan>
        ,
                         public Database2::OwnerT<ObjectStore, ObjectStore_imdbinfo>
    {};

    Catalog()          = default;
    virtual ~Catalog() = default;
    DELETE_COPY_AND_MOVE(Catalog);
    virtual std::vector<::Catalog::File> Search(
        shared_string const& query
        )
        = 0;
    virtual void RequestDownload(
        shared_string const& link
        )
        = 0;
    struct Args_Search
    {
        shared_string arg_query{};
    };

    std::vector<::Catalog::File> Search(Args_Search const& args)
    {
        return this->Search(
            args.query
        );
    }

    std::vector<::Catalog::File> Search(Args_Search&& args)
    {
        return this->Search(
            std::move(args.query)
        );
    }

    struct Args_RequestDownload
    {
        shared_string arg_link{};
    };

    void RequestDownload(Args_RequestDownload const& args)
    {
        return this->RequestDownload(
            args.link
        );
    }

    void RequestDownload(Args_RequestDownload&& args)
    {
        return this->RequestDownload(
            std::move(args.link)
        );
    }

    ObjectStore objects;
};
}    // namespace Catalog
#endif

// SECTION END: Definitions

// SECTION START: Template specializations
#if true

// SECTION:

template <> struct Stencil::TypeTraits<Catalog::ImdbInfo>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<Catalog::ImdbInfo>
{
    enum class Fields
    {
        Invalid,
        Field_imdbId
,        Field_type
,        Field_ratingM10
,        Field_firstReleased
,        Field_votes
,        Field_title
,        Field_certificate
,        Field_plot
,        Field_season
,        Field_episode
,        Field_showId
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<Catalog::ImdbInfo>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<Catalog::ImdbInfo>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "imdbId"
,        "type"
,        "ratingM10"
,        "firstReleased"
,        "votes"
,        "title"
,        "certificate"
,        "plot"
,        "season"
,        "episode"
,        "showId"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<Catalog::ImdbInfo>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<Catalog::ImdbInfo>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<Catalog::ImdbInfo> : Stencil::TransactionT<Catalog::ImdbInfo>
{
    using TData  = Catalog::ImdbInfo;
    using Fields = Stencil::TypeTraitsForIndexable<Catalog::ImdbInfo>::Fields;

    Transaction<uint64_t> _subtracker_imdbId;
    Transaction<uint8_t> _subtracker_type;
    Transaction<uint8_t> _subtracker_ratingM10;
    Transaction<Stencil::Timestamp> _subtracker_firstReleased;
    Transaction<uint64_t> _subtracker_votes;
    Transaction<shared_string> _subtracker_title;
    Transaction<shared_string> _subtracker_certificate;
    Transaction<shared_string> _subtracker_plot;
    Transaction<uint16_t> _subtracker_season;
    Transaction<uint16_t> _subtracker_episode;
    Transaction<uint64_t> _subtracker_showId;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<Catalog::ImdbInfo>(ptr)
        ,
        _subtracker_imdbId(Obj().imdbId)
        ,
        _subtracker_type(Obj().type)
        ,
        _subtracker_ratingM10(Obj().ratingM10)
        ,
        _subtracker_firstReleased(Obj().firstReleased)
        ,
        _subtracker_votes(Obj().votes)
        ,
        _subtracker_title(Obj().title)
        ,
        _subtracker_certificate(Obj().certificate)
        ,
        _subtracker_plot(Obj().plot)
        ,
        _subtracker_season(Obj().season)
        ,
        _subtracker_episode(Obj().episode)
        ,
        _subtracker_showId(Obj().showId)
    {}

    auto& imdbId()
    {
        MarkFieldEdited_(Fields::Field_imdbId);
        return _subtracker_imdbId;
    }
    auto& type()
    {
        MarkFieldEdited_(Fields::Field_type);
        return _subtracker_type;
    }
    auto& ratingM10()
    {
        MarkFieldEdited_(Fields::Field_ratingM10);
        return _subtracker_ratingM10;
    }
    auto& firstReleased()
    {
        MarkFieldEdited_(Fields::Field_firstReleased);
        return _subtracker_firstReleased;
    }
    auto& votes()
    {
        MarkFieldEdited_(Fields::Field_votes);
        return _subtracker_votes;
    }
    auto& title()
    {
        MarkFieldEdited_(Fields::Field_title);
        return _subtracker_title;
    }
    auto& certificate()
    {
        MarkFieldEdited_(Fields::Field_certificate);
        return _subtracker_certificate;
    }
    auto& plot()
    {
        MarkFieldEdited_(Fields::Field_plot);
        return _subtracker_plot;
    }
    auto& season()
    {
        MarkFieldEdited_(Fields::Field_season);
        return _subtracker_season;
    }
    auto& episode()
    {
        MarkFieldEdited_(Fields::Field_episode);
        return _subtracker_episode;
    }
    auto& showId()
    {
        MarkFieldEdited_(Fields::Field_showId);
        return _subtracker_showId;
    }
    void set_imdbId(uint64_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_imdbId, Obj().imdbId, val);
        Obj().imdbId = std::move(val);
    }

    void set_type(uint8_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_type, Obj().type, val);
        Obj().type = std::move(val);
    }

    void set_ratingM10(uint8_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_ratingM10, Obj().ratingM10, val);
        Obj().ratingM10 = std::move(val);
    }

    void set_firstReleased(Stencil::Timestamp&& val)
    {
        MarkFieldAssigned_(Fields::Field_firstReleased, Obj().firstReleased, val);
        Obj().firstReleased = std::move(val);
    }

    void set_votes(uint64_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_votes, Obj().votes, val);
        Obj().votes = std::move(val);
    }

    void set_title(shared_string&& val)
    {
        MarkFieldAssigned_(Fields::Field_title, Obj().title, val);
        Obj().title = std::move(val);
    }

    void set_certificate(shared_string&& val)
    {
        MarkFieldAssigned_(Fields::Field_certificate, Obj().certificate, val);
        Obj().certificate = std::move(val);
    }

    void set_plot(shared_string&& val)
    {
        MarkFieldAssigned_(Fields::Field_plot, Obj().plot, val);
        Obj().plot = std::move(val);
    }

    void set_season(uint16_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_season, Obj().season, val);
        Obj().season = std::move(val);
    }

    void set_episode(uint16_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_episode, Obj().episode, val);
        Obj().episode = std::move(val);
    }

    void set_showId(uint64_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_showId, Obj().showId, val);
        Obj().showId = std::move(val);
    }

    template <typename TLambda> auto Visit(Fields index, TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_imdbId: return lambda("imdbId", imdbId()); return;
        case Fields::Field_type: return lambda("type", type()); return;
        case Fields::Field_ratingM10: return lambda("ratingM10", ratingM10()); return;
        case Fields::Field_firstReleased: return lambda("firstReleased", firstReleased()); return;
        case Fields::Field_votes: return lambda("votes", votes()); return;
        case Fields::Field_title: return lambda("title", title()); return;
        case Fields::Field_certificate: return lambda("certificate", certificate()); return;
        case Fields::Field_plot: return lambda("plot", plot()); return;
        case Fields::Field_season: return lambda("season", season()); return;
        case Fields::Field_episode: return lambda("episode", episode()); return;
        case Fields::Field_showId: return lambda("showId", showId()); return;
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "imdbId") { return lambda(Fields::Field_imdbId, imdbId()); }
        if (fieldName == "type") { return lambda(Fields::Field_type, type()); }
        if (fieldName == "ratingM10") { return lambda(Fields::Field_ratingM10, ratingM10()); }
        if (fieldName == "firstReleased") { return lambda(Fields::Field_firstReleased, firstReleased()); }
        if (fieldName == "votes") { return lambda(Fields::Field_votes, votes()); }
        if (fieldName == "title") { return lambda(Fields::Field_title, title()); }
        if (fieldName == "certificate") { return lambda(Fields::Field_certificate, certificate()); }
        if (fieldName == "plot") { return lambda(Fields::Field_plot, plot()); }
        if (fieldName == "season") { return lambda(Fields::Field_season, season()); }
        if (fieldName == "episode") { return lambda(Fields::Field_episode, episode()); }
        if (fieldName == "showId") { return lambda(Fields::Field_showId, showId()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("imdbId", Fields::Field_imdbId, imdbId(), Obj().imdbId);
        lambda("type", Fields::Field_type, type(), Obj().type);
        lambda("ratingM10", Fields::Field_ratingM10, ratingM10(), Obj().ratingM10);
        lambda("firstReleased", Fields::Field_firstReleased, firstReleased(), Obj().firstReleased);
        lambda("votes", Fields::Field_votes, votes(), Obj().votes);
        lambda("title", Fields::Field_title, title(), Obj().title);
        lambda("certificate", Fields::Field_certificate, certificate(), Obj().certificate);
        lambda("plot", Fields::Field_plot, plot(), Obj().plot);
        lambda("season", Fields::Field_season, season(), Obj().season);
        lambda("episode", Fields::Field_episode, episode(), Obj().episode);
        lambda("showId", Fields::Field_showId, showId(), Obj().showId);
    }

    void Flush()
    {
        imdbId().Flush();

        if (IsFieldEdited(Fields::Field_imdbId))
        {
            if (!imdbId().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_imdbId));
        }

        type().Flush();

        if (IsFieldEdited(Fields::Field_type))
        {
            if (!type().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_type));
        }

        ratingM10().Flush();

        if (IsFieldEdited(Fields::Field_ratingM10))
        {
            if (!ratingM10().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_ratingM10));
        }

        firstReleased().Flush();

        if (IsFieldEdited(Fields::Field_firstReleased))
        {
            if (!firstReleased().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_firstReleased));
        }

        votes().Flush();

        if (IsFieldEdited(Fields::Field_votes))
        {
            if (!votes().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_votes));
        }

        title().Flush();

        if (IsFieldEdited(Fields::Field_title))
        {
            if (!title().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_title));
        }

        certificate().Flush();

        if (IsFieldEdited(Fields::Field_certificate))
        {
            if (!certificate().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_certificate));
        }

        plot().Flush();

        if (IsFieldEdited(Fields::Field_plot))
        {
            if (!plot().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_plot));
        }

        season().Flush();

        if (IsFieldEdited(Fields::Field_season))
        {
            if (!season().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_season));
        }

        episode().Flush();

        if (IsFieldEdited(Fields::Field_episode))
        {
            if (!episode().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_episode));
        }

        showId().Flush();

        if (IsFieldEdited(Fields::Field_showId))
        {
            if (!showId().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_showId));
        }

        Stencil::TransactionT<Catalog::ImdbInfo>::Flush_();
    }
};

template <> struct Stencil::Visitor<Catalog::ImdbInfo> : Stencil::VisitorT<Catalog::ImdbInfo>
{
    using TData  = Catalog::ImdbInfo;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey(T& obj, Fields field, TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_imdbId: return lambda(obj.imdbId);
        case Fields::Field_type: return lambda(obj.type);
        case Fields::Field_ratingM10: return lambda(obj.ratingM10);
        case Fields::Field_firstReleased: return lambda(obj.firstReleased);
        case Fields::Field_votes: return lambda(obj.votes);
        case Fields::Field_title: return lambda(obj.title);
        case Fields::Field_certificate: return lambda(obj.certificate);
        case Fields::Field_plot: return lambda(obj.plot);
        case Fields::Field_season: return lambda(obj.season);
        case Fields::Field_episode: return lambda(obj.episode);
        case Fields::Field_showId: return lambda(obj.showId);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies(T& obj, TLambda&& lambda)
    {
        lambda(Fields::Field_imdbId, obj.imdbId);
        lambda(Fields::Field_type, obj.type);
        lambda(Fields::Field_ratingM10, obj.ratingM10);
        lambda(Fields::Field_firstReleased, obj.firstReleased);
        lambda(Fields::Field_votes, obj.votes);
        lambda(Fields::Field_title, obj.title);
        lambda(Fields::Field_certificate, obj.certificate);
        lambda(Fields::Field_plot, obj.plot);
        lambda(Fields::Field_season, obj.season);
        lambda(Fields::Field_episode, obj.episode);
        lambda(Fields::Field_showId, obj.showId);
    }
};

template <> struct Stencil::TypeTraits<Catalog::Directory>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<Catalog::Directory>
{
    enum class Fields
    {
        Invalid,
        Field_url
,        Field_created
,        Field_lastchecked
,        Field_maxFrequencyInMins
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<Catalog::Directory>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<Catalog::Directory>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "url"
,        "created"
,        "lastchecked"
,        "maxFrequencyInMins"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<Catalog::Directory>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<Catalog::Directory>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<Catalog::Directory> : Stencil::TransactionT<Catalog::Directory>
{
    using TData  = Catalog::Directory;
    using Fields = Stencil::TypeTraitsForIndexable<Catalog::Directory>::Fields;

    Transaction<shared_string> _subtracker_url;
    Transaction<Stencil::Timestamp> _subtracker_created;
    Transaction<Stencil::Timestamp> _subtracker_lastchecked;
    Transaction<uint32_t> _subtracker_maxFrequencyInMins;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<Catalog::Directory>(ptr)
        ,
        _subtracker_url(Obj().url)
        ,
        _subtracker_created(Obj().created)
        ,
        _subtracker_lastchecked(Obj().lastchecked)
        ,
        _subtracker_maxFrequencyInMins(Obj().maxFrequencyInMins)
    {}

    auto& url()
    {
        MarkFieldEdited_(Fields::Field_url);
        return _subtracker_url;
    }
    auto& created()
    {
        MarkFieldEdited_(Fields::Field_created);
        return _subtracker_created;
    }
    auto& lastchecked()
    {
        MarkFieldEdited_(Fields::Field_lastchecked);
        return _subtracker_lastchecked;
    }
    auto& maxFrequencyInMins()
    {
        MarkFieldEdited_(Fields::Field_maxFrequencyInMins);
        return _subtracker_maxFrequencyInMins;
    }
    void set_url(shared_string&& val)
    {
        MarkFieldAssigned_(Fields::Field_url, Obj().url, val);
        Obj().url = std::move(val);
    }

    void set_created(Stencil::Timestamp&& val)
    {
        MarkFieldAssigned_(Fields::Field_created, Obj().created, val);
        Obj().created = std::move(val);
    }

    void set_lastchecked(Stencil::Timestamp&& val)
    {
        MarkFieldAssigned_(Fields::Field_lastchecked, Obj().lastchecked, val);
        Obj().lastchecked = std::move(val);
    }

    void set_maxFrequencyInMins(uint32_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_maxFrequencyInMins, Obj().maxFrequencyInMins, val);
        Obj().maxFrequencyInMins = std::move(val);
    }

    template <typename TLambda> auto Visit(Fields index, TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_url: return lambda("url", url()); return;
        case Fields::Field_created: return lambda("created", created()); return;
        case Fields::Field_lastchecked: return lambda("lastchecked", lastchecked()); return;
        case Fields::Field_maxFrequencyInMins: return lambda("maxFrequencyInMins", maxFrequencyInMins()); return;
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "url") { return lambda(Fields::Field_url, url()); }
        if (fieldName == "created") { return lambda(Fields::Field_created, created()); }
        if (fieldName == "lastchecked") { return lambda(Fields::Field_lastchecked, lastchecked()); }
        if (fieldName == "maxFrequencyInMins") { return lambda(Fields::Field_maxFrequencyInMins, maxFrequencyInMins()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("url", Fields::Field_url, url(), Obj().url);
        lambda("created", Fields::Field_created, created(), Obj().created);
        lambda("lastchecked", Fields::Field_lastchecked, lastchecked(), Obj().lastchecked);
        lambda("maxFrequencyInMins", Fields::Field_maxFrequencyInMins, maxFrequencyInMins(), Obj().maxFrequencyInMins);
    }

    void Flush()
    {
        url().Flush();

        if (IsFieldEdited(Fields::Field_url))
        {
            if (!url().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_url));
        }

        created().Flush();

        if (IsFieldEdited(Fields::Field_created))
        {
            if (!created().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_created));
        }

        lastchecked().Flush();

        if (IsFieldEdited(Fields::Field_lastchecked))
        {
            if (!lastchecked().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_lastchecked));
        }

        maxFrequencyInMins().Flush();

        if (IsFieldEdited(Fields::Field_maxFrequencyInMins))
        {
            if (!maxFrequencyInMins().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_maxFrequencyInMins));
        }

        Stencil::TransactionT<Catalog::Directory>::Flush_();
    }
};

template <> struct Stencil::Visitor<Catalog::Directory> : Stencil::VisitorT<Catalog::Directory>
{
    using TData  = Catalog::Directory;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey(T& obj, Fields field, TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_url: return lambda(obj.url);
        case Fields::Field_created: return lambda(obj.created);
        case Fields::Field_lastchecked: return lambda(obj.lastchecked);
        case Fields::Field_maxFrequencyInMins: return lambda(obj.maxFrequencyInMins);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies(T& obj, TLambda&& lambda)
    {
        lambda(Fields::Field_url, obj.url);
        lambda(Fields::Field_created, obj.created);
        lambda(Fields::Field_lastchecked, obj.lastchecked);
        lambda(Fields::Field_maxFrequencyInMins, obj.maxFrequencyInMins);
    }
};

template <> struct Stencil::TypeTraits<Catalog::File>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<Catalog::File>
{
    enum class Fields
    {
        Invalid,
        Field_dirId
,        Field_imdbId
,        Field_url
,        Field_created
,        Field_fsize
,        Field_modified
,        Field_seen
,        Field_queued
,        Field_pinned
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<Catalog::File>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<Catalog::File>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "dirId"
,        "imdbId"
,        "url"
,        "created"
,        "fsize"
,        "modified"
,        "seen"
,        "queued"
,        "pinned"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<Catalog::File>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<Catalog::File>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<Catalog::File> : Stencil::TransactionT<Catalog::File>
{
    using TData  = Catalog::File;
    using Fields = Stencil::TypeTraitsForIndexable<Catalog::File>::Fields;

    Transaction<::UuidBasedId<::Catalog::Directory>> _subtracker_dirId;
    Transaction<::UuidBasedId<::Catalog::ImdbInfo>> _subtracker_imdbId;
    Transaction<shared_string> _subtracker_url;
    Transaction<Stencil::Timestamp> _subtracker_created;
    Transaction<uint64_t> _subtracker_fsize;
    Transaction<Stencil::Timestamp> _subtracker_modified;
    Transaction<Stencil::Timestamp> _subtracker_seen;
    Transaction<bool> _subtracker_queued;
    Transaction<bool> _subtracker_pinned;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<Catalog::File>(ptr)
        ,
        _subtracker_dirId(Obj().dirId)
        ,
        _subtracker_imdbId(Obj().imdbId)
        ,
        _subtracker_url(Obj().url)
        ,
        _subtracker_created(Obj().created)
        ,
        _subtracker_fsize(Obj().fsize)
        ,
        _subtracker_modified(Obj().modified)
        ,
        _subtracker_seen(Obj().seen)
        ,
        _subtracker_queued(Obj().queued)
        ,
        _subtracker_pinned(Obj().pinned)
    {}

    auto& dirId()
    {
        MarkFieldEdited_(Fields::Field_dirId);
        return _subtracker_dirId;
    }
    auto& imdbId()
    {
        MarkFieldEdited_(Fields::Field_imdbId);
        return _subtracker_imdbId;
    }
    auto& url()
    {
        MarkFieldEdited_(Fields::Field_url);
        return _subtracker_url;
    }
    auto& created()
    {
        MarkFieldEdited_(Fields::Field_created);
        return _subtracker_created;
    }
    auto& fsize()
    {
        MarkFieldEdited_(Fields::Field_fsize);
        return _subtracker_fsize;
    }
    auto& modified()
    {
        MarkFieldEdited_(Fields::Field_modified);
        return _subtracker_modified;
    }
    auto& seen()
    {
        MarkFieldEdited_(Fields::Field_seen);
        return _subtracker_seen;
    }
    auto& queued()
    {
        MarkFieldEdited_(Fields::Field_queued);
        return _subtracker_queued;
    }
    auto& pinned()
    {
        MarkFieldEdited_(Fields::Field_pinned);
        return _subtracker_pinned;
    }
    void set_dirId(::UuidBasedId<::Catalog::Directory>&& val)
    {
        MarkFieldAssigned_(Fields::Field_dirId, Obj().dirId, val);
        Obj().dirId = std::move(val);
    }

    void set_imdbId(::UuidBasedId<::Catalog::ImdbInfo>&& val)
    {
        MarkFieldAssigned_(Fields::Field_imdbId, Obj().imdbId, val);
        Obj().imdbId = std::move(val);
    }

    void set_url(shared_string&& val)
    {
        MarkFieldAssigned_(Fields::Field_url, Obj().url, val);
        Obj().url = std::move(val);
    }

    void set_created(Stencil::Timestamp&& val)
    {
        MarkFieldAssigned_(Fields::Field_created, Obj().created, val);
        Obj().created = std::move(val);
    }

    void set_fsize(uint64_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_fsize, Obj().fsize, val);
        Obj().fsize = std::move(val);
    }

    void set_modified(Stencil::Timestamp&& val)
    {
        MarkFieldAssigned_(Fields::Field_modified, Obj().modified, val);
        Obj().modified = std::move(val);
    }

    void set_seen(Stencil::Timestamp&& val)
    {
        MarkFieldAssigned_(Fields::Field_seen, Obj().seen, val);
        Obj().seen = std::move(val);
    }

    void set_queued(bool&& val)
    {
        MarkFieldAssigned_(Fields::Field_queued, Obj().queued, val);
        Obj().queued = std::move(val);
    }

    void set_pinned(bool&& val)
    {
        MarkFieldAssigned_(Fields::Field_pinned, Obj().pinned, val);
        Obj().pinned = std::move(val);
    }

    template <typename TLambda> auto Visit(Fields index, TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_dirId: return lambda("dirId", dirId()); return;
        case Fields::Field_imdbId: return lambda("imdbId", imdbId()); return;
        case Fields::Field_url: return lambda("url", url()); return;
        case Fields::Field_created: return lambda("created", created()); return;
        case Fields::Field_fsize: return lambda("fsize", fsize()); return;
        case Fields::Field_modified: return lambda("modified", modified()); return;
        case Fields::Field_seen: return lambda("seen", seen()); return;
        case Fields::Field_queued: return lambda("queued", queued()); return;
        case Fields::Field_pinned: return lambda("pinned", pinned()); return;
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "dirId") { return lambda(Fields::Field_dirId, dirId()); }
        if (fieldName == "imdbId") { return lambda(Fields::Field_imdbId, imdbId()); }
        if (fieldName == "url") { return lambda(Fields::Field_url, url()); }
        if (fieldName == "created") { return lambda(Fields::Field_created, created()); }
        if (fieldName == "fsize") { return lambda(Fields::Field_fsize, fsize()); }
        if (fieldName == "modified") { return lambda(Fields::Field_modified, modified()); }
        if (fieldName == "seen") { return lambda(Fields::Field_seen, seen()); }
        if (fieldName == "queued") { return lambda(Fields::Field_queued, queued()); }
        if (fieldName == "pinned") { return lambda(Fields::Field_pinned, pinned()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("dirId", Fields::Field_dirId, dirId(), Obj().dirId);
        lambda("imdbId", Fields::Field_imdbId, imdbId(), Obj().imdbId);
        lambda("url", Fields::Field_url, url(), Obj().url);
        lambda("created", Fields::Field_created, created(), Obj().created);
        lambda("fsize", Fields::Field_fsize, fsize(), Obj().fsize);
        lambda("modified", Fields::Field_modified, modified(), Obj().modified);
        lambda("seen", Fields::Field_seen, seen(), Obj().seen);
        lambda("queued", Fields::Field_queued, queued(), Obj().queued);
        lambda("pinned", Fields::Field_pinned, pinned(), Obj().pinned);
    }

    void Flush()
    {
        dirId().Flush();

        if (IsFieldEdited(Fields::Field_dirId))
        {
            if (!dirId().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_dirId));
        }

        imdbId().Flush();

        if (IsFieldEdited(Fields::Field_imdbId))
        {
            if (!imdbId().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_imdbId));
        }

        url().Flush();

        if (IsFieldEdited(Fields::Field_url))
        {
            if (!url().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_url));
        }

        created().Flush();

        if (IsFieldEdited(Fields::Field_created))
        {
            if (!created().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_created));
        }

        fsize().Flush();

        if (IsFieldEdited(Fields::Field_fsize))
        {
            if (!fsize().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_fsize));
        }

        modified().Flush();

        if (IsFieldEdited(Fields::Field_modified))
        {
            if (!modified().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_modified));
        }

        seen().Flush();

        if (IsFieldEdited(Fields::Field_seen))
        {
            if (!seen().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_seen));
        }

        queued().Flush();

        if (IsFieldEdited(Fields::Field_queued))
        {
            if (!queued().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_queued));
        }

        pinned().Flush();

        if (IsFieldEdited(Fields::Field_pinned))
        {
            if (!pinned().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_pinned));
        }

        Stencil::TransactionT<Catalog::File>::Flush_();
    }
};

template <> struct Stencil::Visitor<Catalog::File> : Stencil::VisitorT<Catalog::File>
{
    using TData  = Catalog::File;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey(T& obj, Fields field, TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_dirId: return lambda(obj.dirId);
        case Fields::Field_imdbId: return lambda(obj.imdbId);
        case Fields::Field_url: return lambda(obj.url);
        case Fields::Field_created: return lambda(obj.created);
        case Fields::Field_fsize: return lambda(obj.fsize);
        case Fields::Field_modified: return lambda(obj.modified);
        case Fields::Field_seen: return lambda(obj.seen);
        case Fields::Field_queued: return lambda(obj.queued);
        case Fields::Field_pinned: return lambda(obj.pinned);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies(T& obj, TLambda&& lambda)
    {
        lambda(Fields::Field_dirId, obj.dirId);
        lambda(Fields::Field_imdbId, obj.imdbId);
        lambda(Fields::Field_url, obj.url);
        lambda(Fields::Field_created, obj.created);
        lambda(Fields::Field_fsize, obj.fsize);
        lambda(Fields::Field_modified, obj.modified);
        lambda(Fields::Field_seen, obj.seen);
        lambda(Fields::Field_queued, obj.queued);
        lambda(Fields::Field_pinned, obj.pinned);
    }
};

template <> struct Stencil::TypeTraits<Catalog::ScanRequest>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<Catalog::ScanRequest>
{
    enum class Fields
    {
        Invalid,
        Field_dirId
,        Field_scannedAt
,        Field_statusCode
,        Field_statusDesc
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<Catalog::ScanRequest>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<Catalog::ScanRequest>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "dirId"
,        "scannedAt"
,        "statusCode"
,        "statusDesc"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<Catalog::ScanRequest>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<Catalog::ScanRequest>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<Catalog::ScanRequest> : Stencil::TransactionT<Catalog::ScanRequest>
{
    using TData  = Catalog::ScanRequest;
    using Fields = Stencil::TypeTraitsForIndexable<Catalog::ScanRequest>::Fields;

    Transaction<::UuidBasedId<::Catalog::Directory>> _subtracker_dirId;
    Transaction<Stencil::Timestamp> _subtracker_scannedAt;
    Transaction<uint32_t> _subtracker_statusCode;
    Transaction<shared_string> _subtracker_statusDesc;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<Catalog::ScanRequest>(ptr)
        ,
        _subtracker_dirId(Obj().dirId)
        ,
        _subtracker_scannedAt(Obj().scannedAt)
        ,
        _subtracker_statusCode(Obj().statusCode)
        ,
        _subtracker_statusDesc(Obj().statusDesc)
    {}

    auto& dirId()
    {
        MarkFieldEdited_(Fields::Field_dirId);
        return _subtracker_dirId;
    }
    auto& scannedAt()
    {
        MarkFieldEdited_(Fields::Field_scannedAt);
        return _subtracker_scannedAt;
    }
    auto& statusCode()
    {
        MarkFieldEdited_(Fields::Field_statusCode);
        return _subtracker_statusCode;
    }
    auto& statusDesc()
    {
        MarkFieldEdited_(Fields::Field_statusDesc);
        return _subtracker_statusDesc;
    }
    void set_dirId(::UuidBasedId<::Catalog::Directory>&& val)
    {
        MarkFieldAssigned_(Fields::Field_dirId, Obj().dirId, val);
        Obj().dirId = std::move(val);
    }

    void set_scannedAt(Stencil::Timestamp&& val)
    {
        MarkFieldAssigned_(Fields::Field_scannedAt, Obj().scannedAt, val);
        Obj().scannedAt = std::move(val);
    }

    void set_statusCode(uint32_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_statusCode, Obj().statusCode, val);
        Obj().statusCode = std::move(val);
    }

    void set_statusDesc(shared_string&& val)
    {
        MarkFieldAssigned_(Fields::Field_statusDesc, Obj().statusDesc, val);
        Obj().statusDesc = std::move(val);
    }

    template <typename TLambda> auto Visit(Fields index, TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_dirId: return lambda("dirId", dirId()); return;
        case Fields::Field_scannedAt: return lambda("scannedAt", scannedAt()); return;
        case Fields::Field_statusCode: return lambda("statusCode", statusCode()); return;
        case Fields::Field_statusDesc: return lambda("statusDesc", statusDesc()); return;
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "dirId") { return lambda(Fields::Field_dirId, dirId()); }
        if (fieldName == "scannedAt") { return lambda(Fields::Field_scannedAt, scannedAt()); }
        if (fieldName == "statusCode") { return lambda(Fields::Field_statusCode, statusCode()); }
        if (fieldName == "statusDesc") { return lambda(Fields::Field_statusDesc, statusDesc()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("dirId", Fields::Field_dirId, dirId(), Obj().dirId);
        lambda("scannedAt", Fields::Field_scannedAt, scannedAt(), Obj().scannedAt);
        lambda("statusCode", Fields::Field_statusCode, statusCode(), Obj().statusCode);
        lambda("statusDesc", Fields::Field_statusDesc, statusDesc(), Obj().statusDesc);
    }

    void Flush()
    {
        dirId().Flush();

        if (IsFieldEdited(Fields::Field_dirId))
        {
            if (!dirId().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_dirId));
        }

        scannedAt().Flush();

        if (IsFieldEdited(Fields::Field_scannedAt))
        {
            if (!scannedAt().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_scannedAt));
        }

        statusCode().Flush();

        if (IsFieldEdited(Fields::Field_statusCode))
        {
            if (!statusCode().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_statusCode));
        }

        statusDesc().Flush();

        if (IsFieldEdited(Fields::Field_statusDesc))
        {
            if (!statusDesc().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_statusDesc));
        }

        Stencil::TransactionT<Catalog::ScanRequest>::Flush_();
    }
};

template <> struct Stencil::Visitor<Catalog::ScanRequest> : Stencil::VisitorT<Catalog::ScanRequest>
{
    using TData  = Catalog::ScanRequest;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey(T& obj, Fields field, TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_dirId: return lambda(obj.dirId);
        case Fields::Field_scannedAt: return lambda(obj.scannedAt);
        case Fields::Field_statusCode: return lambda(obj.statusCode);
        case Fields::Field_statusDesc: return lambda(obj.statusDesc);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies(T& obj, TLambda&& lambda)
    {
        lambda(Fields::Field_dirId, obj.dirId);
        lambda(Fields::Field_scannedAt, obj.scannedAt);
        lambda(Fields::Field_statusCode, obj.statusCode);
        lambda(Fields::Field_statusDesc, obj.statusDesc);
    }
};

template <> struct Stencil::TypeTraits<Catalog::Catalog_Search_Args>
{
    struct Traits_arg_query
    {
        using TOwner     = Catalog::Catalog_Search_Args;
        using TFieldType = shared_string;

        static constexpr std::string_view Name() { return "query"; }
        static const ::Stencil::Flags     Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TOwner::get_arg_query; }
        static constexpr auto TPropertySetter() { return &TOwner::set_arg_query; }
    };
    static constexpr ::Stencil::DataType Type() { return ::Stencil::DataType::Object; }
    static constexpr std::string_view    Name() { return "Search"; }
    static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

    using ThisType = Catalog::Catalog_Search_Args;

    using Handler = ::ReflectionServices::ReflectedStructHandler<Catalog::Catalog_Search_Args
                                                                     Traits_arg_query
                                                                 >;
};

template <>
Stencil::Comparator<Catalog::Catalog_Search_Args,
                    Catalog::Catalog_Search_Args>{
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2){
        return true
               && Stencil::AreEqual(obj1.get_carg_query(), obj2.get_carg_query())
        ;
}
}
;
template <> struct Stencil::TypeTraits<Catalog::Catalog_RequestDownload_Args>
{
    struct Traits_arg_link
    {
        using TOwner     = Catalog::Catalog_RequestDownload_Args;
        using TFieldType = shared_string;

        static constexpr std::string_view Name() { return "link"; }
        static const ::Stencil::Flags     Flags() { return {}; }
        static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }
        static constexpr auto TPropertyGetter() { return &TOwner::get_arg_link; }
        static constexpr auto TPropertySetter() { return &TOwner::set_arg_link; }
    };
    static constexpr ::Stencil::DataType Type() { return ::Stencil::DataType::Object; }
    static constexpr std::string_view    Name() { return "RequestDownload"; }
    static constexpr auto TAttributeValue(const std::string_view& key) { return ::ReflectionServices::EmptyAttributeValue(key); }

    using ThisType = Catalog::Catalog_RequestDownload_Args;

    using Handler = ::ReflectionServices::ReflectedStructHandler<Catalog::Catalog_RequestDownload_Args
                                                                     Traits_arg_link
                                                                 >;
};

template <>
Stencil::Comparator<Catalog::Catalog_RequestDownload_Args,
                    Catalog::Catalog_RequestDownload_Args>{
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2){
        return true
               && Stencil::AreEqual(obj1.get_carg_link(), obj2.get_carg_link())
        ;
}
}
;
template <> struct Stencil::InterfaceTraits<Catalog::Catalog>
{
    struct ApiTraits_Search
    {
        using TOwner = Catalog::Catalog;
        static const ::Stencil::Flags     Flags() { return {}; }
        static constexpr std::string_view Name() { return "Search"; }
        static constexpr bool             Static = false;
    };
    struct ApiTraits_RequestDownload
    {
        using TOwner = Catalog::Catalog;
        static const ::Stencil::Flags     Flags() { return {}; }
        static constexpr std::string_view Name() { return "RequestDownload"; }
        static constexpr bool             Static = false;
    };
    using Apis = std::tuple<
        ApiTraits_Search
,        ApiTraits_RequestDownload
        >;

    using Objects = std::tuple<
        Catalog::Catalog::ObjectStore_onlinedir
,        Catalog::Catalog::ObjectStore_onlinefile
,        Catalog::Catalog::ObjectStore_onlinescan
,        Catalog::Catalog::ObjectStore_localdir
,        Catalog::Catalog::ObjectStore_localfile
,        Catalog::Catalog::ObjectStore_localscan
,        Catalog::Catalog::ObjectStore_imdbinfo
        >;
};

template <> struct Stencil::TypeTraits<::Catalog::Directory>;
template <>
struct Stencil::TypeTraits<Catalog::Catalog::ObjectStore_onlinedir&>
    : public Stencil::TypeTraits<::Catalog::Directory&>
{};
template <> struct Stencil::TypeTraits<::Catalog::File>;
template <>
struct Stencil::TypeTraits<Catalog::Catalog::ObjectStore_onlinefile&>
    : public Stencil::TypeTraits<::Catalog::File&>
{};
template <> struct Stencil::TypeTraits<::Catalog::ScanRequest>;
template <>
struct Stencil::TypeTraits<Catalog::Catalog::ObjectStore_onlinescan&>
    : public Stencil::TypeTraits<::Catalog::ScanRequest&>
{};
template <> struct Stencil::TypeTraits<::Catalog::Directory>;
template <>
struct Stencil::TypeTraits<Catalog::Catalog::ObjectStore_localdir&>
    : public Stencil::TypeTraits<::Catalog::Directory&>
{};
template <> struct Stencil::TypeTraits<::Catalog::File>;
template <>
struct Stencil::TypeTraits<Catalog::Catalog::ObjectStore_localfile&>
    : public Stencil::TypeTraits<::Catalog::File&>
{};
template <> struct Stencil::TypeTraits<::Catalog::ScanRequest>;
template <>
struct Stencil::TypeTraits<Catalog::Catalog::ObjectStore_localscan&>
    : public Stencil::TypeTraits<::Catalog::ScanRequest&>
{};
template <> struct Stencil::TypeTraits<::Catalog::ImdbInfo>;
template <>
struct Stencil::TypeTraits<Catalog::Catalog::ObjectStore_imdbinfo&>
    : public Stencil::TypeTraits<::Catalog::ImdbInfo&>
{};
template <> struct Stencil::InterfaceApiTraits<Stencil::InterfaceTraits<Catalog::Catalog>::ApiTraits_Search>
{
    using ArgsStruct = Catalog::Catalog_Search_Args;
    static constexpr bool             IsStatic() { return false; }
    static constexpr std::string_view Name() { return "Search"; }

    static auto Invoke(ArgsStruct& args)
    {
        return args.instance->Search(
            args.get_arg_query()
        );
    }
};

template <> struct Stencil::InterfaceApiTraits<Stencil::InterfaceTraits<Catalog::Catalog>::ApiTraits_RequestDownload>
{
    using ArgsStruct = Catalog::Catalog_RequestDownload_Args;
    static constexpr bool             IsStatic() { return false; }
    static constexpr std::string_view Name() { return "RequestDownload"; }

    static auto Invoke(ArgsStruct& args)
    {
        return args.instance->RequestDownload(
            args.get_arg_link()
        );
    }
};

template <> struct Stencil::InterfaceObjectTraits<Catalog::Catalog::ObjectStore_onlinedir>
{
    static constexpr std::string_view Name() { return "onlinedir"; }
};
template <> struct Stencil::InterfaceObjectTraits<Catalog::Catalog::ObjectStore_onlinefile>
{
    static constexpr std::string_view Name() { return "onlinefile"; }
};
template <> struct Stencil::InterfaceObjectTraits<Catalog::Catalog::ObjectStore_onlinescan>
{
    static constexpr std::string_view Name() { return "onlinescan"; }
};
template <> struct Stencil::InterfaceObjectTraits<Catalog::Catalog::ObjectStore_localdir>
{
    static constexpr std::string_view Name() { return "localdir"; }
};
template <> struct Stencil::InterfaceObjectTraits<Catalog::Catalog::ObjectStore_localfile>
{
    static constexpr std::string_view Name() { return "localfile"; }
};
template <> struct Stencil::InterfaceObjectTraits<Catalog::Catalog::ObjectStore_localscan>
{
    static constexpr std::string_view Name() { return "localscan"; }
};
template <> struct Stencil::InterfaceObjectTraits<Catalog::Catalog::ObjectStore_imdbinfo>
{
    static constexpr std::string_view Name() { return "imdbinfo"; }
};
#if ((defined STENCIL_USING_WEBSERVICE) and (STENCIL_USING_WEBSERVICE > 0))
template <> struct Stencil::WebServiceHandlerTraits<Catalog::Catalog>
{
    static constexpr const std::string_view Url() { return std::string_view("Catalog"); }
    // Name to api enum
    // Name to funcptr mapping
    // Name to argstruct mapping
    // Invoke api for each arg struct
    static void HandleRequest(Catalog::Catalog& obj,
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
