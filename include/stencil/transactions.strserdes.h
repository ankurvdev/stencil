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
        static void Apply(T& /* txn */,
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
        static void Add(T& /* txn */, size_t /* listindex */, std::string_view const& /* rhs */) { throw std::logic_error("Invalid"); }

        static void Remove(T& /* txn */, size_t /* listindex */) { throw std::logic_error("Invalid"); }
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
            using TKey = Stencil::TypeTraitsForIndexable<typename TransactionTraits<T>::ElemType>::Key;
            TKey key   = Stencil::Deserialize<TKey, ProtocolString>(fieldname);

            if (mutator == 0)    // Set
            {
                txn.Edit(key, [&](auto& subtxn) { Stencil::SerDesRead<ProtocolJsonVal>(subtxn, rhs); });

                // txn.Visit(fieldname, [&](auto fieldType, auto& subtxn) { _ApplyJson(subtxn , fieldType, rhs); });
            }
            else if (mutator == 1)    // List Add
            {
                auto listval = Stencil::Deserialize<uint32_t, Stencil::ProtocolString>(mutatordata);
                txn.Edit(key, [&](auto& subtxn) { _ListAdd(subtxn, listval, rhs); });
            }
            else if (mutator == 2)    // List remove
            {
                auto listval = Stencil::Deserialize<uint32_t, Stencil::ProtocolString>(mutatordata);
                txn.Edit(key, [&](auto& subtxn) { _ListRemove(subtxn, listval); });
            }
            else { throw std::logic_error("Unknown Mutator"); }
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
                using TKey  = Stencil::TypeTraitsForIndexable<typename TransactionTraits<T>::ElemType>::Key;
                TKey key    = Stencil::Deserialize<TKey, ProtocolString>(keystr);
                ++it;
                size_t retval = 0;
#pragma warning(push, 3)
#pragma warning(disable : 4702)    // unreachable code
                                   // Sometime its a bad visit and we throw exceptions for error
                txn.Edit(key, [&](auto& args) { retval = _Apply(it, args); });
#pragma warnin(pop)
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

            else { throw std::logic_error("Unable to indirect into a non-indexable"); }
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
            else { throw std::logic_error("Invalid Mutator"); }
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
    template <ConceptTransaction T> static auto Apply(T& txn, std::string_view const& txndata)
    {
        size_t startIndex = 0;
        do {
            TokenIterator it(txndata.substr(startIndex));
            startIndex += _Apply(it, txn);
        } while (startIndex < txndata.size());
    }

    template <ConceptTransaction T> static std::ostream& _DeserializeTo(T& txn, std::ostream& ostr, std::vector<std::string>& stack)
    {
        if constexpr (ConceptTransactionForIterable<T>)
        {
            txn.VisitChanges([&](size_t const& index, uint8_t const& mutator, auto const& /* mutatordata */, auto& subtxn) {
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
                else { throw std::logic_error("Unknown mutator"); }
            });
        }

        if constexpr (ConceptTransactionForIndexable<T>)
        {
            txn.VisitChanges([&](auto const& key, auto const& mutator, auto const& /* mutatordata */, auto& subtxn) {
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
                else { throw std::logic_error("Unknown mutator"); }
            });
        }
        return ostr;
    }

    template <ConceptTransaction T> static std::string Deserialize(T& txn)
    {
        std::stringstream        sstr;
        std::vector<std::string> stack;
        _DeserializeTo(txn, sstr, stack);
        return sstr.str();
    }
};
}    // namespace Stencil
