#pragma once
#include <stencil/stencil.h>

// SECTION START: DECLARATIONS
#if true

namespace Avid
{
struct GPS;
struct Aircraft;
struct Traffic;
struct Motion;
struct Pressure;
}    // namespace Avid

template <> struct Stencil::TypeTraits<Avid::GPS>;
template <> struct Stencil::TypeTraits<Avid::Aircraft>;
template <> struct Stencil::TypeTraits<Avid::Traffic>;
template <> struct Stencil::TypeTraits<Avid::Motion>;
template <> struct Stencil::TypeTraits<Avid::Pressure>;
#endif
// SECTION END: DECLARATIONS

// SECTION START: Definitions
#if true
namespace Avid
{
struct GPS :
    public ::Stencil::OptionalPropsT<::Avid::GPS>,
    public ::Stencil::TimestampedT<::Avid::GPS>,
    public Stencil::StructT<GPS>
{
    int32_t heading = {};
    double lat = {};
    double lon = {};
    double speed = {};
    Stencil::Timestamp utc = {};
    double galt = {};
    double climb = {};
    double epx = {};
    double epy = {};
    double epv = {};
    double ept = {};
    double eps = {};
    double epc = {};
    double eph = {};
    double sep = {};
};
struct Aircraft :
    public ::Stencil::TimestampedT<::Avid::Aircraft>,
    public Stencil::StructT<Aircraft>
{
    Stencil::Timestamp seen = {};
    uint32_t addr = {};
    std::array<uint8_t, 8> hexaddr = {};
    std::array<char, 9> flight = {};
    int32_t altitude = {};
    int32_t groundSpeed = {};
    int32_t track = {};
    double lat = {};
    double lon = {};
    int32_t verticalRate = {};
    int32_t messageCount = {};
    int32_t odd_cprlat = {};
    int32_t odd_cprlon = {};
    uint64_t odd_cprtime = {};
    int32_t even_cprlat = {};
    int32_t even_cprlon = {};
    uint64_t even_cprtime = {};
};
struct Traffic :
    public ::Stencil::TimestampedT<::Avid::Traffic>,
    public Stencil::StructT<Traffic>
{
    std::vector<::Avid::Aircraft> aircrafts = {};
};
struct Motion :
    public ::Stencil::TimestampedT<::Avid::Motion>,
    public Stencil::StructT<Motion>
{
    int64_t mag_x = {};
    int64_t mag_y = {};
    int64_t mag_z = {};
    int64_t acc_x = {};
    int64_t acc_y = {};
    int64_t acc_z = {};
    int64_t rot_x = {};
    int64_t rot_y = {};
    int64_t rot_z = {};
    double dmag_x = {};
    double dmag_y = {};
    double dmag_z = {};
    double dacc_x = {};
    double dacc_y = {};
    double dacc_z = {};
    double drot_x = {};
    double drot_y = {};
    double drot_z = {};
};
struct Pressure :
    public ::Stencil::OptionalPropsT<::Avid::Pressure>,
    public ::Stencil::TimestampedT<::Avid::Pressure>,
    public Stencil::StructT<Pressure>
{
    double pressure = {};
    double temperature = {};
};
}    // namespace Avid
#endif

// SECTION END: Definitions

// SECTION START: Template specializations
#if true

// SECTION:

template <> struct Stencil::TypeTraits<Avid::GPS>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<Avid::GPS>
{
    enum class Fields
    {
        Invalid,
        Field_heading
,        Field_lat
,        Field_lon
,        Field_speed
,        Field_utc
,        Field_galt
,        Field_climb
,        Field_epx
,        Field_epy
,        Field_epv
,        Field_ept
,        Field_eps
,        Field_epc
,        Field_eph
,        Field_sep
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<Avid::GPS>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<Avid::GPS>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "heading"
,        "lat"
,        "lon"
,        "speed"
,        "utc"
,        "galt"
,        "climb"
,        "epx"
,        "epy"
,        "epv"
,        "ept"
,        "eps"
,        "epc"
,        "eph"
,        "sep"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<Avid::GPS>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<Avid::GPS>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<Avid::GPS> : Stencil::TransactionT<Avid::GPS>
{
    using TData  = Avid::GPS;
    using Fields = Stencil::TypeTraitsForIndexable<Avid::GPS>::Fields;

    Transaction<int32_t> _subtracker_heading;
    Transaction<double> _subtracker_lat;
    Transaction<double> _subtracker_lon;
    Transaction<double> _subtracker_speed;
    Transaction<Stencil::Timestamp> _subtracker_utc;
    Transaction<double> _subtracker_galt;
    Transaction<double> _subtracker_climb;
    Transaction<double> _subtracker_epx;
    Transaction<double> _subtracker_epy;
    Transaction<double> _subtracker_epv;
    Transaction<double> _subtracker_ept;
    Transaction<double> _subtracker_eps;
    Transaction<double> _subtracker_epc;
    Transaction<double> _subtracker_eph;
    Transaction<double> _subtracker_sep;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<Avid::GPS>(ptr)
        ,
        _subtracker_heading(Obj().heading)
        ,
        _subtracker_lat(Obj().lat)
        ,
        _subtracker_lon(Obj().lon)
        ,
        _subtracker_speed(Obj().speed)
        ,
        _subtracker_utc(Obj().utc)
        ,
        _subtracker_galt(Obj().galt)
        ,
        _subtracker_climb(Obj().climb)
        ,
        _subtracker_epx(Obj().epx)
        ,
        _subtracker_epy(Obj().epy)
        ,
        _subtracker_epv(Obj().epv)
        ,
        _subtracker_ept(Obj().ept)
        ,
        _subtracker_eps(Obj().eps)
        ,
        _subtracker_epc(Obj().epc)
        ,
        _subtracker_eph(Obj().eph)
        ,
        _subtracker_sep(Obj().sep)
    {}

    auto& heading()
    {
        MarkFieldEdited_(Fields::Field_heading);
        return _subtracker_heading;
    }
    auto& lat()
    {
        MarkFieldEdited_(Fields::Field_lat);
        return _subtracker_lat;
    }
    auto& lon()
    {
        MarkFieldEdited_(Fields::Field_lon);
        return _subtracker_lon;
    }
    auto& speed()
    {
        MarkFieldEdited_(Fields::Field_speed);
        return _subtracker_speed;
    }
    auto& utc()
    {
        MarkFieldEdited_(Fields::Field_utc);
        return _subtracker_utc;
    }
    auto& galt()
    {
        MarkFieldEdited_(Fields::Field_galt);
        return _subtracker_galt;
    }
    auto& climb()
    {
        MarkFieldEdited_(Fields::Field_climb);
        return _subtracker_climb;
    }
    auto& epx()
    {
        MarkFieldEdited_(Fields::Field_epx);
        return _subtracker_epx;
    }
    auto& epy()
    {
        MarkFieldEdited_(Fields::Field_epy);
        return _subtracker_epy;
    }
    auto& epv()
    {
        MarkFieldEdited_(Fields::Field_epv);
        return _subtracker_epv;
    }
    auto& ept()
    {
        MarkFieldEdited_(Fields::Field_ept);
        return _subtracker_ept;
    }
    auto& eps()
    {
        MarkFieldEdited_(Fields::Field_eps);
        return _subtracker_eps;
    }
    auto& epc()
    {
        MarkFieldEdited_(Fields::Field_epc);
        return _subtracker_epc;
    }
    auto& eph()
    {
        MarkFieldEdited_(Fields::Field_eph);
        return _subtracker_eph;
    }
    auto& sep()
    {
        MarkFieldEdited_(Fields::Field_sep);
        return _subtracker_sep;
    }
    void set_heading(int32_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_heading, Obj().heading, val);
        Obj().heading = std::move(val);
    }

    void set_lat(double&& val)
    {
        MarkFieldAssigned_(Fields::Field_lat, Obj().lat, val);
        Obj().lat = std::move(val);
    }

    void set_lon(double&& val)
    {
        MarkFieldAssigned_(Fields::Field_lon, Obj().lon, val);
        Obj().lon = std::move(val);
    }

    void set_speed(double&& val)
    {
        MarkFieldAssigned_(Fields::Field_speed, Obj().speed, val);
        Obj().speed = std::move(val);
    }

    void set_utc(Stencil::Timestamp&& val)
    {
        MarkFieldAssigned_(Fields::Field_utc, Obj().utc, val);
        Obj().utc = std::move(val);
    }

    void set_galt(double&& val)
    {
        MarkFieldAssigned_(Fields::Field_galt, Obj().galt, val);
        Obj().galt = std::move(val);
    }

    void set_climb(double&& val)
    {
        MarkFieldAssigned_(Fields::Field_climb, Obj().climb, val);
        Obj().climb = std::move(val);
    }

    void set_epx(double&& val)
    {
        MarkFieldAssigned_(Fields::Field_epx, Obj().epx, val);
        Obj().epx = std::move(val);
    }

    void set_epy(double&& val)
    {
        MarkFieldAssigned_(Fields::Field_epy, Obj().epy, val);
        Obj().epy = std::move(val);
    }

    void set_epv(double&& val)
    {
        MarkFieldAssigned_(Fields::Field_epv, Obj().epv, val);
        Obj().epv = std::move(val);
    }

    void set_ept(double&& val)
    {
        MarkFieldAssigned_(Fields::Field_ept, Obj().ept, val);
        Obj().ept = std::move(val);
    }

    void set_eps(double&& val)
    {
        MarkFieldAssigned_(Fields::Field_eps, Obj().eps, val);
        Obj().eps = std::move(val);
    }

    void set_epc(double&& val)
    {
        MarkFieldAssigned_(Fields::Field_epc, Obj().epc, val);
        Obj().epc = std::move(val);
    }

    void set_eph(double&& val)
    {
        MarkFieldAssigned_(Fields::Field_eph, Obj().eph, val);
        Obj().eph = std::move(val);
    }

    void set_sep(double&& val)
    {
        MarkFieldAssigned_(Fields::Field_sep, Obj().sep, val);
        Obj().sep = std::move(val);
    }

    template <typename TLambda> auto Visit(Fields index, TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_heading: return lambda("heading", heading());
        case Fields::Field_lat: return lambda("lat", lat());
        case Fields::Field_lon: return lambda("lon", lon());
        case Fields::Field_speed: return lambda("speed", speed());
        case Fields::Field_utc: return lambda("utc", utc());
        case Fields::Field_galt: return lambda("galt", galt());
        case Fields::Field_climb: return lambda("climb", climb());
        case Fields::Field_epx: return lambda("epx", epx());
        case Fields::Field_epy: return lambda("epy", epy());
        case Fields::Field_epv: return lambda("epv", epv());
        case Fields::Field_ept: return lambda("ept", ept());
        case Fields::Field_eps: return lambda("eps", eps());
        case Fields::Field_epc: return lambda("epc", epc());
        case Fields::Field_eph: return lambda("eph", eph());
        case Fields::Field_sep: return lambda("sep", sep());
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "heading") { return lambda(Fields::Field_heading, heading()); }
        if (fieldName == "lat") { return lambda(Fields::Field_lat, lat()); }
        if (fieldName == "lon") { return lambda(Fields::Field_lon, lon()); }
        if (fieldName == "speed") { return lambda(Fields::Field_speed, speed()); }
        if (fieldName == "utc") { return lambda(Fields::Field_utc, utc()); }
        if (fieldName == "galt") { return lambda(Fields::Field_galt, galt()); }
        if (fieldName == "climb") { return lambda(Fields::Field_climb, climb()); }
        if (fieldName == "epx") { return lambda(Fields::Field_epx, epx()); }
        if (fieldName == "epy") { return lambda(Fields::Field_epy, epy()); }
        if (fieldName == "epv") { return lambda(Fields::Field_epv, epv()); }
        if (fieldName == "ept") { return lambda(Fields::Field_ept, ept()); }
        if (fieldName == "eps") { return lambda(Fields::Field_eps, eps()); }
        if (fieldName == "epc") { return lambda(Fields::Field_epc, epc()); }
        if (fieldName == "eph") { return lambda(Fields::Field_eph, eph()); }
        if (fieldName == "sep") { return lambda(Fields::Field_sep, sep()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("heading", Fields::Field_heading, heading(), Obj().heading);
        lambda("lat", Fields::Field_lat, lat(), Obj().lat);
        lambda("lon", Fields::Field_lon, lon(), Obj().lon);
        lambda("speed", Fields::Field_speed, speed(), Obj().speed);
        lambda("utc", Fields::Field_utc, utc(), Obj().utc);
        lambda("galt", Fields::Field_galt, galt(), Obj().galt);
        lambda("climb", Fields::Field_climb, climb(), Obj().climb);
        lambda("epx", Fields::Field_epx, epx(), Obj().epx);
        lambda("epy", Fields::Field_epy, epy(), Obj().epy);
        lambda("epv", Fields::Field_epv, epv(), Obj().epv);
        lambda("ept", Fields::Field_ept, ept(), Obj().ept);
        lambda("eps", Fields::Field_eps, eps(), Obj().eps);
        lambda("epc", Fields::Field_epc, epc(), Obj().epc);
        lambda("eph", Fields::Field_eph, eph(), Obj().eph);
        lambda("sep", Fields::Field_sep, sep(), Obj().sep);
    }

    void Flush()
    {
        heading().Flush();

        if (IsFieldEdited(Fields::Field_heading))
        {
            if (!heading().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_heading));
        }

        lat().Flush();

        if (IsFieldEdited(Fields::Field_lat))
        {
            if (!lat().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_lat));
        }

        lon().Flush();

        if (IsFieldEdited(Fields::Field_lon))
        {
            if (!lon().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_lon));
        }

        speed().Flush();

        if (IsFieldEdited(Fields::Field_speed))
        {
            if (!speed().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_speed));
        }

        utc().Flush();

        if (IsFieldEdited(Fields::Field_utc))
        {
            if (!utc().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_utc));
        }

        galt().Flush();

        if (IsFieldEdited(Fields::Field_galt))
        {
            if (!galt().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_galt));
        }

        climb().Flush();

        if (IsFieldEdited(Fields::Field_climb))
        {
            if (!climb().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_climb));
        }

        epx().Flush();

        if (IsFieldEdited(Fields::Field_epx))
        {
            if (!epx().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_epx));
        }

        epy().Flush();

        if (IsFieldEdited(Fields::Field_epy))
        {
            if (!epy().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_epy));
        }

        epv().Flush();

        if (IsFieldEdited(Fields::Field_epv))
        {
            if (!epv().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_epv));
        }

        ept().Flush();

        if (IsFieldEdited(Fields::Field_ept))
        {
            if (!ept().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_ept));
        }

        eps().Flush();

        if (IsFieldEdited(Fields::Field_eps))
        {
            if (!eps().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_eps));
        }

        epc().Flush();

        if (IsFieldEdited(Fields::Field_epc))
        {
            if (!epc().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_epc));
        }

        eph().Flush();

        if (IsFieldEdited(Fields::Field_eph))
        {
            if (!eph().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_eph));
        }

        sep().Flush();

        if (IsFieldEdited(Fields::Field_sep))
        {
            if (!sep().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_sep));
        }

        Stencil::TransactionT<Avid::GPS>::Flush_();
    }
};

template <> struct Stencil::Visitor<Avid::GPS> : Stencil::VisitorT<Avid::GPS>
{
    using TData  = Avid::GPS;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey(T& obj, Fields field, TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_heading: return lambda(obj.heading);
        case Fields::Field_lat: return lambda(obj.lat);
        case Fields::Field_lon: return lambda(obj.lon);
        case Fields::Field_speed: return lambda(obj.speed);
        case Fields::Field_utc: return lambda(obj.utc);
        case Fields::Field_galt: return lambda(obj.galt);
        case Fields::Field_climb: return lambda(obj.climb);
        case Fields::Field_epx: return lambda(obj.epx);
        case Fields::Field_epy: return lambda(obj.epy);
        case Fields::Field_epv: return lambda(obj.epv);
        case Fields::Field_ept: return lambda(obj.ept);
        case Fields::Field_eps: return lambda(obj.eps);
        case Fields::Field_epc: return lambda(obj.epc);
        case Fields::Field_eph: return lambda(obj.eph);
        case Fields::Field_sep: return lambda(obj.sep);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies(T& obj, TLambda&& lambda)
    {
        lambda(Fields::Field_heading, obj.heading);
        lambda(Fields::Field_lat, obj.lat);
        lambda(Fields::Field_lon, obj.lon);
        lambda(Fields::Field_speed, obj.speed);
        lambda(Fields::Field_utc, obj.utc);
        lambda(Fields::Field_galt, obj.galt);
        lambda(Fields::Field_climb, obj.climb);
        lambda(Fields::Field_epx, obj.epx);
        lambda(Fields::Field_epy, obj.epy);
        lambda(Fields::Field_epv, obj.epv);
        lambda(Fields::Field_ept, obj.ept);
        lambda(Fields::Field_eps, obj.eps);
        lambda(Fields::Field_epc, obj.epc);
        lambda(Fields::Field_eph, obj.eph);
        lambda(Fields::Field_sep, obj.sep);
    }
};

template <> struct Stencil::TypeTraits<Avid::Aircraft>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<Avid::Aircraft>
{
    enum class Fields
    {
        Invalid,
        Field_seen
,        Field_addr
,        Field_hexaddr
,        Field_flight
,        Field_altitude
,        Field_groundSpeed
,        Field_track
,        Field_lat
,        Field_lon
,        Field_verticalRate
,        Field_messageCount
,        Field_odd_cprlat
,        Field_odd_cprlon
,        Field_odd_cprtime
,        Field_even_cprlat
,        Field_even_cprlon
,        Field_even_cprtime
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<Avid::Aircraft>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<Avid::Aircraft>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "seen"
,        "addr"
,        "hexaddr"
,        "flight"
,        "altitude"
,        "groundSpeed"
,        "track"
,        "lat"
,        "lon"
,        "verticalRate"
,        "messageCount"
,        "odd_cprlat"
,        "odd_cprlon"
,        "odd_cprtime"
,        "even_cprlat"
,        "even_cprlon"
,        "even_cprtime"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<Avid::Aircraft>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<Avid::Aircraft>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<Avid::Aircraft> : Stencil::TransactionT<Avid::Aircraft>
{
    using TData  = Avid::Aircraft;
    using Fields = Stencil::TypeTraitsForIndexable<Avid::Aircraft>::Fields;

    Transaction<Stencil::Timestamp> _subtracker_seen;
    Transaction<uint32_t> _subtracker_addr;
    Transaction<std::array<uint8_t, 8>> _subtracker_hexaddr;
    Transaction<std::array<char, 9>> _subtracker_flight;
    Transaction<int32_t> _subtracker_altitude;
    Transaction<int32_t> _subtracker_groundSpeed;
    Transaction<int32_t> _subtracker_track;
    Transaction<double> _subtracker_lat;
    Transaction<double> _subtracker_lon;
    Transaction<int32_t> _subtracker_verticalRate;
    Transaction<int32_t> _subtracker_messageCount;
    Transaction<int32_t> _subtracker_odd_cprlat;
    Transaction<int32_t> _subtracker_odd_cprlon;
    Transaction<uint64_t> _subtracker_odd_cprtime;
    Transaction<int32_t> _subtracker_even_cprlat;
    Transaction<int32_t> _subtracker_even_cprlon;
    Transaction<uint64_t> _subtracker_even_cprtime;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<Avid::Aircraft>(ptr)
        ,
        _subtracker_seen(Obj().seen)
        ,
        _subtracker_addr(Obj().addr)
        ,
        _subtracker_hexaddr(Obj().hexaddr)
        ,
        _subtracker_flight(Obj().flight)
        ,
        _subtracker_altitude(Obj().altitude)
        ,
        _subtracker_groundSpeed(Obj().groundSpeed)
        ,
        _subtracker_track(Obj().track)
        ,
        _subtracker_lat(Obj().lat)
        ,
        _subtracker_lon(Obj().lon)
        ,
        _subtracker_verticalRate(Obj().verticalRate)
        ,
        _subtracker_messageCount(Obj().messageCount)
        ,
        _subtracker_odd_cprlat(Obj().odd_cprlat)
        ,
        _subtracker_odd_cprlon(Obj().odd_cprlon)
        ,
        _subtracker_odd_cprtime(Obj().odd_cprtime)
        ,
        _subtracker_even_cprlat(Obj().even_cprlat)
        ,
        _subtracker_even_cprlon(Obj().even_cprlon)
        ,
        _subtracker_even_cprtime(Obj().even_cprtime)
    {}

    auto& seen()
    {
        MarkFieldEdited_(Fields::Field_seen);
        return _subtracker_seen;
    }
    auto& addr()
    {
        MarkFieldEdited_(Fields::Field_addr);
        return _subtracker_addr;
    }
    auto& hexaddr()
    {
        MarkFieldEdited_(Fields::Field_hexaddr);
        return _subtracker_hexaddr;
    }
    auto& flight()
    {
        MarkFieldEdited_(Fields::Field_flight);
        return _subtracker_flight;
    }
    auto& altitude()
    {
        MarkFieldEdited_(Fields::Field_altitude);
        return _subtracker_altitude;
    }
    auto& groundSpeed()
    {
        MarkFieldEdited_(Fields::Field_groundSpeed);
        return _subtracker_groundSpeed;
    }
    auto& track()
    {
        MarkFieldEdited_(Fields::Field_track);
        return _subtracker_track;
    }
    auto& lat()
    {
        MarkFieldEdited_(Fields::Field_lat);
        return _subtracker_lat;
    }
    auto& lon()
    {
        MarkFieldEdited_(Fields::Field_lon);
        return _subtracker_lon;
    }
    auto& verticalRate()
    {
        MarkFieldEdited_(Fields::Field_verticalRate);
        return _subtracker_verticalRate;
    }
    auto& messageCount()
    {
        MarkFieldEdited_(Fields::Field_messageCount);
        return _subtracker_messageCount;
    }
    auto& odd_cprlat()
    {
        MarkFieldEdited_(Fields::Field_odd_cprlat);
        return _subtracker_odd_cprlat;
    }
    auto& odd_cprlon()
    {
        MarkFieldEdited_(Fields::Field_odd_cprlon);
        return _subtracker_odd_cprlon;
    }
    auto& odd_cprtime()
    {
        MarkFieldEdited_(Fields::Field_odd_cprtime);
        return _subtracker_odd_cprtime;
    }
    auto& even_cprlat()
    {
        MarkFieldEdited_(Fields::Field_even_cprlat);
        return _subtracker_even_cprlat;
    }
    auto& even_cprlon()
    {
        MarkFieldEdited_(Fields::Field_even_cprlon);
        return _subtracker_even_cprlon;
    }
    auto& even_cprtime()
    {
        MarkFieldEdited_(Fields::Field_even_cprtime);
        return _subtracker_even_cprtime;
    }
    void set_seen(Stencil::Timestamp&& val)
    {
        MarkFieldAssigned_(Fields::Field_seen, Obj().seen, val);
        Obj().seen = std::move(val);
    }

    void set_addr(uint32_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_addr, Obj().addr, val);
        Obj().addr = std::move(val);
    }

    void set_hexaddr(std::array<uint8_t, 8>&& val)
    {
        MarkFieldAssigned_(Fields::Field_hexaddr, Obj().hexaddr, val);
        Obj().hexaddr = std::move(val);
    }

    void set_flight(std::array<char, 9>&& val)
    {
        MarkFieldAssigned_(Fields::Field_flight, Obj().flight, val);
        Obj().flight = std::move(val);
    }

    void set_altitude(int32_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_altitude, Obj().altitude, val);
        Obj().altitude = std::move(val);
    }

    void set_groundSpeed(int32_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_groundSpeed, Obj().groundSpeed, val);
        Obj().groundSpeed = std::move(val);
    }

    void set_track(int32_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_track, Obj().track, val);
        Obj().track = std::move(val);
    }

    void set_lat(double&& val)
    {
        MarkFieldAssigned_(Fields::Field_lat, Obj().lat, val);
        Obj().lat = std::move(val);
    }

    void set_lon(double&& val)
    {
        MarkFieldAssigned_(Fields::Field_lon, Obj().lon, val);
        Obj().lon = std::move(val);
    }

    void set_verticalRate(int32_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_verticalRate, Obj().verticalRate, val);
        Obj().verticalRate = std::move(val);
    }

    void set_messageCount(int32_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_messageCount, Obj().messageCount, val);
        Obj().messageCount = std::move(val);
    }

    void set_odd_cprlat(int32_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_odd_cprlat, Obj().odd_cprlat, val);
        Obj().odd_cprlat = std::move(val);
    }

    void set_odd_cprlon(int32_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_odd_cprlon, Obj().odd_cprlon, val);
        Obj().odd_cprlon = std::move(val);
    }

    void set_odd_cprtime(uint64_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_odd_cprtime, Obj().odd_cprtime, val);
        Obj().odd_cprtime = std::move(val);
    }

    void set_even_cprlat(int32_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_even_cprlat, Obj().even_cprlat, val);
        Obj().even_cprlat = std::move(val);
    }

    void set_even_cprlon(int32_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_even_cprlon, Obj().even_cprlon, val);
        Obj().even_cprlon = std::move(val);
    }

    void set_even_cprtime(uint64_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_even_cprtime, Obj().even_cprtime, val);
        Obj().even_cprtime = std::move(val);
    }

    template <typename TLambda> auto Visit(Fields index, TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_seen: return lambda("seen", seen());
        case Fields::Field_addr: return lambda("addr", addr());
        case Fields::Field_hexaddr: return lambda("hexaddr", hexaddr());
        case Fields::Field_flight: return lambda("flight", flight());
        case Fields::Field_altitude: return lambda("altitude", altitude());
        case Fields::Field_groundSpeed: return lambda("groundSpeed", groundSpeed());
        case Fields::Field_track: return lambda("track", track());
        case Fields::Field_lat: return lambda("lat", lat());
        case Fields::Field_lon: return lambda("lon", lon());
        case Fields::Field_verticalRate: return lambda("verticalRate", verticalRate());
        case Fields::Field_messageCount: return lambda("messageCount", messageCount());
        case Fields::Field_odd_cprlat: return lambda("odd_cprlat", odd_cprlat());
        case Fields::Field_odd_cprlon: return lambda("odd_cprlon", odd_cprlon());
        case Fields::Field_odd_cprtime: return lambda("odd_cprtime", odd_cprtime());
        case Fields::Field_even_cprlat: return lambda("even_cprlat", even_cprlat());
        case Fields::Field_even_cprlon: return lambda("even_cprlon", even_cprlon());
        case Fields::Field_even_cprtime: return lambda("even_cprtime", even_cprtime());
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "seen") { return lambda(Fields::Field_seen, seen()); }
        if (fieldName == "addr") { return lambda(Fields::Field_addr, addr()); }
        if (fieldName == "hexaddr") { return lambda(Fields::Field_hexaddr, hexaddr()); }
        if (fieldName == "flight") { return lambda(Fields::Field_flight, flight()); }
        if (fieldName == "altitude") { return lambda(Fields::Field_altitude, altitude()); }
        if (fieldName == "groundSpeed") { return lambda(Fields::Field_groundSpeed, groundSpeed()); }
        if (fieldName == "track") { return lambda(Fields::Field_track, track()); }
        if (fieldName == "lat") { return lambda(Fields::Field_lat, lat()); }
        if (fieldName == "lon") { return lambda(Fields::Field_lon, lon()); }
        if (fieldName == "verticalRate") { return lambda(Fields::Field_verticalRate, verticalRate()); }
        if (fieldName == "messageCount") { return lambda(Fields::Field_messageCount, messageCount()); }
        if (fieldName == "odd_cprlat") { return lambda(Fields::Field_odd_cprlat, odd_cprlat()); }
        if (fieldName == "odd_cprlon") { return lambda(Fields::Field_odd_cprlon, odd_cprlon()); }
        if (fieldName == "odd_cprtime") { return lambda(Fields::Field_odd_cprtime, odd_cprtime()); }
        if (fieldName == "even_cprlat") { return lambda(Fields::Field_even_cprlat, even_cprlat()); }
        if (fieldName == "even_cprlon") { return lambda(Fields::Field_even_cprlon, even_cprlon()); }
        if (fieldName == "even_cprtime") { return lambda(Fields::Field_even_cprtime, even_cprtime()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("seen", Fields::Field_seen, seen(), Obj().seen);
        lambda("addr", Fields::Field_addr, addr(), Obj().addr);
        lambda("hexaddr", Fields::Field_hexaddr, hexaddr(), Obj().hexaddr);
        lambda("flight", Fields::Field_flight, flight(), Obj().flight);
        lambda("altitude", Fields::Field_altitude, altitude(), Obj().altitude);
        lambda("groundSpeed", Fields::Field_groundSpeed, groundSpeed(), Obj().groundSpeed);
        lambda("track", Fields::Field_track, track(), Obj().track);
        lambda("lat", Fields::Field_lat, lat(), Obj().lat);
        lambda("lon", Fields::Field_lon, lon(), Obj().lon);
        lambda("verticalRate", Fields::Field_verticalRate, verticalRate(), Obj().verticalRate);
        lambda("messageCount", Fields::Field_messageCount, messageCount(), Obj().messageCount);
        lambda("odd_cprlat", Fields::Field_odd_cprlat, odd_cprlat(), Obj().odd_cprlat);
        lambda("odd_cprlon", Fields::Field_odd_cprlon, odd_cprlon(), Obj().odd_cprlon);
        lambda("odd_cprtime", Fields::Field_odd_cprtime, odd_cprtime(), Obj().odd_cprtime);
        lambda("even_cprlat", Fields::Field_even_cprlat, even_cprlat(), Obj().even_cprlat);
        lambda("even_cprlon", Fields::Field_even_cprlon, even_cprlon(), Obj().even_cprlon);
        lambda("even_cprtime", Fields::Field_even_cprtime, even_cprtime(), Obj().even_cprtime);
    }

    void Flush()
    {
        seen().Flush();

        if (IsFieldEdited(Fields::Field_seen))
        {
            if (!seen().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_seen));
        }

        addr().Flush();

        if (IsFieldEdited(Fields::Field_addr))
        {
            if (!addr().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_addr));
        }

        hexaddr().Flush();

        if (IsFieldEdited(Fields::Field_hexaddr))
        {
            if (!hexaddr().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_hexaddr));
        }

        flight().Flush();

        if (IsFieldEdited(Fields::Field_flight))
        {
            if (!flight().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_flight));
        }

        altitude().Flush();

        if (IsFieldEdited(Fields::Field_altitude))
        {
            if (!altitude().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_altitude));
        }

        groundSpeed().Flush();

        if (IsFieldEdited(Fields::Field_groundSpeed))
        {
            if (!groundSpeed().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_groundSpeed));
        }

        track().Flush();

        if (IsFieldEdited(Fields::Field_track))
        {
            if (!track().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_track));
        }

        lat().Flush();

        if (IsFieldEdited(Fields::Field_lat))
        {
            if (!lat().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_lat));
        }

        lon().Flush();

        if (IsFieldEdited(Fields::Field_lon))
        {
            if (!lon().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_lon));
        }

        verticalRate().Flush();

        if (IsFieldEdited(Fields::Field_verticalRate))
        {
            if (!verticalRate().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_verticalRate));
        }

        messageCount().Flush();

        if (IsFieldEdited(Fields::Field_messageCount))
        {
            if (!messageCount().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_messageCount));
        }

        odd_cprlat().Flush();

        if (IsFieldEdited(Fields::Field_odd_cprlat))
        {
            if (!odd_cprlat().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_odd_cprlat));
        }

        odd_cprlon().Flush();

        if (IsFieldEdited(Fields::Field_odd_cprlon))
        {
            if (!odd_cprlon().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_odd_cprlon));
        }

        odd_cprtime().Flush();

        if (IsFieldEdited(Fields::Field_odd_cprtime))
        {
            if (!odd_cprtime().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_odd_cprtime));
        }

        even_cprlat().Flush();

        if (IsFieldEdited(Fields::Field_even_cprlat))
        {
            if (!even_cprlat().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_even_cprlat));
        }

        even_cprlon().Flush();

        if (IsFieldEdited(Fields::Field_even_cprlon))
        {
            if (!even_cprlon().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_even_cprlon));
        }

        even_cprtime().Flush();

        if (IsFieldEdited(Fields::Field_even_cprtime))
        {
            if (!even_cprtime().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_even_cprtime));
        }

        Stencil::TransactionT<Avid::Aircraft>::Flush_();
    }
};

template <> struct Stencil::Visitor<Avid::Aircraft> : Stencil::VisitorT<Avid::Aircraft>
{
    using TData  = Avid::Aircraft;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey(T& obj, Fields field, TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_seen: return lambda(obj.seen);
        case Fields::Field_addr: return lambda(obj.addr);
        case Fields::Field_hexaddr: return lambda(obj.hexaddr);
        case Fields::Field_flight: return lambda(obj.flight);
        case Fields::Field_altitude: return lambda(obj.altitude);
        case Fields::Field_groundSpeed: return lambda(obj.groundSpeed);
        case Fields::Field_track: return lambda(obj.track);
        case Fields::Field_lat: return lambda(obj.lat);
        case Fields::Field_lon: return lambda(obj.lon);
        case Fields::Field_verticalRate: return lambda(obj.verticalRate);
        case Fields::Field_messageCount: return lambda(obj.messageCount);
        case Fields::Field_odd_cprlat: return lambda(obj.odd_cprlat);
        case Fields::Field_odd_cprlon: return lambda(obj.odd_cprlon);
        case Fields::Field_odd_cprtime: return lambda(obj.odd_cprtime);
        case Fields::Field_even_cprlat: return lambda(obj.even_cprlat);
        case Fields::Field_even_cprlon: return lambda(obj.even_cprlon);
        case Fields::Field_even_cprtime: return lambda(obj.even_cprtime);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies(T& obj, TLambda&& lambda)
    {
        lambda(Fields::Field_seen, obj.seen);
        lambda(Fields::Field_addr, obj.addr);
        lambda(Fields::Field_hexaddr, obj.hexaddr);
        lambda(Fields::Field_flight, obj.flight);
        lambda(Fields::Field_altitude, obj.altitude);
        lambda(Fields::Field_groundSpeed, obj.groundSpeed);
        lambda(Fields::Field_track, obj.track);
        lambda(Fields::Field_lat, obj.lat);
        lambda(Fields::Field_lon, obj.lon);
        lambda(Fields::Field_verticalRate, obj.verticalRate);
        lambda(Fields::Field_messageCount, obj.messageCount);
        lambda(Fields::Field_odd_cprlat, obj.odd_cprlat);
        lambda(Fields::Field_odd_cprlon, obj.odd_cprlon);
        lambda(Fields::Field_odd_cprtime, obj.odd_cprtime);
        lambda(Fields::Field_even_cprlat, obj.even_cprlat);
        lambda(Fields::Field_even_cprlon, obj.even_cprlon);
        lambda(Fields::Field_even_cprtime, obj.even_cprtime);
    }
};

template <> struct Stencil::TypeTraits<Avid::Traffic>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<Avid::Traffic>
{
    enum class Fields
    {
        Invalid,
        Field_aircrafts
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<Avid::Traffic>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<Avid::Traffic>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "aircrafts"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<Avid::Traffic>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<Avid::Traffic>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<Avid::Traffic> : Stencil::TransactionT<Avid::Traffic>
{
    using TData  = Avid::Traffic;
    using Fields = Stencil::TypeTraitsForIndexable<Avid::Traffic>::Fields;

    Transaction<std::vector<::Avid::Aircraft>> _subtracker_aircrafts;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<Avid::Traffic>(ptr)
        ,
        _subtracker_aircrafts(Obj().aircrafts)
    {}

    auto& aircrafts()
    {
        MarkFieldEdited_(Fields::Field_aircrafts);
        return _subtracker_aircrafts;
    }
    void set_aircrafts(std::vector<::Avid::Aircraft>&& val)
    {
        MarkFieldAssigned_(Fields::Field_aircrafts, Obj().aircrafts, val);
        Obj().aircrafts = std::move(val);
    }

    void add_aircrafts(::Avid::Aircraft&& args)
    {
        MarkFieldEdited_(Fields::Field_aircrafts);
        return Stencil::Mutators<std::remove_reference_t<decltype(aircrafts())>>::add(aircrafts(), std::move(args));
    }
    void remove_aircrafts(size_t&& args)
    {
        MarkFieldEdited_(Fields::Field_aircrafts);
        return Stencil::Mutators<std::remove_reference_t<decltype(aircrafts())>>::remove(aircrafts(), std::move(args));
    }
    auto& edit_aircrafts(size_t&& args)
    {
        MarkFieldEdited_(Fields::Field_aircrafts);
        return Stencil::Mutators<std::remove_reference_t<decltype(aircrafts())>>::edit(aircrafts(), std::move(args));
    }
    template <typename TLambda> auto Visit(Fields index, TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_aircrafts: return lambda("aircrafts", aircrafts());
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "aircrafts") { return lambda(Fields::Field_aircrafts, aircrafts()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("aircrafts", Fields::Field_aircrafts, aircrafts(), Obj().aircrafts);
    }

    void Flush()
    {
        aircrafts().Flush();

        if (IsFieldEdited(Fields::Field_aircrafts))
        {
            if (!aircrafts().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_aircrafts));
        }

        Stencil::TransactionT<Avid::Traffic>::Flush_();
    }
};

template <> struct Stencil::Visitor<Avid::Traffic> : Stencil::VisitorT<Avid::Traffic>
{
    using TData  = Avid::Traffic;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey(T& obj, Fields field, TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_aircrafts: return lambda(obj.aircrafts);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies(T& obj, TLambda&& lambda)
    {
        lambda(Fields::Field_aircrafts, obj.aircrafts);
    }
};

template <> struct Stencil::TypeTraits<Avid::Motion>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<Avid::Motion>
{
    enum class Fields
    {
        Invalid,
        Field_mag_x
,        Field_mag_y
,        Field_mag_z
,        Field_acc_x
,        Field_acc_y
,        Field_acc_z
,        Field_rot_x
,        Field_rot_y
,        Field_rot_z
,        Field_dmag_x
,        Field_dmag_y
,        Field_dmag_z
,        Field_dacc_x
,        Field_dacc_y
,        Field_dacc_z
,        Field_drot_x
,        Field_drot_y
,        Field_drot_z
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<Avid::Motion>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<Avid::Motion>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "mag_x"
,        "mag_y"
,        "mag_z"
,        "acc_x"
,        "acc_y"
,        "acc_z"
,        "rot_x"
,        "rot_y"
,        "rot_z"
,        "dmag_x"
,        "dmag_y"
,        "dmag_z"
,        "dacc_x"
,        "dacc_y"
,        "dacc_z"
,        "drot_x"
,        "drot_y"
,        "drot_z"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<Avid::Motion>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<Avid::Motion>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<Avid::Motion> : Stencil::TransactionT<Avid::Motion>
{
    using TData  = Avid::Motion;
    using Fields = Stencil::TypeTraitsForIndexable<Avid::Motion>::Fields;

    Transaction<int64_t> _subtracker_mag_x;
    Transaction<int64_t> _subtracker_mag_y;
    Transaction<int64_t> _subtracker_mag_z;
    Transaction<int64_t> _subtracker_acc_x;
    Transaction<int64_t> _subtracker_acc_y;
    Transaction<int64_t> _subtracker_acc_z;
    Transaction<int64_t> _subtracker_rot_x;
    Transaction<int64_t> _subtracker_rot_y;
    Transaction<int64_t> _subtracker_rot_z;
    Transaction<double> _subtracker_dmag_x;
    Transaction<double> _subtracker_dmag_y;
    Transaction<double> _subtracker_dmag_z;
    Transaction<double> _subtracker_dacc_x;
    Transaction<double> _subtracker_dacc_y;
    Transaction<double> _subtracker_dacc_z;
    Transaction<double> _subtracker_drot_x;
    Transaction<double> _subtracker_drot_y;
    Transaction<double> _subtracker_drot_z;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<Avid::Motion>(ptr)
        ,
        _subtracker_mag_x(Obj().mag_x)
        ,
        _subtracker_mag_y(Obj().mag_y)
        ,
        _subtracker_mag_z(Obj().mag_z)
        ,
        _subtracker_acc_x(Obj().acc_x)
        ,
        _subtracker_acc_y(Obj().acc_y)
        ,
        _subtracker_acc_z(Obj().acc_z)
        ,
        _subtracker_rot_x(Obj().rot_x)
        ,
        _subtracker_rot_y(Obj().rot_y)
        ,
        _subtracker_rot_z(Obj().rot_z)
        ,
        _subtracker_dmag_x(Obj().dmag_x)
        ,
        _subtracker_dmag_y(Obj().dmag_y)
        ,
        _subtracker_dmag_z(Obj().dmag_z)
        ,
        _subtracker_dacc_x(Obj().dacc_x)
        ,
        _subtracker_dacc_y(Obj().dacc_y)
        ,
        _subtracker_dacc_z(Obj().dacc_z)
        ,
        _subtracker_drot_x(Obj().drot_x)
        ,
        _subtracker_drot_y(Obj().drot_y)
        ,
        _subtracker_drot_z(Obj().drot_z)
    {}

    auto& mag_x()
    {
        MarkFieldEdited_(Fields::Field_mag_x);
        return _subtracker_mag_x;
    }
    auto& mag_y()
    {
        MarkFieldEdited_(Fields::Field_mag_y);
        return _subtracker_mag_y;
    }
    auto& mag_z()
    {
        MarkFieldEdited_(Fields::Field_mag_z);
        return _subtracker_mag_z;
    }
    auto& acc_x()
    {
        MarkFieldEdited_(Fields::Field_acc_x);
        return _subtracker_acc_x;
    }
    auto& acc_y()
    {
        MarkFieldEdited_(Fields::Field_acc_y);
        return _subtracker_acc_y;
    }
    auto& acc_z()
    {
        MarkFieldEdited_(Fields::Field_acc_z);
        return _subtracker_acc_z;
    }
    auto& rot_x()
    {
        MarkFieldEdited_(Fields::Field_rot_x);
        return _subtracker_rot_x;
    }
    auto& rot_y()
    {
        MarkFieldEdited_(Fields::Field_rot_y);
        return _subtracker_rot_y;
    }
    auto& rot_z()
    {
        MarkFieldEdited_(Fields::Field_rot_z);
        return _subtracker_rot_z;
    }
    auto& dmag_x()
    {
        MarkFieldEdited_(Fields::Field_dmag_x);
        return _subtracker_dmag_x;
    }
    auto& dmag_y()
    {
        MarkFieldEdited_(Fields::Field_dmag_y);
        return _subtracker_dmag_y;
    }
    auto& dmag_z()
    {
        MarkFieldEdited_(Fields::Field_dmag_z);
        return _subtracker_dmag_z;
    }
    auto& dacc_x()
    {
        MarkFieldEdited_(Fields::Field_dacc_x);
        return _subtracker_dacc_x;
    }
    auto& dacc_y()
    {
        MarkFieldEdited_(Fields::Field_dacc_y);
        return _subtracker_dacc_y;
    }
    auto& dacc_z()
    {
        MarkFieldEdited_(Fields::Field_dacc_z);
        return _subtracker_dacc_z;
    }
    auto& drot_x()
    {
        MarkFieldEdited_(Fields::Field_drot_x);
        return _subtracker_drot_x;
    }
    auto& drot_y()
    {
        MarkFieldEdited_(Fields::Field_drot_y);
        return _subtracker_drot_y;
    }
    auto& drot_z()
    {
        MarkFieldEdited_(Fields::Field_drot_z);
        return _subtracker_drot_z;
    }
    void set_mag_x(int64_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_mag_x, Obj().mag_x, val);
        Obj().mag_x = std::move(val);
    }

    void set_mag_y(int64_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_mag_y, Obj().mag_y, val);
        Obj().mag_y = std::move(val);
    }

    void set_mag_z(int64_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_mag_z, Obj().mag_z, val);
        Obj().mag_z = std::move(val);
    }

    void set_acc_x(int64_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_acc_x, Obj().acc_x, val);
        Obj().acc_x = std::move(val);
    }

    void set_acc_y(int64_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_acc_y, Obj().acc_y, val);
        Obj().acc_y = std::move(val);
    }

    void set_acc_z(int64_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_acc_z, Obj().acc_z, val);
        Obj().acc_z = std::move(val);
    }

    void set_rot_x(int64_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_rot_x, Obj().rot_x, val);
        Obj().rot_x = std::move(val);
    }

    void set_rot_y(int64_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_rot_y, Obj().rot_y, val);
        Obj().rot_y = std::move(val);
    }

    void set_rot_z(int64_t&& val)
    {
        MarkFieldAssigned_(Fields::Field_rot_z, Obj().rot_z, val);
        Obj().rot_z = std::move(val);
    }

    void set_dmag_x(double&& val)
    {
        MarkFieldAssigned_(Fields::Field_dmag_x, Obj().dmag_x, val);
        Obj().dmag_x = std::move(val);
    }

    void set_dmag_y(double&& val)
    {
        MarkFieldAssigned_(Fields::Field_dmag_y, Obj().dmag_y, val);
        Obj().dmag_y = std::move(val);
    }

    void set_dmag_z(double&& val)
    {
        MarkFieldAssigned_(Fields::Field_dmag_z, Obj().dmag_z, val);
        Obj().dmag_z = std::move(val);
    }

    void set_dacc_x(double&& val)
    {
        MarkFieldAssigned_(Fields::Field_dacc_x, Obj().dacc_x, val);
        Obj().dacc_x = std::move(val);
    }

    void set_dacc_y(double&& val)
    {
        MarkFieldAssigned_(Fields::Field_dacc_y, Obj().dacc_y, val);
        Obj().dacc_y = std::move(val);
    }

    void set_dacc_z(double&& val)
    {
        MarkFieldAssigned_(Fields::Field_dacc_z, Obj().dacc_z, val);
        Obj().dacc_z = std::move(val);
    }

    void set_drot_x(double&& val)
    {
        MarkFieldAssigned_(Fields::Field_drot_x, Obj().drot_x, val);
        Obj().drot_x = std::move(val);
    }

    void set_drot_y(double&& val)
    {
        MarkFieldAssigned_(Fields::Field_drot_y, Obj().drot_y, val);
        Obj().drot_y = std::move(val);
    }

    void set_drot_z(double&& val)
    {
        MarkFieldAssigned_(Fields::Field_drot_z, Obj().drot_z, val);
        Obj().drot_z = std::move(val);
    }

    template <typename TLambda> auto Visit(Fields index, TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_mag_x: return lambda("mag_x", mag_x());
        case Fields::Field_mag_y: return lambda("mag_y", mag_y());
        case Fields::Field_mag_z: return lambda("mag_z", mag_z());
        case Fields::Field_acc_x: return lambda("acc_x", acc_x());
        case Fields::Field_acc_y: return lambda("acc_y", acc_y());
        case Fields::Field_acc_z: return lambda("acc_z", acc_z());
        case Fields::Field_rot_x: return lambda("rot_x", rot_x());
        case Fields::Field_rot_y: return lambda("rot_y", rot_y());
        case Fields::Field_rot_z: return lambda("rot_z", rot_z());
        case Fields::Field_dmag_x: return lambda("dmag_x", dmag_x());
        case Fields::Field_dmag_y: return lambda("dmag_y", dmag_y());
        case Fields::Field_dmag_z: return lambda("dmag_z", dmag_z());
        case Fields::Field_dacc_x: return lambda("dacc_x", dacc_x());
        case Fields::Field_dacc_y: return lambda("dacc_y", dacc_y());
        case Fields::Field_dacc_z: return lambda("dacc_z", dacc_z());
        case Fields::Field_drot_x: return lambda("drot_x", drot_x());
        case Fields::Field_drot_y: return lambda("drot_y", drot_y());
        case Fields::Field_drot_z: return lambda("drot_z", drot_z());
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "mag_x") { return lambda(Fields::Field_mag_x, mag_x()); }
        if (fieldName == "mag_y") { return lambda(Fields::Field_mag_y, mag_y()); }
        if (fieldName == "mag_z") { return lambda(Fields::Field_mag_z, mag_z()); }
        if (fieldName == "acc_x") { return lambda(Fields::Field_acc_x, acc_x()); }
        if (fieldName == "acc_y") { return lambda(Fields::Field_acc_y, acc_y()); }
        if (fieldName == "acc_z") { return lambda(Fields::Field_acc_z, acc_z()); }
        if (fieldName == "rot_x") { return lambda(Fields::Field_rot_x, rot_x()); }
        if (fieldName == "rot_y") { return lambda(Fields::Field_rot_y, rot_y()); }
        if (fieldName == "rot_z") { return lambda(Fields::Field_rot_z, rot_z()); }
        if (fieldName == "dmag_x") { return lambda(Fields::Field_dmag_x, dmag_x()); }
        if (fieldName == "dmag_y") { return lambda(Fields::Field_dmag_y, dmag_y()); }
        if (fieldName == "dmag_z") { return lambda(Fields::Field_dmag_z, dmag_z()); }
        if (fieldName == "dacc_x") { return lambda(Fields::Field_dacc_x, dacc_x()); }
        if (fieldName == "dacc_y") { return lambda(Fields::Field_dacc_y, dacc_y()); }
        if (fieldName == "dacc_z") { return lambda(Fields::Field_dacc_z, dacc_z()); }
        if (fieldName == "drot_x") { return lambda(Fields::Field_drot_x, drot_x()); }
        if (fieldName == "drot_y") { return lambda(Fields::Field_drot_y, drot_y()); }
        if (fieldName == "drot_z") { return lambda(Fields::Field_drot_z, drot_z()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("mag_x", Fields::Field_mag_x, mag_x(), Obj().mag_x);
        lambda("mag_y", Fields::Field_mag_y, mag_y(), Obj().mag_y);
        lambda("mag_z", Fields::Field_mag_z, mag_z(), Obj().mag_z);
        lambda("acc_x", Fields::Field_acc_x, acc_x(), Obj().acc_x);
        lambda("acc_y", Fields::Field_acc_y, acc_y(), Obj().acc_y);
        lambda("acc_z", Fields::Field_acc_z, acc_z(), Obj().acc_z);
        lambda("rot_x", Fields::Field_rot_x, rot_x(), Obj().rot_x);
        lambda("rot_y", Fields::Field_rot_y, rot_y(), Obj().rot_y);
        lambda("rot_z", Fields::Field_rot_z, rot_z(), Obj().rot_z);
        lambda("dmag_x", Fields::Field_dmag_x, dmag_x(), Obj().dmag_x);
        lambda("dmag_y", Fields::Field_dmag_y, dmag_y(), Obj().dmag_y);
        lambda("dmag_z", Fields::Field_dmag_z, dmag_z(), Obj().dmag_z);
        lambda("dacc_x", Fields::Field_dacc_x, dacc_x(), Obj().dacc_x);
        lambda("dacc_y", Fields::Field_dacc_y, dacc_y(), Obj().dacc_y);
        lambda("dacc_z", Fields::Field_dacc_z, dacc_z(), Obj().dacc_z);
        lambda("drot_x", Fields::Field_drot_x, drot_x(), Obj().drot_x);
        lambda("drot_y", Fields::Field_drot_y, drot_y(), Obj().drot_y);
        lambda("drot_z", Fields::Field_drot_z, drot_z(), Obj().drot_z);
    }

    void Flush()
    {
        mag_x().Flush();

        if (IsFieldEdited(Fields::Field_mag_x))
        {
            if (!mag_x().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_mag_x));
        }

        mag_y().Flush();

        if (IsFieldEdited(Fields::Field_mag_y))
        {
            if (!mag_y().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_mag_y));
        }

        mag_z().Flush();

        if (IsFieldEdited(Fields::Field_mag_z))
        {
            if (!mag_z().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_mag_z));
        }

        acc_x().Flush();

        if (IsFieldEdited(Fields::Field_acc_x))
        {
            if (!acc_x().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_acc_x));
        }

        acc_y().Flush();

        if (IsFieldEdited(Fields::Field_acc_y))
        {
            if (!acc_y().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_acc_y));
        }

        acc_z().Flush();

        if (IsFieldEdited(Fields::Field_acc_z))
        {
            if (!acc_z().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_acc_z));
        }

        rot_x().Flush();

        if (IsFieldEdited(Fields::Field_rot_x))
        {
            if (!rot_x().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_rot_x));
        }

        rot_y().Flush();

        if (IsFieldEdited(Fields::Field_rot_y))
        {
            if (!rot_y().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_rot_y));
        }

        rot_z().Flush();

        if (IsFieldEdited(Fields::Field_rot_z))
        {
            if (!rot_z().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_rot_z));
        }

        dmag_x().Flush();

        if (IsFieldEdited(Fields::Field_dmag_x))
        {
            if (!dmag_x().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_dmag_x));
        }

        dmag_y().Flush();

        if (IsFieldEdited(Fields::Field_dmag_y))
        {
            if (!dmag_y().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_dmag_y));
        }

        dmag_z().Flush();

        if (IsFieldEdited(Fields::Field_dmag_z))
        {
            if (!dmag_z().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_dmag_z));
        }

        dacc_x().Flush();

        if (IsFieldEdited(Fields::Field_dacc_x))
        {
            if (!dacc_x().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_dacc_x));
        }

        dacc_y().Flush();

        if (IsFieldEdited(Fields::Field_dacc_y))
        {
            if (!dacc_y().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_dacc_y));
        }

        dacc_z().Flush();

        if (IsFieldEdited(Fields::Field_dacc_z))
        {
            if (!dacc_z().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_dacc_z));
        }

        drot_x().Flush();

        if (IsFieldEdited(Fields::Field_drot_x))
        {
            if (!drot_x().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_drot_x));
        }

        drot_y().Flush();

        if (IsFieldEdited(Fields::Field_drot_y))
        {
            if (!drot_y().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_drot_y));
        }

        drot_z().Flush();

        if (IsFieldEdited(Fields::Field_drot_z))
        {
            if (!drot_z().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_drot_z));
        }

        Stencil::TransactionT<Avid::Motion>::Flush_();
    }
};

template <> struct Stencil::Visitor<Avid::Motion> : Stencil::VisitorT<Avid::Motion>
{
    using TData  = Avid::Motion;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey(T& obj, Fields field, TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_mag_x: return lambda(obj.mag_x);
        case Fields::Field_mag_y: return lambda(obj.mag_y);
        case Fields::Field_mag_z: return lambda(obj.mag_z);
        case Fields::Field_acc_x: return lambda(obj.acc_x);
        case Fields::Field_acc_y: return lambda(obj.acc_y);
        case Fields::Field_acc_z: return lambda(obj.acc_z);
        case Fields::Field_rot_x: return lambda(obj.rot_x);
        case Fields::Field_rot_y: return lambda(obj.rot_y);
        case Fields::Field_rot_z: return lambda(obj.rot_z);
        case Fields::Field_dmag_x: return lambda(obj.dmag_x);
        case Fields::Field_dmag_y: return lambda(obj.dmag_y);
        case Fields::Field_dmag_z: return lambda(obj.dmag_z);
        case Fields::Field_dacc_x: return lambda(obj.dacc_x);
        case Fields::Field_dacc_y: return lambda(obj.dacc_y);
        case Fields::Field_dacc_z: return lambda(obj.dacc_z);
        case Fields::Field_drot_x: return lambda(obj.drot_x);
        case Fields::Field_drot_y: return lambda(obj.drot_y);
        case Fields::Field_drot_z: return lambda(obj.drot_z);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies(T& obj, TLambda&& lambda)
    {
        lambda(Fields::Field_mag_x, obj.mag_x);
        lambda(Fields::Field_mag_y, obj.mag_y);
        lambda(Fields::Field_mag_z, obj.mag_z);
        lambda(Fields::Field_acc_x, obj.acc_x);
        lambda(Fields::Field_acc_y, obj.acc_y);
        lambda(Fields::Field_acc_z, obj.acc_z);
        lambda(Fields::Field_rot_x, obj.rot_x);
        lambda(Fields::Field_rot_y, obj.rot_y);
        lambda(Fields::Field_rot_z, obj.rot_z);
        lambda(Fields::Field_dmag_x, obj.dmag_x);
        lambda(Fields::Field_dmag_y, obj.dmag_y);
        lambda(Fields::Field_dmag_z, obj.dmag_z);
        lambda(Fields::Field_dacc_x, obj.dacc_x);
        lambda(Fields::Field_dacc_y, obj.dacc_y);
        lambda(Fields::Field_dacc_z, obj.dacc_z);
        lambda(Fields::Field_drot_x, obj.drot_x);
        lambda(Fields::Field_drot_y, obj.drot_y);
        lambda(Fields::Field_drot_z, obj.drot_z);
    }
};

template <> struct Stencil::TypeTraits<Avid::Pressure>
{
    using Categories = std::tuple<Stencil::Category::Indexable>;
};

template <> struct Stencil::TypeTraitsForIndexable<Avid::Pressure>
{
    enum class Fields
    {
        Invalid,
        Field_pressure
,        Field_temperature
    };

    using Key = Fields;
};

template <> struct Stencil::EnumTraits<Stencil::TypeTraitsForIndexable<Avid::Pressure>::Fields>
{
    using Enum = Stencil::TypeTraitsForIndexable<Avid::Pressure>::Fields;

    static constexpr std::string_view Names[] = {
        "Invalid",
        "pressure"
,        "temperature"
    };

    static std::string_view ToString(Enum type) { return Names[static_cast<size_t>(type)]; }

    static Stencil::TypeTraitsForIndexable<Avid::Pressure>::Fields ForIndex(size_t index)
    {
        return static_cast<Stencil::TypeTraitsForIndexable<Avid::Pressure>::Fields>(index);
    }
};

template <> struct Stencil::Transaction<Avid::Pressure> : Stencil::TransactionT<Avid::Pressure>
{
    using TData  = Avid::Pressure;
    using Fields = Stencil::TypeTraitsForIndexable<Avid::Pressure>::Fields;

    Transaction<double> _subtracker_pressure;
    Transaction<double> _subtracker_temperature;
    CLASS_DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<Avid::Pressure>(ptr)
        ,
        _subtracker_pressure(Obj().pressure)
        ,
        _subtracker_temperature(Obj().temperature)
    {}

    auto& pressure()
    {
        MarkFieldEdited_(Fields::Field_pressure);
        return _subtracker_pressure;
    }
    auto& temperature()
    {
        MarkFieldEdited_(Fields::Field_temperature);
        return _subtracker_temperature;
    }
    void set_pressure(double&& val)
    {
        MarkFieldAssigned_(Fields::Field_pressure, Obj().pressure, val);
        Obj().pressure = std::move(val);
    }

    void set_temperature(double&& val)
    {
        MarkFieldAssigned_(Fields::Field_temperature, Obj().temperature, val);
        Obj().temperature = std::move(val);
    }

    template <typename TLambda> auto Visit(Fields index, TLambda&& lambda)
    {
        switch (index)
        {
        case Fields::Field_pressure: return lambda("pressure", pressure());
        case Fields::Field_temperature: return lambda("temperature", temperature());
        case Fields::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "pressure") { return lambda(Fields::Field_pressure, pressure()); }
        if (fieldName == "temperature") { return lambda(Fields::Field_temperature, temperature()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("pressure", Fields::Field_pressure, pressure(), Obj().pressure);
        lambda("temperature", Fields::Field_temperature, temperature(), Obj().temperature);
    }

    void Flush()
    {
        pressure().Flush();

        if (IsFieldEdited(Fields::Field_pressure))
        {
            if (!pressure().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_pressure));
        }

        temperature().Flush();

        if (IsFieldEdited(Fields::Field_temperature))
        {
            if (!temperature().IsChanged()) _edittracker.reset(static_cast<uint8_t>(Fields::Field_temperature));
        }

        Stencil::TransactionT<Avid::Pressure>::Flush_();
    }
};

template <> struct Stencil::Visitor<Avid::Pressure> : Stencil::VisitorT<Avid::Pressure>
{
    using TData  = Avid::Pressure;
    using Fields = TypeTraitsForIndexable<TData>::Fields;

    template <typename T, typename TLambda> static void VisitKey(T& obj, Fields field, TLambda&& lambda)
    {
        switch (field)
        {
        case Fields::Field_pressure: return lambda(obj.pressure);
        case Fields::Field_temperature: return lambda(obj.temperature);
        case Fields::Invalid: [[fallthrough]];
        default: throw std::logic_error("Invalid Key");
        }
    }

    template <typename T, typename TLambda> static void VisitAllIndicies(T& obj, TLambda&& lambda)
    {
        lambda(Fields::Field_pressure, obj.pressure);
        lambda(Fields::Field_temperature, obj.temperature);
    }
};

#endif
// SECTION END: Template specializations

// SECTION START: Inline Function Definitions
#if true

#endif
// SECTION END: Inline Function Definitions
