#include "DebugInfo.h"
#include "GeneratedCodeFragment.h"
#include "Generator.h"
#include "IDL2.h"
#include "TemplateFragment.h"

#include <EmbeddedResource.h>

SUPPRESS_WARNINGS_START
SUPPRESS_STL_WARNINGS
SUPPRESS_MSVC_WARNING(4061)    // switch case not handled
SUPPRESS_MSVC_WARNING(4583)    // destructor not implicitly called
SUPPRESS_MSVC_WARNING(4582)    // constructor not implicitly called
#include <tinyxml2.h>
#include <toml.hpp>
#include <tsl/ordered_map.h>

#include <fstream>
#include <iostream>
#include <regex>
#include <set>
#include <stdexcept>
#include <string_view>
#include <vector>
SUPPRESS_WARNINGS_END

DECLARE_RESOURCE_COLLECTION(templates);

using namespace tinyxml2;
using Str = Binding::Str;

inline std::string wstring_to_string(std::wstring_view wstr)
{
    std::string out(wstr.size(), 0);
    SUPPRESS_WARNINGS_START
    SUPPRESS_MSVC_WARNING(4996)
    wcstombs(out.data(), wstr.data(), wstr.size());
    SUPPRESS_WARNINGS_END
    return out;
}

inline std::string readfile(std::filesystem::path const& filepath)
{
    std::ifstream file(filepath);
    if (file.fail()) throw std::runtime_error("Cannot Load File : " + filepath.string());
    std::string contents;
    file.seekg(0, std::ios::end);
    contents.reserve(static_cast<unsigned>(file.tellg()));
    file.seekg(0, std::ios::beg);
    contents.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return contents;
}

static std::ostream& operator<<(std::ostream& strm, std::wstring_view wstr)
{
    for (auto& c : wstr) { strm << static_cast<char>(c); }
    return strm;
}

void TypeDefinitions::FieldTypeDecl::Merge(TypeDefinitions::FieldTypeDecl&& decl)
{
    if (!decl.name.empty())
    {
        if (name.empty()) { name = std::move(decl.name); }
        else if (name != decl.name) { throw std::logic_error("Cannot merge differently name FieldTypes"); }
    }

    if (!decl.baseField.empty())
    {
        if (baseField.empty()) { baseField = std::move(decl.baseField); }
        else if (baseField != decl.baseField) { throw std::logic_error("Can only have 1 base field"); }
    }

    if (decl.annotationMap != nullptr)
    {
        if (annotationMap == nullptr) { annotationMap = std::move(decl.annotationMap); }
        else
        {
            for (auto const& [k, v] : decl.annotationMap->GetAttributes())
            {
                auto copied = v;
                annotationMap->AddEntry(k, std::move(copied));
            }
        }
    }

    for (auto& v : decl.mutators) { mutators.push_back(std::move(v)); }

    for (auto& v : decl.accessors) { accessors.push_back(std::move(v)); }
}

#if defined USING_YAML_CPP
namespace YAML
{
template <> struct convert<std::wstring>
{
    static bool decode(YAML::Node const& node, std::wstring& val)
    {
        val = Str::Convert(node.as<std::string>());
        return true;
    }
};

template <> struct convert<std::vector<Generator::MutatorAccessorDefinition>>
{
    static bool decode(YAML::Node const& node, std::vector<Generator::MutatorAccessorDefinition>& valarr)
    {
        for (auto it = node.begin(); it != node.end(); ++it)
        {
            Generator::MutatorAccessorDefinition val;
            val.name = it->first.as<std::wstring>();
            val.id   = it->second["Id"].as<uint8_t>();
            val.returnType
                = Binding::Expression::Create(it->second["ReturnType"].as<std::wstring>(), Str::Create(L"%"), Str::Create(L"%"), L':');
            auto& nargs = it->second["Args"];
            if (nargs.IsDefined() && !nargs.IsNull())
            {
                if (nargs.IsScalar())
                {
                    auto str = nargs.as<std::wstring>();
                    val.args = {Binding::Expression::Create(str, Str::Create(L"%"), Str::Create(L"%"), L':')};
                }
                else
                {
                    for (auto it1 = nargs.begin(); it1 != nargs.end(); ++it1)
                    {
                        val.args.push_back(
                            Binding::Expression::Create(it1->second.as<std::wstring>(), Str::Create(L"%"), Str::Create(L"%"), L':'));
                    }
                }
            }
            else { throw std::invalid_argument("Args not found for :" + it->first.as<std::string>()); }
            valarr.push_back(std::move(val));
        }

        return true;
    }
};

template <> struct convert<Generator::FieldTypeDecl>
{
    static bool decode(YAML::Node const& node, Generator::FieldTypeDecl& val)
    {
        if (node.IsScalar())
        {
            val.baseField = node.as<std::wstring>();
            return true;
        }
        for (auto it1 = node.begin(); it1 != node.end(); ++it1)
        {
            auto propname = it1->first.as<std::wstring>();
            if (propname == L"Mutators") { val.mutators = it1->second.as<std::vector<Generator::MutatorAccessorDefinition>>(); }
            else if (propname == L"Accessors") { val.accessors = it1->second.as<std::vector<Generator::MutatorAccessorDefinition>>(); }
            else if (propname == L"Inherits") { val.baseField = it1->second.as<std::wstring>(); }
            else
            {
                auto annotationMap
                    = val.annotationMap == nullptr ? (val.annotationMap = std::make_shared<Binding::AttributeMap>()) : val.annotationMap;
                auto propval = it1->second.as<std::wstring>();
                auto expr    = Binding::Expression::Create(propval, Str::Create(L"%"), Str::Create(L"%"), L':');
                (*annotationMap).AddEntry(propname, std::move(expr));
            }
        }
        return true;
    }
};

template <> struct convert<Generator::ContainerTypeDecl>
{
    static bool decode(YAML::Node const& node, Generator::ContainerTypeDecl& val)
    {
        if (node.IsScalar())
        {
            val.baseField = node.as<std::wstring>();
            return true;
        }
        for (auto it1 = node.begin(); it1 != node.end(); ++it1)
        {
            auto propname = it1->first.as<std::wstring>();
            if (propname == L"Mutators") { val.mutators = it1->second.as<std::vector<Generator::MutatorAccessorDefinition>>(); }
            else if (propname == L"Accessors") { val.accessors = it1->second.as<std::vector<Generator::MutatorAccessorDefinition>>(); }
            else if (propname == L"Inherits") { val.baseField = it1->second.as<std::wstring>(); }
            else if (propname == L"Params") { val.args = it1->second.as<std::vector<std::wstring>>(); }
            else
            {
                auto annotationMap
                    = val.annotationMap == nullptr ? (val.annotationMap = std::make_shared<Binding::AttributeMap>()) : val.annotationMap;
                auto propval = it1->second.as<std::wstring>();
                auto expr    = Binding::Expression::Create(propval, Str::Create(L"%"), Str::Create(L"%"), L':');
                (*annotationMap).AddEntry(propname, std::move(expr));
            }
        }
        return true;
    }
};

}    // namespace YAML

static void debug(YAML::Node const& node)
{
    std::stringstream ss;
    ss << node;
    ss << "Defined:" << node.IsDefined() << std::endl;
    if (node.IsDefined())
    {
        ss << "Null:" << node.IsNull() << std::endl;
        ss << "Scalar:" << node.IsScalar() << std::endl;
        ss << "Map:" << node.IsMap() << std::endl;
        ss << "Sequence:" << node.IsSequence() << std::endl;
        ss << "Type:" << node.Type() << std::endl;
        auto st = ss.str();
        std::cout << st;
    }
}
#endif

template <typename TTableNode>
static TypeDefinitions::MutatorAccessorDefinition ParseMutatorAccessorDefinitionFromTomlNode(TTableNode const& node, std::wstring&& key)
{
    auto&                                      valtbl = (node.as_table());
    TypeDefinitions::MutatorAccessorDefinition val;
    val.name = std::move(key);
    val.id   = static_cast<uint8_t>(valtbl.at("Id").as_integer());
    val.returnType
        = Binding::Expression::Create(Str::Convert(valtbl.at("ReturnType").as_string().str), Str::Create(L"%"), Str::Create(L"%"), L':');
    auto args = valtbl.at("Args");

    if (args.is_string())
    {
        auto str = Str::Convert(args.as_string().str);
        val.args = {Binding::Expression::Create(str, Str::Create(L"%"), Str::Create(L"%"), L':')};
    }
    else
    {
        for (auto const& sub : args.as_array())
        {
            val.args.push_back(Binding::Expression::Create(Str::Convert(sub.as_string().str), Str::Create(L"%"), Str::Create(L"%"), L':'));
        }
    }
    return val;
}

template <typename TTableNode>
static std::vector<TypeDefinitions::MutatorAccessorDefinition> ParseMutatorAccessorDefinitionArrFromTomlNode(TTableNode const& node)
{
    std::vector<TypeDefinitions::MutatorAccessorDefinition> valarr;
    for (auto [key, value] : node.as_table()) { valarr.push_back(ParseMutatorAccessorDefinitionFromTomlNode(value, Str::Convert(key))); }

    return valarr;
}

template <typename TTableNode> static TypeDefinitions::ContainerTypeDecl ContainerTypeDeclFromTomlNode(TTableNode const& node)
{
    TypeDefinitions::ContainerTypeDecl val;
    if (node.is_string())
    {
        val.baseField = Str::Convert(node.as_string().str);
        return val;
    }
    for (auto [key, value] : node.as_table())
    {
        auto propname = Str::Convert(key);
        if (propname == L"Mutators") { val.mutators = ParseMutatorAccessorDefinitionArrFromTomlNode(value); }
        else if (propname == L"Accessors") { val.accessors = ParseMutatorAccessorDefinitionArrFromTomlNode(value); }
        else if (propname == L"Inherits") { val.baseField = Str::Convert(value.as_string().str); }
        else if (propname == L"Params")
        {
            for (auto const& sub : (value.as_array())) { val.args.push_back(Str::Convert(sub.as_string().str)); }
        }
        else
        {
            auto annotationMap
                = val.annotationMap == nullptr ? (val.annotationMap = std::make_shared<Binding::AttributeMap>()) : val.annotationMap;
            auto propval = Str::Convert(value.as_string().str);
            auto expr    = Binding::Expression::Create(propval, Str::Create(L"%"), Str::Create(L"%"), L':');
            (*annotationMap).AddEntry(propname, std::move(expr));
        }
    }
    return val;
}

template <typename TTableNode> static TypeDefinitions::FieldTypeDecl FieldTypeDeclFromTomlNode(TTableNode const& node)
{
    TypeDefinitions::FieldTypeDecl val;

    if (node.is_string())
    {
        val.baseField = Str::Convert(node.as_string().str);
        return val;
    }
    for (auto [key, value] : node.as_table())
    {
        auto propname = Str::Convert(key);
        if (propname == L"Mutators") { val.mutators = ParseMutatorAccessorDefinitionArrFromTomlNode(value); }
        else if (propname == L"Accessors") { val.accessors = ParseMutatorAccessorDefinitionArrFromTomlNode(value); }
        else if (propname == L"Inherits") { val.baseField = Str::Convert(value.as_string().str); }
        else
        {
            auto annotationMap
                = val.annotationMap == nullptr ? (val.annotationMap = std::make_shared<Binding::AttributeMap>()) : val.annotationMap;
            auto propval = Str::Convert(value.as_string().str);
            auto expr    = Binding::Expression::Create(propval, Str::Create(L"%"), Str::Create(L"%"), L':');
            (*annotationMap).AddEntry(propname, std::move(expr));
        }
    }
    return val;
}

void TypeDefinitions::AddTypeDefinitions(std::string_view const& /*name*/, std::string_view const& text)
{
    std::string       tomltext(text);
    std::stringstream tomlstrm(tomltext);

    auto config = toml::parse<toml::preserve_comments, tsl::ordered_map>(tomlstrm);
    for (auto [propname, node] : config.as_table())
    {
        if (propname == "FieldTypes")
        {
            for (auto [key, node1] : node.as_table())
            {
                auto fieldTypeDecl = FieldTypeDeclFromTomlNode(node1);
                fieldTypeDecl.name = Str::Convert(key);
                _FindOrInsertFieldTypeDecl(fieldTypeDecl.name).Merge(std::move(fieldTypeDecl));
            }
        }
        else if (propname == "Struct") { _structDefault.Merge(FieldTypeDeclFromTomlNode(node)); }
        else if (propname == "Variant") { _unionDefault.Merge(FieldTypeDeclFromTomlNode(node)); }
        else if (propname == "Interface") { _interfaceDefault.Merge(FieldTypeDeclFromTomlNode(node)); }
        else if (propname == "FunctionArgs") { _fnargsDefault.Merge(FieldTypeDeclFromTomlNode(node)); }
        else if (propname == "Typedef") { _typedefDefault.Merge(FieldTypeDeclFromTomlNode(node)); }
        else if (propname == "Enum") { _enumDefault.Merge(FieldTypeDeclFromTomlNode(node)); }
        else if (propname == "EnumValue") { _enumValueDefault.Merge(FieldTypeDeclFromTomlNode(node)); }
        else if (propname == "NamedConst") { _namedConstDefault.Merge(FieldTypeDeclFromTomlNode(node)); }
        else if (propname == "Containers")
        {
            for (auto [key, node1] : node.as_table())
            {
                auto typedecl = ContainerTypeDeclFromTomlNode(node1);
                typedecl.name = Str::Convert(key);
                _FindOrInsertContainerDecls(typedecl.name).Merge(std::move(typedecl));
            }
        }
        else if (propname == "Attributes")
        {
            for (auto [key, node1] : node.as_table())
            {
                auto& objmap = _attributeDefs[Str::Convert(key)];
                for (auto const& [key1, node2] : node1.as_table()) { objmap[Str::Convert(key1)] = Str::Convert(node2.as_string().str); }
            }
        }
        else { throw std::logic_error("Unrecognized type"); }
    }
}

static void CreateTemplateFromNode(tree<TemplateFragment>&          tmpl,
                                   tree<TemplateFragment>::iterator parent,
                                   std::wstring_view const&         name,
                                   XMLNode const&                   xml)
{
    auto textNode = xml.ToText();
    auto elemNode = xml.ToElement();

    TemplateFragment data;
    data.body = [&]() -> decltype(data.body) {
        if (textNode != nullptr)
        {
            auto textval = Str::Convert(textNode->Value());
            if (textval.length() > 0) { return Binding::Expression::Create(textval, Str::Create(L"zz"), Str::Create(L"zz"), L'_'); }
        }
        return {};
    }();

    data.name = elemNode != nullptr ? Str::Convert(elemNode->Value()) : Str::Create(L"");
    for (auto attr = elemNode == nullptr ? nullptr : elemNode->FirstAttribute(); attr; attr = attr->Next())
    {
        data.attributes[attr->Name()] = Str::Value(Str::Convert(attr->Value()));
    }

    data.rowstart = data.rowend = static_cast<size_t>(xml.GetLineNum());
    data.sourceFileName         = name;

    auto it = tmpl.addchild(parent, std::move(data));

    for (auto node = xml.FirstChild(); node != nullptr; node = node->NextSibling()) { CreateTemplateFromNode(tmpl, it, name, *node); }
}

template <typename TMap, typename TKey> auto FindInMapOrDefault(TMap const& map, TKey const& key)
{
    auto it = map.find(key);
    if (it == map.end()) return decltype(it->second){};
    return it->second;
}

static void ExpandTemplate(tree<Str::Type>&                 codetree,
                           tree<Str::Type>::iterator        root,
                           tree<TemplateFragment> const&    tmpl,
                           tree<TemplateFragment>::iterator tmplrootit,
                           Binding::BindingContext&         context,
                           Binding::IBindable&              data)
{

    auto it = codetree.addchild(root, [&]() {
        if (tmplrootit->body != nullptr) { return context.EvaluateExpression(data, *tmplrootit->body)->String(); }
        return std::wstring();
    }());

    auto filterExpr = FindInMapOrDefault(tmplrootit->attributes, "Filter");
    if (!filterExpr.empty())
    {
        auto pos    = filterExpr.find(L':');
        auto prefix = filterExpr.substr(0, pos);
        auto suffix = filterExpr.substr(pos);

        //        auto                   paramName = splitintotwo(filterExpr, ":");
        Binding::Expression expr;
        expr.AddString(Str::Create(prefix));
        if (suffix != Str::Value(context.EvaluateExpression(data, expr)->String())) { return; }
    }
    auto tmplrange = tmpl.children(tmplrootit);
    for (auto tmplit = tmplrange.begin(); tmplit != tmplrange.end(); ++tmplit)
    {
        auto& name = tmplit->name;
        if (Str::IsEmpty(name) || Str::Equal(name, Str::Create(L"Template")) || Str::Equal(name, Str::Create(L"Include")))
        {
            ExpandTemplate(codetree, it, tmpl, tmplit, context, data);
            continue;
        }

        if (Str::Equal(name, Str::Create(L"If")))
        {
            bool allgood = true;
            for (const auto& [lhs, rhs] : tmplit->attributes)
            {
                Binding::Expression    expr;
                std::vector<Str::Type> b;
                b.push_back(Str::Convert(lhs.c_str()));
                expr.AddBindingExpression(std::unique_ptr<Binding::BindingExpr>(new Binding::BindingExpr{std::move(b)}));
                auto rslt = context.EvaluateExpression(data, expr)->String();
                if (!Str::Equal(rslt, Str::Create(rhs)))
                {
                    allgood = false;
                    break;
                }
            }
            if (allgood) ExpandTemplate(codetree, it, tmpl, tmplit, context, data);

            continue;
        }

        it = codetree.addsibling(it, [&]() {
            if (tmplrootit->body != nullptr) { return context.EvaluateExpression(data, *tmplit->body)->String(); }
            return std::wstring();
        }());

        auto actionctxvar = IDLDebug::ThreadActionContext(L"", [&]() {
            return fmt::format(L"Template : {}:{}-{} :: TagName: {}  {}",
                               tmplit->sourceFileName,
                               tmplit->rowstart,
                               tmplit->rowend,
                               tmplit->name,
                               tmplit->body ? tmplit->body->Stringify() : L"<empty>");
        });

        Binding::BindingExpr bexpr;
        for (size_t i = 0; i < Str::Size(name);)
        {
            auto j = Str::Find(name, L'_', i);
            bexpr.binding.push_back(Str::Create(Str::SubString(name, i, j)));
            if (j == std::wstring_view::npos) break;
            i = j + 1;
        }

        auto joinStr = FindInMapOrDefault(tmplit->attributes, "Join");
        bool first   = true;
        for (auto& cdata : context.GetRange(data, bexpr))
        {
            auto scope = context.ContextScope(cdata);
            if (!first && !joinStr.empty()) { codetree.addchild(it, Str::Create(joinStr)); }
            first = false;
            ExpandTemplate(codetree, it, tmpl, tmplit, context, cdata);
        }
    }
}

static tree<Str::Type> ExpandTemplate(tree<TemplateFragment> const& tmpl, Binding::BindingContext& context, Binding::IBindable& data)
{
    tree<Str::Type> codetree;
    for (auto it = tmpl.rootbegin(); it != tmpl.rootend(); ++it)
    {
        ExpandTemplate(codetree, codetree.rootbegin(), tmpl, it, context, data);
    }

    return codetree;
}

static Template CreateTemplate(XMLElement const& element, std::string_view const& name)
{
    Template templ;
    CreateTemplateFromNode(templ.root, templ.root.rootbegin(), Str::Convert(name), element);
    templ.fileName   = Binding::Expression::Create(Str::Convert(element.Attribute("file")), Str::Create(L"zz"), Str::Create(L"zz"), L'_');
    templ.dataSource = Str::Convert(element.Attribute("datasource"));
    return templ;
}

static std::wstring AddCDataBegin(std::wstring const& tmplview)
{
    std::wstringstream    sstr;
    size_t                index = 0;
    std::wregex           re(L"[ \t]*//(<[^/>]+>\\s*[\r\n])");
    std::wsregex_iterator begin(tmplview.begin(), tmplview.end(), re);
    std::wsregex_iterator end;

    for (std::wsregex_iterator i = begin; i != end; ++i)
    {

        std::wsmatch match = *i;
        assert(match.size() == 2);
        sstr << match.prefix();
        sstr << L"]]>" << match[1].str() << L"<![CDATA[";
        index = static_cast<size_t>(match.position(0u) + match.length(0u));
    }
    sstr << tmplview.substr(index);
    return sstr.str();
}

static std::wstring AddCDataEnd(std::wstring const& tmplview)
{
    std::wstringstream    sstr;
    size_t                index = 0;
    std::wregex           re(L"[ \t]*//(</[^>]+>\\s*[\r\n])");
    std::wsregex_iterator begin(tmplview.begin(), tmplview.end(), re);
    std::wsregex_iterator end;

    for (std::wsregex_iterator i = begin; i != end; ++i)
    {

        std::wsmatch match = *i;
        assert(match.size() == 2);
        sstr << match.prefix();
        sstr << L"]]>" << match[1].str() << L"<![CDATA[";
        index = static_cast<size_t>(match.position(0u) + match.length(0u));
    }
    sstr << tmplview.substr(index);
    return sstr.str();
}

void Generator::_AddTemplate(std::string_view const& name, std::string_view const& text)
{
    if (text.length() == 0) { return; }
    auto fullTemplateContents = Str::Create(L"<ModelGenerator><![CDATA[" + Str::Value(Str::Convert(text)) + L"]]></ModelGenerator>");
    auto modified             = AddCDataEnd(AddCDataBegin(fullTemplateContents));

    tinyxml2::XMLDocument doc(false);
    auto                  rc = doc.Parse(wstring_to_string(modified).c_str());
    if (rc != XMLError::XML_SUCCESS) { throw std::logic_error(("Error Loading Template: \n\t%s\n" + std::string(doc.ErrorStr())).c_str()); }

    auto fullTemplateElem = doc.FirstChildElement();

    for (auto child = fullTemplateElem->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
    {
        if (std::string_view(child->Name()) != "Template")
        {
            throw std::logic_error("Error Loading Template: \n\tUnknown XML Element: %s" /*, child->Name()*/);
        }
        _templates.push_back(CreateTemplate(*child, name));
    }
}

void Generator::_AddContent(std::string_view const& name, std::string_view const& text)
{
    if (name.find("typedecl") != std::string_view ::npos) { _typeDefinitions->AddTypeDefinitions(name, text); }
    else if (name.find("template") != std::string_view ::npos) { _AddTemplate(name, text); }
}

void TypeDefinitions::_RegisterFieldDefForProgram(TypeDefinitions::FieldTypeDecl const& v, IDL::Program& program) const
{
    if (!v.baseField.empty()) { _RegisterFieldDefForProgram(_fieldTypeDecls[_fieldTypeDeclMap.at(v.baseField)], program); }

    if (program.TryGetFieldTypeName(v.name).has_value()) { return; }

    auto fieldtype
        = program.CreateFieldTypeObject<IDL::NativeFieldType>(Str::Copy(v.name), program.TryGetFieldTypeName(v.baseField), v.annotationMap);
    for (auto& m : v.mutators)
    {
        if (m.args.size() > 1) { throw std::logic_error("Multiple args not yet supported"); }

        fieldtype->CreateMutator(Str::Copy(m.name), m.id, m.returnType, m.args[0]);
    }
    for (auto& m : v.accessors)
    {
        if (m.args.size() > 1) { throw std::logic_error("Multiple args not yet supported"); }

        fieldtype->CreateAccessor(Str::Copy(m.name), m.id, m.returnType, m.args[0]);
    }
};

void Generator::FinalizeTypeDefinitions()
{
    if (_finalized) { return; }

    _finalized = true;
}
void TypeDefinitions::FinalizeTypeDefinitions()
{
    auto defaultFieldTypeDecl = &_fieldTypeDecls[_fieldTypeDeclMap[L"default"]];

    // Relay inheritance
    for (auto& v : _fieldTypeDecls)
    {
        if (v.name == L"default") continue;
        auto baseFieldType = defaultFieldTypeDecl;
        if (!v.baseField.empty()) { baseFieldType = &_fieldTypeDecls[_fieldTypeDeclMap[v.baseField]]; }
        for (auto& m : baseFieldType->mutators) { v.mutators.push_back(m); }
        for (auto& a : baseFieldType->accessors) { v.accessors.push_back(a); }
    }
    for (auto& v : _containerDecls)
    {
        if (v.baseField.empty()) { continue; }
        auto baseFieldType = &_containerDecls[_containerDeclMap[v.baseField]];
        for (auto& m : baseFieldType->mutators) { v.mutators.push_back(m); }
        for (auto& a : baseFieldType->accessors) { v.accessors.push_back(a); }
    }
}

void TypeDefinitions::LoadIntoProgram(IDL::Program& program) const
{
    for (auto& v : _fieldTypeDecls)
    {
        if (v.name.substr(0, 7) == L"default") _RegisterFieldDefForProgram(v, program);
    }

    for (auto& v : _fieldTypeDecls)
    {
        if (v.name.substr(0, 7) != L"default") _RegisterFieldDefForProgram(v, program);
    }

    for (auto& v : _containerDecls)
    {
        auto base = program.TryLookup<IDL::Container>(v.baseField);
        auto container
            = program.CreateNamedObject<IDL::Container>(Str::Copy(v.name), std::vector<Str::Type>(v.args), base, v.annotationMap);
        for (auto& m : v.mutators) { container->AddMutator({m}); }
        for (auto& m : v.accessors) { container->AddAccessor({m}); }
    }

    for (auto& [k, v] : _attributeDefs)
    {
        program.CreateNamedObject<IDL::AttributeDefinition>(Str::Create(k), nullptr, IDL::AttributeDefinition::AttributeComponentMap(v));
    }

    std::optional<std::shared_ptr<IDLGenerics::IFieldType>> emptyBaseField;
    program.CreateFieldTypeObject<IDL::NativeFieldType>(L"default_struct", emptyBaseField, _structDefault.annotationMap);
    program.CreateFieldTypeObject<IDL::NativeFieldType>(L"default_union", emptyBaseField, _unionDefault.annotationMap);
    program.CreateFieldTypeObject<IDL::NativeFieldType>(L"default_interface", emptyBaseField, _interfaceDefault.annotationMap);
    program.CreateFieldTypeObject<IDL::NativeFieldType>(L"default_typedef", emptyBaseField, _typedefDefault.annotationMap);
    program.CreateFieldTypeObject<IDL::NativeFieldType>(L"default_functionargs", emptyBaseField, _fnargsDefault.annotationMap);
    program.CreateFieldTypeObject<IDL::NativeFieldType>(L"default_enum", emptyBaseField, _enumDefault.annotationMap);
    program.CreateFieldTypeObject<IDL::NativeFieldType>(L"default_enumvalue", emptyBaseField, _enumValueDefault.annotationMap);
    program.CreateFieldTypeObject<IDL::NativeFieldType>(L"default_namedconst", emptyBaseField, _namedConstDefault.annotationMap);
}

void Generator::LoadBuilltinTemplates()
{
    for (const auto res : LOAD_RESOURCE_COLLECTION(templates)) { _AddContent(wstring_to_string(res.name()), res.string()); }
}

void Generator::LoadTemplate(std::filesystem::path const& templateFilePath)
{
    if (std::filesystem::is_directory(templateFilePath))
    {
        for (auto const& f : std::filesystem::directory_iterator(templateFilePath)) { LoadTemplate(f); }
        return;
    }

    _AddContent(templateFilePath.filename().string(), readfile(templateFilePath));
}

static GeneratedCode GenerateCode(Template const& tmpl, Binding::IBindable& data)
{
    Binding::BindingContext context;

    GeneratedCode code{};

    auto scope    = context.ContextScope(data);
    code.content  = ExpandTemplate(tmpl.root, context, data);
    code.filename = std::filesystem::path(Str::Value(context.EvaluateExpression(data, *tmpl.fileName)->String()));
    return code;
}

inline void WriteCodeToStream(std::ostream&                    strm,
                              tree<Str::Type> const&           codetree,
                              tree<Str::Type>::iterator const& b,
                              tree<Str::Type>::iterator const& e)
{
    for (auto it = b; it != e; ++it)
    {
        strm << *it;
        auto range = codetree.children(it);
        WriteCodeToStream(strm, codetree, range.begin(), range.end());
    }
}

std::vector<std::filesystem::path> Generator::Generate(bool isDryRun, std::filesystem::path const& outDir)
{
    std::vector<std::filesystem::path> outfiles;
    IDLDebug::ErrorAggregator          errorAggregator;
    for (auto& tmpl : _templates)
    {
        if (!Str::IsEmpty(tmpl.dataSource) && !_program->TryLookup<IDL::DataSource>(Str::Create(tmpl.dataSource)).has_value()) { continue; }

        auto code     = GenerateCode(tmpl, *_program);
        auto filepath = outDir / code.filename;
        if (!isDryRun)
        {
            std::ofstream file(filepath);
            if (!file.is_open()) { throw std::runtime_error("Cannot Write to file : " + filepath.string()); }
            WriteCodeToStream(file, code.content, code.content.rootbegin(), code.content.rootend());
            file.close();
        }

        outfiles.push_back(std::move(filepath));
    }

    return outfiles;
}
