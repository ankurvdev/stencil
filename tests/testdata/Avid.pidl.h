#pragma once
#include <stencil/stencil.h>

// SECTION START: DECLARATIONS
#if true
namespace Avid::GPS
{
struct Data;
}
template <> struct ReflectionBase::TypeTraits<Avid::GPS::Data&>;
namespace Avid::Aircraft
{
struct Data;
}
template <> struct ReflectionBase::TypeTraits<Avid::Aircraft::Data&>;
namespace Avid::Traffic
{
struct Data;
}
template <> struct ReflectionBase::TypeTraits<Avid::Traffic::Data&>;
namespace Avid::Motion
{
struct Data;
}
template <> struct ReflectionBase::TypeTraits<Avid::Motion::Data&>;
namespace Avid::Pressure
{
struct Data;
}
template <> struct ReflectionBase::TypeTraits<Avid::Pressure::Data&>;
#endif
// SECTION END: DECLARATIONS

// SECTION START: Definitions
#if true
namespace Avid
{
namespace GPS
{
struct Data;
}
namespace Aircraft
{
struct Data;
}
namespace Traffic
{
struct Data;
}
namespace Motion
{
struct Data;
}
namespace Pressure
{
struct Data;
}
namespace GPS
{

struct Data :
    public ::Stencil::OptionalPropsT<::Avid::GPS::Data>,
    public ::Stencil::TimestampedT<::Avid::GPS::Data>,
    public ReflectionBase::ObjMarker
{
    /*template <typename...TArgs> Data(TArgs&& ... args)
    {
        ReflectionBase::Construct<Data>(this, std::forward<TArgs>(args)...);
    }*/

    enum class FieldIndex
    {
        Invalid,
        heading
,        lat
,        lon
,        speed
,        utc
,        galt
,        climb
,        epx
,        epy
,        epv
,        ept
,        eps
,        epc
,        eph
,        sep
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
               + 1u
               + 1u
               + 1u
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
        case FieldIndex::heading:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::lat:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::lon:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::speed:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::utc:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::galt:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::climb:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::epx:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::epy:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::epv:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::ept:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::eps:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::epc:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::eph:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::sep:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::Invalid: break;

        default: break;
        }
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    private:
    int32_t _heading = {};

    public:
    int32_t&       heading() { return _heading; }
    const int32_t& heading() const { return _heading; }
    void                            heading(int32_t&& val) { _heading = std::move(val); }
    int32_t&       get_heading() { return _heading; }

    bool isset_heading() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::heading); }

    void set_heading(int32_t&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::heading, _heading, val);
        _heading = std::move(val);
    }

#if 0
#endif
    private:
    double _lat = {};

    public:
    double&       lat() { return _lat; }
    const double& lat() const { return _lat; }
    void                            lat(double&& val) { _lat = std::move(val); }
    double&       get_lat() { return _lat; }

    bool isset_lat() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::lat); }

    void set_lat(double&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::lat, _lat, val);
        _lat = std::move(val);
    }

#if 0
#endif
    private:
    double _lon = {};

    public:
    double&       lon() { return _lon; }
    const double& lon() const { return _lon; }
    void                            lon(double&& val) { _lon = std::move(val); }
    double&       get_lon() { return _lon; }

    bool isset_lon() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::lon); }

    void set_lon(double&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::lon, _lon, val);
        _lon = std::move(val);
    }

#if 0
#endif
    private:
    double _speed = {};

    public:
    double&       speed() { return _speed; }
    const double& speed() const { return _speed; }
    void                            speed(double&& val) { _speed = std::move(val); }
    double&       get_speed() { return _speed; }

    bool isset_speed() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::speed); }

    void set_speed(double&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::speed, _speed, val);
        _speed = std::move(val);
    }

#if 0
#endif
    private:
    timestamp _utc = {};

    public:
    timestamp&       utc() { return _utc; }
    const timestamp& utc() const { return _utc; }
    void                            utc(timestamp&& val) { _utc = std::move(val); }
    timestamp&       get_utc() { return _utc; }

    bool isset_utc() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::utc); }

    void set_utc(timestamp&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::utc, _utc, val);
        _utc = std::move(val);
    }

#if 0
#endif
    private:
    double _galt = {};

    public:
    double&       galt() { return _galt; }
    const double& galt() const { return _galt; }
    void                            galt(double&& val) { _galt = std::move(val); }
    double&       get_galt() { return _galt; }

    bool isset_galt() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::galt); }

    void set_galt(double&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::galt, _galt, val);
        _galt = std::move(val);
    }

#if 0
#endif
    private:
    double _climb = {};

    public:
    double&       climb() { return _climb; }
    const double& climb() const { return _climb; }
    void                            climb(double&& val) { _climb = std::move(val); }
    double&       get_climb() { return _climb; }

    bool isset_climb() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::climb); }

    void set_climb(double&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::climb, _climb, val);
        _climb = std::move(val);
    }

#if 0
#endif
    private:
    double _epx = {};

    public:
    double&       epx() { return _epx; }
    const double& epx() const { return _epx; }
    void                            epx(double&& val) { _epx = std::move(val); }
    double&       get_epx() { return _epx; }

    bool isset_epx() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::epx); }

    void set_epx(double&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::epx, _epx, val);
        _epx = std::move(val);
    }

#if 0
#endif
    private:
    double _epy = {};

    public:
    double&       epy() { return _epy; }
    const double& epy() const { return _epy; }
    void                            epy(double&& val) { _epy = std::move(val); }
    double&       get_epy() { return _epy; }

    bool isset_epy() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::epy); }

    void set_epy(double&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::epy, _epy, val);
        _epy = std::move(val);
    }

#if 0
#endif
    private:
    double _epv = {};

    public:
    double&       epv() { return _epv; }
    const double& epv() const { return _epv; }
    void                            epv(double&& val) { _epv = std::move(val); }
    double&       get_epv() { return _epv; }

    bool isset_epv() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::epv); }

    void set_epv(double&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::epv, _epv, val);
        _epv = std::move(val);
    }

#if 0
#endif
    private:
    double _ept = {};

    public:
    double&       ept() { return _ept; }
    const double& ept() const { return _ept; }
    void                            ept(double&& val) { _ept = std::move(val); }
    double&       get_ept() { return _ept; }

    bool isset_ept() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::ept); }

    void set_ept(double&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::ept, _ept, val);
        _ept = std::move(val);
    }

#if 0
#endif
    private:
    double _eps = {};

    public:
    double&       eps() { return _eps; }
    const double& eps() const { return _eps; }
    void                            eps(double&& val) { _eps = std::move(val); }
    double&       get_eps() { return _eps; }

    bool isset_eps() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::eps); }

    void set_eps(double&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::eps, _eps, val);
        _eps = std::move(val);
    }

#if 0
#endif
    private:
    double _epc = {};

    public:
    double&       epc() { return _epc; }
    const double& epc() const { return _epc; }
    void                            epc(double&& val) { _epc = std::move(val); }
    double&       get_epc() { return _epc; }

    bool isset_epc() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::epc); }

    void set_epc(double&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::epc, _epc, val);
        _epc = std::move(val);
    }

#if 0
#endif
    private:
    double _eph = {};

    public:
    double&       eph() { return _eph; }
    const double& eph() const { return _eph; }
    void                            eph(double&& val) { _eph = std::move(val); }
    double&       get_eph() { return _eph; }

    bool isset_eph() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::eph); }

    void set_eph(double&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::eph, _eph, val);
        _eph = std::move(val);
    }

#if 0
#endif
    private:
    double _sep = {};

    public:
    double&       sep() { return _sep; }
    const double& sep() const { return _sep; }
    void                            sep(double&& val) { _sep = std::move(val); }
    double&       get_sep() { return _sep; }

    bool isset_sep() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::sep); }

    void set_sep(double&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::sep, _sep, val);
        _sep = std::move(val);
    }

#if 0
#endif
};

}    // namespace GPS
namespace Aircraft
{

struct Data :
    public ::Stencil::TimestampedT<::Avid::Aircraft::Data>,
    public ReflectionBase::ObjMarker
{
    /*template <typename...TArgs> Data(TArgs&& ... args)
    {
        ReflectionBase::Construct<Data>(this, std::forward<TArgs>(args)...);
    }*/

    enum class FieldIndex
    {
        Invalid,
        seen
,        addr
,        hexaddr
,        flight
,        altitude
,        groundSpeed
,        track
,        lat
,        lon
,        verticalRate
,        messageCount
,        odd_cprlat
,        odd_cprlon
,        odd_cprtime
,        even_cprlat
,        even_cprlon
,        even_cprtime
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
               + 1u
               + 1u
               + 1u
               + 1u
               + 1u
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
        case FieldIndex::seen:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::addr:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::hexaddr:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::flight:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::altitude:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::groundSpeed:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::track:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::lat:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::lon:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::verticalRate:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::messageCount:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::odd_cprlat:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::odd_cprlon:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::odd_cprtime:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::even_cprlat:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::even_cprlon:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::even_cprtime:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::Invalid: break;

        default: break;
        }
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    private:
    timestamp _seen = {};

    public:
    timestamp&       seen() { return _seen; }
    const timestamp& seen() const { return _seen; }
    void                            seen(timestamp&& val) { _seen = std::move(val); }
    timestamp&       get_seen() { return _seen; }

    bool isset_seen() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::seen); }

    void set_seen(timestamp&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::seen, _seen, val);
        _seen = std::move(val);
    }

#if 0
#endif
    private:
    uint32_t _addr = {};

    public:
    uint32_t&       addr() { return _addr; }
    const uint32_t& addr() const { return _addr; }
    void                            addr(uint32_t&& val) { _addr = std::move(val); }
    uint32_t&       get_addr() { return _addr; }

    bool isset_addr() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::addr); }

    void set_addr(uint32_t&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::addr, _addr, val);
        _addr = std::move(val);
    }

#if 0
#endif
    private:
    std::array<uint8_t, 7> _hexaddr = {};

    public:
    std::array<uint8_t, 7>&       hexaddr() { return _hexaddr; }
    const std::array<uint8_t, 7>& hexaddr() const { return _hexaddr; }
    void                            hexaddr(std::array<uint8_t, 7>&& val) { _hexaddr = std::move(val); }
    std::array<uint8_t, 7>&       get_hexaddr() { return _hexaddr; }

    bool isset_hexaddr() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::hexaddr); }

    void set_hexaddr(std::array<uint8_t, 7>&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::hexaddr, _hexaddr, val);
        _hexaddr = std::move(val);
    }

#if 0
    uint8_t at_hexaddr(size_t const& args) const;
#endif
    private:
    std::array<char, 9> _flight = {};

    public:
    std::array<char, 9>&       flight() { return _flight; }
    const std::array<char, 9>& flight() const { return _flight; }
    void                            flight(std::array<char, 9>&& val) { _flight = std::move(val); }
    std::array<char, 9>&       get_flight() { return _flight; }

    bool isset_flight() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::flight); }

    void set_flight(std::array<char, 9>&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::flight, _flight, val);
        _flight = std::move(val);
    }

#if 0
    char at_flight(size_t const& args) const;
#endif
    private:
    int32_t _altitude = {};

    public:
    int32_t&       altitude() { return _altitude; }
    const int32_t& altitude() const { return _altitude; }
    void                            altitude(int32_t&& val) { _altitude = std::move(val); }
    int32_t&       get_altitude() { return _altitude; }

    bool isset_altitude() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::altitude); }

    void set_altitude(int32_t&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::altitude, _altitude, val);
        _altitude = std::move(val);
    }

#if 0
#endif
    private:
    int32_t _groundSpeed = {};

    public:
    int32_t&       groundSpeed() { return _groundSpeed; }
    const int32_t& groundSpeed() const { return _groundSpeed; }
    void                            groundSpeed(int32_t&& val) { _groundSpeed = std::move(val); }
    int32_t&       get_groundSpeed() { return _groundSpeed; }

    bool isset_groundSpeed() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::groundSpeed); }

    void set_groundSpeed(int32_t&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::groundSpeed, _groundSpeed, val);
        _groundSpeed = std::move(val);
    }

#if 0
#endif
    private:
    int32_t _track = {};

    public:
    int32_t&       track() { return _track; }
    const int32_t& track() const { return _track; }
    void                            track(int32_t&& val) { _track = std::move(val); }
    int32_t&       get_track() { return _track; }

    bool isset_track() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::track); }

    void set_track(int32_t&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::track, _track, val);
        _track = std::move(val);
    }

#if 0
#endif
    private:
    double _lat = {};

    public:
    double&       lat() { return _lat; }
    const double& lat() const { return _lat; }
    void                            lat(double&& val) { _lat = std::move(val); }
    double&       get_lat() { return _lat; }

    bool isset_lat() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::lat); }

    void set_lat(double&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::lat, _lat, val);
        _lat = std::move(val);
    }

#if 0
#endif
    private:
    double _lon = {};

    public:
    double&       lon() { return _lon; }
    const double& lon() const { return _lon; }
    void                            lon(double&& val) { _lon = std::move(val); }
    double&       get_lon() { return _lon; }

    bool isset_lon() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::lon); }

    void set_lon(double&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::lon, _lon, val);
        _lon = std::move(val);
    }

#if 0
#endif
    private:
    int32_t _verticalRate = {};

    public:
    int32_t&       verticalRate() { return _verticalRate; }
    const int32_t& verticalRate() const { return _verticalRate; }
    void                            verticalRate(int32_t&& val) { _verticalRate = std::move(val); }
    int32_t&       get_verticalRate() { return _verticalRate; }

    bool isset_verticalRate() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::verticalRate); }

    void set_verticalRate(int32_t&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::verticalRate, _verticalRate, val);
        _verticalRate = std::move(val);
    }

#if 0
#endif
    private:
    int32_t _messageCount = {};

    public:
    int32_t&       messageCount() { return _messageCount; }
    const int32_t& messageCount() const { return _messageCount; }
    void                            messageCount(int32_t&& val) { _messageCount = std::move(val); }
    int32_t&       get_messageCount() { return _messageCount; }

    bool isset_messageCount() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::messageCount); }

    void set_messageCount(int32_t&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::messageCount, _messageCount, val);
        _messageCount = std::move(val);
    }

#if 0
#endif
    private:
    int32_t _odd_cprlat = {};

    public:
    int32_t&       odd_cprlat() { return _odd_cprlat; }
    const int32_t& odd_cprlat() const { return _odd_cprlat; }
    void                            odd_cprlat(int32_t&& val) { _odd_cprlat = std::move(val); }
    int32_t&       get_odd_cprlat() { return _odd_cprlat; }

    bool isset_odd_cprlat() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::odd_cprlat); }

    void set_odd_cprlat(int32_t&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::odd_cprlat, _odd_cprlat, val);
        _odd_cprlat = std::move(val);
    }

#if 0
#endif
    private:
    int32_t _odd_cprlon = {};

    public:
    int32_t&       odd_cprlon() { return _odd_cprlon; }
    const int32_t& odd_cprlon() const { return _odd_cprlon; }
    void                            odd_cprlon(int32_t&& val) { _odd_cprlon = std::move(val); }
    int32_t&       get_odd_cprlon() { return _odd_cprlon; }

    bool isset_odd_cprlon() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::odd_cprlon); }

    void set_odd_cprlon(int32_t&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::odd_cprlon, _odd_cprlon, val);
        _odd_cprlon = std::move(val);
    }

#if 0
#endif
    private:
    timestamp _odd_cprtime = {};

    public:
    timestamp&       odd_cprtime() { return _odd_cprtime; }
    const timestamp& odd_cprtime() const { return _odd_cprtime; }
    void                            odd_cprtime(timestamp&& val) { _odd_cprtime = std::move(val); }
    timestamp&       get_odd_cprtime() { return _odd_cprtime; }

    bool isset_odd_cprtime() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::odd_cprtime); }

    void set_odd_cprtime(timestamp&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::odd_cprtime, _odd_cprtime, val);
        _odd_cprtime = std::move(val);
    }

#if 0
#endif
    private:
    int32_t _even_cprlat = {};

    public:
    int32_t&       even_cprlat() { return _even_cprlat; }
    const int32_t& even_cprlat() const { return _even_cprlat; }
    void                            even_cprlat(int32_t&& val) { _even_cprlat = std::move(val); }
    int32_t&       get_even_cprlat() { return _even_cprlat; }

    bool isset_even_cprlat() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::even_cprlat); }

    void set_even_cprlat(int32_t&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::even_cprlat, _even_cprlat, val);
        _even_cprlat = std::move(val);
    }

#if 0
#endif
    private:
    int32_t _even_cprlon = {};

    public:
    int32_t&       even_cprlon() { return _even_cprlon; }
    const int32_t& even_cprlon() const { return _even_cprlon; }
    void                            even_cprlon(int32_t&& val) { _even_cprlon = std::move(val); }
    int32_t&       get_even_cprlon() { return _even_cprlon; }

    bool isset_even_cprlon() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::even_cprlon); }

    void set_even_cprlon(int32_t&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::even_cprlon, _even_cprlon, val);
        _even_cprlon = std::move(val);
    }

#if 0
#endif
    private:
    timestamp _even_cprtime = {};

    public:
    timestamp&       even_cprtime() { return _even_cprtime; }
    const timestamp& even_cprtime() const { return _even_cprtime; }
    void                            even_cprtime(timestamp&& val) { _even_cprtime = std::move(val); }
    timestamp&       get_even_cprtime() { return _even_cprtime; }

    bool isset_even_cprtime() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::even_cprtime); }

    void set_even_cprtime(timestamp&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::even_cprtime, _even_cprtime, val);
        _even_cprtime = std::move(val);
    }

#if 0
#endif
};

}    // namespace Aircraft
namespace Traffic
{

struct Data :
    public ::Stencil::TimestampedT<::Avid::Traffic::Data>,
    public ReflectionBase::ObjMarker
{
    /*template <typename...TArgs> Data(TArgs&& ... args)
    {
        ReflectionBase::Construct<Data>(this, std::forward<TArgs>(args)...);
    }*/

    enum class FieldIndex
    {
        Invalid,
        aircrafts
    };

    static constexpr size_t FieldCount()
    {
        return 0u
               + 1u
            ;
    }

    static constexpr std::string_view FieldAttributeValue(FieldIndex index, const std::string_view& key)
    {
        switch (index)
        {
        case FieldIndex::aircrafts:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::Invalid: break;

        default: break;
        }
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    private:
    std::vector<::Avid::Aircraft::Data> _aircrafts = {};

    public:
    std::vector<::Avid::Aircraft::Data>&       aircrafts() { return _aircrafts; }
    const std::vector<::Avid::Aircraft::Data>& aircrafts() const { return _aircrafts; }
    void                            aircrafts(std::vector<::Avid::Aircraft::Data>&& val) { _aircrafts = std::move(val); }
    std::vector<::Avid::Aircraft::Data>&       get_aircrafts() { return _aircrafts; }

    bool isset_aircrafts() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::aircrafts); }

    void set_aircrafts(std::vector<::Avid::Aircraft::Data>&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::aircrafts, _aircrafts, val);
        _aircrafts = std::move(val);
    }

#if 0
    void add_aircrafts(::Avid::Aircraft::Data&& args);
    void remove_aircrafts(size_t&& args);
    Stencil::Transaction<::Avid::Aircraft::Data> edit_aircrafts(size_t&& args);
    ::Avid::Aircraft::Data at_aircrafts(size_t const& args) const;
#endif
};

}    // namespace Traffic
namespace Motion
{

struct Data :
    public ::Stencil::TimestampedT<::Avid::Motion::Data>,
    public ReflectionBase::ObjMarker
{
    /*template <typename...TArgs> Data(TArgs&& ... args)
    {
        ReflectionBase::Construct<Data>(this, std::forward<TArgs>(args)...);
    }*/

    enum class FieldIndex
    {
        Invalid,
        mag_x
,        mag_y
,        mag_z
,        acc_x
,        acc_y
,        acc_z
,        rot_x
,        rot_y
,        rot_z
,        dmag_x
,        dmag_y
,        dmag_z
,        dacc_x
,        dacc_y
,        dacc_z
,        drot_x
,        drot_y
,        drot_z
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
               + 1u
               + 1u
               + 1u
               + 1u
               + 1u
               + 1u
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
        case FieldIndex::mag_x:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::mag_y:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::mag_z:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::acc_x:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::acc_y:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::acc_z:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::rot_x:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::rot_y:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::rot_z:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::dmag_x:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::dmag_y:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::dmag_z:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::dacc_x:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::dacc_y:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::dacc_z:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::drot_x:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::drot_y:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::drot_z:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::Invalid: break;

        default: break;
        }
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    private:
    int64_t _mag_x = {};

    public:
    int64_t&       mag_x() { return _mag_x; }
    const int64_t& mag_x() const { return _mag_x; }
    void                            mag_x(int64_t&& val) { _mag_x = std::move(val); }
    int64_t&       get_mag_x() { return _mag_x; }

    bool isset_mag_x() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::mag_x); }

    void set_mag_x(int64_t&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::mag_x, _mag_x, val);
        _mag_x = std::move(val);
    }

#if 0
#endif
    private:
    int64_t _mag_y = {};

    public:
    int64_t&       mag_y() { return _mag_y; }
    const int64_t& mag_y() const { return _mag_y; }
    void                            mag_y(int64_t&& val) { _mag_y = std::move(val); }
    int64_t&       get_mag_y() { return _mag_y; }

    bool isset_mag_y() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::mag_y); }

    void set_mag_y(int64_t&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::mag_y, _mag_y, val);
        _mag_y = std::move(val);
    }

#if 0
#endif
    private:
    int64_t _mag_z = {};

    public:
    int64_t&       mag_z() { return _mag_z; }
    const int64_t& mag_z() const { return _mag_z; }
    void                            mag_z(int64_t&& val) { _mag_z = std::move(val); }
    int64_t&       get_mag_z() { return _mag_z; }

    bool isset_mag_z() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::mag_z); }

    void set_mag_z(int64_t&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::mag_z, _mag_z, val);
        _mag_z = std::move(val);
    }

#if 0
#endif
    private:
    int64_t _acc_x = {};

    public:
    int64_t&       acc_x() { return _acc_x; }
    const int64_t& acc_x() const { return _acc_x; }
    void                            acc_x(int64_t&& val) { _acc_x = std::move(val); }
    int64_t&       get_acc_x() { return _acc_x; }

    bool isset_acc_x() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::acc_x); }

    void set_acc_x(int64_t&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::acc_x, _acc_x, val);
        _acc_x = std::move(val);
    }

#if 0
#endif
    private:
    int64_t _acc_y = {};

    public:
    int64_t&       acc_y() { return _acc_y; }
    const int64_t& acc_y() const { return _acc_y; }
    void                            acc_y(int64_t&& val) { _acc_y = std::move(val); }
    int64_t&       get_acc_y() { return _acc_y; }

    bool isset_acc_y() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::acc_y); }

    void set_acc_y(int64_t&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::acc_y, _acc_y, val);
        _acc_y = std::move(val);
    }

#if 0
#endif
    private:
    int64_t _acc_z = {};

    public:
    int64_t&       acc_z() { return _acc_z; }
    const int64_t& acc_z() const { return _acc_z; }
    void                            acc_z(int64_t&& val) { _acc_z = std::move(val); }
    int64_t&       get_acc_z() { return _acc_z; }

    bool isset_acc_z() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::acc_z); }

    void set_acc_z(int64_t&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::acc_z, _acc_z, val);
        _acc_z = std::move(val);
    }

#if 0
#endif
    private:
    int64_t _rot_x = {};

    public:
    int64_t&       rot_x() { return _rot_x; }
    const int64_t& rot_x() const { return _rot_x; }
    void                            rot_x(int64_t&& val) { _rot_x = std::move(val); }
    int64_t&       get_rot_x() { return _rot_x; }

    bool isset_rot_x() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::rot_x); }

    void set_rot_x(int64_t&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::rot_x, _rot_x, val);
        _rot_x = std::move(val);
    }

#if 0
#endif
    private:
    int64_t _rot_y = {};

    public:
    int64_t&       rot_y() { return _rot_y; }
    const int64_t& rot_y() const { return _rot_y; }
    void                            rot_y(int64_t&& val) { _rot_y = std::move(val); }
    int64_t&       get_rot_y() { return _rot_y; }

    bool isset_rot_y() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::rot_y); }

    void set_rot_y(int64_t&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::rot_y, _rot_y, val);
        _rot_y = std::move(val);
    }

#if 0
#endif
    private:
    int64_t _rot_z = {};

    public:
    int64_t&       rot_z() { return _rot_z; }
    const int64_t& rot_z() const { return _rot_z; }
    void                            rot_z(int64_t&& val) { _rot_z = std::move(val); }
    int64_t&       get_rot_z() { return _rot_z; }

    bool isset_rot_z() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::rot_z); }

    void set_rot_z(int64_t&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::rot_z, _rot_z, val);
        _rot_z = std::move(val);
    }

#if 0
#endif
    private:
    double _dmag_x = {};

    public:
    double&       dmag_x() { return _dmag_x; }
    const double& dmag_x() const { return _dmag_x; }
    void                            dmag_x(double&& val) { _dmag_x = std::move(val); }
    double&       get_dmag_x() { return _dmag_x; }

    bool isset_dmag_x() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::dmag_x); }

    void set_dmag_x(double&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::dmag_x, _dmag_x, val);
        _dmag_x = std::move(val);
    }

#if 0
#endif
    private:
    double _dmag_y = {};

    public:
    double&       dmag_y() { return _dmag_y; }
    const double& dmag_y() const { return _dmag_y; }
    void                            dmag_y(double&& val) { _dmag_y = std::move(val); }
    double&       get_dmag_y() { return _dmag_y; }

    bool isset_dmag_y() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::dmag_y); }

    void set_dmag_y(double&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::dmag_y, _dmag_y, val);
        _dmag_y = std::move(val);
    }

#if 0
#endif
    private:
    double _dmag_z = {};

    public:
    double&       dmag_z() { return _dmag_z; }
    const double& dmag_z() const { return _dmag_z; }
    void                            dmag_z(double&& val) { _dmag_z = std::move(val); }
    double&       get_dmag_z() { return _dmag_z; }

    bool isset_dmag_z() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::dmag_z); }

    void set_dmag_z(double&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::dmag_z, _dmag_z, val);
        _dmag_z = std::move(val);
    }

#if 0
#endif
    private:
    double _dacc_x = {};

    public:
    double&       dacc_x() { return _dacc_x; }
    const double& dacc_x() const { return _dacc_x; }
    void                            dacc_x(double&& val) { _dacc_x = std::move(val); }
    double&       get_dacc_x() { return _dacc_x; }

    bool isset_dacc_x() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::dacc_x); }

    void set_dacc_x(double&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::dacc_x, _dacc_x, val);
        _dacc_x = std::move(val);
    }

#if 0
#endif
    private:
    double _dacc_y = {};

    public:
    double&       dacc_y() { return _dacc_y; }
    const double& dacc_y() const { return _dacc_y; }
    void                            dacc_y(double&& val) { _dacc_y = std::move(val); }
    double&       get_dacc_y() { return _dacc_y; }

    bool isset_dacc_y() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::dacc_y); }

    void set_dacc_y(double&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::dacc_y, _dacc_y, val);
        _dacc_y = std::move(val);
    }

#if 0
#endif
    private:
    double _dacc_z = {};

    public:
    double&       dacc_z() { return _dacc_z; }
    const double& dacc_z() const { return _dacc_z; }
    void                            dacc_z(double&& val) { _dacc_z = std::move(val); }
    double&       get_dacc_z() { return _dacc_z; }

    bool isset_dacc_z() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::dacc_z); }

    void set_dacc_z(double&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::dacc_z, _dacc_z, val);
        _dacc_z = std::move(val);
    }

#if 0
#endif
    private:
    double _drot_x = {};

    public:
    double&       drot_x() { return _drot_x; }
    const double& drot_x() const { return _drot_x; }
    void                            drot_x(double&& val) { _drot_x = std::move(val); }
    double&       get_drot_x() { return _drot_x; }

    bool isset_drot_x() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::drot_x); }

    void set_drot_x(double&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::drot_x, _drot_x, val);
        _drot_x = std::move(val);
    }

#if 0
#endif
    private:
    double _drot_y = {};

    public:
    double&       drot_y() { return _drot_y; }
    const double& drot_y() const { return _drot_y; }
    void                            drot_y(double&& val) { _drot_y = std::move(val); }
    double&       get_drot_y() { return _drot_y; }

    bool isset_drot_y() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::drot_y); }

    void set_drot_y(double&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::drot_y, _drot_y, val);
        _drot_y = std::move(val);
    }

#if 0
#endif
    private:
    double _drot_z = {};

    public:
    double&       drot_z() { return _drot_z; }
    const double& drot_z() const { return _drot_z; }
    void                            drot_z(double&& val) { _drot_z = std::move(val); }
    double&       get_drot_z() { return _drot_z; }

    bool isset_drot_z() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::drot_z); }

    void set_drot_z(double&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::drot_z, _drot_z, val);
        _drot_z = std::move(val);
    }

#if 0
#endif
};

}    // namespace Motion
namespace Pressure
{

struct Data :
    public ::Stencil::OptionalPropsT<::Avid::Pressure::Data>,
    public ::Stencil::TimestampedT<::Avid::Pressure::Data>,
    public ReflectionBase::ObjMarker
{
    /*template <typename...TArgs> Data(TArgs&& ... args)
    {
        ReflectionBase::Construct<Data>(this, std::forward<TArgs>(args)...);
    }*/

    enum class FieldIndex
    {
        Invalid,
        pressure
,        temperature
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
        case FieldIndex::pressure:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::temperature:
            return ::ReflectionServices::EmptyAttributeValue(key);
        case FieldIndex::Invalid: break;

        default: break;
        }
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    private:
    double _pressure = {};

    public:
    double&       pressure() { return _pressure; }
    const double& pressure() const { return _pressure; }
    void                            pressure(double&& val) { _pressure = std::move(val); }
    double&       get_pressure() { return _pressure; }

    bool isset_pressure() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::pressure); }

    void set_pressure(double&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::pressure, _pressure, val);
        _pressure = std::move(val);
    }

#if 0
#endif
    private:
    double _temperature = {};

    public:
    double&       temperature() { return _temperature; }
    const double& temperature() const { return _temperature; }
    void                            temperature(double&& val) { _temperature = std::move(val); }
    double&       get_temperature() { return _temperature; }

    bool isset_temperature() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::temperature); }

    void set_temperature(double&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::temperature, _temperature, val);
        _temperature = std::move(val);
    }

#if 0
#endif
};

}    // namespace Pressure
}    // namespace Avid
#endif
// SECTION END: Definitions

// SECTION START: Template specializations
#if true

// SECTION:
template <> struct ReflectionBase::TypeTraits<Avid::GPS::Data&>
{
    struct Traits_heading
    {
        using TOwner     = Avid::GPS::Data;
        using TFieldType = int32_t;

        static constexpr std::string_view Name() { return "heading"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_heading; }
        static constexpr auto TPropertySetter() { return &TOwner::set_heading; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::heading, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_lat
    {
        using TOwner     = Avid::GPS::Data;
        using TFieldType = double;

        static constexpr std::string_view Name() { return "lat"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_lat; }
        static constexpr auto TPropertySetter() { return &TOwner::set_lat; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::lat, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_lon
    {
        using TOwner     = Avid::GPS::Data;
        using TFieldType = double;

        static constexpr std::string_view Name() { return "lon"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_lon; }
        static constexpr auto TPropertySetter() { return &TOwner::set_lon; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::lon, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_speed
    {
        using TOwner     = Avid::GPS::Data;
        using TFieldType = double;

        static constexpr std::string_view Name() { return "speed"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_speed; }
        static constexpr auto TPropertySetter() { return &TOwner::set_speed; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::speed, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_utc
    {
        using TOwner     = Avid::GPS::Data;
        using TFieldType = timestamp;

        static constexpr std::string_view Name() { return "utc"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_utc; }
        static constexpr auto TPropertySetter() { return &TOwner::set_utc; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::utc, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_galt
    {
        using TOwner     = Avid::GPS::Data;
        using TFieldType = double;

        static constexpr std::string_view Name() { return "galt"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_galt; }
        static constexpr auto TPropertySetter() { return &TOwner::set_galt; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::galt, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_climb
    {
        using TOwner     = Avid::GPS::Data;
        using TFieldType = double;

        static constexpr std::string_view Name() { return "climb"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_climb; }
        static constexpr auto TPropertySetter() { return &TOwner::set_climb; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::climb, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_epx
    {
        using TOwner     = Avid::GPS::Data;
        using TFieldType = double;

        static constexpr std::string_view Name() { return "epx"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_epx; }
        static constexpr auto TPropertySetter() { return &TOwner::set_epx; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::epx, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_epy
    {
        using TOwner     = Avid::GPS::Data;
        using TFieldType = double;

        static constexpr std::string_view Name() { return "epy"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_epy; }
        static constexpr auto TPropertySetter() { return &TOwner::set_epy; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::epy, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_epv
    {
        using TOwner     = Avid::GPS::Data;
        using TFieldType = double;

        static constexpr std::string_view Name() { return "epv"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_epv; }
        static constexpr auto TPropertySetter() { return &TOwner::set_epv; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::epv, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_ept
    {
        using TOwner     = Avid::GPS::Data;
        using TFieldType = double;

        static constexpr std::string_view Name() { return "ept"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_ept; }
        static constexpr auto TPropertySetter() { return &TOwner::set_ept; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::ept, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_eps
    {
        using TOwner     = Avid::GPS::Data;
        using TFieldType = double;

        static constexpr std::string_view Name() { return "eps"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_eps; }
        static constexpr auto TPropertySetter() { return &TOwner::set_eps; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::eps, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_epc
    {
        using TOwner     = Avid::GPS::Data;
        using TFieldType = double;

        static constexpr std::string_view Name() { return "epc"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_epc; }
        static constexpr auto TPropertySetter() { return &TOwner::set_epc; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::epc, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_eph
    {
        using TOwner     = Avid::GPS::Data;
        using TFieldType = double;

        static constexpr std::string_view Name() { return "eph"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_eph; }
        static constexpr auto TPropertySetter() { return &TOwner::set_eph; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::eph, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_sep
    {
        using TOwner     = Avid::GPS::Data;
        using TFieldType = double;

        static constexpr std::string_view Name() { return "sep"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_sep; }
        static constexpr auto TPropertySetter() { return &TOwner::set_sep; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::sep, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "GPS"; }
    static constexpr std::string_view           AttributeValue(const std::string_view& key)
    {
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    using ThisType = Avid::GPS::Data;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && ReflectionBase::AreEqual(obj1.heading(), obj2.heading())
               && ReflectionBase::AreEqual(obj1.lat(), obj2.lat())
               && ReflectionBase::AreEqual(obj1.lon(), obj2.lon())
               && ReflectionBase::AreEqual(obj1.speed(), obj2.speed())
               && ReflectionBase::AreEqual(obj1.utc(), obj2.utc())
               && ReflectionBase::AreEqual(obj1.galt(), obj2.galt())
               && ReflectionBase::AreEqual(obj1.climb(), obj2.climb())
               && ReflectionBase::AreEqual(obj1.epx(), obj2.epx())
               && ReflectionBase::AreEqual(obj1.epy(), obj2.epy())
               && ReflectionBase::AreEqual(obj1.epv(), obj2.epv())
               && ReflectionBase::AreEqual(obj1.ept(), obj2.ept())
               && ReflectionBase::AreEqual(obj1.eps(), obj2.eps())
               && ReflectionBase::AreEqual(obj1.epc(), obj2.epc())
               && ReflectionBase::AreEqual(obj1.eph(), obj2.eph())
               && ReflectionBase::AreEqual(obj1.sep(), obj2.sep())
            ;
    }

    using Handler = ::ReflectionServices::ReflectedStructHandler<Avid::GPS::Data,
                                                                 Traits_heading
,                                                                 Traits_lat
,                                                                 Traits_lon
,                                                                 Traits_speed
,                                                                 Traits_utc
,                                                                 Traits_galt
,                                                                 Traits_climb
,                                                                 Traits_epx
,                                                                 Traits_epy
,                                                                 Traits_epv
,                                                                 Traits_ept
,                                                                 Traits_eps
,                                                                 Traits_epc
,                                                                 Traits_eph
,                                                                 Traits_sep
                                                                 >;
};

template <>
struct Stencil::Transaction<Avid::GPS::Data> : Stencil::TransactionT<Avid::GPS::Data>
{
    using TData = Avid::GPS::Data;

    Transaction<int32_t> _subtracker_heading;
    Transaction<double> _subtracker_lat;
    Transaction<double> _subtracker_lon;
    Transaction<double> _subtracker_speed;
    Transaction<timestamp> _subtracker_utc;
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
    DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<Avid::GPS::Data>(ptr)
        ,
        _subtracker_heading(Obj().heading())
        ,
        _subtracker_lat(Obj().lat())
        ,
        _subtracker_lon(Obj().lon())
        ,
        _subtracker_speed(Obj().speed())
        ,
        _subtracker_utc(Obj().utc())
        ,
        _subtracker_galt(Obj().galt())
        ,
        _subtracker_climb(Obj().climb())
        ,
        _subtracker_epx(Obj().epx())
        ,
        _subtracker_epy(Obj().epy())
        ,
        _subtracker_epv(Obj().epv())
        ,
        _subtracker_ept(Obj().ept())
        ,
        _subtracker_eps(Obj().eps())
        ,
        _subtracker_epc(Obj().epc())
        ,
        _subtracker_eph(Obj().eph())
        ,
        _subtracker_sep(Obj().sep())
    {}

    auto& heading()
    {
        MarkFieldEdited_(TData::FieldIndex::heading);
        return _subtracker_heading;
    }
    auto& lat()
    {
        MarkFieldEdited_(TData::FieldIndex::lat);
        return _subtracker_lat;
    }
    auto& lon()
    {
        MarkFieldEdited_(TData::FieldIndex::lon);
        return _subtracker_lon;
    }
    auto& speed()
    {
        MarkFieldEdited_(TData::FieldIndex::speed);
        return _subtracker_speed;
    }
    auto& utc()
    {
        MarkFieldEdited_(TData::FieldIndex::utc);
        return _subtracker_utc;
    }
    auto& galt()
    {
        MarkFieldEdited_(TData::FieldIndex::galt);
        return _subtracker_galt;
    }
    auto& climb()
    {
        MarkFieldEdited_(TData::FieldIndex::climb);
        return _subtracker_climb;
    }
    auto& epx()
    {
        MarkFieldEdited_(TData::FieldIndex::epx);
        return _subtracker_epx;
    }
    auto& epy()
    {
        MarkFieldEdited_(TData::FieldIndex::epy);
        return _subtracker_epy;
    }
    auto& epv()
    {
        MarkFieldEdited_(TData::FieldIndex::epv);
        return _subtracker_epv;
    }
    auto& ept()
    {
        MarkFieldEdited_(TData::FieldIndex::ept);
        return _subtracker_ept;
    }
    auto& eps()
    {
        MarkFieldEdited_(TData::FieldIndex::eps);
        return _subtracker_eps;
    }
    auto& epc()
    {
        MarkFieldEdited_(TData::FieldIndex::epc);
        return _subtracker_epc;
    }
    auto& eph()
    {
        MarkFieldEdited_(TData::FieldIndex::eph);
        return _subtracker_eph;
    }
    auto& sep()
    {
        MarkFieldEdited_(TData::FieldIndex::sep);
        return _subtracker_sep;
    }
    void set_heading(int32_t&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::heading, Obj().heading(), val);
        Obj().set_heading(std::move(val));
    }

    void set_lat(double&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::lat, Obj().lat(), val);
        Obj().set_lat(std::move(val));
    }

    void set_lon(double&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::lon, Obj().lon(), val);
        Obj().set_lon(std::move(val));
    }

    void set_speed(double&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::speed, Obj().speed(), val);
        Obj().set_speed(std::move(val));
    }

    void set_utc(timestamp&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::utc, Obj().utc(), val);
        Obj().set_utc(std::move(val));
    }

    void set_galt(double&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::galt, Obj().galt(), val);
        Obj().set_galt(std::move(val));
    }

    void set_climb(double&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::climb, Obj().climb(), val);
        Obj().set_climb(std::move(val));
    }

    void set_epx(double&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::epx, Obj().epx(), val);
        Obj().set_epx(std::move(val));
    }

    void set_epy(double&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::epy, Obj().epy(), val);
        Obj().set_epy(std::move(val));
    }

    void set_epv(double&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::epv, Obj().epv(), val);
        Obj().set_epv(std::move(val));
    }

    void set_ept(double&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::ept, Obj().ept(), val);
        Obj().set_ept(std::move(val));
    }

    void set_eps(double&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::eps, Obj().eps(), val);
        Obj().set_eps(std::move(val));
    }

    void set_epc(double&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::epc, Obj().epc(), val);
        Obj().set_epc(std::move(val));
    }

    void set_eph(double&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::eph, Obj().eph(), val);
        Obj().set_eph(std::move(val));
    }

    void set_sep(double&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::sep, Obj().sep(), val);
        Obj().set_sep(std::move(val));
    }

    template <typename TLambda> auto Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::heading: return lambda("heading", heading()); return;
        case TData::FieldIndex::lat: return lambda("lat", lat()); return;
        case TData::FieldIndex::lon: return lambda("lon", lon()); return;
        case TData::FieldIndex::speed: return lambda("speed", speed()); return;
        case TData::FieldIndex::utc: return lambda("utc", utc()); return;
        case TData::FieldIndex::galt: return lambda("galt", galt()); return;
        case TData::FieldIndex::climb: return lambda("climb", climb()); return;
        case TData::FieldIndex::epx: return lambda("epx", epx()); return;
        case TData::FieldIndex::epy: return lambda("epy", epy()); return;
        case TData::FieldIndex::epv: return lambda("epv", epv()); return;
        case TData::FieldIndex::ept: return lambda("ept", ept()); return;
        case TData::FieldIndex::eps: return lambda("eps", eps()); return;
        case TData::FieldIndex::epc: return lambda("epc", epc()); return;
        case TData::FieldIndex::eph: return lambda("eph", eph()); return;
        case TData::FieldIndex::sep: return lambda("sep", sep()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "heading") { return lambda(TData::FieldIndex::heading, heading()); }
        if (fieldName == "lat") { return lambda(TData::FieldIndex::lat, lat()); }
        if (fieldName == "lon") { return lambda(TData::FieldIndex::lon, lon()); }
        if (fieldName == "speed") { return lambda(TData::FieldIndex::speed, speed()); }
        if (fieldName == "utc") { return lambda(TData::FieldIndex::utc, utc()); }
        if (fieldName == "galt") { return lambda(TData::FieldIndex::galt, galt()); }
        if (fieldName == "climb") { return lambda(TData::FieldIndex::climb, climb()); }
        if (fieldName == "epx") { return lambda(TData::FieldIndex::epx, epx()); }
        if (fieldName == "epy") { return lambda(TData::FieldIndex::epy, epy()); }
        if (fieldName == "epv") { return lambda(TData::FieldIndex::epv, epv()); }
        if (fieldName == "ept") { return lambda(TData::FieldIndex::ept, ept()); }
        if (fieldName == "eps") { return lambda(TData::FieldIndex::eps, eps()); }
        if (fieldName == "epc") { return lambda(TData::FieldIndex::epc, epc()); }
        if (fieldName == "eph") { return lambda(TData::FieldIndex::eph, eph()); }
        if (fieldName == "sep") { return lambda(TData::FieldIndex::sep, sep()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("heading", TData::FieldIndex::heading, heading(), Obj().heading());
        lambda("lat", TData::FieldIndex::lat, lat(), Obj().lat());
        lambda("lon", TData::FieldIndex::lon, lon(), Obj().lon());
        lambda("speed", TData::FieldIndex::speed, speed(), Obj().speed());
        lambda("utc", TData::FieldIndex::utc, utc(), Obj().utc());
        lambda("galt", TData::FieldIndex::galt, galt(), Obj().galt());
        lambda("climb", TData::FieldIndex::climb, climb(), Obj().climb());
        lambda("epx", TData::FieldIndex::epx, epx(), Obj().epx());
        lambda("epy", TData::FieldIndex::epy, epy(), Obj().epy());
        lambda("epv", TData::FieldIndex::epv, epv(), Obj().epv());
        lambda("ept", TData::FieldIndex::ept, ept(), Obj().ept());
        lambda("eps", TData::FieldIndex::eps, eps(), Obj().eps());
        lambda("epc", TData::FieldIndex::epc, epc(), Obj().epc());
        lambda("eph", TData::FieldIndex::eph, eph(), Obj().eph());
        lambda("sep", TData::FieldIndex::sep, sep(), Obj().sep());
    }
};

template <>
struct Stencil::Visitor<Avid::GPS::Data, void> : Stencil::VisitorT<Avid::GPS::Data>
{
    using TData = Avid::GPS::Data;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::heading: lambda("heading", _ref.get().heading()); return;
        case TData::FieldIndex::lat: lambda("lat", _ref.get().lat()); return;
        case TData::FieldIndex::lon: lambda("lon", _ref.get().lon()); return;
        case TData::FieldIndex::speed: lambda("speed", _ref.get().speed()); return;
        case TData::FieldIndex::utc: lambda("utc", _ref.get().utc()); return;
        case TData::FieldIndex::galt: lambda("galt", _ref.get().galt()); return;
        case TData::FieldIndex::climb: lambda("climb", _ref.get().climb()); return;
        case TData::FieldIndex::epx: lambda("epx", _ref.get().epx()); return;
        case TData::FieldIndex::epy: lambda("epy", _ref.get().epy()); return;
        case TData::FieldIndex::epv: lambda("epv", _ref.get().epv()); return;
        case TData::FieldIndex::ept: lambda("ept", _ref.get().ept()); return;
        case TData::FieldIndex::eps: lambda("eps", _ref.get().eps()); return;
        case TData::FieldIndex::epc: lambda("epc", _ref.get().epc()); return;
        case TData::FieldIndex::eph: lambda("eph", _ref.get().eph()); return;
        case TData::FieldIndex::sep: lambda("sep", _ref.get().sep()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::heading: lambda("heading", _ref.get().heading()); return;
        case TData::FieldIndex::lat: lambda("lat", _ref.get().lat()); return;
        case TData::FieldIndex::lon: lambda("lon", _ref.get().lon()); return;
        case TData::FieldIndex::speed: lambda("speed", _ref.get().speed()); return;
        case TData::FieldIndex::utc: lambda("utc", _ref.get().utc()); return;
        case TData::FieldIndex::galt: lambda("galt", _ref.get().galt()); return;
        case TData::FieldIndex::climb: lambda("climb", _ref.get().climb()); return;
        case TData::FieldIndex::epx: lambda("epx", _ref.get().epx()); return;
        case TData::FieldIndex::epy: lambda("epy", _ref.get().epy()); return;
        case TData::FieldIndex::epv: lambda("epv", _ref.get().epv()); return;
        case TData::FieldIndex::ept: lambda("ept", _ref.get().ept()); return;
        case TData::FieldIndex::eps: lambda("eps", _ref.get().eps()); return;
        case TData::FieldIndex::epc: lambda("epc", _ref.get().epc()); return;
        case TData::FieldIndex::eph: lambda("eph", _ref.get().eph()); return;
        case TData::FieldIndex::sep: lambda("sep", _ref.get().sep()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        lambda("heading", _ref.get().heading());
        lambda("lat", _ref.get().lat());
        lambda("lon", _ref.get().lon());
        lambda("speed", _ref.get().speed());
        lambda("utc", _ref.get().utc());
        lambda("galt", _ref.get().galt());
        lambda("climb", _ref.get().climb());
        lambda("epx", _ref.get().epx());
        lambda("epy", _ref.get().epy());
        lambda("epv", _ref.get().epv());
        lambda("ept", _ref.get().ept());
        lambda("eps", _ref.get().eps());
        lambda("epc", _ref.get().epc());
        lambda("eph", _ref.get().eph());
        lambda("sep", _ref.get().sep());
    }

    std::reference_wrapper<TData> _ref;
};

template <>
struct Stencil::Visitor<const Avid::GPS::Data, void>
    : Stencil::VisitorT<const Avid::GPS::Data>
{
    using TData = Avid::GPS::Data const;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::heading: lambda("heading", _ref.get().heading()); return;
        case TData::FieldIndex::lat: lambda("lat", _ref.get().lat()); return;
        case TData::FieldIndex::lon: lambda("lon", _ref.get().lon()); return;
        case TData::FieldIndex::speed: lambda("speed", _ref.get().speed()); return;
        case TData::FieldIndex::utc: lambda("utc", _ref.get().utc()); return;
        case TData::FieldIndex::galt: lambda("galt", _ref.get().galt()); return;
        case TData::FieldIndex::climb: lambda("climb", _ref.get().climb()); return;
        case TData::FieldIndex::epx: lambda("epx", _ref.get().epx()); return;
        case TData::FieldIndex::epy: lambda("epy", _ref.get().epy()); return;
        case TData::FieldIndex::epv: lambda("epv", _ref.get().epv()); return;
        case TData::FieldIndex::ept: lambda("ept", _ref.get().ept()); return;
        case TData::FieldIndex::eps: lambda("eps", _ref.get().eps()); return;
        case TData::FieldIndex::epc: lambda("epc", _ref.get().epc()); return;
        case TData::FieldIndex::eph: lambda("eph", _ref.get().eph()); return;
        case TData::FieldIndex::sep: lambda("sep", _ref.get().sep()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        lambda("heading", _ref.get().heading());
        lambda("lat", _ref.get().lat());
        lambda("lon", _ref.get().lon());
        lambda("speed", _ref.get().speed());
        lambda("utc", _ref.get().utc());
        lambda("galt", _ref.get().galt());
        lambda("climb", _ref.get().climb());
        lambda("epx", _ref.get().epx());
        lambda("epy", _ref.get().epy());
        lambda("epv", _ref.get().epv());
        lambda("ept", _ref.get().ept());
        lambda("eps", _ref.get().eps());
        lambda("epc", _ref.get().epc());
        lambda("eph", _ref.get().eph());
        lambda("sep", _ref.get().sep());
    }

    std::reference_wrapper<TData> _ref;
};

template <> struct ReflectionBase::TypeTraits<Avid::Aircraft::Data&>
{
    struct Traits_seen
    {
        using TOwner     = Avid::Aircraft::Data;
        using TFieldType = timestamp;

        static constexpr std::string_view Name() { return "seen"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_seen; }
        static constexpr auto TPropertySetter() { return &TOwner::set_seen; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::seen, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_addr
    {
        using TOwner     = Avid::Aircraft::Data;
        using TFieldType = uint32_t;

        static constexpr std::string_view Name() { return "addr"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_addr; }
        static constexpr auto TPropertySetter() { return &TOwner::set_addr; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::addr, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_hexaddr
    {
        using TOwner     = Avid::Aircraft::Data;
        using TFieldType = std::array<uint8_t, 7>;

        static constexpr std::string_view Name() { return "hexaddr"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_hexaddr; }
        static constexpr auto TPropertySetter() { return &TOwner::set_hexaddr; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::hexaddr, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_flight
    {
        using TOwner     = Avid::Aircraft::Data;
        using TFieldType = std::array<char, 9>;

        static constexpr std::string_view Name() { return "flight"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_flight; }
        static constexpr auto TPropertySetter() { return &TOwner::set_flight; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::flight, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_altitude
    {
        using TOwner     = Avid::Aircraft::Data;
        using TFieldType = int32_t;

        static constexpr std::string_view Name() { return "altitude"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_altitude; }
        static constexpr auto TPropertySetter() { return &TOwner::set_altitude; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::altitude, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_groundSpeed
    {
        using TOwner     = Avid::Aircraft::Data;
        using TFieldType = int32_t;

        static constexpr std::string_view Name() { return "groundSpeed"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_groundSpeed; }
        static constexpr auto TPropertySetter() { return &TOwner::set_groundSpeed; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::groundSpeed, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_track
    {
        using TOwner     = Avid::Aircraft::Data;
        using TFieldType = int32_t;

        static constexpr std::string_view Name() { return "track"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_track; }
        static constexpr auto TPropertySetter() { return &TOwner::set_track; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::track, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_lat
    {
        using TOwner     = Avid::Aircraft::Data;
        using TFieldType = double;

        static constexpr std::string_view Name() { return "lat"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_lat; }
        static constexpr auto TPropertySetter() { return &TOwner::set_lat; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::lat, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_lon
    {
        using TOwner     = Avid::Aircraft::Data;
        using TFieldType = double;

        static constexpr std::string_view Name() { return "lon"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_lon; }
        static constexpr auto TPropertySetter() { return &TOwner::set_lon; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::lon, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_verticalRate
    {
        using TOwner     = Avid::Aircraft::Data;
        using TFieldType = int32_t;

        static constexpr std::string_view Name() { return "verticalRate"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_verticalRate; }
        static constexpr auto TPropertySetter() { return &TOwner::set_verticalRate; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::verticalRate, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_messageCount
    {
        using TOwner     = Avid::Aircraft::Data;
        using TFieldType = int32_t;

        static constexpr std::string_view Name() { return "messageCount"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_messageCount; }
        static constexpr auto TPropertySetter() { return &TOwner::set_messageCount; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::messageCount, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_odd_cprlat
    {
        using TOwner     = Avid::Aircraft::Data;
        using TFieldType = int32_t;

        static constexpr std::string_view Name() { return "odd_cprlat"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_odd_cprlat; }
        static constexpr auto TPropertySetter() { return &TOwner::set_odd_cprlat; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::odd_cprlat, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_odd_cprlon
    {
        using TOwner     = Avid::Aircraft::Data;
        using TFieldType = int32_t;

        static constexpr std::string_view Name() { return "odd_cprlon"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_odd_cprlon; }
        static constexpr auto TPropertySetter() { return &TOwner::set_odd_cprlon; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::odd_cprlon, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_odd_cprtime
    {
        using TOwner     = Avid::Aircraft::Data;
        using TFieldType = timestamp;

        static constexpr std::string_view Name() { return "odd_cprtime"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_odd_cprtime; }
        static constexpr auto TPropertySetter() { return &TOwner::set_odd_cprtime; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::odd_cprtime, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_even_cprlat
    {
        using TOwner     = Avid::Aircraft::Data;
        using TFieldType = int32_t;

        static constexpr std::string_view Name() { return "even_cprlat"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_even_cprlat; }
        static constexpr auto TPropertySetter() { return &TOwner::set_even_cprlat; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::even_cprlat, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_even_cprlon
    {
        using TOwner     = Avid::Aircraft::Data;
        using TFieldType = int32_t;

        static constexpr std::string_view Name() { return "even_cprlon"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_even_cprlon; }
        static constexpr auto TPropertySetter() { return &TOwner::set_even_cprlon; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::even_cprlon, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_even_cprtime
    {
        using TOwner     = Avid::Aircraft::Data;
        using TFieldType = timestamp;

        static constexpr std::string_view Name() { return "even_cprtime"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_even_cprtime; }
        static constexpr auto TPropertySetter() { return &TOwner::set_even_cprtime; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::even_cprtime, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "Aircraft"; }
    static constexpr std::string_view           AttributeValue(const std::string_view& key)
    {
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    using ThisType = Avid::Aircraft::Data;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && ReflectionBase::AreEqual(obj1.seen(), obj2.seen())
               && ReflectionBase::AreEqual(obj1.addr(), obj2.addr())
               && ReflectionBase::AreEqual(obj1.hexaddr(), obj2.hexaddr())
               && ReflectionBase::AreEqual(obj1.flight(), obj2.flight())
               && ReflectionBase::AreEqual(obj1.altitude(), obj2.altitude())
               && ReflectionBase::AreEqual(obj1.groundSpeed(), obj2.groundSpeed())
               && ReflectionBase::AreEqual(obj1.track(), obj2.track())
               && ReflectionBase::AreEqual(obj1.lat(), obj2.lat())
               && ReflectionBase::AreEqual(obj1.lon(), obj2.lon())
               && ReflectionBase::AreEqual(obj1.verticalRate(), obj2.verticalRate())
               && ReflectionBase::AreEqual(obj1.messageCount(), obj2.messageCount())
               && ReflectionBase::AreEqual(obj1.odd_cprlat(), obj2.odd_cprlat())
               && ReflectionBase::AreEqual(obj1.odd_cprlon(), obj2.odd_cprlon())
               && ReflectionBase::AreEqual(obj1.odd_cprtime(), obj2.odd_cprtime())
               && ReflectionBase::AreEqual(obj1.even_cprlat(), obj2.even_cprlat())
               && ReflectionBase::AreEqual(obj1.even_cprlon(), obj2.even_cprlon())
               && ReflectionBase::AreEqual(obj1.even_cprtime(), obj2.even_cprtime())
            ;
    }

    using Handler = ::ReflectionServices::ReflectedStructHandler<Avid::Aircraft::Data,
                                                                 Traits_seen
,                                                                 Traits_addr
,                                                                 Traits_hexaddr
,                                                                 Traits_flight
,                                                                 Traits_altitude
,                                                                 Traits_groundSpeed
,                                                                 Traits_track
,                                                                 Traits_lat
,                                                                 Traits_lon
,                                                                 Traits_verticalRate
,                                                                 Traits_messageCount
,                                                                 Traits_odd_cprlat
,                                                                 Traits_odd_cprlon
,                                                                 Traits_odd_cprtime
,                                                                 Traits_even_cprlat
,                                                                 Traits_even_cprlon
,                                                                 Traits_even_cprtime
                                                                 >;
};

template <>
struct Stencil::Transaction<Avid::Aircraft::Data> : Stencil::TransactionT<Avid::Aircraft::Data>
{
    using TData = Avid::Aircraft::Data;

    Transaction<timestamp> _subtracker_seen;
    Transaction<uint32_t> _subtracker_addr;
    Transaction<std::array<uint8_t, 7>> _subtracker_hexaddr;
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
    Transaction<timestamp> _subtracker_odd_cprtime;
    Transaction<int32_t> _subtracker_even_cprlat;
    Transaction<int32_t> _subtracker_even_cprlon;
    Transaction<timestamp> _subtracker_even_cprtime;
    DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<Avid::Aircraft::Data>(ptr)
        ,
        _subtracker_seen(Obj().seen())
        ,
        _subtracker_addr(Obj().addr())
        ,
        _subtracker_hexaddr(Obj().hexaddr())
        ,
        _subtracker_flight(Obj().flight())
        ,
        _subtracker_altitude(Obj().altitude())
        ,
        _subtracker_groundSpeed(Obj().groundSpeed())
        ,
        _subtracker_track(Obj().track())
        ,
        _subtracker_lat(Obj().lat())
        ,
        _subtracker_lon(Obj().lon())
        ,
        _subtracker_verticalRate(Obj().verticalRate())
        ,
        _subtracker_messageCount(Obj().messageCount())
        ,
        _subtracker_odd_cprlat(Obj().odd_cprlat())
        ,
        _subtracker_odd_cprlon(Obj().odd_cprlon())
        ,
        _subtracker_odd_cprtime(Obj().odd_cprtime())
        ,
        _subtracker_even_cprlat(Obj().even_cprlat())
        ,
        _subtracker_even_cprlon(Obj().even_cprlon())
        ,
        _subtracker_even_cprtime(Obj().even_cprtime())
    {}

    auto& seen()
    {
        MarkFieldEdited_(TData::FieldIndex::seen);
        return _subtracker_seen;
    }
    auto& addr()
    {
        MarkFieldEdited_(TData::FieldIndex::addr);
        return _subtracker_addr;
    }
    auto& hexaddr()
    {
        MarkFieldEdited_(TData::FieldIndex::hexaddr);
        return _subtracker_hexaddr;
    }
    auto& flight()
    {
        MarkFieldEdited_(TData::FieldIndex::flight);
        return _subtracker_flight;
    }
    auto& altitude()
    {
        MarkFieldEdited_(TData::FieldIndex::altitude);
        return _subtracker_altitude;
    }
    auto& groundSpeed()
    {
        MarkFieldEdited_(TData::FieldIndex::groundSpeed);
        return _subtracker_groundSpeed;
    }
    auto& track()
    {
        MarkFieldEdited_(TData::FieldIndex::track);
        return _subtracker_track;
    }
    auto& lat()
    {
        MarkFieldEdited_(TData::FieldIndex::lat);
        return _subtracker_lat;
    }
    auto& lon()
    {
        MarkFieldEdited_(TData::FieldIndex::lon);
        return _subtracker_lon;
    }
    auto& verticalRate()
    {
        MarkFieldEdited_(TData::FieldIndex::verticalRate);
        return _subtracker_verticalRate;
    }
    auto& messageCount()
    {
        MarkFieldEdited_(TData::FieldIndex::messageCount);
        return _subtracker_messageCount;
    }
    auto& odd_cprlat()
    {
        MarkFieldEdited_(TData::FieldIndex::odd_cprlat);
        return _subtracker_odd_cprlat;
    }
    auto& odd_cprlon()
    {
        MarkFieldEdited_(TData::FieldIndex::odd_cprlon);
        return _subtracker_odd_cprlon;
    }
    auto& odd_cprtime()
    {
        MarkFieldEdited_(TData::FieldIndex::odd_cprtime);
        return _subtracker_odd_cprtime;
    }
    auto& even_cprlat()
    {
        MarkFieldEdited_(TData::FieldIndex::even_cprlat);
        return _subtracker_even_cprlat;
    }
    auto& even_cprlon()
    {
        MarkFieldEdited_(TData::FieldIndex::even_cprlon);
        return _subtracker_even_cprlon;
    }
    auto& even_cprtime()
    {
        MarkFieldEdited_(TData::FieldIndex::even_cprtime);
        return _subtracker_even_cprtime;
    }
    void set_seen(timestamp&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::seen, Obj().seen(), val);
        Obj().set_seen(std::move(val));
    }

    void set_addr(uint32_t&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::addr, Obj().addr(), val);
        Obj().set_addr(std::move(val));
    }

    void set_hexaddr(std::array<uint8_t, 7>&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::hexaddr, Obj().hexaddr(), val);
        Obj().set_hexaddr(std::move(val));
    }

    void set_flight(std::array<char, 9>&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::flight, Obj().flight(), val);
        Obj().set_flight(std::move(val));
    }

    void set_altitude(int32_t&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::altitude, Obj().altitude(), val);
        Obj().set_altitude(std::move(val));
    }

    void set_groundSpeed(int32_t&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::groundSpeed, Obj().groundSpeed(), val);
        Obj().set_groundSpeed(std::move(val));
    }

    void set_track(int32_t&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::track, Obj().track(), val);
        Obj().set_track(std::move(val));
    }

    void set_lat(double&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::lat, Obj().lat(), val);
        Obj().set_lat(std::move(val));
    }

    void set_lon(double&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::lon, Obj().lon(), val);
        Obj().set_lon(std::move(val));
    }

    void set_verticalRate(int32_t&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::verticalRate, Obj().verticalRate(), val);
        Obj().set_verticalRate(std::move(val));
    }

    void set_messageCount(int32_t&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::messageCount, Obj().messageCount(), val);
        Obj().set_messageCount(std::move(val));
    }

    void set_odd_cprlat(int32_t&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::odd_cprlat, Obj().odd_cprlat(), val);
        Obj().set_odd_cprlat(std::move(val));
    }

    void set_odd_cprlon(int32_t&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::odd_cprlon, Obj().odd_cprlon(), val);
        Obj().set_odd_cprlon(std::move(val));
    }

    void set_odd_cprtime(timestamp&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::odd_cprtime, Obj().odd_cprtime(), val);
        Obj().set_odd_cprtime(std::move(val));
    }

    void set_even_cprlat(int32_t&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::even_cprlat, Obj().even_cprlat(), val);
        Obj().set_even_cprlat(std::move(val));
    }

    void set_even_cprlon(int32_t&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::even_cprlon, Obj().even_cprlon(), val);
        Obj().set_even_cprlon(std::move(val));
    }

    void set_even_cprtime(timestamp&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::even_cprtime, Obj().even_cprtime(), val);
        Obj().set_even_cprtime(std::move(val));
    }

    template <typename TLambda> auto Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::seen: return lambda("seen", seen()); return;
        case TData::FieldIndex::addr: return lambda("addr", addr()); return;
        case TData::FieldIndex::hexaddr: return lambda("hexaddr", hexaddr()); return;
        case TData::FieldIndex::flight: return lambda("flight", flight()); return;
        case TData::FieldIndex::altitude: return lambda("altitude", altitude()); return;
        case TData::FieldIndex::groundSpeed: return lambda("groundSpeed", groundSpeed()); return;
        case TData::FieldIndex::track: return lambda("track", track()); return;
        case TData::FieldIndex::lat: return lambda("lat", lat()); return;
        case TData::FieldIndex::lon: return lambda("lon", lon()); return;
        case TData::FieldIndex::verticalRate: return lambda("verticalRate", verticalRate()); return;
        case TData::FieldIndex::messageCount: return lambda("messageCount", messageCount()); return;
        case TData::FieldIndex::odd_cprlat: return lambda("odd_cprlat", odd_cprlat()); return;
        case TData::FieldIndex::odd_cprlon: return lambda("odd_cprlon", odd_cprlon()); return;
        case TData::FieldIndex::odd_cprtime: return lambda("odd_cprtime", odd_cprtime()); return;
        case TData::FieldIndex::even_cprlat: return lambda("even_cprlat", even_cprlat()); return;
        case TData::FieldIndex::even_cprlon: return lambda("even_cprlon", even_cprlon()); return;
        case TData::FieldIndex::even_cprtime: return lambda("even_cprtime", even_cprtime()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "seen") { return lambda(TData::FieldIndex::seen, seen()); }
        if (fieldName == "addr") { return lambda(TData::FieldIndex::addr, addr()); }
        if (fieldName == "hexaddr") { return lambda(TData::FieldIndex::hexaddr, hexaddr()); }
        if (fieldName == "flight") { return lambda(TData::FieldIndex::flight, flight()); }
        if (fieldName == "altitude") { return lambda(TData::FieldIndex::altitude, altitude()); }
        if (fieldName == "groundSpeed") { return lambda(TData::FieldIndex::groundSpeed, groundSpeed()); }
        if (fieldName == "track") { return lambda(TData::FieldIndex::track, track()); }
        if (fieldName == "lat") { return lambda(TData::FieldIndex::lat, lat()); }
        if (fieldName == "lon") { return lambda(TData::FieldIndex::lon, lon()); }
        if (fieldName == "verticalRate") { return lambda(TData::FieldIndex::verticalRate, verticalRate()); }
        if (fieldName == "messageCount") { return lambda(TData::FieldIndex::messageCount, messageCount()); }
        if (fieldName == "odd_cprlat") { return lambda(TData::FieldIndex::odd_cprlat, odd_cprlat()); }
        if (fieldName == "odd_cprlon") { return lambda(TData::FieldIndex::odd_cprlon, odd_cprlon()); }
        if (fieldName == "odd_cprtime") { return lambda(TData::FieldIndex::odd_cprtime, odd_cprtime()); }
        if (fieldName == "even_cprlat") { return lambda(TData::FieldIndex::even_cprlat, even_cprlat()); }
        if (fieldName == "even_cprlon") { return lambda(TData::FieldIndex::even_cprlon, even_cprlon()); }
        if (fieldName == "even_cprtime") { return lambda(TData::FieldIndex::even_cprtime, even_cprtime()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("seen", TData::FieldIndex::seen, seen(), Obj().seen());
        lambda("addr", TData::FieldIndex::addr, addr(), Obj().addr());
        lambda("hexaddr", TData::FieldIndex::hexaddr, hexaddr(), Obj().hexaddr());
        lambda("flight", TData::FieldIndex::flight, flight(), Obj().flight());
        lambda("altitude", TData::FieldIndex::altitude, altitude(), Obj().altitude());
        lambda("groundSpeed", TData::FieldIndex::groundSpeed, groundSpeed(), Obj().groundSpeed());
        lambda("track", TData::FieldIndex::track, track(), Obj().track());
        lambda("lat", TData::FieldIndex::lat, lat(), Obj().lat());
        lambda("lon", TData::FieldIndex::lon, lon(), Obj().lon());
        lambda("verticalRate", TData::FieldIndex::verticalRate, verticalRate(), Obj().verticalRate());
        lambda("messageCount", TData::FieldIndex::messageCount, messageCount(), Obj().messageCount());
        lambda("odd_cprlat", TData::FieldIndex::odd_cprlat, odd_cprlat(), Obj().odd_cprlat());
        lambda("odd_cprlon", TData::FieldIndex::odd_cprlon, odd_cprlon(), Obj().odd_cprlon());
        lambda("odd_cprtime", TData::FieldIndex::odd_cprtime, odd_cprtime(), Obj().odd_cprtime());
        lambda("even_cprlat", TData::FieldIndex::even_cprlat, even_cprlat(), Obj().even_cprlat());
        lambda("even_cprlon", TData::FieldIndex::even_cprlon, even_cprlon(), Obj().even_cprlon());
        lambda("even_cprtime", TData::FieldIndex::even_cprtime, even_cprtime(), Obj().even_cprtime());
    }
};

template <>
struct Stencil::Visitor<Avid::Aircraft::Data, void> : Stencil::VisitorT<Avid::Aircraft::Data>
{
    using TData = Avid::Aircraft::Data;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::seen: lambda("seen", _ref.get().seen()); return;
        case TData::FieldIndex::addr: lambda("addr", _ref.get().addr()); return;
        case TData::FieldIndex::hexaddr: lambda("hexaddr", _ref.get().hexaddr()); return;
        case TData::FieldIndex::flight: lambda("flight", _ref.get().flight()); return;
        case TData::FieldIndex::altitude: lambda("altitude", _ref.get().altitude()); return;
        case TData::FieldIndex::groundSpeed: lambda("groundSpeed", _ref.get().groundSpeed()); return;
        case TData::FieldIndex::track: lambda("track", _ref.get().track()); return;
        case TData::FieldIndex::lat: lambda("lat", _ref.get().lat()); return;
        case TData::FieldIndex::lon: lambda("lon", _ref.get().lon()); return;
        case TData::FieldIndex::verticalRate: lambda("verticalRate", _ref.get().verticalRate()); return;
        case TData::FieldIndex::messageCount: lambda("messageCount", _ref.get().messageCount()); return;
        case TData::FieldIndex::odd_cprlat: lambda("odd_cprlat", _ref.get().odd_cprlat()); return;
        case TData::FieldIndex::odd_cprlon: lambda("odd_cprlon", _ref.get().odd_cprlon()); return;
        case TData::FieldIndex::odd_cprtime: lambda("odd_cprtime", _ref.get().odd_cprtime()); return;
        case TData::FieldIndex::even_cprlat: lambda("even_cprlat", _ref.get().even_cprlat()); return;
        case TData::FieldIndex::even_cprlon: lambda("even_cprlon", _ref.get().even_cprlon()); return;
        case TData::FieldIndex::even_cprtime: lambda("even_cprtime", _ref.get().even_cprtime()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::seen: lambda("seen", _ref.get().seen()); return;
        case TData::FieldIndex::addr: lambda("addr", _ref.get().addr()); return;
        case TData::FieldIndex::hexaddr: lambda("hexaddr", _ref.get().hexaddr()); return;
        case TData::FieldIndex::flight: lambda("flight", _ref.get().flight()); return;
        case TData::FieldIndex::altitude: lambda("altitude", _ref.get().altitude()); return;
        case TData::FieldIndex::groundSpeed: lambda("groundSpeed", _ref.get().groundSpeed()); return;
        case TData::FieldIndex::track: lambda("track", _ref.get().track()); return;
        case TData::FieldIndex::lat: lambda("lat", _ref.get().lat()); return;
        case TData::FieldIndex::lon: lambda("lon", _ref.get().lon()); return;
        case TData::FieldIndex::verticalRate: lambda("verticalRate", _ref.get().verticalRate()); return;
        case TData::FieldIndex::messageCount: lambda("messageCount", _ref.get().messageCount()); return;
        case TData::FieldIndex::odd_cprlat: lambda("odd_cprlat", _ref.get().odd_cprlat()); return;
        case TData::FieldIndex::odd_cprlon: lambda("odd_cprlon", _ref.get().odd_cprlon()); return;
        case TData::FieldIndex::odd_cprtime: lambda("odd_cprtime", _ref.get().odd_cprtime()); return;
        case TData::FieldIndex::even_cprlat: lambda("even_cprlat", _ref.get().even_cprlat()); return;
        case TData::FieldIndex::even_cprlon: lambda("even_cprlon", _ref.get().even_cprlon()); return;
        case TData::FieldIndex::even_cprtime: lambda("even_cprtime", _ref.get().even_cprtime()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        lambda("seen", _ref.get().seen());
        lambda("addr", _ref.get().addr());
        lambda("hexaddr", _ref.get().hexaddr());
        lambda("flight", _ref.get().flight());
        lambda("altitude", _ref.get().altitude());
        lambda("groundSpeed", _ref.get().groundSpeed());
        lambda("track", _ref.get().track());
        lambda("lat", _ref.get().lat());
        lambda("lon", _ref.get().lon());
        lambda("verticalRate", _ref.get().verticalRate());
        lambda("messageCount", _ref.get().messageCount());
        lambda("odd_cprlat", _ref.get().odd_cprlat());
        lambda("odd_cprlon", _ref.get().odd_cprlon());
        lambda("odd_cprtime", _ref.get().odd_cprtime());
        lambda("even_cprlat", _ref.get().even_cprlat());
        lambda("even_cprlon", _ref.get().even_cprlon());
        lambda("even_cprtime", _ref.get().even_cprtime());
    }

    std::reference_wrapper<TData> _ref;
};

template <>
struct Stencil::Visitor<const Avid::Aircraft::Data, void>
    : Stencil::VisitorT<const Avid::Aircraft::Data>
{
    using TData = Avid::Aircraft::Data const;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::seen: lambda("seen", _ref.get().seen()); return;
        case TData::FieldIndex::addr: lambda("addr", _ref.get().addr()); return;
        case TData::FieldIndex::hexaddr: lambda("hexaddr", _ref.get().hexaddr()); return;
        case TData::FieldIndex::flight: lambda("flight", _ref.get().flight()); return;
        case TData::FieldIndex::altitude: lambda("altitude", _ref.get().altitude()); return;
        case TData::FieldIndex::groundSpeed: lambda("groundSpeed", _ref.get().groundSpeed()); return;
        case TData::FieldIndex::track: lambda("track", _ref.get().track()); return;
        case TData::FieldIndex::lat: lambda("lat", _ref.get().lat()); return;
        case TData::FieldIndex::lon: lambda("lon", _ref.get().lon()); return;
        case TData::FieldIndex::verticalRate: lambda("verticalRate", _ref.get().verticalRate()); return;
        case TData::FieldIndex::messageCount: lambda("messageCount", _ref.get().messageCount()); return;
        case TData::FieldIndex::odd_cprlat: lambda("odd_cprlat", _ref.get().odd_cprlat()); return;
        case TData::FieldIndex::odd_cprlon: lambda("odd_cprlon", _ref.get().odd_cprlon()); return;
        case TData::FieldIndex::odd_cprtime: lambda("odd_cprtime", _ref.get().odd_cprtime()); return;
        case TData::FieldIndex::even_cprlat: lambda("even_cprlat", _ref.get().even_cprlat()); return;
        case TData::FieldIndex::even_cprlon: lambda("even_cprlon", _ref.get().even_cprlon()); return;
        case TData::FieldIndex::even_cprtime: lambda("even_cprtime", _ref.get().even_cprtime()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        lambda("seen", _ref.get().seen());
        lambda("addr", _ref.get().addr());
        lambda("hexaddr", _ref.get().hexaddr());
        lambda("flight", _ref.get().flight());
        lambda("altitude", _ref.get().altitude());
        lambda("groundSpeed", _ref.get().groundSpeed());
        lambda("track", _ref.get().track());
        lambda("lat", _ref.get().lat());
        lambda("lon", _ref.get().lon());
        lambda("verticalRate", _ref.get().verticalRate());
        lambda("messageCount", _ref.get().messageCount());
        lambda("odd_cprlat", _ref.get().odd_cprlat());
        lambda("odd_cprlon", _ref.get().odd_cprlon());
        lambda("odd_cprtime", _ref.get().odd_cprtime());
        lambda("even_cprlat", _ref.get().even_cprlat());
        lambda("even_cprlon", _ref.get().even_cprlon());
        lambda("even_cprtime", _ref.get().even_cprtime());
    }

    std::reference_wrapper<TData> _ref;
};

template <> struct ReflectionBase::TypeTraits<Avid::Traffic::Data&>
{
    struct Traits_aircrafts
    {
        using TOwner     = Avid::Traffic::Data;
        using TFieldType = std::vector<::Avid::Aircraft::Data>;

        static constexpr std::string_view Name() { return "aircrafts"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_aircrafts; }
        static constexpr auto TPropertySetter() { return &TOwner::set_aircrafts; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::aircrafts, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "Traffic"; }
    static constexpr std::string_view           AttributeValue(const std::string_view& key)
    {
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    using ThisType = Avid::Traffic::Data;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && ReflectionBase::AreEqual(obj1.aircrafts(), obj2.aircrafts())
            ;
    }

    using Handler = ::ReflectionServices::ReflectedStructHandler<Avid::Traffic::Data,
                                                                 Traits_aircrafts
                                                                 >;
};

template <>
struct Stencil::Transaction<Avid::Traffic::Data> : Stencil::TransactionT<Avid::Traffic::Data>
{
    using TData = Avid::Traffic::Data;

    Transaction<std::vector<::Avid::Aircraft::Data>> _subtracker_aircrafts;
    DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<Avid::Traffic::Data>(ptr)
        ,
        _subtracker_aircrafts(Obj().aircrafts())
    {}

    auto& aircrafts()
    {
        MarkFieldEdited_(TData::FieldIndex::aircrafts);
        return _subtracker_aircrafts;
    }
    void set_aircrafts(std::vector<::Avid::Aircraft::Data>&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::aircrafts, Obj().aircrafts(), val);
        Obj().set_aircrafts(std::move(val));
    }

    void add_aircrafts(::Avid::Aircraft::Data&& args)
    {
        MarkFieldEdited_(TData::FieldIndex::aircrafts);
        aircrafts().RecordMutation_add_(args);
        return Stencil::Mutators<std::vector<::Avid::Aircraft::Data>>::add(Obj().aircrafts(), std::move(args));
    }
    void remove_aircrafts(size_t&& args)
    {
        MarkFieldEdited_(TData::FieldIndex::aircrafts);
        aircrafts().RecordMutation_remove_(args);
        return Stencil::Mutators<std::vector<::Avid::Aircraft::Data>>::remove(Obj().aircrafts(), std::move(args));
    }
    Stencil::Transaction<::Avid::Aircraft::Data> edit_aircrafts(size_t&& args)
    {
        MarkFieldEdited_(TData::FieldIndex::aircrafts);
        aircrafts().RecordMutation_edit_(args);
        return Stencil::Mutators<std::vector<::Avid::Aircraft::Data>>::edit(Obj().aircrafts(), std::move(args));
    }
    template <typename TLambda> auto Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::aircrafts: return lambda("aircrafts", aircrafts()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "aircrafts") { return lambda(TData::FieldIndex::aircrafts, aircrafts()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("aircrafts", TData::FieldIndex::aircrafts, aircrafts(), Obj().aircrafts());
    }
};

template <>
struct Stencil::Visitor<Avid::Traffic::Data, void> : Stencil::VisitorT<Avid::Traffic::Data>
{
    using TData = Avid::Traffic::Data;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::aircrafts: lambda("aircrafts", _ref.get().aircrafts()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::aircrafts: lambda("aircrafts", _ref.get().aircrafts()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        lambda("aircrafts", _ref.get().aircrafts());
    }

    std::reference_wrapper<TData> _ref;
};

template <>
struct Stencil::Visitor<const Avid::Traffic::Data, void>
    : Stencil::VisitorT<const Avid::Traffic::Data>
{
    using TData = Avid::Traffic::Data const;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::aircrafts: lambda("aircrafts", _ref.get().aircrafts()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        lambda("aircrafts", _ref.get().aircrafts());
    }

    std::reference_wrapper<TData> _ref;
};

template <> struct ReflectionBase::TypeTraits<Avid::Motion::Data&>
{
    struct Traits_mag_x
    {
        using TOwner     = Avid::Motion::Data;
        using TFieldType = int64_t;

        static constexpr std::string_view Name() { return "mag_x"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_mag_x; }
        static constexpr auto TPropertySetter() { return &TOwner::set_mag_x; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::mag_x, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_mag_y
    {
        using TOwner     = Avid::Motion::Data;
        using TFieldType = int64_t;

        static constexpr std::string_view Name() { return "mag_y"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_mag_y; }
        static constexpr auto TPropertySetter() { return &TOwner::set_mag_y; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::mag_y, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_mag_z
    {
        using TOwner     = Avid::Motion::Data;
        using TFieldType = int64_t;

        static constexpr std::string_view Name() { return "mag_z"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_mag_z; }
        static constexpr auto TPropertySetter() { return &TOwner::set_mag_z; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::mag_z, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_acc_x
    {
        using TOwner     = Avid::Motion::Data;
        using TFieldType = int64_t;

        static constexpr std::string_view Name() { return "acc_x"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_acc_x; }
        static constexpr auto TPropertySetter() { return &TOwner::set_acc_x; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::acc_x, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_acc_y
    {
        using TOwner     = Avid::Motion::Data;
        using TFieldType = int64_t;

        static constexpr std::string_view Name() { return "acc_y"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_acc_y; }
        static constexpr auto TPropertySetter() { return &TOwner::set_acc_y; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::acc_y, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_acc_z
    {
        using TOwner     = Avid::Motion::Data;
        using TFieldType = int64_t;

        static constexpr std::string_view Name() { return "acc_z"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_acc_z; }
        static constexpr auto TPropertySetter() { return &TOwner::set_acc_z; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::acc_z, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_rot_x
    {
        using TOwner     = Avid::Motion::Data;
        using TFieldType = int64_t;

        static constexpr std::string_view Name() { return "rot_x"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_rot_x; }
        static constexpr auto TPropertySetter() { return &TOwner::set_rot_x; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::rot_x, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_rot_y
    {
        using TOwner     = Avid::Motion::Data;
        using TFieldType = int64_t;

        static constexpr std::string_view Name() { return "rot_y"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_rot_y; }
        static constexpr auto TPropertySetter() { return &TOwner::set_rot_y; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::rot_y, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_rot_z
    {
        using TOwner     = Avid::Motion::Data;
        using TFieldType = int64_t;

        static constexpr std::string_view Name() { return "rot_z"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_rot_z; }
        static constexpr auto TPropertySetter() { return &TOwner::set_rot_z; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::rot_z, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_dmag_x
    {
        using TOwner     = Avid::Motion::Data;
        using TFieldType = double;

        static constexpr std::string_view Name() { return "dmag_x"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_dmag_x; }
        static constexpr auto TPropertySetter() { return &TOwner::set_dmag_x; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::dmag_x, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_dmag_y
    {
        using TOwner     = Avid::Motion::Data;
        using TFieldType = double;

        static constexpr std::string_view Name() { return "dmag_y"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_dmag_y; }
        static constexpr auto TPropertySetter() { return &TOwner::set_dmag_y; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::dmag_y, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_dmag_z
    {
        using TOwner     = Avid::Motion::Data;
        using TFieldType = double;

        static constexpr std::string_view Name() { return "dmag_z"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_dmag_z; }
        static constexpr auto TPropertySetter() { return &TOwner::set_dmag_z; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::dmag_z, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_dacc_x
    {
        using TOwner     = Avid::Motion::Data;
        using TFieldType = double;

        static constexpr std::string_view Name() { return "dacc_x"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_dacc_x; }
        static constexpr auto TPropertySetter() { return &TOwner::set_dacc_x; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::dacc_x, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_dacc_y
    {
        using TOwner     = Avid::Motion::Data;
        using TFieldType = double;

        static constexpr std::string_view Name() { return "dacc_y"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_dacc_y; }
        static constexpr auto TPropertySetter() { return &TOwner::set_dacc_y; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::dacc_y, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_dacc_z
    {
        using TOwner     = Avid::Motion::Data;
        using TFieldType = double;

        static constexpr std::string_view Name() { return "dacc_z"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_dacc_z; }
        static constexpr auto TPropertySetter() { return &TOwner::set_dacc_z; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::dacc_z, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_drot_x
    {
        using TOwner     = Avid::Motion::Data;
        using TFieldType = double;

        static constexpr std::string_view Name() { return "drot_x"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_drot_x; }
        static constexpr auto TPropertySetter() { return &TOwner::set_drot_x; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::drot_x, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_drot_y
    {
        using TOwner     = Avid::Motion::Data;
        using TFieldType = double;

        static constexpr std::string_view Name() { return "drot_y"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_drot_y; }
        static constexpr auto TPropertySetter() { return &TOwner::set_drot_y; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::drot_y, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_drot_z
    {
        using TOwner     = Avid::Motion::Data;
        using TFieldType = double;

        static constexpr std::string_view Name() { return "drot_z"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_drot_z; }
        static constexpr auto TPropertySetter() { return &TOwner::set_drot_z; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::drot_z, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "Motion"; }
    static constexpr std::string_view           AttributeValue(const std::string_view& key)
    {
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    using ThisType = Avid::Motion::Data;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && ReflectionBase::AreEqual(obj1.mag_x(), obj2.mag_x())
               && ReflectionBase::AreEqual(obj1.mag_y(), obj2.mag_y())
               && ReflectionBase::AreEqual(obj1.mag_z(), obj2.mag_z())
               && ReflectionBase::AreEqual(obj1.acc_x(), obj2.acc_x())
               && ReflectionBase::AreEqual(obj1.acc_y(), obj2.acc_y())
               && ReflectionBase::AreEqual(obj1.acc_z(), obj2.acc_z())
               && ReflectionBase::AreEqual(obj1.rot_x(), obj2.rot_x())
               && ReflectionBase::AreEqual(obj1.rot_y(), obj2.rot_y())
               && ReflectionBase::AreEqual(obj1.rot_z(), obj2.rot_z())
               && ReflectionBase::AreEqual(obj1.dmag_x(), obj2.dmag_x())
               && ReflectionBase::AreEqual(obj1.dmag_y(), obj2.dmag_y())
               && ReflectionBase::AreEqual(obj1.dmag_z(), obj2.dmag_z())
               && ReflectionBase::AreEqual(obj1.dacc_x(), obj2.dacc_x())
               && ReflectionBase::AreEqual(obj1.dacc_y(), obj2.dacc_y())
               && ReflectionBase::AreEqual(obj1.dacc_z(), obj2.dacc_z())
               && ReflectionBase::AreEqual(obj1.drot_x(), obj2.drot_x())
               && ReflectionBase::AreEqual(obj1.drot_y(), obj2.drot_y())
               && ReflectionBase::AreEqual(obj1.drot_z(), obj2.drot_z())
            ;
    }

    using Handler = ::ReflectionServices::ReflectedStructHandler<Avid::Motion::Data,
                                                                 Traits_mag_x
,                                                                 Traits_mag_y
,                                                                 Traits_mag_z
,                                                                 Traits_acc_x
,                                                                 Traits_acc_y
,                                                                 Traits_acc_z
,                                                                 Traits_rot_x
,                                                                 Traits_rot_y
,                                                                 Traits_rot_z
,                                                                 Traits_dmag_x
,                                                                 Traits_dmag_y
,                                                                 Traits_dmag_z
,                                                                 Traits_dacc_x
,                                                                 Traits_dacc_y
,                                                                 Traits_dacc_z
,                                                                 Traits_drot_x
,                                                                 Traits_drot_y
,                                                                 Traits_drot_z
                                                                 >;
};

template <>
struct Stencil::Transaction<Avid::Motion::Data> : Stencil::TransactionT<Avid::Motion::Data>
{
    using TData = Avid::Motion::Data;

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
    DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<Avid::Motion::Data>(ptr)
        ,
        _subtracker_mag_x(Obj().mag_x())
        ,
        _subtracker_mag_y(Obj().mag_y())
        ,
        _subtracker_mag_z(Obj().mag_z())
        ,
        _subtracker_acc_x(Obj().acc_x())
        ,
        _subtracker_acc_y(Obj().acc_y())
        ,
        _subtracker_acc_z(Obj().acc_z())
        ,
        _subtracker_rot_x(Obj().rot_x())
        ,
        _subtracker_rot_y(Obj().rot_y())
        ,
        _subtracker_rot_z(Obj().rot_z())
        ,
        _subtracker_dmag_x(Obj().dmag_x())
        ,
        _subtracker_dmag_y(Obj().dmag_y())
        ,
        _subtracker_dmag_z(Obj().dmag_z())
        ,
        _subtracker_dacc_x(Obj().dacc_x())
        ,
        _subtracker_dacc_y(Obj().dacc_y())
        ,
        _subtracker_dacc_z(Obj().dacc_z())
        ,
        _subtracker_drot_x(Obj().drot_x())
        ,
        _subtracker_drot_y(Obj().drot_y())
        ,
        _subtracker_drot_z(Obj().drot_z())
    {}

    auto& mag_x()
    {
        MarkFieldEdited_(TData::FieldIndex::mag_x);
        return _subtracker_mag_x;
    }
    auto& mag_y()
    {
        MarkFieldEdited_(TData::FieldIndex::mag_y);
        return _subtracker_mag_y;
    }
    auto& mag_z()
    {
        MarkFieldEdited_(TData::FieldIndex::mag_z);
        return _subtracker_mag_z;
    }
    auto& acc_x()
    {
        MarkFieldEdited_(TData::FieldIndex::acc_x);
        return _subtracker_acc_x;
    }
    auto& acc_y()
    {
        MarkFieldEdited_(TData::FieldIndex::acc_y);
        return _subtracker_acc_y;
    }
    auto& acc_z()
    {
        MarkFieldEdited_(TData::FieldIndex::acc_z);
        return _subtracker_acc_z;
    }
    auto& rot_x()
    {
        MarkFieldEdited_(TData::FieldIndex::rot_x);
        return _subtracker_rot_x;
    }
    auto& rot_y()
    {
        MarkFieldEdited_(TData::FieldIndex::rot_y);
        return _subtracker_rot_y;
    }
    auto& rot_z()
    {
        MarkFieldEdited_(TData::FieldIndex::rot_z);
        return _subtracker_rot_z;
    }
    auto& dmag_x()
    {
        MarkFieldEdited_(TData::FieldIndex::dmag_x);
        return _subtracker_dmag_x;
    }
    auto& dmag_y()
    {
        MarkFieldEdited_(TData::FieldIndex::dmag_y);
        return _subtracker_dmag_y;
    }
    auto& dmag_z()
    {
        MarkFieldEdited_(TData::FieldIndex::dmag_z);
        return _subtracker_dmag_z;
    }
    auto& dacc_x()
    {
        MarkFieldEdited_(TData::FieldIndex::dacc_x);
        return _subtracker_dacc_x;
    }
    auto& dacc_y()
    {
        MarkFieldEdited_(TData::FieldIndex::dacc_y);
        return _subtracker_dacc_y;
    }
    auto& dacc_z()
    {
        MarkFieldEdited_(TData::FieldIndex::dacc_z);
        return _subtracker_dacc_z;
    }
    auto& drot_x()
    {
        MarkFieldEdited_(TData::FieldIndex::drot_x);
        return _subtracker_drot_x;
    }
    auto& drot_y()
    {
        MarkFieldEdited_(TData::FieldIndex::drot_y);
        return _subtracker_drot_y;
    }
    auto& drot_z()
    {
        MarkFieldEdited_(TData::FieldIndex::drot_z);
        return _subtracker_drot_z;
    }
    void set_mag_x(int64_t&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::mag_x, Obj().mag_x(), val);
        Obj().set_mag_x(std::move(val));
    }

    void set_mag_y(int64_t&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::mag_y, Obj().mag_y(), val);
        Obj().set_mag_y(std::move(val));
    }

    void set_mag_z(int64_t&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::mag_z, Obj().mag_z(), val);
        Obj().set_mag_z(std::move(val));
    }

    void set_acc_x(int64_t&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::acc_x, Obj().acc_x(), val);
        Obj().set_acc_x(std::move(val));
    }

    void set_acc_y(int64_t&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::acc_y, Obj().acc_y(), val);
        Obj().set_acc_y(std::move(val));
    }

    void set_acc_z(int64_t&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::acc_z, Obj().acc_z(), val);
        Obj().set_acc_z(std::move(val));
    }

    void set_rot_x(int64_t&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::rot_x, Obj().rot_x(), val);
        Obj().set_rot_x(std::move(val));
    }

    void set_rot_y(int64_t&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::rot_y, Obj().rot_y(), val);
        Obj().set_rot_y(std::move(val));
    }

    void set_rot_z(int64_t&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::rot_z, Obj().rot_z(), val);
        Obj().set_rot_z(std::move(val));
    }

    void set_dmag_x(double&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::dmag_x, Obj().dmag_x(), val);
        Obj().set_dmag_x(std::move(val));
    }

    void set_dmag_y(double&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::dmag_y, Obj().dmag_y(), val);
        Obj().set_dmag_y(std::move(val));
    }

    void set_dmag_z(double&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::dmag_z, Obj().dmag_z(), val);
        Obj().set_dmag_z(std::move(val));
    }

    void set_dacc_x(double&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::dacc_x, Obj().dacc_x(), val);
        Obj().set_dacc_x(std::move(val));
    }

    void set_dacc_y(double&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::dacc_y, Obj().dacc_y(), val);
        Obj().set_dacc_y(std::move(val));
    }

    void set_dacc_z(double&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::dacc_z, Obj().dacc_z(), val);
        Obj().set_dacc_z(std::move(val));
    }

    void set_drot_x(double&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::drot_x, Obj().drot_x(), val);
        Obj().set_drot_x(std::move(val));
    }

    void set_drot_y(double&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::drot_y, Obj().drot_y(), val);
        Obj().set_drot_y(std::move(val));
    }

    void set_drot_z(double&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::drot_z, Obj().drot_z(), val);
        Obj().set_drot_z(std::move(val));
    }

    template <typename TLambda> auto Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::mag_x: return lambda("mag_x", mag_x()); return;
        case TData::FieldIndex::mag_y: return lambda("mag_y", mag_y()); return;
        case TData::FieldIndex::mag_z: return lambda("mag_z", mag_z()); return;
        case TData::FieldIndex::acc_x: return lambda("acc_x", acc_x()); return;
        case TData::FieldIndex::acc_y: return lambda("acc_y", acc_y()); return;
        case TData::FieldIndex::acc_z: return lambda("acc_z", acc_z()); return;
        case TData::FieldIndex::rot_x: return lambda("rot_x", rot_x()); return;
        case TData::FieldIndex::rot_y: return lambda("rot_y", rot_y()); return;
        case TData::FieldIndex::rot_z: return lambda("rot_z", rot_z()); return;
        case TData::FieldIndex::dmag_x: return lambda("dmag_x", dmag_x()); return;
        case TData::FieldIndex::dmag_y: return lambda("dmag_y", dmag_y()); return;
        case TData::FieldIndex::dmag_z: return lambda("dmag_z", dmag_z()); return;
        case TData::FieldIndex::dacc_x: return lambda("dacc_x", dacc_x()); return;
        case TData::FieldIndex::dacc_y: return lambda("dacc_y", dacc_y()); return;
        case TData::FieldIndex::dacc_z: return lambda("dacc_z", dacc_z()); return;
        case TData::FieldIndex::drot_x: return lambda("drot_x", drot_x()); return;
        case TData::FieldIndex::drot_y: return lambda("drot_y", drot_y()); return;
        case TData::FieldIndex::drot_z: return lambda("drot_z", drot_z()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "mag_x") { return lambda(TData::FieldIndex::mag_x, mag_x()); }
        if (fieldName == "mag_y") { return lambda(TData::FieldIndex::mag_y, mag_y()); }
        if (fieldName == "mag_z") { return lambda(TData::FieldIndex::mag_z, mag_z()); }
        if (fieldName == "acc_x") { return lambda(TData::FieldIndex::acc_x, acc_x()); }
        if (fieldName == "acc_y") { return lambda(TData::FieldIndex::acc_y, acc_y()); }
        if (fieldName == "acc_z") { return lambda(TData::FieldIndex::acc_z, acc_z()); }
        if (fieldName == "rot_x") { return lambda(TData::FieldIndex::rot_x, rot_x()); }
        if (fieldName == "rot_y") { return lambda(TData::FieldIndex::rot_y, rot_y()); }
        if (fieldName == "rot_z") { return lambda(TData::FieldIndex::rot_z, rot_z()); }
        if (fieldName == "dmag_x") { return lambda(TData::FieldIndex::dmag_x, dmag_x()); }
        if (fieldName == "dmag_y") { return lambda(TData::FieldIndex::dmag_y, dmag_y()); }
        if (fieldName == "dmag_z") { return lambda(TData::FieldIndex::dmag_z, dmag_z()); }
        if (fieldName == "dacc_x") { return lambda(TData::FieldIndex::dacc_x, dacc_x()); }
        if (fieldName == "dacc_y") { return lambda(TData::FieldIndex::dacc_y, dacc_y()); }
        if (fieldName == "dacc_z") { return lambda(TData::FieldIndex::dacc_z, dacc_z()); }
        if (fieldName == "drot_x") { return lambda(TData::FieldIndex::drot_x, drot_x()); }
        if (fieldName == "drot_y") { return lambda(TData::FieldIndex::drot_y, drot_y()); }
        if (fieldName == "drot_z") { return lambda(TData::FieldIndex::drot_z, drot_z()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("mag_x", TData::FieldIndex::mag_x, mag_x(), Obj().mag_x());
        lambda("mag_y", TData::FieldIndex::mag_y, mag_y(), Obj().mag_y());
        lambda("mag_z", TData::FieldIndex::mag_z, mag_z(), Obj().mag_z());
        lambda("acc_x", TData::FieldIndex::acc_x, acc_x(), Obj().acc_x());
        lambda("acc_y", TData::FieldIndex::acc_y, acc_y(), Obj().acc_y());
        lambda("acc_z", TData::FieldIndex::acc_z, acc_z(), Obj().acc_z());
        lambda("rot_x", TData::FieldIndex::rot_x, rot_x(), Obj().rot_x());
        lambda("rot_y", TData::FieldIndex::rot_y, rot_y(), Obj().rot_y());
        lambda("rot_z", TData::FieldIndex::rot_z, rot_z(), Obj().rot_z());
        lambda("dmag_x", TData::FieldIndex::dmag_x, dmag_x(), Obj().dmag_x());
        lambda("dmag_y", TData::FieldIndex::dmag_y, dmag_y(), Obj().dmag_y());
        lambda("dmag_z", TData::FieldIndex::dmag_z, dmag_z(), Obj().dmag_z());
        lambda("dacc_x", TData::FieldIndex::dacc_x, dacc_x(), Obj().dacc_x());
        lambda("dacc_y", TData::FieldIndex::dacc_y, dacc_y(), Obj().dacc_y());
        lambda("dacc_z", TData::FieldIndex::dacc_z, dacc_z(), Obj().dacc_z());
        lambda("drot_x", TData::FieldIndex::drot_x, drot_x(), Obj().drot_x());
        lambda("drot_y", TData::FieldIndex::drot_y, drot_y(), Obj().drot_y());
        lambda("drot_z", TData::FieldIndex::drot_z, drot_z(), Obj().drot_z());
    }
};

template <>
struct Stencil::Visitor<Avid::Motion::Data, void> : Stencil::VisitorT<Avid::Motion::Data>
{
    using TData = Avid::Motion::Data;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::mag_x: lambda("mag_x", _ref.get().mag_x()); return;
        case TData::FieldIndex::mag_y: lambda("mag_y", _ref.get().mag_y()); return;
        case TData::FieldIndex::mag_z: lambda("mag_z", _ref.get().mag_z()); return;
        case TData::FieldIndex::acc_x: lambda("acc_x", _ref.get().acc_x()); return;
        case TData::FieldIndex::acc_y: lambda("acc_y", _ref.get().acc_y()); return;
        case TData::FieldIndex::acc_z: lambda("acc_z", _ref.get().acc_z()); return;
        case TData::FieldIndex::rot_x: lambda("rot_x", _ref.get().rot_x()); return;
        case TData::FieldIndex::rot_y: lambda("rot_y", _ref.get().rot_y()); return;
        case TData::FieldIndex::rot_z: lambda("rot_z", _ref.get().rot_z()); return;
        case TData::FieldIndex::dmag_x: lambda("dmag_x", _ref.get().dmag_x()); return;
        case TData::FieldIndex::dmag_y: lambda("dmag_y", _ref.get().dmag_y()); return;
        case TData::FieldIndex::dmag_z: lambda("dmag_z", _ref.get().dmag_z()); return;
        case TData::FieldIndex::dacc_x: lambda("dacc_x", _ref.get().dacc_x()); return;
        case TData::FieldIndex::dacc_y: lambda("dacc_y", _ref.get().dacc_y()); return;
        case TData::FieldIndex::dacc_z: lambda("dacc_z", _ref.get().dacc_z()); return;
        case TData::FieldIndex::drot_x: lambda("drot_x", _ref.get().drot_x()); return;
        case TData::FieldIndex::drot_y: lambda("drot_y", _ref.get().drot_y()); return;
        case TData::FieldIndex::drot_z: lambda("drot_z", _ref.get().drot_z()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::mag_x: lambda("mag_x", _ref.get().mag_x()); return;
        case TData::FieldIndex::mag_y: lambda("mag_y", _ref.get().mag_y()); return;
        case TData::FieldIndex::mag_z: lambda("mag_z", _ref.get().mag_z()); return;
        case TData::FieldIndex::acc_x: lambda("acc_x", _ref.get().acc_x()); return;
        case TData::FieldIndex::acc_y: lambda("acc_y", _ref.get().acc_y()); return;
        case TData::FieldIndex::acc_z: lambda("acc_z", _ref.get().acc_z()); return;
        case TData::FieldIndex::rot_x: lambda("rot_x", _ref.get().rot_x()); return;
        case TData::FieldIndex::rot_y: lambda("rot_y", _ref.get().rot_y()); return;
        case TData::FieldIndex::rot_z: lambda("rot_z", _ref.get().rot_z()); return;
        case TData::FieldIndex::dmag_x: lambda("dmag_x", _ref.get().dmag_x()); return;
        case TData::FieldIndex::dmag_y: lambda("dmag_y", _ref.get().dmag_y()); return;
        case TData::FieldIndex::dmag_z: lambda("dmag_z", _ref.get().dmag_z()); return;
        case TData::FieldIndex::dacc_x: lambda("dacc_x", _ref.get().dacc_x()); return;
        case TData::FieldIndex::dacc_y: lambda("dacc_y", _ref.get().dacc_y()); return;
        case TData::FieldIndex::dacc_z: lambda("dacc_z", _ref.get().dacc_z()); return;
        case TData::FieldIndex::drot_x: lambda("drot_x", _ref.get().drot_x()); return;
        case TData::FieldIndex::drot_y: lambda("drot_y", _ref.get().drot_y()); return;
        case TData::FieldIndex::drot_z: lambda("drot_z", _ref.get().drot_z()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        lambda("mag_x", _ref.get().mag_x());
        lambda("mag_y", _ref.get().mag_y());
        lambda("mag_z", _ref.get().mag_z());
        lambda("acc_x", _ref.get().acc_x());
        lambda("acc_y", _ref.get().acc_y());
        lambda("acc_z", _ref.get().acc_z());
        lambda("rot_x", _ref.get().rot_x());
        lambda("rot_y", _ref.get().rot_y());
        lambda("rot_z", _ref.get().rot_z());
        lambda("dmag_x", _ref.get().dmag_x());
        lambda("dmag_y", _ref.get().dmag_y());
        lambda("dmag_z", _ref.get().dmag_z());
        lambda("dacc_x", _ref.get().dacc_x());
        lambda("dacc_y", _ref.get().dacc_y());
        lambda("dacc_z", _ref.get().dacc_z());
        lambda("drot_x", _ref.get().drot_x());
        lambda("drot_y", _ref.get().drot_y());
        lambda("drot_z", _ref.get().drot_z());
    }

    std::reference_wrapper<TData> _ref;
};

template <>
struct Stencil::Visitor<const Avid::Motion::Data, void>
    : Stencil::VisitorT<const Avid::Motion::Data>
{
    using TData = Avid::Motion::Data const;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::mag_x: lambda("mag_x", _ref.get().mag_x()); return;
        case TData::FieldIndex::mag_y: lambda("mag_y", _ref.get().mag_y()); return;
        case TData::FieldIndex::mag_z: lambda("mag_z", _ref.get().mag_z()); return;
        case TData::FieldIndex::acc_x: lambda("acc_x", _ref.get().acc_x()); return;
        case TData::FieldIndex::acc_y: lambda("acc_y", _ref.get().acc_y()); return;
        case TData::FieldIndex::acc_z: lambda("acc_z", _ref.get().acc_z()); return;
        case TData::FieldIndex::rot_x: lambda("rot_x", _ref.get().rot_x()); return;
        case TData::FieldIndex::rot_y: lambda("rot_y", _ref.get().rot_y()); return;
        case TData::FieldIndex::rot_z: lambda("rot_z", _ref.get().rot_z()); return;
        case TData::FieldIndex::dmag_x: lambda("dmag_x", _ref.get().dmag_x()); return;
        case TData::FieldIndex::dmag_y: lambda("dmag_y", _ref.get().dmag_y()); return;
        case TData::FieldIndex::dmag_z: lambda("dmag_z", _ref.get().dmag_z()); return;
        case TData::FieldIndex::dacc_x: lambda("dacc_x", _ref.get().dacc_x()); return;
        case TData::FieldIndex::dacc_y: lambda("dacc_y", _ref.get().dacc_y()); return;
        case TData::FieldIndex::dacc_z: lambda("dacc_z", _ref.get().dacc_z()); return;
        case TData::FieldIndex::drot_x: lambda("drot_x", _ref.get().drot_x()); return;
        case TData::FieldIndex::drot_y: lambda("drot_y", _ref.get().drot_y()); return;
        case TData::FieldIndex::drot_z: lambda("drot_z", _ref.get().drot_z()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        lambda("mag_x", _ref.get().mag_x());
        lambda("mag_y", _ref.get().mag_y());
        lambda("mag_z", _ref.get().mag_z());
        lambda("acc_x", _ref.get().acc_x());
        lambda("acc_y", _ref.get().acc_y());
        lambda("acc_z", _ref.get().acc_z());
        lambda("rot_x", _ref.get().rot_x());
        lambda("rot_y", _ref.get().rot_y());
        lambda("rot_z", _ref.get().rot_z());
        lambda("dmag_x", _ref.get().dmag_x());
        lambda("dmag_y", _ref.get().dmag_y());
        lambda("dmag_z", _ref.get().dmag_z());
        lambda("dacc_x", _ref.get().dacc_x());
        lambda("dacc_y", _ref.get().dacc_y());
        lambda("dacc_z", _ref.get().dacc_z());
        lambda("drot_x", _ref.get().drot_x());
        lambda("drot_y", _ref.get().drot_y());
        lambda("drot_z", _ref.get().drot_z());
    }

    std::reference_wrapper<TData> _ref;
};

template <> struct ReflectionBase::TypeTraits<Avid::Pressure::Data&>
{
    struct Traits_pressure
    {
        using TOwner     = Avid::Pressure::Data;
        using TFieldType = double;

        static constexpr std::string_view Name() { return "pressure"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_pressure; }
        static constexpr auto TPropertySetter() { return &TOwner::set_pressure; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::pressure, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    struct Traits_temperature
    {
        using TOwner     = Avid::Pressure::Data;
        using TFieldType = double;

        static constexpr std::string_view Name() { return "temperature"; }

        static constexpr auto TPropertyGetter() { return &TOwner::get_temperature; }
        static constexpr auto TPropertySetter() { return &TOwner::set_temperature; }
        static constexpr auto TAttributeValue(const std::string_view& key)
        {
            return TOwner::FieldAttributeValue(TOwner::FieldIndex::temperature, key);
        }

        static const ::ReflectionBase::Flags Flags()
        {
            return ::ReflectionBase::Flags{                                           ::ReflectionBase::Flag::Max};
        }
    };
    static constexpr ::ReflectionBase::DataType Type() { return ::ReflectionBase::DataType::Object; }
    static constexpr std::string_view           Name() { return "Pressure"; }
    static constexpr std::string_view           AttributeValue(const std::string_view& key)
    {
        return ::ReflectionServices::EmptyAttributeValue(key);
    }

    using ThisType = Avid::Pressure::Data;
    static bool AreEqual([[maybe_unused]] ThisType const& obj1, [[maybe_unused]] ThisType const& obj2)
    {
        return true
               && ReflectionBase::AreEqual(obj1.pressure(), obj2.pressure())
               && ReflectionBase::AreEqual(obj1.temperature(), obj2.temperature())
            ;
    }

    using Handler = ::ReflectionServices::ReflectedStructHandler<Avid::Pressure::Data,
                                                                 Traits_pressure
,                                                                 Traits_temperature
                                                                 >;
};

template <>
struct Stencil::Transaction<Avid::Pressure::Data> : Stencil::TransactionT<Avid::Pressure::Data>
{
    using TData = Avid::Pressure::Data;

    Transaction<double> _subtracker_pressure;
    Transaction<double> _subtracker_temperature;
    DELETE_COPY_AND_MOVE(Transaction);

    Transaction(TData& ptr) :
        Stencil::TransactionT<Avid::Pressure::Data>(ptr)
        ,
        _subtracker_pressure(Obj().pressure())
        ,
        _subtracker_temperature(Obj().temperature())
    {}

    auto& pressure()
    {
        MarkFieldEdited_(TData::FieldIndex::pressure);
        return _subtracker_pressure;
    }
    auto& temperature()
    {
        MarkFieldEdited_(TData::FieldIndex::temperature);
        return _subtracker_temperature;
    }
    void set_pressure(double&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::pressure, Obj().pressure(), val);
        Obj().set_pressure(std::move(val));
    }

    void set_temperature(double&& val)
    {
        MarkFieldAssigned_(TData::FieldIndex::temperature, Obj().temperature(), val);
        Obj().set_temperature(std::move(val));
    }

    template <typename TLambda> auto Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::pressure: return lambda("pressure", pressure()); return;
        case TData::FieldIndex::temperature: return lambda("temperature", temperature()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        if (fieldName == "pressure") { return lambda(TData::FieldIndex::pressure, pressure()); }
        if (fieldName == "temperature") { return lambda(TData::FieldIndex::temperature, temperature()); }
        throw std::invalid_argument("Asked to visit invalid field");
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda)
    {
        lambda("pressure", TData::FieldIndex::pressure, pressure(), Obj().pressure());
        lambda("temperature", TData::FieldIndex::temperature, temperature(), Obj().temperature());
    }
};

template <>
struct Stencil::Visitor<Avid::Pressure::Data, void> : Stencil::VisitorT<Avid::Pressure::Data>
{
    using TData = Avid::Pressure::Data;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::pressure: lambda("pressure", _ref.get().pressure()); return;
        case TData::FieldIndex::temperature: lambda("temperature", _ref.get().temperature()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::pressure: lambda("pressure", _ref.get().pressure()); return;
        case TData::FieldIndex::temperature: lambda("temperature", _ref.get().temperature()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        lambda("pressure", _ref.get().pressure());
        lambda("temperature", _ref.get().temperature());
    }

    std::reference_wrapper<TData> _ref;
};

template <>
struct Stencil::Visitor<const Avid::Pressure::Data, void>
    : Stencil::VisitorT<const Avid::Pressure::Data>
{
    using TData = Avid::Pressure::Data const;

    Visitor(TData& obj) : VisitorT<TData>(obj), _ref(obj) {}

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::pressure: lambda("pressure", _ref.get().pressure()); return;
        case TData::FieldIndex::temperature: lambda("temperature", _ref.get().temperature()); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void VisitAll(TLambda&& lambda) const
    {
        lambda("pressure", _ref.get().pressure());
        lambda("temperature", _ref.get().temperature());
    }

    std::reference_wrapper<TData> _ref;
};

#endif
// SECTION END: Template specializations

// SECTION START: Inline Function Definitions
#if true

#endif
// SECTION END: Inline Function Definitions
