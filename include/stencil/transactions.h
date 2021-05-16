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
        if (!ReflectionBase::AreEqual(curval, newval))
        {
            _settracker.set(static_cast<uint8_t>(field));
        }
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

    std::reference_wrapper<TObj> _ref;
    TransactionRecorder&         _recorder;
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
