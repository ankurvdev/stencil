#pragma once
#include "protocol_json.h"
#include "transactions.h"

#include <stdexcept>
#include <string_view>
#include <type_traits>

namespace Stencil
{

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

    template <typename T> struct _StructApplicator
    {
        [[noreturn]] static void Apply(T& /* txn */,
                                       std::string_view const& /* fieldname */,
                                       uint8_t /* mutator */,
                                       std::string_view const& /* mutatordata */,
                                       std::string_view const& /* rhs */)
        {
            throw std::logic_error("Invalid");
        }
    };

    template <typename T> struct _ListApplicator
    {
        [[noreturn]] static void Add(T& /* txn */, size_t /* listindex */, std::string_view const& /* rhs */)
        {
            throw std::logic_error("Invalid");
        }

        [[noreturn]] static void Remove(T& /* txn */, size_t /* listindex */) { throw std::logic_error("Invalid"); }
    };

    template <ConceptTransactionForIterable T> struct _ListApplicator<T>
    {
        static void Add(T& txn, uint32_t /* listindex */, std::string_view const& rhs)
        {
            using ElemType = typename Stencil::TypeTraitsForIterable<typename TransactionTraits<T>::ElemType>::ElementType;
            ElemType obj;
            Stencil::SerDes<decltype(obj), ProtocolJsonVal>::Read(obj, rhs);
            txn.Add(std::move(obj));
        }
        static void Remove(T& txn, uint32_t listindex) { txn.Remove(listindex); }
    };

    template <ConceptTransaction T> static void _ListAdd(T& txn, uint32_t listindex, std::string_view const& rhs)
    {
        _ListApplicator<T>::Add(txn, listindex, rhs);
    }

    template <ConceptTransaction T> static void _ListRemove(T& txn, uint32_t listindex) { _ListApplicator<T>::Remove(txn, listindex); }

    template <ConceptTransactionForIndexable T> struct _StructApplicator<T>
    {
        static void
        Apply(T& txn, std::string_view const& fieldname, uint8_t mutator, std::string_view const& mutatordata, std::string_view const& rhs)
        {
            using TKey = typename Stencil::TypeTraitsForIndexable<typename TransactionTraits<T>::ElemType>::Key;
            TKey key   = Stencil::Deserialize<TKey, ProtocolString>(fieldname);

            if (mutator == 0)    // Set
            {
                txn.Assign(key, [&](auto& subtxn) {
                    using ElemType = typename Stencil::TransactionTraits<std::remove_cvref_t<decltype(subtxn)>>::ElemType;
                    ElemType rhsval{};
                    Stencil::SerDesRead<ProtocolJsonVal>(rhsval, rhs);
                    subtxn.Assign(std::move(rhsval));
                });

                // txn.Visit(fieldname, [&](auto fieldType, auto& subtxn) { _ApplyJson(subtxn , fieldType, rhs); });
            }
            else if (mutator == 1)    // List Add
            {
                txn.Edit(key, [&]([[maybe_unused]] auto& subtxn) {
                    using TSubTxn = std::remove_cvref_t<decltype(subtxn)>;
                    if constexpr (Stencil::ConceptTransactionForIterable<TSubTxn>)
                    {
                        auto subkey = Stencil::Deserialize<uint32_t, Stencil::ProtocolString>(mutatordata);
                        _ListAdd(subtxn, subkey, rhs);
                    }
                    else
                    {
                        throw std::logic_error("Invalid operation");
                    }
                });
            }
            else if (mutator == 2)    // List remove
            {
                txn.Edit(key, [&]([[maybe_unused]] auto& subtxn) {
                    using TSubTxn = std::remove_cvref_t<decltype(subtxn)>;

                    if constexpr (Stencil::ConceptTransactionForIndexable<TSubTxn>)
                    {
                        using TSubKey = typename Stencil::TypeTraitsForIndexable<typename TransactionTraits<TSubTxn>::ElemType>::Key;
                        auto subkey   = Stencil::Deserialize<TSubKey, Stencil::ProtocolString>(mutatordata);
                        subtxn.Remove(subkey);
                    }
                    else if constexpr (Stencil::ConceptTransactionForIterable<TSubTxn>)
                    {
                        auto subkey = Stencil::Deserialize<uint32_t, Stencil::ProtocolString>(mutatordata);
                        subtxn.Remove(subkey);
                    }
                    else
                    {
                        throw std::logic_error("Invalid operation");
                    }
                });
            }
            else
            {
                throw std::logic_error("Unknown Mutator");
            }
        }
    };

    template <ConceptTransaction T>
    static void _ApplyOnStruct(T&                      txn,
                               std::string_view const& fieldname,
                               uint8_t                 mutator,
                               std::string_view const& mutatordata,
                               std::string_view const& rhs)
    {
        _StructApplicator<T>::Apply(txn, fieldname, mutator, mutatordata, rhs);
    }

    template <ConceptTransaction T> static size_t _Apply(TokenIterator& it, T& txn)
    {
        if (!(it.delimiter == ':' || it.delimiter == ' ' || it.delimiter == '='))
        {
            if constexpr (Stencil::ConceptTransactionForIndexable<T>)
            {
                auto keystr = it.token;
                using TKey  = typename Stencil::TypeTraitsForIndexable<typename TransactionTraits<T>::ElemType>::Key;
                TKey key    = Stencil::Deserialize<TKey, ProtocolString>(keystr);
                ++it;
                size_t retval = 0;
                SUPPRESS_WARNINGS_START
                SUPPRESS_MSVC_WARNING(4702) /*Unreachable code*/
                // Sometime its a bad visit and we throw exceptions for error
                txn.Edit(key, [&](auto& args) { retval = _Apply(it, args); });
                SUPPRESS_WARNINGS_END
                return retval;
            }
            else if constexpr (Stencil::ConceptTransactionForIterable<T>)
            {
                auto     keystr = it.token;
                uint32_t key    = Stencil::Deserialize<uint32_t, ProtocolString>(keystr);
                ++it;
                size_t retval = 0;
                // Sometime its a bad visit and we throw exceptions for error
                txn.Edit(key, [&](auto& args) { retval = _Apply(it, args); });
                return retval;
            }

            else
            {
                throw std::logic_error("Unable to indirect into a non-indexable");
            }
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
            else if (mutatorname == "remove") { mutator = 2; }
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
        if constexpr (Stencil::ConceptTransactionForIndexable<T>) { _ApplyOnStruct(txn, name, mutator, mutatordata, rhs); }
        else if constexpr (Stencil::ConceptTransactionForIterable<T>)
        {
            uint32_t index = Stencil::Deserialize<uint32_t, ProtocolString>(name);
            if (mutator == 0)
            {
                txn.Edit(index, [&](auto& args) { _Apply(it, args); });
            }
            else if (mutator == 1) { _ListApplicator<T>::Add(txn, index, rhs); }
            else if (mutator == 2) { _ListApplicator<T>::Remove(txn, index); }
            else
            {
                throw std::logic_error("Invalid List operation");
            }
        }
        return i + 1;
    }

    public:
    template <ConceptTransaction T> static auto Apply(T& txn, std::string_view const& txndata)
    {
        size_t startIndex = 0;
        do
        {
            TokenIterator it(txndata.substr(startIndex));
            startIndex += _Apply(it, txn);
        } while (startIndex < txndata.size());
    }

    template <ConceptTransactionView T>
    static std::ostream& _DeserializeTo(T const& txn, std::ostream& ostr, std::vector<std::string>& stack)
    {
        if constexpr (ConceptTransactionViewForIterable<T>)
        {
            txn.VisitChanges([&](size_t const& index, uint8_t const& mutator, auto const& /* mutatordata */, auto const& subtxn) {
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
                    ostr << "." << index << " = " << Stencil::Json::Stringify(subtxn.Elem()) << ";";
                }
                else if (mutator == 1) { ostr << ":add[" << index << "] = " << Stencil::Json::Stringify(subtxn.Elem()) << ";"; }
                else if (mutator == 2) { ostr << ":remove[" << index << "] = {};"; }
                else
                {
                    throw std::logic_error("Unknown mutator");
                }
            });
            return ostr;
        }

        else if constexpr (ConceptTransactionViewForIndexable<T>)
        {
            txn.VisitChanges([&](auto const& key, auto const& mutator, auto const& /* mutatordata */, auto const& subtxn) {
                auto name = Stencil::Serialize<Stencil::ProtocolString>(key).str();
                if (mutator == 0)
                {
                    for (auto& s : stack) { ostr << s << "."; }
                    ostr << name << " = " << Stencil::Json::Stringify(subtxn.Elem()) << ";";
                }
                else if (mutator == 2)
                {
                    bool first = true;
                    for (auto& s : stack)
                    {
                        if (!first) ostr << ".";
                        first = false;
                        ostr << s;
                    }
                    ostr << ":remove[" << name << "] = {};";
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
            return ostr;
        }
        else
        {
            throw std::logic_error("Unsupported transaction type");
        }
    }

    template <ConceptTransactionView T> static std::string Deserialize(T const& txn)
    {
        std::stringstream        sstr;
        std::vector<std::string> stack;
        _DeserializeTo(txn, sstr, stack);
        return sstr.str();
    }
    template <ConceptTransaction T> static std::string Deserialize(T const& txn) { return Deserialize(static_cast<T::View>(txn)); }
};
}    // namespace Stencil
