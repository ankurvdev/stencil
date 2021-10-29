#pragma once
#include "jsonserdes.h"
#include "optionalprops.h"
#include "visitor.h"

#include <algorithm>
#include <array>
#include <bitset>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <span>
#include <unordered_map>

namespace Stencil
{
/*
 */

template <typename TObj, typename _Ts = void> struct Transaction;

template <typename TObj, typename _Ts = void> struct TransactionT
{
    TransactionT(TObj& /*obj*/) {}

    template <typename TLambda> auto Visit(size_t /* fieldIndex */, TLambda&& /* lambda */)
    {
        throw std::logic_error("Visit Not supported on Transaction");
    }

    template <typename TLambda> auto Visit(std::string_view const& /* fieldName */, TLambda&& /* lambda */)
    {
        throw std::logic_error("Visit Not supported on Transaction");
    }

    template <typename TLambda> void VisitAll(TLambda&& /* lambda */) { throw std::logic_error("Visit Not supported on Transaction"); }

    TObj& Obj() { throw std::logic_error("Obj Not supported on Transaction"); }

#if 0
    unsigned GetFieldTypeByName(std::string_view const& /* name */) { throw std::logic_error("Add Not supported on Transaction"); }
    template <typename TSerDes, typename... TArgs> void Add(TArgs&&...) { throw std::logic_error("Add Not supported on Transaction"); }
    template <typename... TArgs> void Remove(TArgs&&...) { throw std::logic_error("Remove Not supported on Transaction"); }

    template <typename TSerDes, typename... TArgs> void Set(TArgs&&...) { throw std::logic_error("Set Not supported on Transaction"); }
#endif
};

template <typename TObj>
struct TransactionT<TObj, std::enable_if_t<ReflectionBase::TypeTraits<TObj&>::Type() == ReflectionBase::DataType::Object>>
{
    TransactionT(TObj& obj) : _ref(std::ref(obj)) {}
#if 0
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
#endif
    DELETE_COPY_AND_MOVE(TransactionT);

    TObj& Obj() { return _ref; }

    private:
    using TTransaction = Transaction<TObj>;

    auto& _GetTransactionObj()
    {
        static_assert(std::is_base_of_v<TransactionT<TObj>, TTransaction>, "Transaction<TObj> must derive TransactionT<TObj>");
        return *static_cast<TTransaction*>(this);
    }

    public:
    template <typename TEnum> bool IsFieldAssigned(TEnum field) const { return _assigntracker.test(static_cast<uint8_t>(field)); }
    template <typename TEnum> bool IsFieldEdited(TEnum field) const { return _edittracker.test(static_cast<uint8_t>(field)); }
    template <typename TEnum> bool IsFieldChanged(TEnum field) const { return IsFieldAssigned(field) || IsFieldEdited(field); }

    size_t CountFieldsChanged() const { return (_assigntracker | _edittracker).count(); }

    template <typename TLambda> void VisitChanges(TLambda&& lambda)
    {
        _GetTransactionObj().VisitAll([&](auto const& name, auto const& type, auto& subtxn, auto& obj) {
            if (IsFieldAssigned(type)) lambda(name, type, 0, 0, subtxn, obj);
            if (IsFieldEdited(type)) lambda(name, type, 3, 0, subtxn, obj);
        });
    }
    template <typename TEnum> void MarkFieldAssigned_(TEnum field) { _assigntracker.set(static_cast<uint8_t>(field)); }

    protected:
    // template <typename TEnum> void MarkFieldAssigned_(TEnum field) { _assigntracker.set(static_cast<uint8_t>(field)); }

    template <typename TEnum, typename TVal> void MarkFieldAssigned_(TEnum field, TVal const& curval, TVal const& newval)
    {
        if constexpr (std::is_base_of_v<Stencil::OptionalPropsT<TObj>, TObj>)
        {
            if (!Obj().IsValid(field))
            {
                _assigntracker.set(static_cast<uint8_t>(field));
                return;
            }
        }
        if (!ReflectionBase::AreEqual(curval, newval)) { _assigntracker.set(static_cast<uint8_t>(field)); }
    }

    template <typename TEnum> void MarkFieldEdited_(TEnum field) { _edittracker.set(static_cast<uint8_t>(field)); }

#if 0
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
    }

    template <typename TSerDes, typename TEnum, typename TVal> void Add(TEnum const& field, size_t index, TVal&& val)
    {
        _edittracker.set(static_cast<uint8_t>(field));
        Visitor<TObj> visitor(Obj());
        visitor.Select(field);
        visitor.Select(index);
        TSerDes::Deserialize(visitor, val);
    }

    template <typename TEnum> void Remove(TEnum const& field, size_t index)
    {
        _edittracker.set(static_cast<uint8_t>(field));
        Visitor<TObj> visitor(Obj());
        visitor.Select(field);
        auto& upcasted = *static_cast<Transaction<TObj>*>(this);
        upcasted.Visit(field, [&](auto& subtxn) { subtxn.Remove(index); });
    }

    template <typename TSerDes, typename TEnum, typename TVal> void Set(TEnum const& field, TVal&& val)
    {
        _settracker.set(static_cast<uint8_t>(field));
        Visitor<TObj> visitor(Obj());
        visitor.Select(static_cast<uint8_t>((static_cast<unsigned>(field) - 1)));
        TSerDes::Deserialize(visitor, val);
    }
#endif

    std::reference_wrapper<TObj>        _ref;
    std::bitset<TObj::FieldCount() + 1> _assigntracker;
    std::bitset<TObj::FieldCount() + 1> _edittracker;
};

template <typename TObj>
struct TransactionT<TObj, std::enable_if_t<ReflectionBase::TypeTraits<TObj&>::Type() == ReflectionBase::DataType::List>>
{
    using ListObjType = typename ReflectionBase::TypeTraits<TObj&>::ListObjType;

    TransactionT(TObj& obj) : _ref(std::ref(obj)) {}
    ~TransactionT() {}

    DELETE_COPY_AND_MOVE(TransactionT);

    TObj& Obj() { return _ref; }

    template <typename TArg> void RecordMutation_add_(TArg&) { _changes.push_back({1u, Obj().size()}); }

    void RecordMutation_remove_(size_t index) { _changes.push_back({2u, index}); }
    void RecordMutation_edit_(size_t index) { _changes.push_back({3u, index}); }
    void RecordMutation_assign_(size_t index) { _changes.push_back({0u, index}); }

    void add(ListObjType&& obj)
    {
        RecordMutation_add_(obj);
        Mutators<TObj>::add(Obj(), std::move(obj));
    }

    void remove(size_t index)
    {
        RecordMutation_remove_(index);
        Mutators<TObj>::remove(Obj(), index);
    }

    template <typename TLambda> auto Visit(size_t fieldIndex, TLambda&& lambda)
    {
        Visitor<TObj> visitor(_ref.get());
        visitor.Visit(fieldIndex, [&](auto index, auto& obj) {
            RecordMutation_edit_(index);
            auto subtxnptr = std::make_unique<Transaction<ListObjType>>(obj);
            lambda(index, *subtxnptr);
            // TODO : only do it if there was a change;
            _edited.insert(std::make_pair(index, std::move(subtxnptr)));
        });
    }

    template <typename TLambda> auto Visit(std::string_view const& fieldName, TLambda&& lambda)
    {
        return Visit(Value(fieldName).convert<size_t>(), std::forward<TLambda>(lambda));
    }

    template <typename TLambda> void VisitAll(TLambda&& /* lambda */) { throw std::logic_error("Visit Not supported on Transaction"); }
    template <typename TLambda> void VisitChanges(TLambda&& lambda)
    {
        if (_changes.size() == 0) return;
        for (auto& c : _changes)
        {
            // TODO : Fix me. This is horrible
            auto& obj = Obj()[c.index];
            auto  it  = _edited.find(c.index);
            if (it == _edited.end())
            {
                it = _edited.insert(std::make_pair(c.index, std::make_unique<Transaction<ListObjType>>(obj))).first;
            }
            lambda(nullptr, nullptr, c.mutationtype, c.index, *it->second, obj);
        }
    }

    struct _Record
    {
        uint8_t mutationtype;
        size_t  index;
    };

    std::unordered_map<size_t, std::unique_ptr<Transaction<ListObjType>>> _edited;

    std::vector<_Record>         _changes;
    std::reference_wrapper<TObj> _ref;
};

}    // namespace Stencil

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
                //                auto changedFieldType = static_cast<typename TObj::FieldIndex>(changedFieldIndex);
                visitor.Select(static_cast<uint8_t>(changedFieldIndex - 1));
                if (action == 0)    // Set
                {
                    BinarySerDes::Deserialize(visitor, reader);
                }
                else if (action == 1)    // Edit
                {
                    throw std::logic_error("Unknown action");

                    //                    ctx.Visit(changedFieldType, [&](auto& subctx) { _Apply(reader, visitor, subctx); });
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

struct TransactionRecorder
{
    virtual ~TransactionRecorder() = default;

    template <typename TObj> Transaction<TObj> Start(TObj& obj) { return Transaction<TObj>(obj); }

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

struct BinaryTransactionDataReader
{};
struct StringTransactionSerDes
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
                    token     = data.substr(startIndex, i - startIndex);
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

    template <typename T, typename F = void> struct _StructApplicator
    {
        static void Apply(Transaction<T>& /* txn */,
                          std::string_view const& /* fieldname */,
                          uint8_t /* mutator */,
                          std::string_view const& /* mutatordata */,
                          std::string_view const& /* rhs */)
        {
            throw std::logic_error("Invalid");
        }
    };

    template <typename T, typename F = void> struct _ListApplicator
    {
        static void Add(Transaction<T>& /* txn */, size_t /* listindex */, std::string_view const& /* rhs */)
        {
            throw std::logic_error("Invalid");
        }

        static void Remove(Transaction<T>& /* txn */, size_t /* listindex */) { throw std::logic_error("Invalid"); }
    };

    template <typename T>
    struct _ListApplicator<T, std::enable_if_t<ReflectionBase::TypeTraits<T&>::Type() == ReflectionBase::DataType::List>>
    {
        static void Add(Transaction<T>& txn, size_t /* listindex */, std::string_view const& rhs)
        {
            typename Stencil::Mutators<T>::ListObj obj;

            Visitor<decltype(obj)> visitor(obj);
            JsonSerDes::Deserialize(visitor, rhs);
            txn.add(std::move(obj));
        }
        static void Remove(Transaction<T>& txn, size_t listindex) { txn.remove(listindex); }
    };

    template <typename TObj> static void _ListAdd(Transaction<TObj>& txn, size_t listindex, std::string_view const& rhs)
    {
        _ListApplicator<TObj>::Add(txn, listindex, rhs);
    }

    template <typename TObj> static void _ListRemove(Transaction<TObj>& txn, size_t listindex)
    {
        _ListApplicator<TObj>::Remove(txn, listindex);
    }

    template <typename T>
    struct _StructApplicator<T, std::enable_if_t<ReflectionBase::TypeTraits<T&>::Type() == ReflectionBase::DataType::Object>>
    {
        static void Apply(Transaction<T>&         txn,
                          std::string_view const& fieldname,
                          uint8_t                 mutator,
                          std::string_view const& mutatordata,
                          std::string_view const& rhs)
        {
            if (mutator == 0)    // Set
            {
                txn.Visit(fieldname, [&](auto fieldType, auto& /* subtxn */) {
                    Visitor<T> visitor(txn.Obj());
                    visitor.Select(fieldname);
                    txn.MarkFieldAssigned_(fieldType);
                    JsonSerDes::Deserialize(visitor, rhs);
                });

                // txn.Visit(fieldname, [&](auto fieldType, auto& subtxn) { _ApplyJson(subtxn , fieldType, rhs); });
            }
            else if (mutator == 1)    // List Add
            {
                txn.Visit(fieldname,
                          [&](auto /* fieldType */, auto& subtxn) { _ListAdd(subtxn, Value(mutatordata).convert<size_t>(), rhs); });
            }
            else if (mutator == 2)    // List remove
            {
                txn.Visit(fieldname,
                          [&](auto /* fieldType */, auto& subtxn) { _ListRemove(subtxn, Value(mutatordata).convert<size_t>()); });
            }
            else
            {
                throw std::logic_error("Unknown Mutator");
            }
        }
    };

    template <typename T>
    static void _ApplyOnStruct(Transaction<T>&         txn,
                               std::string_view const& fieldname,
                               uint8_t                 mutator,
                               std::string_view const& mutatordata,
                               std::string_view const& rhs)
    {
        _StructApplicator<T>::Apply(txn, fieldname, mutator, mutatordata, rhs);
    }

    template <typename T> static size_t _Apply(TokenIterator& it, Transaction<T>& txn)
    {
        if (!(it.delimiter == ':' || it.delimiter == ' ' || it.delimiter == '='))
        {
            auto name = it.token;
            ++it;
            size_t retval;
            txn.Visit(name, [&](auto /* fieldType */, auto& args) { retval = _Apply(it, args); });
            return retval;
        }

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
                size_t s = it.startIndex + it.token.size() + 1;
                while (i < it.data.size() && it.data[i] != ']') i++;
                if (i == it.data.size()) throw std::logic_error("Invalid Format. Cannot find end ']'");
                mutatordata   = it.data.substr(s, i - s);
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
        while (i < it.data.size() && it.data[i] == ' ') i++;
        if (i == it.data.size() || it.data[i] != '=') throw std::logic_error("Invalid Format. Expected '='");
        ++i;    // skip =
        while (i < it.data.size() && it.data[i] == ' ') i++;
        if (i == it.data.size()) throw std::logic_error("Invalid Format. Cannot find rhs");
        size_t rhsS = i;
        while (i < it.data.size() && it.data[i] != ';') i++;
        auto rhs = it.data.substr(rhsS, i - rhsS);
        _ApplyOnStruct(txn, name, mutator, mutatordata, rhs);
        return i + 1;
    }

    public:
    template <typename T> static auto Apply(Transaction<T>& txn, std::string_view const& txndata)
    {
        size_t startIndex = 0;
        do {
            TokenIterator it(txndata.substr(startIndex));
            startIndex += _Apply(it, txn);
        } while (startIndex < txndata.size());
    }

    template <typename T> static std::ostream& _DeserializeTo(Transaction<T>& txn, std::ostream& ostr, std::vector<std::string>& stack)
    {
        using Traits = ReflectionBase::TypeTraits<T&>;

        if constexpr (Traits::Type() == ReflectionBase::DataType::List)
        {
            txn.VisitChanges(
                [&](auto const& /* name */, auto const& /* type */, uint8_t const& mutator, size_t const& index, auto& subtxn, auto& obj) {
                    if (mutator == 3)
                    {
                        stack.push_back(std::to_string(index));
                        _DeserializeTo(subtxn, ostr, stack);
                        stack.pop_back();
                        return;
                    }
                    bool first = true;
                    for (auto& s : stack)
                    {
                        if (!first) ostr << ".";
                        first = false;
                        ostr << s;
                    }
                    if (mutator == 0)
                    {
                        // Assign
                        ostr << "." << index << " = " << Stencil::Json::Stringify(obj) << ";";
                    }
                    else if (mutator == 1)
                    {

                        ostr << ":add[" << index << "] = " << Stencil::Json::Stringify(obj) << ";";
                    }
                    else if (mutator == 2)
                    {
                        ostr << ":remove[" << index << "] = {};";
                    }
                    else
                    {
                        throw std::logic_error("Unknown mutator");
                    }
                });
        }

        if constexpr (ReflectionBase::TypeTraits<T&>::Type() == ReflectionBase::DataType::Object)
        {
            txn.VisitChanges(
                [&](auto const& name, auto const& /* type */, auto const& mutator, auto const& /* mutatordata */, auto& subtxn, auto& obj) {
                    if (mutator == 0)
                    {
                        for (auto& s : stack) { ostr << s << "."; }
                        ostr << name << " = " << Stencil::Json::Stringify(obj) << ";";
                    }
                    else if (mutator == 3)
                    {
                        stack.push_back(name);
                        _DeserializeTo(subtxn, ostr, stack);
                        stack.pop_back();
                    }
                    else
                    {
                        throw std::logic_error("Unknown mutator");
                    }
                });
        }
        return ostr;
    }
    template <typename T> static std::string Deserialize(Transaction<T>& txn)
    {
        std::stringstream        sstr;
        std::vector<std::string> stack;
        _DeserializeTo(txn, sstr, stack);
        return sstr.str();
    }
};
}    // namespace Stencil
template <typename T, typename _Ts> struct Stencil::Transaction : Stencil::TransactionT<T>
{
    Transaction(T& obj) : Stencil::TransactionT<T>(obj) {}
    DELETE_COPY_AND_MOVE(Transaction);
};

template <typename T> struct Stencil::Transaction<T, std::enable_if_t<Value::Supported<T>::value>> : Stencil::TransactionT<T>
{
    Transaction(T& obj) : Stencil::TransactionT<T>(obj) {}
    DELETE_COPY_AND_MOVE(Transaction);
};
