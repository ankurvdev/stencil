#pragma once
#include "timestamped.h"
#include "transactions.binserdes.h"
#include "tuple_utils.h"

#include <chrono>
#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>
#include <type_traits>

namespace Stencil
{
template <typename T> using Lock = std::unique_lock<std::mutex>;

template <typename... Ts> struct DataPlayerT : std::enable_shared_from_this<DataPlayerT<Ts...>>
{
    using Timestamp = Timestamp;

    CLASS_DELETE_COPY_AND_MOVE(DataPlayerT);
    DataPlayerT(std::filesystem::path const& replayFile, bool repeat = true) : _repeat(repeat), _file(replayFile) { Start(); }

    ~DataPlayerT() { _thrd.join(); }

    void Start() { _state = State::Working; }

    void Wait(size_t n = 1)
    {
        auto lock = std::unique_lock<std::mutex>(_mutex);
        _cv.wait(lock, [&]() { return _counter >= n; });
    }

    template <typename T> T const& Get() const { return std::get<T>(_data); }

    private:
    enum class State : unsigned
    {
        Initialized,
        Working,
        StopRequested
    };

    template <typename T> T read(std::ifstream& fs)
    {
        if (!_file.good()) { throw std::logic_error("File Error"); }
        T val;
        fs.read(reinterpret_cast<char*>(&val), std::streamsize{sizeof(T)});
        std::streamsize bytes = fs.gcount();
        if (!_file.good()) { throw std::logic_error("File Error"); }
        if (bytes != sizeof(T)) { throw std::logic_error("Not enough bytes read"); }
        return val;
    }

    template <typename T> void ReadChangeDescAndNotify(T& obj, std::istream& ostrm)
    {
        Stencil::Transaction<T> txn(obj);
        Stencil::BinaryTransactionSerDes::Apply(txn, ostrm);
    }

    void _ThreadFunc()
    {
        while (_state != State::Working) std::this_thread::sleep_for(std::chrono::milliseconds{1});
        _file.seekg(0, _file.end);
        auto length = _file.tellg();
        _file.seekg(0, _file.beg);

        while (_state != State::StopRequested)
        {
            if (!_file.good() || _file.tellg() == length) { return; }
            std::this_thread::sleep_for(std::chrono::microseconds{read<std::chrono::microseconds::rep>(_file)});
            {
                size_t index = read<uint8_t>(_file);
                TupleVisitAt(_data, index, [&](auto& arg) { ReadChangeDescAndNotify(arg, _file); });
                {
                    auto lock = std::unique_lock<std::mutex>(_mutex);
                    _counter++;
                    _cv.notify_all();
                }
            }
        }
    }

    bool               _repeat{true};
    std::ifstream      _file;
    std::tuple<Ts...>  _data;
    std::thread        _thrd{[this]() { _ThreadFunc(); }};
    std::atomic<State> _state{State::Initialized};
    Timestamp          _init_time = Timestamp::clock::now();

    std::mutex              _mutex;
    size_t                  _counter{0};
    std::condition_variable _cv;
};

template <typename... Ts> struct DataRecorder
{
    template <typename T, std::enable_if_t<std::is_trivially_default_constructible<T>::value, bool> = true>
    DataRecorder& operator<<(T const& val)
    {
        (*_ost).write(reinterpret_cast<char const*>(&val), std::streamsize{sizeof(T)});
        return *this;
    }

    DataRecorder& operator<<(std::vector<uint8_t> const& val)
    {
        (*_ost).write(reinterpret_cast<char const*>(val.data()), static_cast<std::streamsize>(val.size()));
        return *this;
    }

    CLASS_DELETE_COPY_DEFAULT_MOVE(DataRecorder);
    DataRecorder(std::filesystem::path const& f)
    {
        if (f.empty()) { return; }

        _ofst = std::ofstream(f, std::fstream::out | std::fstream::app);
        _ost  = &_ofst;
    }

    // void OnChanged(LockT const& /*lock*/, TransactionT const& /*ctx*/, T const& /*data*/) override { TODO(); }
    template <ConceptTransaction T> void Record(T& ctx)
    {
        if (!ctx.IsChanged()) { return; }

        auto now = Timestamp::clock::now();

        auto delta   = now - _init_time;
        auto deltaus = std::chrono::duration_cast<std::chrono::microseconds>(delta).count();

        _lastnotif = now;
        (*this) << deltaus << static_cast<uint8_t>(TupleIndexOf<T, Ts...>());
        Stencil::BinaryTransactionSerDes::Deserialize(ctx, *_ost);

        //<< static_cast<uint16_t>(serializedPatch.size()) << serializedPatch;
    }

    Timestamp     _init_time = Timestamp::clock::now();
    Timestamp     _lastnotif = _init_time;
    std::ostream* _ost       = &std::cout;
    std::ofstream _ofst;
};

}    // namespace Stencil
