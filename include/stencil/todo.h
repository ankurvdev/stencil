#pragma once
#include "base.h"
#include "enums.h"

namespace Stencil
{
#if defined TODO

template <typename T1, typename T2> struct Assign;

template <typename TInterfaceApi> struct InterfaceApiTraits
{
    using InterfaceType = typename TInterfaceApi::InterfaceType;
    struct ArgsStruct
    {
        InterfaceType* instance;
    };

    struct ReturnStruct
    {};

    static ReturnStruct Invoke(ArgsStruct& args);

    //    using Handler = void;
    //    using ReturnDataType = void;
    //    using ArgsDataType = void;
    static constexpr bool IsStatic()
    {
        static_assert(!std::is_same<TInterfaceApi, TInterfaceApi>::value, "Specialization Required for InterfaceApiTraits::IsStatic");
        return false;
    }
    static constexpr std::string_view Name()
    {
        static_assert(!std::is_same<TInterfaceApi, TInterfaceApi>::value, "Specialization Required for InterfaceApiTraits::Name");
        return std::string_view("", 0);
    }
};

template <typename T> struct InterfaceObjectTraits
{
    static constexpr std::string_view Name() { return "Unknown"; }
};

template <typename TInterface> struct InterfaceTraits
{
    static TInterface* FindInstance(const std::string_view& instanceId);
};

template <typename TDataType, typename... TArgs> void Construct(TDataType* /*ptr*/, TArgs&&... /*args*/)
{}

template <typename T, typename = void> struct Functions
{
    static bool AreEqual(T const& obj1, T const& obj2) { return std::memcmp(&obj1, &obj2, sizeof(T)) == 0; }
};

template <typename T> struct Functions<T, std::enable_if_t<std::is_base_of<::Stencil::StructMarker, T>::value>>
{
    static bool AreEqual(T const& obj1, T const& obj2) { return Stencil::TypeTraits<T>::AreEqual(obj1, obj2); }
};

template <typename T> bool AreEqual(T const& obj1, T const& obj2)
{
    return Functions<T>::AreEqual(obj1, obj2);
}
#endif
}    // namespace Stencil

#pragma warning(push, 3)
template <ConceptValue T> struct Stencil::TypeTraits<T>
{
    using Types = std::tuple<Type::Primitive>;
};

template <ConceptValue T1> struct Stencil::Assign<T1, Value>
{
    void operator()(T1& dst, Value const& val) { dst = val.template cast<T1>(); }
};

template <ConceptValueFloat T1> struct Stencil::Assign<T1, std::string_view>
{
    void operator()(T1& dst, std::string_view const& val) { TODO(""); }
};

template <ConceptValueSigned T1> struct Stencil::Assign<T1, std::string_view>
{
    void operator()(T1& dst, std::string_view const& sv)
    {
        if constexpr (std::is_same_v<T1, char>)
        {
            if (sv.size() == 1)
            {
                dst = sv[0];
                return;
            }
        }
        else if constexpr (std::is_same_v<T1, std::array<float, 2>>)
        {
            TODO();
        }
        else if constexpr (std::is_same_v<T1, std::array<char, 8>>)
        {
            TODO();
        }
        else if constexpr (std::is_same_v<T1, std::array<uint16_t, 4>>)
        {
            TODO();
        }

        else
        {
            int64_t ival;
            auto    result = std::from_chars(sv.data(), sv.data() + sv.size(), ival);
            if (result.ec == std::errc::invalid_argument) throw std::logic_error("Cannot convert");
            if (ival > std::numeric_limits<T1>::max() || ival < std::numeric_limits<T1>::min()) { throw std::logic_error("Out of range"); }
            dst = static_cast<T1>(ival);
        }
    }
};

template <ConceptValueUnsigned T1> struct Stencil::Assign<T1, std::string_view>
{
    void operator()(T1& dst, std::string_view const& sv)
    {
        uint64_t val;
        auto     result = std::from_chars(sv.data(), sv.data() + sv.size(), val);
        if (result.ec == std::errc::invalid_argument) throw std::logic_error("Cannot convert");
        // TODO : if (val > std::numeric_limits<T1>::max() || val < std::numeric_limits<T1>::min()) { throw std::logic_error("Out of
        // range"); }
        dst = Value::ValueTraits<T1>::Convert(val);
    }
};

template <ConceptValueFloat T1> struct Stencil::Assign<T1, std::wstring_view>
{
    void operator()(T1& dst, std::wstring_view const& val) { TODO(""); }
};

template <ConceptValueSigned T1> struct Stencil::Assign<T1, std::wstring_view>
{
    void operator()(T1& dst, std::wstring_view const& val) { TODO(""); }
};

template <ConceptValueUnsigned T1> struct Stencil::Assign<T1, std::wstring_view>
{
    void operator()(T1& dst, std::wstring_view const& val) { TODO(""); }
};

template <size_t N> struct Stencil::Assign<std::array<char, N>, std::wstring_view>
{
    void operator()(std::array<char, N>& dst, std::wstring_view const& val) { TODO(""); }
};

template <size_t N> struct Stencil::Assign<std::array<char, N>, std::string_view>
{
    void operator()(std::array<char, N>& dst, std::string_view const& val)
    {
        for (size_t i = 0; i < N; i++) { dst[i] = i < val.size() ? val[i] : 0; }
    }
};

template <size_t N> struct Stencil::Assign<std::array<char, N>, Value>
{
    void operator()(std::array<char, N>& dst, Value const& val) { TODO(""); }
};

template <> struct Stencil::Assign<uuids::uuid, std::wstring_view>
{
    void operator()(uuids::uuid& dst, std::wstring_view const& val) { TODO(""); }
};

template <> struct Stencil::Assign<uuids::uuid, std::string_view>
{
    void operator()(uuids::uuid& dst, std::string_view const& val) { TODO(""); }
};

template <> struct Stencil::Assign<uuids::uuid, Value>
{
    void operator()(uuids::uuid& dst, Value const& val) { TODO(""); }
};

#pragma warning(pop)

template <Stencil::ConceptEnum TOut, typename T> struct Stencil::Assign<TOut, std::basic_string_view<T>>
{
    using EnumTrait = typename EnumTraits<TOut>;
    void operator()(TOut& lhs, std::basic_string_view<T> const& rhs)
    {
        for (size_t i = 0; i < std::size(EnumTrait::Names); i++)
        {
            auto name = EnumTrait::Names[i];
            if (std::equal(std::begin(rhs), std::end(rhs), std::begin(name), std::end(name), [](auto l, auto r) {
                    return std::tolower(l) == std::tolower(r);
                }))
            {
                lhs = EnumTrait::ForIndex(i);
                return;
            }
        }
        throw std::invalid_argument("Invalid");
    }
};

template <Stencil::ConceptEnum TOut> struct Stencil::Assign<TOut, Value>
{
    using EnumTrait = typename EnumTraits<TOut>;
    void operator()(TOut& lhs, Value const& rhs) { lhs = EnumTrait::ForIndex(rhs); }
};
/*
 * OpenQuestions
 * TypeTraits.Categories Should there be a preferred
 * TypeTraits.Indexable.Key : Should FixedSize Structs also be allowed, how about any type ? Do we care ?
 * Visitor.Indexable.Key needs key to be a temp object outside data scope . How to get rid of it
 * Should Structs be assignable via indexable json
 *      Maybe : But we should validate the json array size and it matches the fixed size
 *      Fixed size iterable ?
 *
 * Types.Indexable: unordered_map
 * Types.Primitives: Are 128 bits and bigger Values Primitives ?
 * StackVisitor: Find a better name
 *
 * TypeTraits
 *      Categories(Primitive/Iterable/Indexable)
 *          Can be multiple. Should we denote a preferred ?
 *          Serialiation / Deserialization controlled using Visitors.
 *          StackVisitor might not know which one to select.
 *              Precedence Indexable > Iterable > Primitive doesnt always work
 *                  Iterable > Primitive breaks for GUIDs and std::array<char>
 *                  Indexale > Iterable breaks for ? [OPEN QUESTION]
 *
 * Indexable:
 *      Key :
 *          Justification : [STRONG].
 *              Everyone needs to know what indexable types are allowed
 *          Can only be a primitive ?
 *          What about Types with hash functions
 *          Key Visitor requires storage of temp key object IndexableVisitorTypeHandler<T> has a mutable
 *      ValueTypes:
 *          Justification : [WEAK]
 *          Unique only ?
 *          What about duplicate ? Codegen wont be able to dedup. Templating technique available for auto dedup
 *          Only needed because StackVisitor needs all Type Handlers instantiated . Can we make them ondemand ? [OPEN-QUESTION]
 *
 *      Unhandled Types:
 *          std::unordered_map<std::string, FixedSize>
 *          std::unordered_map<uint64_t, FixedSize>
 *
 * Iterable
 *      Variable size: // Any length allowed
 *      Fixed size : // Validate length (post / pre ?)
 *
 *
 * Primitives
 *      Values (64-Bit)
 *      Blobs (String/Wide-String)
 *      Wht about GUIDs , Crypto-Shas ? 128 Bit ?
 *      Custom Conversion Routines
 *
 * StackVisitor
 *      Rename to something else ?
 *      All TypeHandlers for all nested types are available and instantiated
 *      What about duplicate types ?
 *      TypeTraits
 *
 */