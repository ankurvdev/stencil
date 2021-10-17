#pragma once
#include "optionalprops.h"
#include "serdes.h"
#include "visitor.h"

#include <algorithm>
#include <array>
#include <bitset>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <span>

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

struct NullTransactionRecorder : TransactionRecorder
{
    NullTransactionRecorder() = default;
    virtual void Write_(std::span<uint8_t const> /*buffer*/) override {}
};

struct MemTransactionRecorder : Stencil::TransactionRecorder
{
    MemTransactionRecorder() = default;
    CLASS_DELETE_COPY_AND_MOVE(MemTransactionRecorder);
    virtual void Write_(std::span<uint8_t const> buffer) override
    {
        std::copy(buffer.begin(), buffer.end(), std::back_inserter(_recording));
    }

    std::vector<uint8_t> _recording;
};

template <typename TObj, typename _Ts = void> struct TransactionT
{
    TransactionT(TObj& /*obj*/, TransactionRecorder& /*rec*/) {}

    template <typename... TArgs> void Add(TArgs&...) { throw std::logic_error("Add Not supported on Transaction"); }
    template <typename... TArgs> void Remove(TArgs&...) { throw std::logic_error("Remove Not supported on Transaction"); }

    template <typename TSerDes, typename... TArgs> void Set(TArgs&...) { throw std::logic_error("Set Not supported on Transaction"); }

    template <typename... TArgs> void Visit(TArgs&...) { throw std::logic_error("Visit Not supported on Transaction"); }
};

template <typename TObj>
struct TransactionT<TObj, std::enable_if_t<ReflectionBase::TypeTraits<TObj&>::Type() == ReflectionBase::DataType::Object>>
{
    TransactionT(TObj& obj, TransactionRecorder& rec) : _ref(std::ref(obj)), _recorder(rec) {}
    ~TransactionT()
    {
        if (_settracker.any())
        {
            for (size_t i = 0; i < _settracker.size(); i++)
            {
                if (!_settracker.test(i)) continue;
                Visitor<TObj> visitor(_ref);
                visitor.Select(Value{i - 1});
                _recorder << static_cast<uint8_t>(i);
                _recorder << uint8_t{0};    // set
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

    template <typename TEnum> bool IsFieldChanged(TEnum field) const
    {
        return _settracker.test(static_cast<uint8_t>(field)) || _edittracker.test(static_cast<uint8_t>(field));
    }

    size_t CountFieldsChanged() const { return (_settracker | _edittracker).count(); }

    template <typename TEnum, typename TVal> void OnStructFieldChangeRequested(TEnum field, TVal const& curval, TVal const& newval)
    {
        if constexpr (std::is_base_of_v<Stencil::OptionalPropsT<TObj>, TObj>)
        {
            if (!Obj().IsValid(field))
            {
                _settracker.set(static_cast<uint8_t>(field));
                return;
            }
        }
        if (!ReflectionBase::AreEqual(curval, newval)) { _settracker.set(static_cast<uint8_t>(field)); }
    }

    template <typename TEnum, typename TFieldType, typename TVal> void OnMutation_add(TEnum field, TFieldType const& obj, TVal const& val)
    {
        _edittracker.set(static_cast<uint8_t>(field));
        _recorder << static_cast<uint8_t>(field);
        _recorder << uint8_t{1};    // edit
        _recorder << uint8_t{1};    // mutation add
        _recorder << static_cast<uint32_t>(obj.size());

        Writer              writer;
        Visitor<TVal const> visitor(val);
        BinarySerDes::Serialize(visitor, writer);
        _recorder << writer.Reset();
    }

    template <typename TEnum, typename TFieldType, typename TVal>
    void OnMutation_remove(TEnum field, TFieldType const& /*obj*/, TVal const& val)
    {
        _edittracker.set(static_cast<uint8_t>(field));

        _recorder << static_cast<uint8_t>(field);
        _recorder << uint8_t{1};    // edit
        _recorder << uint8_t{2};    // mutation remove
        _recorder << static_cast<uint32_t>(val);
    }

    template <typename TEnum, typename TFieldType, typename TVal>
    void OnMutation_edit(TEnum field, TFieldType const& /*obj*/, TVal const& index)
    {
        _edittracker.set(static_cast<uint8_t>(field));

        _recorder << static_cast<uint8_t>(field);
        _recorder << uint8_t{1};    // edit
        _recorder << uint8_t{3};    // mutation edit
        _recorder << static_cast<uint32_t>(index);
    }

    template <typename... TArgs> void Add(TArgs&...) { throw std::logic_error("Add Not supported on Transaction"); }
    template <typename... TArgs> void Remove(TArgs&...) { throw std::logic_error("Remove Not supported on Transaction"); }

    template <typename TSerDes, typename TEnum, typename TVal> void Set(TEnum const& field, TVal&& val)
    {
        _edittracker.set(static_cast<uint8_t>(field));
        Visitor<TObj> visitor(Obj());
        visitor.Select(static_cast<uint8_t>((static_cast<unsigned>(field) - 1)));
        TSerDes::Deserialize(visitor, val);
    }
    template <typename TSerDes, typename TVal> void Set(std::string_view const& fieldName, TVal&& val)
    {
        uint8_t       index = 1;
        Visitor<TObj> visitor(Obj());
        visitor.VisitAll([&](auto& name, auto& /*obj*/) {
            if (name == fieldName) { Set<TSerDes>(static_cast<TObj::FieldIndex>(index), std::forward<TVal>(val)); }
            index++;
        });
    }

    template <typename... TArgs> void Visit(TArgs&...) { throw std::logic_error("Visit Not supported on Transaction"); }

    std::reference_wrapper<TObj>        _ref;
    TransactionRecorder&                _recorder;
    std::bitset<TObj::FieldCount() + 1> _settracker;
    std::bitset<TObj::FieldCount() + 1> _edittracker;
};

template <typename TObj>
struct TransactionT<TObj, std::enable_if_t<ReflectionBase::TypeTraits<TObj&>::Type() == ReflectionBase::DataType::List>>
{
    TransactionT(TObj& obj, TransactionRecorder& rec) : _ref(std::ref(obj)), _recorder(rec) {}
    ~TransactionT() {}

    DELETE_COPY_AND_MOVE(TransactionT);

    TObj& Obj() { return _ref; }

    auto GetSubObjectTracker(TObj& obj, size_t index)
    {
        auto& subobj = obj[index];
        //_recorder << static_cast<uint32_t>(index);
        return Stencil::Transaction<std::remove_reference_t<decltype(subobj)>>(subobj, _recorder);
    }

    template <typename... TArgs> void Add(TArgs&...) { throw std::logic_error("Add Not supported on Transaction"); }
    template <typename... TArgs> void Remove(TArgs&...) { throw std::logic_error("Remove Not supported on Transaction"); }

    template <typename TSerDes, typename... TArgs> void Set(TArgs&...) { throw std::logic_error("Set Not supported on Transaction"); }
    template <typename TLambda> void                    Visit(std::string_view const& /* index */, TLambda&& /* lambda */)
    {
        throw std::logic_error("String index visit on Transaction");
    }
    template <typename TLambda> void Visit(size_t index, TLambda&& lambda) { lambda(GetSubObjectTracker(index)); }

    std::reference_wrapper<TObj> _ref;
    TransactionRecorder&         _recorder;
};

struct BinaryTransactionDataReader
{};

struct StringTransactionDataReader
{
    private:
    struct TokenIterator
    {
        std::string_view data;
        std::string_view token;
        char             delimiter  = 0;
        size_t           startIndex = 0;

        TokenIterator() = default;
        TokenIterator(std::string_view const& dataIn) : data(dataIn) { _Init(); }
        bool           operator!=(TokenIterator const& rhs) const { return data != rhs.data || startIndex != rhs.startIndex; }
        TokenIterator& operator=(TokenIterator const& rhs)
        {
            data       = rhs.data;
            token      = rhs.token;
            delimiter  = rhs.delimiter;
            startIndex = rhs.startIndex;
            return *this;
        }
        void _Init()
        {
            for (size_t i = startIndex; i < data.size(); i++)
            {
                if (data[i] == '.' || data[i] == '[' || data[i] == ' ' || data[i] == '=' || data[i] == ':')
                {
                    token     = std::string_view(data.begin() + startIndex, data.begin() + i);
                    delimiter = data[i];
                    return;
                }
            }
            throw std::logic_error("Invalid Format");
        }
        TokenIterator& operator++()
        {
            if (delimiter == 0)
            {
                // Its either end or beginning or empty data
                if (startIndex != 0)
                {
                    *this = TokenIterator();
                    // Its the end
                    return *this;
                }
            }
            startIndex = startIndex + token.size() + 1;    // for delimiter;
            _Init();
            return *this;
        }
    };

    template <typename T, typename TLambda> static size_t _Replay(T& txn, TokenIterator& it, TLambda&& lambda)
    {
        if (it.delimiter == ':' || it.delimiter == ' ' || it.delimiter == '=')
        {
            auto name = it.token;
            // Reached the end
            uint8_t          mutator = 0;
            std::string_view mutatordata;
            if (it.delimiter == ':')
            {
                ++it;
                auto mutatorname = it.token;
                if (it.delimiter == '[')
                {
                    size_t i = it.startIndex;
                    while (it.data[i] != ']' && i < it.data.size()) i++;
                    if (i == it.data.size()) throw std::logic_error("Invalid Format. Cannot find end ']'");
                    mutatordata   = std::string_view(it.data.begin() + it.startIndex, it.data.begin() + i);
                    it.startIndex = i;
                    it.token      = {};
                }
                if (mutatorname == "add") { mutator = 1; }
                else if (mutatorname == "remove")
                {
                    mutator = 2;
                }
                else
                {
                    throw std::logic_error("Invalid Mutator");
                }
            }
            size_t i = it.startIndex + it.token.size() + 1;
            while (it.data[i] == ' ' && i < it.data.size()) i++;
            if (i == it.data.size() || it.data[i] != '=') throw std::logic_error("Invalid Format. Expected '='");
            ++i;    // skip =
            while (it.data[i] == ' ' && i < it.data.size()) i++;
            if (i == it.data.size()) throw std::logic_error("Invalid Format. Cannot find rhs");
            size_t rhsS = i;
            while (it.data[i] != ';' && i < it.data.size()) i++;
            auto rhs = std::string_view(it.data.begin() + rhsS, it.data.begin() + i);
            lambda(txn, name, mutator, mutatordata, rhs);
            return it.data.size() + 1;
        }
        else
        {
            size_t retval;
            auto   name = it.token;
            ++it;
            txn.Visit(name, [&](auto& subtxn) { retval = _Replay(subtxn, it, std::move(lambda)); });
            return retval;
        }
    }

    public:
    template <typename T> static void Replay(Transaction<T>& txn, std::string_view const& txndata)
    {
        size_t startIndex = 0;
        do {
            TokenIterator it(txndata.substr(startIndex));
            startIndex += _Replay(
                txn,
                it,
                [&](auto& subtxn, std::string_view fieldname, uint8_t mutator, std::string_view mutatordata, std::string_view rhs) {
                    if (mutator == 0)    // Set
                    {
                        subtxn.template Set<JsonSerDes>(fieldname, rhs);
                    }
                    else if (mutator == 1)    // List Add
                    {
                        throw std::logic_error("Not implemented");

                        // subtxn.Add(mutatordata).template Deserialize<JsonSerDes>(rhs);
                    }
                    else if (mutator == 2)    // List remove
                    {
                        subtxn.Remove(mutatordata);
                    }
                    else
                    {
                        throw std::logic_error("Unknown Mutator");
                    }
                });
        } while (startIndex < txndata.size());
    }
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
    template <typename... TArgs> void Visit(TArgs&&...) const { throw std::logic_error("Visiting Values does not make sense"); }
    DELETE_COPY_AND_MOVE(Transaction);
};