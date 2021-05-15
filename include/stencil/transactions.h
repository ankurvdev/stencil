
namespace Stencil
{
template <typename TObj, typename _Ts = void> struct Transaction;

struct TransactionRecorder
{
    virtual ~TransactionRecorder() = default;

    template <typename TObj> Transaction<TObj> Start(TObj& obj) { return Transaction<TObj>(obj, *this); }

    template <typename T> TransactionRecorder& operator<<(T const& obj)
    {
        auto byteSpan = std::span<uint8_t const>(reinterpret_cast<uint8_t const*>(&obj), sizeof(T));
        _it           = std::copy(byteSpan.begin(), byteSpan.end(), _it);
        return *this;
    }

    template <typename T> TransactionRecorder& operator<<(std::vector<T> const& obj)
    {
        auto byteSpan = std::span<uint8_t const>(reinterpret_cast<uint8_t const*>(obj.data()), sizeof(T) * obj.size());
        _it           = std::copy(byteSpan.begin(), byteSpan.end(), _it);
        return *this;
    }
    void Flush_()
    {
        Write_(std::span<uint8_t const>(_buffer.begin(), _it));
        _it = _buffer.begin();
    }

    virtual void Write_(std::span<uint8_t const> buffer) = 0;

    std::array<uint8_t, 1024>           _buffer;
    std::array<uint8_t, 1024>::iterator _it{_buffer.begin()};
};

template <typename TObj> struct FileTransactionRecorder : TransactionRecorder
{
    FileTransactionRecorder(std::filesystem::path const& fpath) : _fpath(fpath) {}

    virtual void Write_(std::span<uint8_t const> buffer) override
    {
        std::ofstream fstrm(_fpath, std::fstream::out | std::fstream::app);
        auto          now = std::chrono::system_clock::now();

        auto delta   = now - _init_time;
        auto deltaus = std::chrono::duration_cast<std::chrono::microseconds>(delta).count();
        _lastnotif   = now;
        _write(fstrm, deltaus);
        _write(fstrm, uint8_t{0});
        _write(fstrm, static_cast<uint16_t>(buffer.size()));
        _write(fstrm, buffer);
    }

    template <typename T, std::enable_if_t<std::is_trivial<T>::value, bool> = true>
    std::ofstream& _write(std::ofstream& fstrm, T const& val)
    {
        fstrm.write(reinterpret_cast<const char*>(&val), std::streamsize{sizeof(T)});
        return fstrm;
    }

    std::ofstream& _write(std::ofstream& fstrm, std::span<uint8_t const> const& val)
    {
        fstrm.write(reinterpret_cast<const char*>(val.data()), static_cast<std::streamsize>(val.size()));
        return fstrm;
    }

    private:
    std::filesystem::path                 _fpath;
    std::chrono::system_clock::time_point _init_time = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point _lastnotif = _init_time;
};

template <typename TObj, typename _Ts = void> struct TransactionT
{
    TransactionT(TObj& /*obj*/, TransactionRecorder& /*rec*/) {}
};

template <typename TObj>
struct TransactionT<TObj, std::enable_if_t<ReflectionBase::TypeTraits<TObj&>::Type() == ReflectionBase::DataType::Object>>
{
    TransactionT(TObj& obj, TransactionRecorder& rec) : _ref(std::ref(obj)), _recorder(rec) {}
    ~TransactionT()
    {
        if (_fieldtracker.any())
        {
            for (size_t i = 0; i < _fieldtracker.size(); i++)
            {
                if (!_fieldtracker.test(i)) continue;
                Visitor<TObj> visitor(_ref);
                visitor.Select(Value{i - 1});
                _recorder << static_cast<uint8_t>(i);
                Writer writer;
                BinarySerDes::Serialize(visitor, writer);
                _recorder << writer.Reset();
            }
        }
        _recorder << uint16_t{0};
        _recorder.Flush_();
    }

    DELETE_COPY_AND_MOVE(TransactionT);

    TObj& Obj() { return _ref; }

    template <typename TEnum, typename TVal> void OnStructFieldChangeRequested(TEnum field, TVal const& /*curval*/, TVal const& /*newval*/)
    {
        _fieldtracker.set(static_cast<uint8_t>(field));
    }

    template <typename TEnum, typename TVal> void OnListMutation_add(TEnum /*field*/, TVal const& /*val*/) { TODO(); }
    template <typename TEnum, typename TVal> void OnListMutation_remove(TEnum /*field*/, TVal const& /*val*/) { TODO(); }

    template <typename TEnum, typename TNestedType> Stencil::Transaction<TNestedType> CreateNestedTransaction(TEnum field, TNestedType& obj)
    {
        _recorder << static_cast<uint8_t>(field);
        return Stencil::Transaction<TNestedType>(obj, _recorder);
    }

    std::reference_wrapper<TObj>        _ref;
    TransactionRecorder&                _recorder;
    std::bitset<TObj::FieldCount() + 1> _fieldtracker;
};

}    // namespace Stencil
template <typename T, typename _Ts> struct Stencil::Transaction : Stencil::TransactionT<T>
{
    Transaction(T& obj, TransactionRecorder& rec) : Stencil::TransactionT<T>(obj, rec) {}
    DELETE_COPY_AND_MOVE(Transaction);

    friend struct TransactionRecorder;
};

template <typename T> struct Stencil::Transaction<T, std::enable_if_t<Value::Supported<T>::value>> : Stencil::TransactionT<T>
{
    Transaction(T& obj, TransactionRecorder& rec) : Stencil::TransactionT<T>(obj, rec) {}
    DELETE_COPY_AND_MOVE(Transaction);
};
