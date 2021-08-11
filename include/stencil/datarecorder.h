#pragma once
#include "visitor.h"

#include <chrono>
#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <span>
#include <thread>
#include <type_traits>

namespace Stencil
{

template <typename T> struct PatchHandler
{
    static std::vector<uint8_t> Create(T const& obj, Transaction<T> const& ctx)
    {
        Writer           writer;
        Visitor<T const> visitor(obj);
        _Write(writer, visitor, ctx);
        return writer.Reset();
    }

    static ByteIt Apply(Transaction<T>& ctx, ByteIt const& itbeg)
    {
        Reader     reader(itbeg);
        Visitor<T> visitor(ctx.Obj());
        _Apply(reader, visitor, ctx);
        return reader.GetIterator();
    }

    template <typename TObj, typename TVisitor> static void _Write(Writer& writer, TVisitor& visitor, Transaction<TObj> const& ctx)
    {

        // Create(int, ctx) = nochange=>{}  change=>{0,0,0,1}
        // Create(string, ctx) = nochange=>{}  change=>{n, e, w, s, t, r, i, n, g}
        // Create(list<int>, ctx) = nochange=>{} change=>{set/add/remove, serialized list / index {0, 0, 0, 1} / index }
        // Create(struct, ctx) = nochange=>{} change=>{fieldindex, Create<fieldIndex>(val, subctx)}
        // Create(list<struct>, ctx) = nochange=>{} change=>{set/add/remove, serialized value / index Create<struct>
        if (!ctx.IsChanged()) { return; }
        static_assert(Transaction<TObj>::Type() != ReflectionBase::DataType::Invalid, "Cannot Create Patch for Invalid DataType");

        if constexpr (Transaction<TObj>::Type() == ReflectionBase::DataType::Enum
                      || Transaction<TObj>::Type() == ReflectionBase::DataType::Value)
        {
            // Dump the entire value
            BinarySerDes::Serialize(visitor, writer);
        }
        else if constexpr (Transaction<TObj>::Type() == ReflectionBase::DataType::List)
        {
            writer << static_cast<uint8_t>(ctx.MutationCount());
            for (auto& mutation : ctx.Mutations())
            {
                auto mutationIndex = mutation.mutationIndex;
                auto listIndex     = mutation.listIndex;
                writer << static_cast<uint8_t>(mutationIndex);
                if (mutationIndex == 0)    // Set
                {
                    BinarySerDes::Serialize(visitor, writer);
                }
                else if (mutationIndex == 1)    // List Add
                {
                    writer << static_cast<uint32_t>(listIndex);
                    visitor.Select(listIndex);
                    BinarySerDes::Serialize(visitor, writer);
                    //_Write(writer, visitor, ctx.GetSubObjectTracker(ctx.ListIndex()));
                    visitor.GoBackUp();
                }
                else if (mutationIndex == 2)    // List remove
                {
                    writer << static_cast<uint32_t>(listIndex);
                }
                else if (mutationIndex == 3)    // List edit
                {
                    writer << static_cast<uint32_t>(listIndex);
                    visitor.Select(listIndex);
                }
                else
                {
                    throw std::logic_error("Unknown Mutator");
                }
            }
        }
        else if constexpr (Transaction<TObj>::Type() == ReflectionBase::DataType::Object)
        {
            // 0 is always invalid field
            auto fieldsChanged = ctx.CountFieldsChanged();
            if (ctx.NumFields() > 254) { throw std::logic_error("Too Many fields. Not supported yet"); }
            writer << static_cast<uint8_t>(fieldsChanged);
            for (size_t i = 0u; i < ctx.NumFields(); i++)
            {
                auto fieldIndex = static_cast<typename TObj::FieldIndex>(i + 1);
                ctx.Visit(fieldIndex, [&](auto const& subctx) {
                    if (!ctx.IsFieldChanged(fieldIndex)) return;
                    if (fieldsChanged == 0) { throw std::logic_error("Something doesnt add up. Too many changed fields visited"); }

                    fieldsChanged--;

                    visitor.Select(i);
                    writer << static_cast<uint8_t>(i);
                    _Write(writer, visitor, subctx);
                    visitor.GoBackUp();
                });
            }
        }
        else if constexpr (Transaction<TObj>::Type() == ReflectionBase::DataType::Union)
        {
            throw std::logic_error("unions not supported for patch yet");
            /*
            writer << ctx.MutatorIndex();
            if (mutatorIndex == 0)    // Set Value
            {
                writer << _Create(visitor, )
            }
            else if (mutatorIndex == 1)    // Set Type
            {
            }
            else
            {
                throw std::logic_error("Unknown Mutator");
            }*/
        }
        else
        {
            static_assert(Transaction<TObj>::Type() == ReflectionBase::DataType::Unknown);
            static_assert(Transaction<TObj>::Type() != ReflectionBase::DataType::Unknown, "Cannot Create Patch for Unknown DataType");
        }
    }

    template <typename TObj, typename TVisitor> static void _Apply(Reader& reader, TVisitor& visitor, Transaction<TObj>& ctx)
    {
        // TODO: TypeTraits should not have reference type
        static_assert(ReflectionBase::TypeTraits<TObj&>::Type() != ReflectionBase::DataType::Invalid,
                      "Cannot Create Patch for Invalid DataType");

        if constexpr (ReflectionBase::TypeTraits<TObj&>::Type() == ReflectionBase::DataType::Enum
                      || ReflectionBase::TypeTraits<TObj&>::Type() == ReflectionBase::DataType::Value)
        {
            BinarySerDes::Deserialize(visitor, reader);
        }

        else if constexpr (ReflectionBase::TypeTraits<TObj&>::Type() == ReflectionBase::DataType::List)
        {
            auto mutatorIndex = reader.read<uint8_t>();
            if (mutatorIndex == 0)    // Set
            {
                BinarySerDes::Deserialize(visitor, reader);
            }
            else if (mutatorIndex == 1)    // List Add
            {
                auto listIndex = reader.read<uint32_t>();
                visitor.Select(listIndex);
                BinarySerDes::Deserialize(visitor, reader);

                // auto subctx = ctx.GetSubObjectTracker(listIndex);
                //_Apply(reader, visitor, subctx);
                visitor.GoBackUp();
            }
            else if (mutatorIndex == 2)    // List remove
            {
                auto listIndex = reader.read<uint32_t>();
                Stencil::Mutators<TObj>::remove(ctx, ctx.Obj(), listIndex);
            }
            else if (mutatorIndex == 3)    // List edit
            {
                auto listIndex = reader.read<uint32_t>();
                visitor.Select(listIndex);
                auto subctx = ctx.GetSubObjectTracker(ctx.Obj(), listIndex);
                _Apply(reader, visitor, subctx);
                visitor.GoBackUp();
            }
            else
            {
                throw std::logic_error("Unknown Mutator");
            }
        }
        else if constexpr (ReflectionBase::TypeTraits<TObj&>::Type() == ReflectionBase::DataType::Object)
        {
            for (auto changedFieldIndex = reader.read<uint8_t>(); changedFieldIndex != 0; changedFieldIndex = reader.read<uint8_t>())
            {
                auto action = reader.read<uint8_t>();
                // assert(changedFieldIndex <= ctx.NumFields());
                auto changedFieldType = static_cast<typename TObj::FieldIndex>(changedFieldIndex);
                visitor.Select(static_cast<uint8_t>(changedFieldIndex - 1));
                if (action == 0)    // Set
                {
                    BinarySerDes::Deserialize(visitor, reader);
                }
                else if (action == 1)    // Edit
                {
                    ctx.Visit(changedFieldType, [&](auto& subctx) { _Apply(reader, visitor, subctx); });
                }
                else
                {
                    throw std::logic_error("Unknown action");
                }
                visitor.GoBackUp();
            }
        }
        else if constexpr (ReflectionBase::TypeTraits<TObj&>::Type() == ReflectionBase::DataType::Union)
        {
            throw std::logic_error("unions not supported for patch yet");
        }
        else
        {
            static_assert(ReflectionBase::TypeTraits<TObj&>::Type() == ReflectionBase::DataType::Unknown);
            static_assert(ReflectionBase::TypeTraits<TObj&>::Type() != ReflectionBase::DataType::Unknown,
                          "Cannot Create Patch for Unknown DataType");
        }
    }
};

template <typename T> struct BinarySerDesHandler
{
    static std::vector<uint8_t> Serialize(T const& obj)
    {
        Writer           writer;
        Visitor<T const> visitor(obj);
        BinarySerDes::Serialize(visitor, writer);
        return writer.Reset();
    }
};

template <typename T> using Lock = std::unique_lock<std::mutex>;

template <typename... Ts> struct DataPlayerT : std::enable_shared_from_this<DataPlayerT<Ts...>>
{
    using time_point = std::chrono::system_clock::time_point;
    using clock      = std::chrono::system_clock;

    DELETE_COPY_AND_MOVE(DataPlayerT);
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

    template <typename T> auto ReadChangeDescAndNotify(T& obj, std::span<const uint8_t>::iterator const& dataIt)
    {
        Stencil::NullTransactionRecorder recorder;
        auto                             ctx = recorder.Start(obj);
        return PatchHandler<T>::Apply(ctx, dataIt);
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
                size_t               index = read<uint8_t>(_file);
                auto                 bytes = read<uint16_t>(_file);
                std::vector<uint8_t> data(bytes);
                _file.read(reinterpret_cast<char*>(data.data()), bytes);
                std::span<const uint8_t> dataSpan(data);
                auto                     it = dataSpan.begin();
                VisitAt(_data, index, [&](auto& arg) { it = ReadChangeDescAndNotify<std::remove_reference_t<decltype(arg)>>(arg, it); });

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
    time_point         _init_time = clock::now();

    std::mutex              _mutex;
    size_t                  _counter{0};
    std::condition_variable _cv;
};

template <typename... Ts> struct DataRecorder
{
    template <typename T, std::enable_if_t<std::is_trivial<T>::value, bool> = true> DataRecorder& operator<<(T const& val)
    {
        (*_ost).write(reinterpret_cast<const char*>(&val), std::streamsize{sizeof(T)});
        return *this;
    }

    DataRecorder& operator<<(std::vector<uint8_t> const& val)
    {
        (*_ost).write(reinterpret_cast<const char*>(val.data()), static_cast<std::streamsize>(val.size()));
        return *this;
    }

    DELETE_COPY_DEFAULT_MOVE(DataRecorder);
    DataRecorder(std::filesystem::path const& f)
    {
        if (f.empty()) { return; }

        _ofst = std::ofstream(f, std::fstream::out | std::fstream::app);
        _ost  = &_ofst;
    }

    // void OnChanged(LockT const& /*lock*/, TransactionT const& /*ctx*/, T const& /*data*/) override { TODO(); }
    template <typename T> void Record(Transaction<T> const& ctx)
    {
        if (!ctx.IsChanged()) { return; }

        auto now = std::chrono::system_clock::now();

        auto delta   = now - _init_time;
        auto deltaus = std::chrono::duration_cast<std::chrono::microseconds>(delta).count();

        _lastnotif           = now;
        auto serializedPatch = PatchHandler<T>::Create(ctx.Obj(), ctx);
        (*this) << deltaus << static_cast<uint8_t>(IndexOf<T, Ts...>()) << static_cast<uint16_t>(serializedPatch.size()) << serializedPatch;
    }

    std::chrono::system_clock::time_point _init_time = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point _lastnotif = _init_time;
    std::ostream*                         _ost       = &std::cout;
    std::ofstream                         _ofst;
};

}    // namespace Stencil
