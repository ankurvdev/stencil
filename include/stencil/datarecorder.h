#pragma once
#include "observableprops.h"

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

template <typename TTup, typename TLambda, size_t TIndex = 0> constexpr void VisitAt(TTup& tup, const size_t idx, TLambda&& lambda)
{
    if constexpr (TIndex == std::tuple_size<TTup>::value)
    {
        throw std::out_of_range("Exceeded maximum tuple range");
    }
    else
    {
        if (TIndex == idx)
        {
            lambda(std::get<TIndex>(tup));
            return;
        }
        return VisitAt<TTup, TLambda, TIndex + 1>(tup, idx, std::forward<TLambda>(lambda));
    }
}

template <typename TI, typename T, typename... Ts> constexpr size_t IndexOf(size_t index = 0)
{
    if constexpr (std::is_same<TI, T>::value)
    {
        return index;
    }
    else
    {
        if constexpr (sizeof...(Ts) == 0)
        {
            static_assert(sizeof...(Ts) != 0, "Cannot Match anything in tuple");
        }
        else
        {
            return IndexOf<TI, Ts...>(index + 1);
        }
    }
}

using ByteIt = std::span<const uint8_t>::iterator;

template <typename TVal, typename = typename std::enable_if<std::is_trivial<TVal>::value>::type>
static std::span<const uint8_t> AsCSpan(TVal const& val)
{
    return std::span(reinterpret_cast<uint8_t const*>(&val), sizeof(TVal));
}

template <typename TVal, typename = typename std::enable_if<std::is_trivial<TVal>::value>::type> static std::span<uint8_t> AsSpan(TVal& val)
{
    return std::span(reinterpret_cast<uint8_t*>(&val), sizeof(TVal));
}

struct Writer
{
    Writer() = default;

    template <typename TVal, std::enable_if_t<std::is_trivial<TVal>::value, bool> = true> Writer& operator<<(TVal const& val)
    {
        auto spn = AsCSpan(val);
        std::copy(spn.begin(), spn.end(), back_inserter(_buffer));
        return *this;
    }

    Writer& operator<<(shared_string const& val)
    {
        *this << val.size();
        auto cursize = _buffer.size();
        _buffer.resize(_buffer.size() + val.size());
        auto bufferPos = _buffer.begin() + static_cast<std::vector<uint8_t>::difference_type>(cursize);
        std::transform(val.begin(), val.end(), bufferPos, [](auto c) { return static_cast<uint8_t>(c); });
        return *this;
    }

    std::vector<uint8_t> Reset() { return std::move(_buffer); }

    std::vector<uint8_t> _buffer;
};

struct Reader
{
    Reader(ByteIt const& itbeg) : _it(itbeg) {}

    template <typename TVal, std::enable_if_t<std::is_trivial<TVal>::value, bool> = true> TVal read()
    {
        TVal val;
        auto endIt = _it + sizeof(TVal);
        std::copy(_it, endIt, AsSpan(val).begin());
        _it = endIt;
        return val;
    }

    shared_string read_shared_string()
    {
        size_t        size  = read<size_t>();
        shared_string str   = shared_string::make(std::string(size, 0));
        auto          endIt = _it + static_cast<ByteIt::difference_type>(size);
        std::transform(_it, endIt, str.begin(), [](auto c) { return static_cast<char>(c); });
        _it = endIt;
        return str;
    }

    auto   GetIterator() { return _it; }
    ByteIt _it;
};

struct BinarySerDes
{
    template <typename TVisitor> static void Serialize(TVisitor& visitor, Writer& writer)
    {
        switch (visitor.GetDataTypeHint())
        {
        case ReflectionBase::DataType::Value:
        {
            auto val = visitor.GetValue();
            writer << static_cast<uint8_t>(val.GetType());
            switch (val.GetType())
            {
            case Value::Type::Double:
            {
                writer << val.template convert<double>();
            }
            break;

            case Value::Type::Empty: break;
            case Value::Type::Signed:
            {
                writer << val.template convert<int64_t>();
            }
            break;

            case Value::Type::String:

                // TODO : See if this can be removed and renamed Value to Value64Bit
                // Value::Type::String -> TrivialConstArray
                {
                    auto str = val.template convert<shared_string>();
                    writer << str;
                }
                break;

            case Value::Type::Unsigned:
            {
                writer << val.template convert<uint64_t>();
            }
            break;
            case Value::Type::Unknown: throw std::logic_error("Unknown Value Type");
            }
        }
        break;
        case ReflectionBase::DataType::Object: [[fallthrough]];
        case ReflectionBase::DataType::List:
        {
            for (size_t i = 0; visitor.TrySelect(i); i++)
            {
                Serialize(visitor, writer);
                visitor.GoBackUp();
            }
        }
        break;
        case ReflectionBase::DataType::Enum: TODO();
        case ReflectionBase::DataType::Union: TODO();
        case ReflectionBase::DataType::Invalid: [[fallthrough]];
        case ReflectionBase::DataType::Unknown: throw std::runtime_error("Unsupported Data Type");
        }
    }

    template <typename TVisitor> static void Deserialize(TVisitor& visitor, Reader& reader)
    {
        switch (visitor.GetDataTypeHint())
        {
        case ReflectionBase::DataType::Value:
        {
            auto valType = static_cast<Value::Type>(reader.read<uint8_t>());
            switch (valType)
            {
            case Value::Type::Double: visitor.SetValue(Value{reader.read<double>()}); break;
            case Value::Type::Empty: break;
            case Value::Type::Signed: visitor.SetValue(Value{reader.read<int64_t>()}); break;
            case Value::Type::String:
                // TODO : See if this can be removed and renamed Value to Value64Bit
                // Value::Type::String -> TrivialConstArray
                visitor.SetValue(Value{reader.read_shared_string()});
                break;
            case Value::Type::Unsigned: visitor.SetValue(Value{reader.read<uint64_t>()}); break;
            case Value::Type::Unknown: throw std::logic_error("Unknown Value Type");
            }
        }
        break;
        case ReflectionBase::DataType::Object: TODO();
        case ReflectionBase::DataType::Enum: TODO();
        case ReflectionBase::DataType::List: TODO();
        case ReflectionBase::DataType::Union: TODO();
        case ReflectionBase::DataType::Invalid: [[fallthrough]];
        case ReflectionBase::DataType::Unknown: throw std::runtime_error("Unsupported Data Type");
        }
    }
};

template <typename T> struct Visitor
{
    static constexpr auto GetPtrType()
    {
        if constexpr (std::is_const_v<T>)
        {
            void const* ptr{nullptr};
            return ptr;
        }
        else
        {
            void* ptr{nullptr};
            return ptr;
        }
    }
    /*
    ReflectionServices should provide an Object Browser(Mutator/Accessor), (Const)Visitor ?
        - Ability to Dive into List, Objects
        - No need for Handlers.
        - Serialize/Deserialize using a class Handler at any point during the browser.
        - Optionally carry DeltaTracker data but no need to explicity specify

    Usage Spec
        Deserialize : Visit for edit
            ReflectionServices::ObjectVisitor<Avid::GPS::Data const> visitor;
        Serialize: Visit for read
            ReflectionServices::ObjectVisitor<Avid::GPS::Data> visitor;
        Ability to dive into List, objects, maps etc
            visitor.Select(Value{});
            visitor.Select<T, Deserializer>(Value{});
            visitor.Select<T>(T const&);
            visitor.GoBackUp(); // Go back up
        Serialize Deserialize using a class Handler at any point during the browser
            visitor.Serialize<TSerializer>(); // Current DeltaTracker
            visitor.Deserialize<TDeserializer>(TDeserizlier::Input const&) // Current DeltaTracker
        Optionally carry DeltaTracker data but no need to explicitly specify
            ?? template<typename T, typename TDeltaTracker = void *> // Try void but void* should work too.
            ?? All Select variants and constructors have TDeltaTracker ctx = {} optional param
        Note:
        Handlers accept const for browsing and non-const for editing
        Ediiting on non-const throws exception
    */
    // TODO: const T&
    public:    // Declarations
    enum Mode
    {
        Default,
        Enum,
        List,
        Obj,
        Union,
        Key,    // List: Integer, Union: Integer, Obj: String/Integer
    };

    struct StateStack
    {
        const ReflectionBase::IDataTypeHandler<ReflectionBase::DataType::Unknown>* Handler = nullptr;
        decltype(GetPtrType())                                                     Ptr     = nullptr;
        Mode                                                                       mode;
    };

    Visitor(T& obj) { _stack.push_back(StateStack{&_rootHandler, static_cast<decltype(GetPtrType())>(&obj), Mode::Obj}); }

    bool TrySelect(Value const& val)
    {
        {
            switch (GetDataTypeHint())
            {
            case ReflectionBase::DataType::Object:
            {
                auto& state = _stack.back();
                auto  cptr  = const_cast<void*>(state.Ptr);    // Shhh... Thats ok.
                ReflectionBase::IDataTypeHandler<ReflectionBase::DataType::Object>::SubComponent sub;

                if (!state.Handler->ObjectHandler()->TryGetSubComponent(cptr, val, sub))
                {
                    return false;
                }
                _stack.push_back(StateStack{sub.handler, sub.ptr, Mode::Obj});
                return true;
            }

            case ReflectionBase::DataType::List:
            {
                auto& state = _stack.back();
                auto  cptr  = const_cast<void*>(state.Ptr);    // Shhh... Thats ok.
                ReflectionBase::IDataTypeHandler<ReflectionBase::DataType::List>::SubComponent sub;

                if (!state.Handler->ListHandler()->TryGetSubComponent(cptr, val, sub))
                {
                    return false;
                }
                _stack.push_back(StateStack{sub.handler, sub.ptr, Mode::Obj});
                return true;
            }

            case ReflectionBase::DataType::Union: TODO(); throw std::runtime_error("Not yet supported. Get to work");

            case ReflectionBase::DataType::Value: [[fallthrough]];
            case ReflectionBase::DataType::Enum: [[fallthrough]];
            case ReflectionBase::DataType::Invalid: [[fallthrough]];
            case ReflectionBase::DataType::Unknown: break;
            }
            throw std::runtime_error("Unsupported Data Type");
        }
    }

    Visitor& Select(Value const& val)
    {
        switch (GetDataTypeHint())
        {
        case ReflectionBase::DataType::Object:
        {
            auto& state = _stack.back();
            auto  cptr  = const_cast<void*>(state.Ptr);    // Shhh... Thats ok.
            auto  sub   = state.Handler->ObjectHandler()->GetSubComponentAt(cptr, val.convert<size_t>());
            _stack.push_back(StateStack{sub.handler, sub.ptr, Mode::Obj});
            return *this;
        }
        case ReflectionBase::DataType::List: TODO();
        case ReflectionBase::DataType::Union: TODO(); throw std::runtime_error("Not yet supported. Get to work");

        case ReflectionBase::DataType::Value: [[fallthrough]];
        case ReflectionBase::DataType::Enum: [[fallthrough]];
        case ReflectionBase::DataType::Invalid: [[fallthrough]];
        case ReflectionBase::DataType::Unknown: break;
        }
        throw std::runtime_error("Unsupported Data Type");
    }

    template <typename TKey, typename TSerDes = BinarySerDes> Visitor& Select(TKey const& key)
    {
        if constexpr (Value::Supported<TKey>::value)
        {
            return Select(Value{key});
        }
        else
        {
            TODO();
            return *this;
        }
    }

    Visitor& GoBackUp()
    {
        _stack.pop_back();
        return *this;
    }

    auto  GetDataTypeHint() const { return _stack.back().Handler->GetDataType(); }
    Value GetValue() const
    {
        switch (GetDataTypeHint())
        {

        case ReflectionBase::DataType::Value:
        {
            auto& state = _stack.back();
            auto  cptr  = const_cast<void*>(state.Ptr);    // Shhh... Thats ok.
            return state.Handler->ValueHandler()->Read(cptr);
        }
        case ReflectionBase::DataType::Enum: [[fallthrough]];

        case ReflectionBase::DataType::Object: [[fallthrough]];
        case ReflectionBase::DataType::List: [[fallthrough]];
        case ReflectionBase::DataType::Union: [[fallthrough]];

        case ReflectionBase::DataType::Invalid: [[fallthrough]];
        case ReflectionBase::DataType::Unknown: break;
        }
        throw std::runtime_error("Unsupported Data Type");
    }

    void SetValue(Value const& val)
    {
        static_assert(!std::is_const_v<T>, "Cannot SetValue into a const type");

        switch (GetDataTypeHint())
        {
        case ReflectionBase::DataType::Value:
        {
            auto& state = _stack.back();
            auto  cptr  = const_cast<void*>(state.Ptr);    // Shhh... Thats ok.
            return state.Handler->ValueHandler()->Write(cptr, val);
        }
        case ReflectionBase::DataType::Enum: [[fallthrough]];

        case ReflectionBase::DataType::Object: [[fallthrough]];
        case ReflectionBase::DataType::List: [[fallthrough]];
        case ReflectionBase::DataType::Union: [[fallthrough]];

        case ReflectionBase::DataType::Invalid: [[fallthrough]];
        case ReflectionBase::DataType::Unknown: break;
        }
        throw std::runtime_error("Unsupported Data Type");
    }

    template <typename TSerDes = BinarySerDes> void Serialize(Writer& writer) { return BinarySerDes::Serialize(*this, writer); }

    template <typename TSerDes = BinarySerDes> void Deserialize(Reader& reader)
    {
        static_assert(!std::is_const_v<T>, "Cannot Deserialize into a const type");
        BinarySerDes::Deserialize(*this, reader);
    }

    private:
    typename ReflectionBase::TypeTraits<std::remove_const_t<T>&>::Handler _rootHandler;
    std::vector<StateStack>                                               _stack;
};

template <typename T> struct PatchHandler
{
    static std::vector<uint8_t> Create(T const& obj, DeltaTracker<T> const& ctx)
    {
        Writer           writer;
        Visitor<T const> visitor(obj);
        _Write(writer, visitor, ctx);
        return writer.Reset();
    }

    template <typename TObj, typename TVisitor> static void _Write(Writer& writer, TVisitor& visitor, DeltaTracker<TObj> const& ctx)
    {

        // Create(int, ctx) = nochange=>{}  change=>{0,0,0,1}
        // Create(string, ctx) = nochange=>{}  change=>{n, e, w, s, t, r, i, n, g}
        // Create(list<int>, ctx) = nochange=>{} change=>{set/add/remove, serialized list / index {0, 0, 0, 1} / index }
        // Create(struct, ctx) = nochange=>{} change=>{fieldindex, Create<fieldIndex>(val, subctx)}
        // Create(list<struct>, ctx) = nochange=>{} change=>{set/add/remove, serialized value / index Create<struct>
        if (!ctx.IsChanged())
        {
            return;
        }
        static_assert(DeltaTracker<TObj>::Type() != ReflectionBase::DataType::Invalid, "Cannot Create Patch for Invalid DataType");

        if constexpr (DeltaTracker<TObj>::Type() == ReflectionBase::DataType::Enum
                      || DeltaTracker<TObj>::Type() == ReflectionBase::DataType::Value)
        {
            // Dump the entire value
            visitor.Serialize(writer);
        }
        else if constexpr (DeltaTracker<TObj>::Type() == ReflectionBase::DataType::List)
        {
            writer << ctx.MutatorIndex();
            if (ctx.MutatorIndex() == 0)    // Set
            {
                visitor.Serialize(writer);
            }
            else if (ctx.MutatorIndex() == 1)    // List Add
            {
                writer << ctx.ListIndex();
                visitor.Select(ctx.ListIndex());
                _Write(writer, visitor, ctx.GetSubObjectTracker(ctx.ListIndex()));
                visitor.GoBackUp();
            }
            else if (ctx.MutatorIndex() == 2)    // List remove
            {
                writer << ctx.ListIndex();
            }
            else
            {
                throw std::logic_error("Unknown Mutator");
            }
        }
        else if constexpr (DeltaTracker<TObj>::Type() == ReflectionBase::DataType::Object)
        {
            // 0 is always invalid field
            auto fieldsChanged = ctx.CountFieldsChanged();
            if (ctx.NumFields() > 254)
            {
                throw std::logic_error("Too Many fields. Not supported yet");
            }
            writer << static_cast<uint8_t>(fieldsChanged);
            for (size_t i = 0u; i < ctx.NumFields(); i++)
            {
                ctx.Visit(static_cast<typename TObj::FieldIndex>(i + 1), [&](auto const& subctx) {
                    if (!subctx.IsChanged()) return;
                    if (fieldsChanged == 0)
                    {
                        throw std::logic_error("Something doesnt add up. Too many changed fields visited");
                    }

                    fieldsChanged--;

                    visitor.Select(i);
                    writer << static_cast<uint8_t>(i);
                    _Write(writer, visitor, subctx);
                    visitor.GoBackUp();
                });
            }
        }
        else if constexpr (DeltaTracker<TObj>::Type() == ReflectionBase::DataType::Union)
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
            static_assert(DeltaTracker<TObj>::Type() == ReflectionBase::DataType::Unknown);
            static_assert(DeltaTracker<TObj>::Type() != ReflectionBase::DataType::Unknown, "Cannot Create Patch for Unknown DataType");
        }
    }

    static ByteIt Apply(T& obj, ByteIt const& itbeg)
    {
        Reader     reader(itbeg);
        Visitor<T> visitor(obj);

        // TODO: TypeTraits should not have reference type
        static_assert(ReflectionBase::TypeTraits<T&>::Type() != ReflectionBase::DataType::Invalid,
                      "Cannot Create Patch for Invalid DataType");

        if constexpr (ReflectionBase::TypeTraits<T&>::Type() == ReflectionBase::DataType::Enum
                      || ReflectionBase::TypeTraits<T&>::Type() == ReflectionBase::DataType::Value)
        {
            visitor.Deserialize(reader);
        }

        else if constexpr (ReflectionBase::TypeTraits<T&>::Type() == ReflectionBase::DataType::List)
        {
            throw std::logic_error("lists not supported for patch yet");
#if 0
            auto mutatorIndex = reader.read<uint8_t>();

            if (mutatorIndex == 0)    // Set
            {
                visitor.Deserialize(reader);
            }
            else if (mutatorIndex == 1)    // List Add
            {
                auto listIndex = reader.read<uint8_t>();
                visitor.Select(listIndex);
                visitor.Deserialize(reader);
                visitor.GoBackUp();
            }
            else if (mutatorIndex == 2)    // List remove
            {
                auto listIndex = reader.read<uint8_t>();
                throw std::logic_error("Dont know how to remove using visitor");
            }
            else
            {
                throw std::logic_error("Unknown Mutator");
            }
#endif
        }
        else if constexpr (ReflectionBase::TypeTraits<T&>::Type() == ReflectionBase::DataType::Object)
        {
            auto fieldCount = reader.read<uint8_t>();
            for (uint8_t i = 0; i < fieldCount; i++)
            {
                visitor.Select(reader.read<uint8_t>());
                visitor.Deserialize(reader);
                visitor.GoBackUp();
            }
        }
        else if constexpr (ReflectionBase::TypeTraits<T&>::Type() == ReflectionBase::DataType::Union)
        {
            throw std::logic_error("unions not supported for patch yet");
        }
        else
        {
            static_assert(ReflectionBase::TypeTraits<T&>::Type() == ReflectionBase::DataType::Unknown);
            static_assert(ReflectionBase::TypeTraits<T&>::Type() != ReflectionBase::DataType::Unknown,
                          "Cannot Create Patch for Unknown DataType");
        }

        return reader.GetIterator();
    }
};

template <typename T> struct BinarySerDesHandler
{
    static std::vector<uint8_t> Serialize(T const& obj)
    {
        Writer           writer;
        Visitor<T const> visitor(obj);
        visitor.Serialize(writer);
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
        if (!_file.good())
        {
            throw std::logic_error("File Error");
        }
        T val;
        fs.read(reinterpret_cast<char*>(&val), std::streamsize{sizeof(T)});
        std::streamsize bytes = fs.gcount();
        if (!_file.good())
        {
            throw std::logic_error("File Error");
        }
        if (bytes != sizeof(T))
        {
            throw std::logic_error("Not enough bytes read");
        }
        return val;
    }

    template <typename T> auto ReadChangeDescAndNotify(T& obj, std::span<const uint8_t>::iterator const& dataIt)
    {
        [[maybe_unused]] auto ctx = obj.Edit();
        return PatchHandler<T>::Apply(obj, dataIt);
    }

    void _ThreadFunc()
    {
        while (_state != State::Working) std::this_thread::sleep_for(std::chrono::milliseconds{1});
        _file.seekg(0, _file.end);
        auto length = _file.tellg();
        _file.seekg(0, _file.beg);

        while (_state != State::StopRequested)
        {
            if (!_file.good() || _file.tellg() == length)
            {
                return;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds{read<std::chrono::milliseconds::rep>(_file)});
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
        if (f.empty())
        {
            return;
        }

        _ofst = std::ofstream(f, std::fstream::out | std::fstream::app);
        _ost  = &_ofst;
    }

    // void OnChanged(LockT const& /*lock*/, DeltaTrackerT const& /*ctx*/, T const& /*data*/) override { TODO(); }
    template <typename T> void Record(T const& data, DeltaTracker<T> const& ctx)
    {
        if (!ctx.IsChanged())
        {
            return;
        }

        auto now = std::chrono::system_clock::now();

        auto delta   = now - _init_time;
        auto deltaus = std::chrono::duration_cast<std::chrono::microseconds>(delta).count();

        _lastnotif           = now;
        auto serializedPatch = PatchHandler<T>::Create(data, ctx);
        (*this) << deltaus << static_cast<uint8_t>(IndexOf<T, Ts...>()) << static_cast<uint16_t>(serializedPatch.size()) << serializedPatch;
    }

    std::chrono::system_clock::time_point _init_time = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point _lastnotif = _init_time;
    std::ostream*                         _ost       = &std::cout;
    std::ofstream                         _ofst;
};

}    // namespace Stencil