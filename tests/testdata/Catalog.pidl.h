#pragma once
#include <stencil/stencil.h>

// SECTION START: DECLARATIONS
#if true

namespace Catalog
{
struct ImdbInfo;
struct FolderInfo;
struct Source;
struct ContentInfo;
struct Folder;
struct Data;
struct ScanRequest;
}    // namespace Catalog

template <> struct Stencil::TypeTraits<Catalog::ImdbInfo>;
template <> struct Stencil::TypeTraits<Catalog::FolderInfo>;
template <> struct Stencil::TypeTraits<Catalog::Source>;
template <> struct Stencil::TypeTraits<Catalog::ContentInfo>;
template <> struct Stencil::TypeTraits<Catalog::Folder>;
template <> struct Stencil::TypeTraits<Catalog::Data>;
template <> struct Stencil::TypeTraits<Catalog::ScanRequest>;
#endif
// SECTION END: DECLARATIONS

// SECTION START: Definitions
#if true
namespace Catalog
{
struct ImdbInfo :
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
struct FolderInfo :
    public Stencil::StructT<FolderInfo>
{
};
struct Source :
    public Stencil::StructT<Source>
{
    char type = {};
    shared_string path = {};
    Stencil::Timestamp added = {};
    Stencil::Timestamp last_checked = {};
    uint32_t frequencyInSecs = {};
    shared_string status_info = {};
};
struct ContentInfo :
    public Stencil::StructT<ContentInfo>
{
    char type = {};
    Stencil::Ref<::Catalog::Source> source = {};
    Stencil::Ref<::Catalog::FolderInfo> folder = {};
    shared_string path = {};
    uint64_t size = {};
    Stencil::Timestamp added = {};
    Stencil::Timestamp modified = {};
    shared_string status_info = {};
};
struct Folder :
    public Stencil::StructT<Folder>
{
    Stencil::Ref<::Catalog::FolderInfo> info = {};
    std::vector<Stencil::Ref<::Catalog::ContentInfo>> contents = {};
};
struct Data :
    public Stencil::StructT<Data>
{
    Stencil::RefMap<::Catalog::ContentInfo> contentinfo = {};
    Stencil::RefMap<::Catalog::Source> sources = {};
    Stencil::RefMap<::Catalog::FolderInfo> folderinfo = {};
    shared_tree<::Catalog::Folder> rootfolders = {};
};
struct ScanRequest :
    public Stencil::StructT<ScanRequest>
{
    Stencil::Ref<::Catalog::Source> source = {};
    Stencil::Timestamp scanned_at = {};
    uint32_t status_code = {};
    shared_string status_info = {};
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
        case Fields::Field_imdbId: return lambda("imdbId", imdbId());
        case Fields::Field_type: return lambda("type", type());
        case Fields::Field_ratingM10: return lambda("ratingM10", ratingM10());
        case Fields::Field_firstReleased: return lambda("firstReleased", firstReleased());
        case Fields::Field_votes: return lambda("votes", votes());
        case Fields::Field_title: return lambda("title", title());
        case Fields::Field_certificate: return lambda("certificate", certificate());
        case Fields::Field_plot: return lambda("plot", plot());
        case Fields::Field_season: return lambda("season", season());
        case Fields::Field_episode: return lambda("episode", episode());
        case Fields::Field_showId: return lambda("showId", showId());
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

template <> struct Stencil::TypeTraits<Catalog::FolderInfo>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<Catalog::FolderInfo>
{
    enum class Fields
    {
        Invalid,
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<Catalog::FolderInfo>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<Catalog::FolderInfo>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<Catalog::FolderInfo>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<Catalog::FolderInfo>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<Catalog::FolderInfo> : Stencil::TransactionT<Catalog::FolderInfo>
{
    using TData  = Catalog::FolderInfo;
    using Fields = Stencil::TypeTraitsForIndexable<Catalog::FolderInfo>::Fields;

    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<Catalog::FolderInfo>(ptr)
    {}

    template <typename TLambda> auto Visit(Fields index, TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
    }

    void Flush()
    {
        Stencil::TransactionT<Catalog::FolderInfo>::Flush_();
    }
};

template <> struct Stencil::Visitor<Catalog::FolderInfo> : Stencil::VisitorT<Catalog::FolderInfo>
{
    using TData  = Catalog::FolderInfo;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey(T& obj, Fields field, TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies(T& obj, TLambda&& lambda)
    {
    }
};

template <> struct Stencil::TypeTraits<Catalog::Source>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<Catalog::Source>
{
    enum class Fields
    {
        Invalid,
        Field_type
,        Field_path
,        Field_added
,        Field_last_checked
,        Field_frequencyInSecs
,        Field_status_info
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<Catalog::Source>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<Catalog::Source>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "type"
,        "path"
,        "added"
,        "last_checked"
,        "frequencyInSecs"
,        "status_info"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<Catalog::Source>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<Catalog::Source>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<Catalog::Source> : Stencil::TransactionT<Catalog::Source>
{
    using TData  = Catalog::Source;
    using Fields = Stencil::TypeTraitsForIndexable<Catalog::Source>::Fields;

    Transaction<char> _subtracker_type;
    Transaction<shared_string> _subtracker_path;
    Transaction<Stencil::Timestamp> _subtracker_added;
    Transaction<Stencil::Timestamp> _subtracker_last_checked;
    Transaction<uint32_t> _subtracker_frequencyInSecs;
    Transaction<shared_string> _subtracker_status_info;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<Catalog::Source>(ptr)
        ,
        _subtracker_type(Obj().type)
        ,
        _subtracker_path(Obj().path)
        ,
        _subtracker_added(Obj().added)
        ,
        _subtracker_last_checked(Obj().last_checked)
        ,
        _subtracker_frequencyInSecs(Obj().frequencyInSecs)
        ,
        _subtracker_status_info(Obj().status_info)
    {}

    auto& type()
    {
        MarkFieldEdited_(Fields::Field_type);
        return _subtracker_type;
    }
    auto& path()
    {
        MarkFieldEdited_(Fields::Field_path);
        return _subtracker_path;
    }
    auto& added()
    {
        MarkFieldEdited_(Fields::Field_added);
        return _subtracker_added;
    }
    auto& last_checked()
    {
        MarkFieldEdited_(Fields::Field_last_checked);
        return _subtracker_last_checked;
    }
    auto& frequencyInSecs()
    {
        MarkFieldEdited_(Fields::Field_frequencyInSecs);
        return _subtracker_frequencyInSecs;
    }
    auto& status_info()
    {
        MarkFieldEdited_(Fields::Field_status_info);
        return _subtracker_status_info;
    }
    void set_type(char&& val)
    {
        MarkFieldAssigned_(Fields::Field_type, Obj().type, val);
        Obj().type = std::move(val);
    }

    void set_path(shared_string&& val)
    {
        MarkFieldAssigned_(Fields::Field_path, Obj().path, val);
        Obj().path = std::move(val);
    }

    void set_added(Stencil::Timestamp&& val)
    {
        MarkFieldAssigned_(Fields::Field_added, Obj().added, val);
        Obj().added = std::move(val);
    }

    void set_last_checked(Stencil::Timestamp&& val)
    {
        MarkFieldAssigned_(Fields::Field_last_checked, Obj().last_checked, val);
        Obj().last_checked = std::move(val);
    }

    void set_frequencyInSecs(uint32_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_frequencyInSecs, Obj().frequencyInSecs, val);
        Obj().frequencyInSecs = std::move(val);
    }

    void set_status_info(shared_string&& val)
    {
        MarkFieldAssigned_(Fields::Field_status_info, Obj().status_info, val);
        Obj().status_info = std::move(val);
    }

    template <typename TLambda> auto Visit(Fields index, TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_type: return lambda("type", type());
        case Fields::Field_path: return lambda("path", path());
        case Fields::Field_added: return lambda("added", added());
        case Fields::Field_last_checked: return lambda("last_checked", last_checked());
        case Fields::Field_frequencyInSecs: return lambda("frequencyInSecs", frequencyInSecs());
        case Fields::Field_status_info: return lambda("status_info", status_info());
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "type") { return lambda(Fields::Field_type, type()); }
        if (fieldName == "path") { return lambda(Fields::Field_path, path()); }
        if (fieldName == "added") { return lambda(Fields::Field_added, added()); }
        if (fieldName == "last_checked") { return lambda(Fields::Field_last_checked, last_checked()); }
        if (fieldName == "frequencyInSecs") { return lambda(Fields::Field_frequencyInSecs, frequencyInSecs()); }
        if (fieldName == "status_info") { return lambda(Fields::Field_status_info, status_info()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("type", Fields::Field_type, type(), Obj().type);
        lambda("path", Fields::Field_path, path(), Obj().path);
        lambda("added", Fields::Field_added, added(), Obj().added);
        lambda("last_checked", Fields::Field_last_checked, last_checked(), Obj().last_checked);
        lambda("frequencyInSecs", Fields::Field_frequencyInSecs, frequencyInSecs(), Obj().frequencyInSecs);
        lambda("status_info", Fields::Field_status_info, status_info(), Obj().status_info);
    }

    void Flush()
    {
        type().Flush();

        if (IsFieldEdited(Fields::Field_type))
        {
            if (!type().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_type));
        }

        path().Flush();

        if (IsFieldEdited(Fields::Field_path))
        {
            if (!path().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_path));
        }

        added().Flush();

        if (IsFieldEdited(Fields::Field_added))
        {
            if (!added().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_added));
        }

        last_checked().Flush();

        if (IsFieldEdited(Fields::Field_last_checked))
        {
            if (!last_checked().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_last_checked));
        }

        frequencyInSecs().Flush();

        if (IsFieldEdited(Fields::Field_frequencyInSecs))
        {
            if (!frequencyInSecs().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_frequencyInSecs));
        }

        status_info().Flush();

        if (IsFieldEdited(Fields::Field_status_info))
        {
            if (!status_info().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_status_info));
        }

        Stencil::TransactionT<Catalog::Source>::Flush_();
    }
};

template <> struct Stencil::Visitor<Catalog::Source> : Stencil::VisitorT<Catalog::Source>
{
    using TData  = Catalog::Source;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey(T& obj, Fields field, TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_type: return lambda(obj.type);
        case Fields::Field_path: return lambda(obj.path);
        case Fields::Field_added: return lambda(obj.added);
        case Fields::Field_last_checked: return lambda(obj.last_checked);
        case Fields::Field_frequencyInSecs: return lambda(obj.frequencyInSecs);
        case Fields::Field_status_info: return lambda(obj.status_info);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies(T& obj, TLambda&& lambda)
    {
        lambda(Fields::Field_type, obj.type);
        lambda(Fields::Field_path, obj.path);
        lambda(Fields::Field_added, obj.added);
        lambda(Fields::Field_last_checked, obj.last_checked);
        lambda(Fields::Field_frequencyInSecs, obj.frequencyInSecs);
        lambda(Fields::Field_status_info, obj.status_info);
    }
};

template <> struct Stencil::TypeTraits<Catalog::ContentInfo>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<Catalog::ContentInfo>
{
    enum class Fields
    {
        Invalid,
        Field_type
,        Field_source
,        Field_folder
,        Field_path
,        Field_size
,        Field_added
,        Field_modified
,        Field_status_info
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<Catalog::ContentInfo>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<Catalog::ContentInfo>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "type"
,        "source"
,        "folder"
,        "path"
,        "size"
,        "added"
,        "modified"
,        "status_info"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<Catalog::ContentInfo>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<Catalog::ContentInfo>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<Catalog::ContentInfo> : Stencil::TransactionT<Catalog::ContentInfo>
{
    using TData  = Catalog::ContentInfo;
    using Fields = Stencil::TypeTraitsForIndexable<Catalog::ContentInfo>::Fields;

    Transaction<char> _subtracker_type;
    Transaction<Stencil::Ref<::Catalog::Source>> _subtracker_source;
    Transaction<Stencil::Ref<::Catalog::FolderInfo>> _subtracker_folder;
    Transaction<shared_string> _subtracker_path;
    Transaction<uint64_t> _subtracker_size;
    Transaction<Stencil::Timestamp> _subtracker_added;
    Transaction<Stencil::Timestamp> _subtracker_modified;
    Transaction<shared_string> _subtracker_status_info;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<Catalog::ContentInfo>(ptr)
        ,
        _subtracker_type(Obj().type)
        ,
        _subtracker_source(Obj().source)
        ,
        _subtracker_folder(Obj().folder)
        ,
        _subtracker_path(Obj().path)
        ,
        _subtracker_size(Obj().size)
        ,
        _subtracker_added(Obj().added)
        ,
        _subtracker_modified(Obj().modified)
        ,
        _subtracker_status_info(Obj().status_info)
    {}

    auto& type()
    {
        MarkFieldEdited_(Fields::Field_type);
        return _subtracker_type;
    }
    auto& source()
    {
        MarkFieldEdited_(Fields::Field_source);
        return _subtracker_source;
    }
    auto& folder()
    {
        MarkFieldEdited_(Fields::Field_folder);
        return _subtracker_folder;
    }
    auto& path()
    {
        MarkFieldEdited_(Fields::Field_path);
        return _subtracker_path;
    }
    auto& size()
    {
        MarkFieldEdited_(Fields::Field_size);
        return _subtracker_size;
    }
    auto& added()
    {
        MarkFieldEdited_(Fields::Field_added);
        return _subtracker_added;
    }
    auto& modified()
    {
        MarkFieldEdited_(Fields::Field_modified);
        return _subtracker_modified;
    }
    auto& status_info()
    {
        MarkFieldEdited_(Fields::Field_status_info);
        return _subtracker_status_info;
    }
    void set_type(char&& val)
    {
        MarkFieldAssigned_(Fields::Field_type, Obj().type, val);
        Obj().type = std::move(val);
    }

    void set_source(Stencil::Ref<::Catalog::Source>&& val)
    {
        MarkFieldAssigned_(Fields::Field_source, Obj().source, val);
        Obj().source = std::move(val);
    }

    void set_folder(Stencil::Ref<::Catalog::FolderInfo>&& val)
    {
        MarkFieldAssigned_(Fields::Field_folder, Obj().folder, val);
        Obj().folder = std::move(val);
    }

    void set_path(shared_string&& val)
    {
        MarkFieldAssigned_(Fields::Field_path, Obj().path, val);
        Obj().path = std::move(val);
    }

    void set_size(uint64_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_size, Obj().size, val);
        Obj().size = std::move(val);
    }

    void set_added(Stencil::Timestamp&& val)
    {
        MarkFieldAssigned_(Fields::Field_added, Obj().added, val);
        Obj().added = std::move(val);
    }

    void set_modified(Stencil::Timestamp&& val)
    {
        MarkFieldAssigned_(Fields::Field_modified, Obj().modified, val);
        Obj().modified = std::move(val);
    }

    void set_status_info(shared_string&& val)
    {
        MarkFieldAssigned_(Fields::Field_status_info, Obj().status_info, val);
        Obj().status_info = std::move(val);
    }

    template <typename TLambda> auto Visit(Fields index, TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_type: return lambda("type", type());
        case Fields::Field_source: return lambda("source", source());
        case Fields::Field_folder: return lambda("folder", folder());
        case Fields::Field_path: return lambda("path", path());
        case Fields::Field_size: return lambda("size", size());
        case Fields::Field_added: return lambda("added", added());
        case Fields::Field_modified: return lambda("modified", modified());
        case Fields::Field_status_info: return lambda("status_info", status_info());
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "type") { return lambda(Fields::Field_type, type()); }
        if (fieldName == "source") { return lambda(Fields::Field_source, source()); }
        if (fieldName == "folder") { return lambda(Fields::Field_folder, folder()); }
        if (fieldName == "path") { return lambda(Fields::Field_path, path()); }
        if (fieldName == "size") { return lambda(Fields::Field_size, size()); }
        if (fieldName == "added") { return lambda(Fields::Field_added, added()); }
        if (fieldName == "modified") { return lambda(Fields::Field_modified, modified()); }
        if (fieldName == "status_info") { return lambda(Fields::Field_status_info, status_info()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("type", Fields::Field_type, type(), Obj().type);
        lambda("source", Fields::Field_source, source(), Obj().source);
        lambda("folder", Fields::Field_folder, folder(), Obj().folder);
        lambda("path", Fields::Field_path, path(), Obj().path);
        lambda("size", Fields::Field_size, size(), Obj().size);
        lambda("added", Fields::Field_added, added(), Obj().added);
        lambda("modified", Fields::Field_modified, modified(), Obj().modified);
        lambda("status_info", Fields::Field_status_info, status_info(), Obj().status_info);
    }

    void Flush()
    {
        type().Flush();

        if (IsFieldEdited(Fields::Field_type))
        {
            if (!type().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_type));
        }

        source().Flush();

        if (IsFieldEdited(Fields::Field_source))
        {
            if (!source().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_source));
        }

        folder().Flush();

        if (IsFieldEdited(Fields::Field_folder))
        {
            if (!folder().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_folder));
        }

        path().Flush();

        if (IsFieldEdited(Fields::Field_path))
        {
            if (!path().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_path));
        }

        size().Flush();

        if (IsFieldEdited(Fields::Field_size))
        {
            if (!size().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_size));
        }

        added().Flush();

        if (IsFieldEdited(Fields::Field_added))
        {
            if (!added().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_added));
        }

        modified().Flush();

        if (IsFieldEdited(Fields::Field_modified))
        {
            if (!modified().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_modified));
        }

        status_info().Flush();

        if (IsFieldEdited(Fields::Field_status_info))
        {
            if (!status_info().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_status_info));
        }

        Stencil::TransactionT<Catalog::ContentInfo>::Flush_();
    }
};

template <> struct Stencil::Visitor<Catalog::ContentInfo> : Stencil::VisitorT<Catalog::ContentInfo>
{
    using TData  = Catalog::ContentInfo;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey(T& obj, Fields field, TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_type: return lambda(obj.type);
        case Fields::Field_source: return lambda(obj.source);
        case Fields::Field_folder: return lambda(obj.folder);
        case Fields::Field_path: return lambda(obj.path);
        case Fields::Field_size: return lambda(obj.size);
        case Fields::Field_added: return lambda(obj.added);
        case Fields::Field_modified: return lambda(obj.modified);
        case Fields::Field_status_info: return lambda(obj.status_info);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies(T& obj, TLambda&& lambda)
    {
        lambda(Fields::Field_type, obj.type);
        lambda(Fields::Field_source, obj.source);
        lambda(Fields::Field_folder, obj.folder);
        lambda(Fields::Field_path, obj.path);
        lambda(Fields::Field_size, obj.size);
        lambda(Fields::Field_added, obj.added);
        lambda(Fields::Field_modified, obj.modified);
        lambda(Fields::Field_status_info, obj.status_info);
    }
};

template <> struct Stencil::TypeTraits<Catalog::Folder>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<Catalog::Folder>
{
    enum class Fields
    {
        Invalid,
        Field_info
,        Field_contents
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<Catalog::Folder>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<Catalog::Folder>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "info"
,        "contents"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<Catalog::Folder>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<Catalog::Folder>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<Catalog::Folder> : Stencil::TransactionT<Catalog::Folder>
{
    using TData  = Catalog::Folder;
    using Fields = Stencil::TypeTraitsForIndexable<Catalog::Folder>::Fields;

    Transaction<Stencil::Ref<::Catalog::FolderInfo>> _subtracker_info;
    Transaction<std::vector<Stencil::Ref<::Catalog::ContentInfo>>> _subtracker_contents;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<Catalog::Folder>(ptr)
        ,
        _subtracker_info(Obj().info)
        ,
        _subtracker_contents(Obj().contents)
    {}

    auto& info()
    {
        MarkFieldEdited_(Fields::Field_info);
        return _subtracker_info;
    }
    auto& contents()
    {
        MarkFieldEdited_(Fields::Field_contents);
        return _subtracker_contents;
    }
    void set_info(Stencil::Ref<::Catalog::FolderInfo>&& val)
    {
        MarkFieldAssigned_(Fields::Field_info, Obj().info, val);
        Obj().info = std::move(val);
    }

    void set_contents(std::vector<Stencil::Ref<::Catalog::ContentInfo>>&& val)
    {
        MarkFieldAssigned_(Fields::Field_contents, Obj().contents, val);
        Obj().contents = std::move(val);
    }

    void add_contents(Stencil::Ref<::Catalog::ContentInfo>&& args)
    {
        MarkFieldEdited_(Fields::Field_contents);
        return Stencil::Mutators<std::remove_reference_t<decltype(contents())>>::add(contents(), std::move(args));
    }
    void remove_contents(size_t&& args)
    {
        MarkFieldEdited_(Fields::Field_contents);
        return Stencil::Mutators<std::remove_reference_t<decltype(contents())>>::remove(contents(), std::move(args));
    }
    auto& edit_contents(size_t&& args)
    {
        MarkFieldEdited_(Fields::Field_contents);
        return Stencil::Mutators<std::remove_reference_t<decltype(contents())>>::edit(contents(), std::move(args));
    }
    template <typename TLambda> auto Visit(Fields index, TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_info: return lambda("info", info());
        case Fields::Field_contents: return lambda("contents", contents());
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "info") { return lambda(Fields::Field_info, info()); }
        if (fieldName == "contents") { return lambda(Fields::Field_contents, contents()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("info", Fields::Field_info, info(), Obj().info);
        lambda("contents", Fields::Field_contents, contents(), Obj().contents);
    }

    void Flush()
    {
        info().Flush();

        if (IsFieldEdited(Fields::Field_info))
        {
            if (!info().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_info));
        }

        contents().Flush();

        if (IsFieldEdited(Fields::Field_contents))
        {
            if (!contents().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_contents));
        }

        Stencil::TransactionT<Catalog::Folder>::Flush_();
    }
};

template <> struct Stencil::Visitor<Catalog::Folder> : Stencil::VisitorT<Catalog::Folder>
{
    using TData  = Catalog::Folder;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey(T& obj, Fields field, TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_info: return lambda(obj.info);
        case Fields::Field_contents: return lambda(obj.contents);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies(T& obj, TLambda&& lambda)
    {
        lambda(Fields::Field_info, obj.info);
        lambda(Fields::Field_contents, obj.contents);
    }
};

template <> struct Stencil::TypeTraits<Catalog::Data>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<Catalog::Data>
{
    enum class Fields
    {
        Invalid,
        Field_contentinfo
,        Field_sources
,        Field_folderinfo
,        Field_rootfolders
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<Catalog::Data>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<Catalog::Data>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "contentinfo"
,        "sources"
,        "folderinfo"
,        "rootfolders"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<Catalog::Data>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<Catalog::Data>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<Catalog::Data> : Stencil::TransactionT<Catalog::Data>
{
    using TData  = Catalog::Data;
    using Fields = Stencil::TypeTraitsForIndexable<Catalog::Data>::Fields;

    Transaction<Stencil::RefMap<::Catalog::ContentInfo>> _subtracker_contentinfo;
    Transaction<Stencil::RefMap<::Catalog::Source>> _subtracker_sources;
    Transaction<Stencil::RefMap<::Catalog::FolderInfo>> _subtracker_folderinfo;
    Transaction<shared_tree<::Catalog::Folder>> _subtracker_rootfolders;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<Catalog::Data>(ptr)
        ,
        _subtracker_contentinfo(Obj().contentinfo)
        ,
        _subtracker_sources(Obj().sources)
        ,
        _subtracker_folderinfo(Obj().folderinfo)
        ,
        _subtracker_rootfolders(Obj().rootfolders)
    {}

    auto& contentinfo()
    {
        MarkFieldEdited_(Fields::Field_contentinfo);
        return _subtracker_contentinfo;
    }
    auto& sources()
    {
        MarkFieldEdited_(Fields::Field_sources);
        return _subtracker_sources;
    }
    auto& folderinfo()
    {
        MarkFieldEdited_(Fields::Field_folderinfo);
        return _subtracker_folderinfo;
    }
    auto& rootfolders()
    {
        MarkFieldEdited_(Fields::Field_rootfolders);
        return _subtracker_rootfolders;
    }
    void set_contentinfo(Stencil::RefMap<::Catalog::ContentInfo>&& val)
    {
        MarkFieldAssigned_(Fields::Field_contentinfo, Obj().contentinfo, val);
        Obj().contentinfo = std::move(val);
    }

    void set_sources(Stencil::RefMap<::Catalog::Source>&& val)
    {
        MarkFieldAssigned_(Fields::Field_sources, Obj().sources, val);
        Obj().sources = std::move(val);
    }

    void set_folderinfo(Stencil::RefMap<::Catalog::FolderInfo>&& val)
    {
        MarkFieldAssigned_(Fields::Field_folderinfo, Obj().folderinfo, val);
        Obj().folderinfo = std::move(val);
    }

    void set_rootfolders(shared_tree<::Catalog::Folder>&& val)
    {
        MarkFieldAssigned_(Fields::Field_rootfolders, Obj().rootfolders, val);
        Obj().rootfolders = std::move(val);
    }

    template <typename TLambda> auto Visit(Fields index, TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_contentinfo: return lambda("contentinfo", contentinfo());
        case Fields::Field_sources: return lambda("sources", sources());
        case Fields::Field_folderinfo: return lambda("folderinfo", folderinfo());
        case Fields::Field_rootfolders: return lambda("rootfolders", rootfolders());
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "contentinfo") { return lambda(Fields::Field_contentinfo, contentinfo()); }
        if (fieldName == "sources") { return lambda(Fields::Field_sources, sources()); }
        if (fieldName == "folderinfo") { return lambda(Fields::Field_folderinfo, folderinfo()); }
        if (fieldName == "rootfolders") { return lambda(Fields::Field_rootfolders, rootfolders()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("contentinfo", Fields::Field_contentinfo, contentinfo(), Obj().contentinfo);
        lambda("sources", Fields::Field_sources, sources(), Obj().sources);
        lambda("folderinfo", Fields::Field_folderinfo, folderinfo(), Obj().folderinfo);
        lambda("rootfolders", Fields::Field_rootfolders, rootfolders(), Obj().rootfolders);
    }

    void Flush()
    {
        contentinfo().Flush();

        if (IsFieldEdited(Fields::Field_contentinfo))
        {
            if (!contentinfo().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_contentinfo));
        }

        sources().Flush();

        if (IsFieldEdited(Fields::Field_sources))
        {
            if (!sources().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_sources));
        }

        folderinfo().Flush();

        if (IsFieldEdited(Fields::Field_folderinfo))
        {
            if (!folderinfo().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_folderinfo));
        }

        rootfolders().Flush();

        if (IsFieldEdited(Fields::Field_rootfolders))
        {
            if (!rootfolders().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_rootfolders));
        }

        Stencil::TransactionT<Catalog::Data>::Flush_();
    }
};

template <> struct Stencil::Visitor<Catalog::Data> : Stencil::VisitorT<Catalog::Data>
{
    using TData  = Catalog::Data;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey(T& obj, Fields field, TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_contentinfo: return lambda(obj.contentinfo);
        case Fields::Field_sources: return lambda(obj.sources);
        case Fields::Field_folderinfo: return lambda(obj.folderinfo);
        case Fields::Field_rootfolders: return lambda(obj.rootfolders);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies(T& obj, TLambda&& lambda)
    {
        lambda(Fields::Field_contentinfo, obj.contentinfo);
        lambda(Fields::Field_sources, obj.sources);
        lambda(Fields::Field_folderinfo, obj.folderinfo);
        lambda(Fields::Field_rootfolders, obj.rootfolders);
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
        Field_source
,        Field_scanned_at
,        Field_status_code
,        Field_status_info
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<Catalog::ScanRequest>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<Catalog::ScanRequest>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "source"
,        "scanned_at"
,        "status_code"
,        "status_info"
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

    Transaction<Stencil::Ref<::Catalog::Source>> _subtracker_source;
    Transaction<Stencil::Timestamp> _subtracker_scanned_at;
    Transaction<uint32_t> _subtracker_status_code;
    Transaction<shared_string> _subtracker_status_info;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<Catalog::ScanRequest>(ptr)
        ,
        _subtracker_source(Obj().source)
        ,
        _subtracker_scanned_at(Obj().scanned_at)
        ,
        _subtracker_status_code(Obj().status_code)
        ,
        _subtracker_status_info(Obj().status_info)
    {}

    auto& source()
    {
        MarkFieldEdited_(Fields::Field_source);
        return _subtracker_source;
    }
    auto& scanned_at()
    {
        MarkFieldEdited_(Fields::Field_scanned_at);
        return _subtracker_scanned_at;
    }
    auto& status_code()
    {
        MarkFieldEdited_(Fields::Field_status_code);
        return _subtracker_status_code;
    }
    auto& status_info()
    {
        MarkFieldEdited_(Fields::Field_status_info);
        return _subtracker_status_info;
    }
    void set_source(Stencil::Ref<::Catalog::Source>&& val)
    {
        MarkFieldAssigned_(Fields::Field_source, Obj().source, val);
        Obj().source = std::move(val);
    }

    void set_scanned_at(Stencil::Timestamp&& val)
    {
        MarkFieldAssigned_(Fields::Field_scanned_at, Obj().scanned_at, val);
        Obj().scanned_at = std::move(val);
    }

    void set_status_code(uint32_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_status_code, Obj().status_code, val);
        Obj().status_code = std::move(val);
    }

    void set_status_info(shared_string&& val)
    {
        MarkFieldAssigned_(Fields::Field_status_info, Obj().status_info, val);
        Obj().status_info = std::move(val);
    }

    template <typename TLambda> auto Visit(Fields index, TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_source: return lambda("source", source());
        case Fields::Field_scanned_at: return lambda("scanned_at", scanned_at());
        case Fields::Field_status_code: return lambda("status_code", status_code());
        case Fields::Field_status_info: return lambda("status_info", status_info());
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "source") { return lambda(Fields::Field_source, source()); }
        if (fieldName == "scanned_at") { return lambda(Fields::Field_scanned_at, scanned_at()); }
        if (fieldName == "status_code") { return lambda(Fields::Field_status_code, status_code()); }
        if (fieldName == "status_info") { return lambda(Fields::Field_status_info, status_info()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("source", Fields::Field_source, source(), Obj().source);
        lambda("scanned_at", Fields::Field_scanned_at, scanned_at(), Obj().scanned_at);
        lambda("status_code", Fields::Field_status_code, status_code(), Obj().status_code);
        lambda("status_info", Fields::Field_status_info, status_info(), Obj().status_info);
    }

    void Flush()
    {
        source().Flush();

        if (IsFieldEdited(Fields::Field_source))
        {
            if (!source().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_source));
        }

        scanned_at().Flush();

        if (IsFieldEdited(Fields::Field_scanned_at))
        {
            if (!scanned_at().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_scanned_at));
        }

        status_code().Flush();

        if (IsFieldEdited(Fields::Field_status_code))
        {
            if (!status_code().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_status_code));
        }

        status_info().Flush();

        if (IsFieldEdited(Fields::Field_status_info))
        {
            if (!status_info().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_status_info));
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
        case Fields::Field_source: return lambda(obj.source);
        case Fields::Field_scanned_at: return lambda(obj.scanned_at);
        case Fields::Field_status_code: return lambda(obj.status_code);
        case Fields::Field_status_info: return lambda(obj.status_info);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies(T& obj, TLambda&& lambda)
    {
        lambda(Fields::Field_source, obj.source);
        lambda(Fields::Field_scanned_at, obj.scanned_at);
        lambda(Fields::Field_status_code, obj.status_code);
        lambda(Fields::Field_status_info, obj.status_info);
    }
};

#endif
// SECTION END: Template specializations

// SECTION START: Inline Function Definitions
#if true

#endif
// SECTION END: Inline Function Definitions
