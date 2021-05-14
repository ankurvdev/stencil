#pragma once
#include <stencil/stencil.h>
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
    public ::Stencil::ObservablePropsT<::Avid::GPS::Data>,
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

};

}    // namespace GPS
namespace Aircraft
{

struct Data :
    public ::Stencil::ObservablePropsT<::Avid::Aircraft::Data>,
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

    private:
    std::array<char, 7> _hexaddr = {};

    public:
    std::array<char, 7>&       hexaddr() { return _hexaddr; }
    const std::array<char, 7>& hexaddr() const { return _hexaddr; }
    void                            hexaddr(std::array<char, 7>&& val) { _hexaddr = std::move(val); }
    std::array<char, 7>&       get_hexaddr() { return _hexaddr; }

    bool isset_hexaddr() const { return Stencil::OptionalPropsT<Data>::IsSet(*this, FieldIndex::hexaddr); }

    void set_hexaddr(std::array<char, 7>&& val)
    {
        Stencil::OptionalPropsT<Data>::OnChangeRequested(*this, FieldIndex::hexaddr, _hexaddr, val);
        _hexaddr = std::move(val);
    }

    char at_hexaddr(size_t&& args)
    {
        return Stencil::Accessors<std::array<char, 7>>::at(_hexaddr, args);
    }
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

    char at_flight(size_t&& args)
    {
        return Stencil::Accessors<std::array<char, 9>>::at(_flight, args);
    }
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

};

}    // namespace Aircraft
namespace Traffic
{

struct Data :
    public ::Stencil::ObservablePropsT<::Avid::Traffic::Data>,
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

    void add_aircrafts(::Avid::Aircraft::Data&& args)
    {
        return Stencil::Mutators<std::vector<::Avid::Aircraft::Data>>::add(_aircrafts, std::move(args));
    }
    void remove_aircrafts(size_t&& args)
    {
        return Stencil::Mutators<std::vector<::Avid::Aircraft::Data>>::remove(_aircrafts, std::move(args));
    }
    ::Avid::Aircraft::Data at_aircrafts(size_t&& args)
    {
        return Stencil::Accessors<std::vector<::Avid::Aircraft::Data>>::at(_aircrafts, args);
    }
};

}    // namespace Traffic
namespace Motion
{

struct Data :
    public ::Stencil::ObservablePropsT<::Avid::Motion::Data>,
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

};

}    // namespace Motion
namespace Pressure
{

struct Data :
    public ::Stencil::ObservablePropsT<::Avid::Pressure::Data>,
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

};

}    // namespace Pressure
}    // namespace Avid

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

template <typename T> struct Stencil::DeltaTracker<T, std::enable_if_t<std::is_same_v<T, Avid::GPS::Data>>>
{
    using TData = T;

    // TODO : Tentative: We hate pointers
    TData* const _ptr;
    // TODO : Better way to unify creation interface

    std::bitset<TData::FieldCount() + 1> _fieldtracker;
    DeltaTracker<int32_t> _subtracker_heading;
    DeltaTracker<double> _subtracker_lat;
    DeltaTracker<double> _subtracker_lon;
    DeltaTracker<double> _subtracker_speed;
    DeltaTracker<timestamp> _subtracker_utc;
    DeltaTracker<double> _subtracker_galt;
    DeltaTracker<double> _subtracker_climb;
    DeltaTracker<double> _subtracker_epx;
    DeltaTracker<double> _subtracker_epy;
    DeltaTracker<double> _subtracker_epv;
    DeltaTracker<double> _subtracker_ept;
    DeltaTracker<double> _subtracker_eps;
    DeltaTracker<double> _subtracker_epc;
    DeltaTracker<double> _subtracker_eph;
    DeltaTracker<double> _subtracker_sep;
    DELETE_COPY_AND_MOVE(DeltaTracker);

    DeltaTracker(TData* ptr) :
        _ptr(ptr)
        ,
        _subtracker_heading(&_ptr->heading())
        ,
        _subtracker_lat(&_ptr->lat())
        ,
        _subtracker_lon(&_ptr->lon())
        ,
        _subtracker_speed(&_ptr->speed())
        ,
        _subtracker_utc(&_ptr->utc())
        ,
        _subtracker_galt(&_ptr->galt())
        ,
        _subtracker_climb(&_ptr->climb())
        ,
        _subtracker_epx(&_ptr->epx())
        ,
        _subtracker_epy(&_ptr->epy())
        ,
        _subtracker_epv(&_ptr->epv())
        ,
        _subtracker_ept(&_ptr->ept())
        ,
        _subtracker_eps(&_ptr->eps())
        ,
        _subtracker_epc(&_ptr->epc())
        ,
        _subtracker_eph(&_ptr->eph())
        ,
        _subtracker_sep(&_ptr->sep())
    {
        // TODO: Tentative
        static_assert(std::is_base_of<Stencil::ObservablePropsT<TData>, TData>::value);
    }

    TData& Obj() { return *_ptr; }

    static constexpr auto Type() { return ReflectionBase::TypeTraits<TData&>::Type(); }

    size_t NumFields() const { return TData::FieldCount(); }
    bool   IsChanged() const { return _fieldtracker.any(); }

    uint8_t MutatorIndex() const;
    bool    OnlyHasDefaultMutator() const;

    void MarkFieldChanged(typename TData::FieldIndex index) { _fieldtracker.set(static_cast<size_t>(index)); }
    bool IsFieldChanged(typename TData::FieldIndex index) const { return _fieldtracker.test(static_cast<size_t>(index)); }

    size_t CountFieldsChanged() const { return _fieldtracker.count(); }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::heading: lambda(_subtracker_heading); return;
        case TData::FieldIndex::lat: lambda(_subtracker_lat); return;
        case TData::FieldIndex::lon: lambda(_subtracker_lon); return;
        case TData::FieldIndex::speed: lambda(_subtracker_speed); return;
        case TData::FieldIndex::utc: lambda(_subtracker_utc); return;
        case TData::FieldIndex::galt: lambda(_subtracker_galt); return;
        case TData::FieldIndex::climb: lambda(_subtracker_climb); return;
        case TData::FieldIndex::epx: lambda(_subtracker_epx); return;
        case TData::FieldIndex::epy: lambda(_subtracker_epy); return;
        case TData::FieldIndex::epv: lambda(_subtracker_epv); return;
        case TData::FieldIndex::ept: lambda(_subtracker_ept); return;
        case TData::FieldIndex::eps: lambda(_subtracker_eps); return;
        case TData::FieldIndex::epc: lambda(_subtracker_epc); return;
        case TData::FieldIndex::eph: lambda(_subtracker_eph); return;
        case TData::FieldIndex::sep: lambda(_subtracker_sep); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::heading: lambda(_subtracker_heading); return;
        case TData::FieldIndex::lat: lambda(_subtracker_lat); return;
        case TData::FieldIndex::lon: lambda(_subtracker_lon); return;
        case TData::FieldIndex::speed: lambda(_subtracker_speed); return;
        case TData::FieldIndex::utc: lambda(_subtracker_utc); return;
        case TData::FieldIndex::galt: lambda(_subtracker_galt); return;
        case TData::FieldIndex::climb: lambda(_subtracker_climb); return;
        case TData::FieldIndex::epx: lambda(_subtracker_epx); return;
        case TData::FieldIndex::epy: lambda(_subtracker_epy); return;
        case TData::FieldIndex::epv: lambda(_subtracker_epv); return;
        case TData::FieldIndex::ept: lambda(_subtracker_ept); return;
        case TData::FieldIndex::eps: lambda(_subtracker_eps); return;
        case TData::FieldIndex::epc: lambda(_subtracker_epc); return;
        case TData::FieldIndex::eph: lambda(_subtracker_eph); return;
        case TData::FieldIndex::sep: lambda(_subtracker_sep); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    void set_heading(int32_t&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::heading, _ptr->heading(), val);
        _ptr->set_heading(std::move(val));
    }

    void set_lat(double&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::lat, _ptr->lat(), val);
        _ptr->set_lat(std::move(val));
    }

    void set_lon(double&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::lon, _ptr->lon(), val);
        _ptr->set_lon(std::move(val));
    }

    void set_speed(double&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::speed, _ptr->speed(), val);
        _ptr->set_speed(std::move(val));
    }

    void set_utc(timestamp&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::utc, _ptr->utc(), val);
        _ptr->set_utc(std::move(val));
    }

    void set_galt(double&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::galt, _ptr->galt(), val);
        _ptr->set_galt(std::move(val));
    }

    void set_climb(double&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::climb, _ptr->climb(), val);
        _ptr->set_climb(std::move(val));
    }

    void set_epx(double&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::epx, _ptr->epx(), val);
        _ptr->set_epx(std::move(val));
    }

    void set_epy(double&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::epy, _ptr->epy(), val);
        _ptr->set_epy(std::move(val));
    }

    void set_epv(double&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::epv, _ptr->epv(), val);
        _ptr->set_epv(std::move(val));
    }

    void set_ept(double&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::ept, _ptr->ept(), val);
        _ptr->set_ept(std::move(val));
    }

    void set_eps(double&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::eps, _ptr->eps(), val);
        _ptr->set_eps(std::move(val));
    }

    void set_epc(double&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::epc, _ptr->epc(), val);
        _ptr->set_epc(std::move(val));
    }

    void set_eph(double&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::eph, _ptr->eph(), val);
        _ptr->set_eph(std::move(val));
    }

    void set_sep(double&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::sep, _ptr->sep(), val);
        _ptr->set_sep(std::move(val));
    }

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
        using TFieldType = std::array<char, 7>;

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

template <typename T> struct Stencil::DeltaTracker<T, std::enable_if_t<std::is_same_v<T, Avid::Aircraft::Data>>>
{
    using TData = T;

    // TODO : Tentative: We hate pointers
    TData* const _ptr;
    // TODO : Better way to unify creation interface

    std::bitset<TData::FieldCount() + 1> _fieldtracker;
    DeltaTracker<timestamp> _subtracker_seen;
    DeltaTracker<uint32_t> _subtracker_addr;
    DeltaTracker<std::array<char, 7>> _subtracker_hexaddr;
    DeltaTracker<std::array<char, 9>> _subtracker_flight;
    DeltaTracker<int32_t> _subtracker_altitude;
    DeltaTracker<int32_t> _subtracker_groundSpeed;
    DeltaTracker<int32_t> _subtracker_track;
    DeltaTracker<double> _subtracker_lat;
    DeltaTracker<double> _subtracker_lon;
    DeltaTracker<int32_t> _subtracker_verticalRate;
    DeltaTracker<int32_t> _subtracker_messageCount;
    DeltaTracker<int32_t> _subtracker_odd_cprlat;
    DeltaTracker<int32_t> _subtracker_odd_cprlon;
    DeltaTracker<timestamp> _subtracker_odd_cprtime;
    DeltaTracker<int32_t> _subtracker_even_cprlat;
    DeltaTracker<int32_t> _subtracker_even_cprlon;
    DeltaTracker<timestamp> _subtracker_even_cprtime;
    DELETE_COPY_AND_MOVE(DeltaTracker);

    DeltaTracker(TData* ptr) :
        _ptr(ptr)
        ,
        _subtracker_seen(&_ptr->seen())
        ,
        _subtracker_addr(&_ptr->addr())
        ,
        _subtracker_hexaddr(&_ptr->hexaddr())
        ,
        _subtracker_flight(&_ptr->flight())
        ,
        _subtracker_altitude(&_ptr->altitude())
        ,
        _subtracker_groundSpeed(&_ptr->groundSpeed())
        ,
        _subtracker_track(&_ptr->track())
        ,
        _subtracker_lat(&_ptr->lat())
        ,
        _subtracker_lon(&_ptr->lon())
        ,
        _subtracker_verticalRate(&_ptr->verticalRate())
        ,
        _subtracker_messageCount(&_ptr->messageCount())
        ,
        _subtracker_odd_cprlat(&_ptr->odd_cprlat())
        ,
        _subtracker_odd_cprlon(&_ptr->odd_cprlon())
        ,
        _subtracker_odd_cprtime(&_ptr->odd_cprtime())
        ,
        _subtracker_even_cprlat(&_ptr->even_cprlat())
        ,
        _subtracker_even_cprlon(&_ptr->even_cprlon())
        ,
        _subtracker_even_cprtime(&_ptr->even_cprtime())
    {
        // TODO: Tentative
        static_assert(std::is_base_of<Stencil::ObservablePropsT<TData>, TData>::value);
    }

    TData& Obj() { return *_ptr; }

    static constexpr auto Type() { return ReflectionBase::TypeTraits<TData&>::Type(); }

    size_t NumFields() const { return TData::FieldCount(); }
    bool   IsChanged() const { return _fieldtracker.any(); }

    uint8_t MutatorIndex() const;
    bool    OnlyHasDefaultMutator() const;

    void MarkFieldChanged(typename TData::FieldIndex index) { _fieldtracker.set(static_cast<size_t>(index)); }
    bool IsFieldChanged(typename TData::FieldIndex index) const { return _fieldtracker.test(static_cast<size_t>(index)); }

    size_t CountFieldsChanged() const { return _fieldtracker.count(); }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::seen: lambda(_subtracker_seen); return;
        case TData::FieldIndex::addr: lambda(_subtracker_addr); return;
        case TData::FieldIndex::hexaddr: lambda(_subtracker_hexaddr); return;
        case TData::FieldIndex::flight: lambda(_subtracker_flight); return;
        case TData::FieldIndex::altitude: lambda(_subtracker_altitude); return;
        case TData::FieldIndex::groundSpeed: lambda(_subtracker_groundSpeed); return;
        case TData::FieldIndex::track: lambda(_subtracker_track); return;
        case TData::FieldIndex::lat: lambda(_subtracker_lat); return;
        case TData::FieldIndex::lon: lambda(_subtracker_lon); return;
        case TData::FieldIndex::verticalRate: lambda(_subtracker_verticalRate); return;
        case TData::FieldIndex::messageCount: lambda(_subtracker_messageCount); return;
        case TData::FieldIndex::odd_cprlat: lambda(_subtracker_odd_cprlat); return;
        case TData::FieldIndex::odd_cprlon: lambda(_subtracker_odd_cprlon); return;
        case TData::FieldIndex::odd_cprtime: lambda(_subtracker_odd_cprtime); return;
        case TData::FieldIndex::even_cprlat: lambda(_subtracker_even_cprlat); return;
        case TData::FieldIndex::even_cprlon: lambda(_subtracker_even_cprlon); return;
        case TData::FieldIndex::even_cprtime: lambda(_subtracker_even_cprtime); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::seen: lambda(_subtracker_seen); return;
        case TData::FieldIndex::addr: lambda(_subtracker_addr); return;
        case TData::FieldIndex::hexaddr: lambda(_subtracker_hexaddr); return;
        case TData::FieldIndex::flight: lambda(_subtracker_flight); return;
        case TData::FieldIndex::altitude: lambda(_subtracker_altitude); return;
        case TData::FieldIndex::groundSpeed: lambda(_subtracker_groundSpeed); return;
        case TData::FieldIndex::track: lambda(_subtracker_track); return;
        case TData::FieldIndex::lat: lambda(_subtracker_lat); return;
        case TData::FieldIndex::lon: lambda(_subtracker_lon); return;
        case TData::FieldIndex::verticalRate: lambda(_subtracker_verticalRate); return;
        case TData::FieldIndex::messageCount: lambda(_subtracker_messageCount); return;
        case TData::FieldIndex::odd_cprlat: lambda(_subtracker_odd_cprlat); return;
        case TData::FieldIndex::odd_cprlon: lambda(_subtracker_odd_cprlon); return;
        case TData::FieldIndex::odd_cprtime: lambda(_subtracker_odd_cprtime); return;
        case TData::FieldIndex::even_cprlat: lambda(_subtracker_even_cprlat); return;
        case TData::FieldIndex::even_cprlon: lambda(_subtracker_even_cprlon); return;
        case TData::FieldIndex::even_cprtime: lambda(_subtracker_even_cprtime); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    void set_seen(timestamp&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::seen, _ptr->seen(), val);
        _ptr->set_seen(std::move(val));
    }

    void set_addr(uint32_t&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::addr, _ptr->addr(), val);
        _ptr->set_addr(std::move(val));
    }

    void set_hexaddr(std::array<char, 7>&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::hexaddr, _ptr->hexaddr(), val);
        _ptr->set_hexaddr(std::move(val));
    }

    char at_hexaddr(size_t&& args) const { return _ptr->at_hexaddr(args); }
    void set_flight(std::array<char, 9>&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::flight, _ptr->flight(), val);
        _ptr->set_flight(std::move(val));
    }

    char at_flight(size_t&& args) const { return _ptr->at_flight(args); }
    void set_altitude(int32_t&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::altitude, _ptr->altitude(), val);
        _ptr->set_altitude(std::move(val));
    }

    void set_groundSpeed(int32_t&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::groundSpeed, _ptr->groundSpeed(), val);
        _ptr->set_groundSpeed(std::move(val));
    }

    void set_track(int32_t&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::track, _ptr->track(), val);
        _ptr->set_track(std::move(val));
    }

    void set_lat(double&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::lat, _ptr->lat(), val);
        _ptr->set_lat(std::move(val));
    }

    void set_lon(double&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::lon, _ptr->lon(), val);
        _ptr->set_lon(std::move(val));
    }

    void set_verticalRate(int32_t&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::verticalRate, _ptr->verticalRate(), val);
        _ptr->set_verticalRate(std::move(val));
    }

    void set_messageCount(int32_t&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::messageCount, _ptr->messageCount(), val);
        _ptr->set_messageCount(std::move(val));
    }

    void set_odd_cprlat(int32_t&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::odd_cprlat, _ptr->odd_cprlat(), val);
        _ptr->set_odd_cprlat(std::move(val));
    }

    void set_odd_cprlon(int32_t&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::odd_cprlon, _ptr->odd_cprlon(), val);
        _ptr->set_odd_cprlon(std::move(val));
    }

    void set_odd_cprtime(timestamp&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::odd_cprtime, _ptr->odd_cprtime(), val);
        _ptr->set_odd_cprtime(std::move(val));
    }

    void set_even_cprlat(int32_t&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::even_cprlat, _ptr->even_cprlat(), val);
        _ptr->set_even_cprlat(std::move(val));
    }

    void set_even_cprlon(int32_t&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::even_cprlon, _ptr->even_cprlon(), val);
        _ptr->set_even_cprlon(std::move(val));
    }

    void set_even_cprtime(timestamp&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::even_cprtime, _ptr->even_cprtime(), val);
        _ptr->set_even_cprtime(std::move(val));
    }

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

template <typename T> struct Stencil::DeltaTracker<T, std::enable_if_t<std::is_same_v<T, Avid::Traffic::Data>>>
{
    using TData = T;

    // TODO : Tentative: We hate pointers
    TData* const _ptr;
    // TODO : Better way to unify creation interface

    std::bitset<TData::FieldCount() + 1> _fieldtracker;
    DeltaTracker<std::vector<::Avid::Aircraft::Data>> _subtracker_aircrafts;
    DELETE_COPY_AND_MOVE(DeltaTracker);

    DeltaTracker(TData* ptr) :
        _ptr(ptr)
        ,
        _subtracker_aircrafts(&_ptr->aircrafts())
    {
        // TODO: Tentative
        static_assert(std::is_base_of<Stencil::ObservablePropsT<TData>, TData>::value);
    }

    TData& Obj() { return *_ptr; }

    static constexpr auto Type() { return ReflectionBase::TypeTraits<TData&>::Type(); }

    size_t NumFields() const { return TData::FieldCount(); }
    bool   IsChanged() const { return _fieldtracker.any(); }

    uint8_t MutatorIndex() const;
    bool    OnlyHasDefaultMutator() const;

    void MarkFieldChanged(typename TData::FieldIndex index) { _fieldtracker.set(static_cast<size_t>(index)); }
    bool IsFieldChanged(typename TData::FieldIndex index) const { return _fieldtracker.test(static_cast<size_t>(index)); }

    size_t CountFieldsChanged() const { return _fieldtracker.count(); }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::aircrafts: lambda(_subtracker_aircrafts); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::aircrafts: lambda(_subtracker_aircrafts); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    void set_aircrafts(std::vector<::Avid::Aircraft::Data>&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::aircrafts, _ptr->aircrafts(), val);
        _ptr->set_aircrafts(std::move(val));
    }

    void add_aircrafts(::Avid::Aircraft::Data&& args)
    {
        Stencil::ObservablePropsT<TData>::OnMutationRequested(
            *this, TData::FieldIndex::aircrafts, uint8_t{1}, _ptr->aircrafts(), args);

        return _ptr->add_aircrafts(std::move(args));
    }
    void remove_aircrafts(size_t&& args)
    {
        Stencil::ObservablePropsT<TData>::OnMutationRequested(
            *this, TData::FieldIndex::aircrafts, uint8_t{2}, _ptr->aircrafts(), args);

        return _ptr->remove_aircrafts(std::move(args));
    }
    ::Avid::Aircraft::Data at_aircrafts(size_t&& args) const { return _ptr->at_aircrafts(args); }
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

template <typename T> struct Stencil::DeltaTracker<T, std::enable_if_t<std::is_same_v<T, Avid::Motion::Data>>>
{
    using TData = T;

    // TODO : Tentative: We hate pointers
    TData* const _ptr;
    // TODO : Better way to unify creation interface

    std::bitset<TData::FieldCount() + 1> _fieldtracker;
    DeltaTracker<int64_t> _subtracker_mag_x;
    DeltaTracker<int64_t> _subtracker_mag_y;
    DeltaTracker<int64_t> _subtracker_mag_z;
    DeltaTracker<int64_t> _subtracker_acc_x;
    DeltaTracker<int64_t> _subtracker_acc_y;
    DeltaTracker<int64_t> _subtracker_acc_z;
    DeltaTracker<int64_t> _subtracker_rot_x;
    DeltaTracker<int64_t> _subtracker_rot_y;
    DeltaTracker<int64_t> _subtracker_rot_z;
    DeltaTracker<double> _subtracker_dmag_x;
    DeltaTracker<double> _subtracker_dmag_y;
    DeltaTracker<double> _subtracker_dmag_z;
    DeltaTracker<double> _subtracker_dacc_x;
    DeltaTracker<double> _subtracker_dacc_y;
    DeltaTracker<double> _subtracker_dacc_z;
    DeltaTracker<double> _subtracker_drot_x;
    DeltaTracker<double> _subtracker_drot_y;
    DeltaTracker<double> _subtracker_drot_z;
    DELETE_COPY_AND_MOVE(DeltaTracker);

    DeltaTracker(TData* ptr) :
        _ptr(ptr)
        ,
        _subtracker_mag_x(&_ptr->mag_x())
        ,
        _subtracker_mag_y(&_ptr->mag_y())
        ,
        _subtracker_mag_z(&_ptr->mag_z())
        ,
        _subtracker_acc_x(&_ptr->acc_x())
        ,
        _subtracker_acc_y(&_ptr->acc_y())
        ,
        _subtracker_acc_z(&_ptr->acc_z())
        ,
        _subtracker_rot_x(&_ptr->rot_x())
        ,
        _subtracker_rot_y(&_ptr->rot_y())
        ,
        _subtracker_rot_z(&_ptr->rot_z())
        ,
        _subtracker_dmag_x(&_ptr->dmag_x())
        ,
        _subtracker_dmag_y(&_ptr->dmag_y())
        ,
        _subtracker_dmag_z(&_ptr->dmag_z())
        ,
        _subtracker_dacc_x(&_ptr->dacc_x())
        ,
        _subtracker_dacc_y(&_ptr->dacc_y())
        ,
        _subtracker_dacc_z(&_ptr->dacc_z())
        ,
        _subtracker_drot_x(&_ptr->drot_x())
        ,
        _subtracker_drot_y(&_ptr->drot_y())
        ,
        _subtracker_drot_z(&_ptr->drot_z())
    {
        // TODO: Tentative
        static_assert(std::is_base_of<Stencil::ObservablePropsT<TData>, TData>::value);
    }

    TData& Obj() { return *_ptr; }

    static constexpr auto Type() { return ReflectionBase::TypeTraits<TData&>::Type(); }

    size_t NumFields() const { return TData::FieldCount(); }
    bool   IsChanged() const { return _fieldtracker.any(); }

    uint8_t MutatorIndex() const;
    bool    OnlyHasDefaultMutator() const;

    void MarkFieldChanged(typename TData::FieldIndex index) { _fieldtracker.set(static_cast<size_t>(index)); }
    bool IsFieldChanged(typename TData::FieldIndex index) const { return _fieldtracker.test(static_cast<size_t>(index)); }

    size_t CountFieldsChanged() const { return _fieldtracker.count(); }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::mag_x: lambda(_subtracker_mag_x); return;
        case TData::FieldIndex::mag_y: lambda(_subtracker_mag_y); return;
        case TData::FieldIndex::mag_z: lambda(_subtracker_mag_z); return;
        case TData::FieldIndex::acc_x: lambda(_subtracker_acc_x); return;
        case TData::FieldIndex::acc_y: lambda(_subtracker_acc_y); return;
        case TData::FieldIndex::acc_z: lambda(_subtracker_acc_z); return;
        case TData::FieldIndex::rot_x: lambda(_subtracker_rot_x); return;
        case TData::FieldIndex::rot_y: lambda(_subtracker_rot_y); return;
        case TData::FieldIndex::rot_z: lambda(_subtracker_rot_z); return;
        case TData::FieldIndex::dmag_x: lambda(_subtracker_dmag_x); return;
        case TData::FieldIndex::dmag_y: lambda(_subtracker_dmag_y); return;
        case TData::FieldIndex::dmag_z: lambda(_subtracker_dmag_z); return;
        case TData::FieldIndex::dacc_x: lambda(_subtracker_dacc_x); return;
        case TData::FieldIndex::dacc_y: lambda(_subtracker_dacc_y); return;
        case TData::FieldIndex::dacc_z: lambda(_subtracker_dacc_z); return;
        case TData::FieldIndex::drot_x: lambda(_subtracker_drot_x); return;
        case TData::FieldIndex::drot_y: lambda(_subtracker_drot_y); return;
        case TData::FieldIndex::drot_z: lambda(_subtracker_drot_z); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::mag_x: lambda(_subtracker_mag_x); return;
        case TData::FieldIndex::mag_y: lambda(_subtracker_mag_y); return;
        case TData::FieldIndex::mag_z: lambda(_subtracker_mag_z); return;
        case TData::FieldIndex::acc_x: lambda(_subtracker_acc_x); return;
        case TData::FieldIndex::acc_y: lambda(_subtracker_acc_y); return;
        case TData::FieldIndex::acc_z: lambda(_subtracker_acc_z); return;
        case TData::FieldIndex::rot_x: lambda(_subtracker_rot_x); return;
        case TData::FieldIndex::rot_y: lambda(_subtracker_rot_y); return;
        case TData::FieldIndex::rot_z: lambda(_subtracker_rot_z); return;
        case TData::FieldIndex::dmag_x: lambda(_subtracker_dmag_x); return;
        case TData::FieldIndex::dmag_y: lambda(_subtracker_dmag_y); return;
        case TData::FieldIndex::dmag_z: lambda(_subtracker_dmag_z); return;
        case TData::FieldIndex::dacc_x: lambda(_subtracker_dacc_x); return;
        case TData::FieldIndex::dacc_y: lambda(_subtracker_dacc_y); return;
        case TData::FieldIndex::dacc_z: lambda(_subtracker_dacc_z); return;
        case TData::FieldIndex::drot_x: lambda(_subtracker_drot_x); return;
        case TData::FieldIndex::drot_y: lambda(_subtracker_drot_y); return;
        case TData::FieldIndex::drot_z: lambda(_subtracker_drot_z); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    void set_mag_x(int64_t&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::mag_x, _ptr->mag_x(), val);
        _ptr->set_mag_x(std::move(val));
    }

    void set_mag_y(int64_t&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::mag_y, _ptr->mag_y(), val);
        _ptr->set_mag_y(std::move(val));
    }

    void set_mag_z(int64_t&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::mag_z, _ptr->mag_z(), val);
        _ptr->set_mag_z(std::move(val));
    }

    void set_acc_x(int64_t&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::acc_x, _ptr->acc_x(), val);
        _ptr->set_acc_x(std::move(val));
    }

    void set_acc_y(int64_t&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::acc_y, _ptr->acc_y(), val);
        _ptr->set_acc_y(std::move(val));
    }

    void set_acc_z(int64_t&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::acc_z, _ptr->acc_z(), val);
        _ptr->set_acc_z(std::move(val));
    }

    void set_rot_x(int64_t&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::rot_x, _ptr->rot_x(), val);
        _ptr->set_rot_x(std::move(val));
    }

    void set_rot_y(int64_t&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::rot_y, _ptr->rot_y(), val);
        _ptr->set_rot_y(std::move(val));
    }

    void set_rot_z(int64_t&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::rot_z, _ptr->rot_z(), val);
        _ptr->set_rot_z(std::move(val));
    }

    void set_dmag_x(double&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::dmag_x, _ptr->dmag_x(), val);
        _ptr->set_dmag_x(std::move(val));
    }

    void set_dmag_y(double&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::dmag_y, _ptr->dmag_y(), val);
        _ptr->set_dmag_y(std::move(val));
    }

    void set_dmag_z(double&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::dmag_z, _ptr->dmag_z(), val);
        _ptr->set_dmag_z(std::move(val));
    }

    void set_dacc_x(double&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::dacc_x, _ptr->dacc_x(), val);
        _ptr->set_dacc_x(std::move(val));
    }

    void set_dacc_y(double&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::dacc_y, _ptr->dacc_y(), val);
        _ptr->set_dacc_y(std::move(val));
    }

    void set_dacc_z(double&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::dacc_z, _ptr->dacc_z(), val);
        _ptr->set_dacc_z(std::move(val));
    }

    void set_drot_x(double&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::drot_x, _ptr->drot_x(), val);
        _ptr->set_drot_x(std::move(val));
    }

    void set_drot_y(double&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::drot_y, _ptr->drot_y(), val);
        _ptr->set_drot_y(std::move(val));
    }

    void set_drot_z(double&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::drot_z, _ptr->drot_z(), val);
        _ptr->set_drot_z(std::move(val));
    }

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

template <typename T> struct Stencil::DeltaTracker<T, std::enable_if_t<std::is_same_v<T, Avid::Pressure::Data>>>
{
    using TData = T;

    // TODO : Tentative: We hate pointers
    TData* const _ptr;
    // TODO : Better way to unify creation interface

    std::bitset<TData::FieldCount() + 1> _fieldtracker;
    DeltaTracker<double> _subtracker_pressure;
    DeltaTracker<double> _subtracker_temperature;
    DELETE_COPY_AND_MOVE(DeltaTracker);

    DeltaTracker(TData* ptr) :
        _ptr(ptr)
        ,
        _subtracker_pressure(&_ptr->pressure())
        ,
        _subtracker_temperature(&_ptr->temperature())
    {
        // TODO: Tentative
        static_assert(std::is_base_of<Stencil::ObservablePropsT<TData>, TData>::value);
    }

    TData& Obj() { return *_ptr; }

    static constexpr auto Type() { return ReflectionBase::TypeTraits<TData&>::Type(); }

    size_t NumFields() const { return TData::FieldCount(); }
    bool   IsChanged() const { return _fieldtracker.any(); }

    uint8_t MutatorIndex() const;
    bool    OnlyHasDefaultMutator() const;

    void MarkFieldChanged(typename TData::FieldIndex index) { _fieldtracker.set(static_cast<size_t>(index)); }
    bool IsFieldChanged(typename TData::FieldIndex index) const { return _fieldtracker.test(static_cast<size_t>(index)); }

    size_t CountFieldsChanged() const { return _fieldtracker.count(); }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda) const
    {
        switch (index)
        {
        case TData::FieldIndex::pressure: lambda(_subtracker_pressure); return;
        case TData::FieldIndex::temperature: lambda(_subtracker_temperature); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    template <typename TLambda> void Visit(typename TData::FieldIndex index, TLambda&& lambda)
    {
        switch (index)
        {
        case TData::FieldIndex::pressure: lambda(_subtracker_pressure); return;
        case TData::FieldIndex::temperature: lambda(_subtracker_temperature); return;
        case TData::FieldIndex::Invalid: throw std::invalid_argument("Asked to visit invalid field");
        }
    }

    void set_pressure(double&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::pressure, _ptr->pressure(), val);
        _ptr->set_pressure(std::move(val));
    }

    void set_temperature(double&& val)
    {
        Stencil::ObservablePropsT<TData>::OnChangeRequested(*this, TData::FieldIndex::temperature, _ptr->temperature(), val);
        _ptr->set_temperature(std::move(val));
    }

};

