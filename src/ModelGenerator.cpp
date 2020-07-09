#include "GeneratedCodeFragment.h"
#include "IDL2.h"
#include "TemplateFragment.h"

#include "TypeDefinition.ly.h"

#pragma warning(push, 0)
#include <tinyxml2.h>
#pragma warning(pop)

#include <EmbeddedResource.h>
#include <fstream>
#include <iostream>
#include <regex>
#include <set>
#include <stdexcept>
#include <string_view>

#include <vector>

DECLARE_RESOURCE_COLLECTION(templates);

using namespace tinyxml2;
using Str = Binding::Str;

inline std::string wstring_to_string(std::wstring_view wstr)
{
    std::string out(wstr.size(), 0);
#pragma warning(push)
#pragma warning(disable : 4996)
    wcstombs(out.data(), wstr.data(), wstr.size());
#pragma warning(pop)
    return out;
}

inline std::string readfile(std::filesystem::path const& filepath)
{
    std::ifstream file(filepath);
    if (file.fail()) throw std::runtime_error("Cannot Load File : " + filepath.string());
    std::string contents;
    file.seekg(0, std::ios::end);
    contents.reserve((unsigned int)file.tellg());
    file.seekg(0, std::ios::beg);
    contents.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return contents;
}

std::ostream& operator<<(std::ostream& strm, std::wstring_view wstr)
{
    for (auto& c : wstr)
    {
        strm << static_cast<char>(c);
    }
    return strm;
}

void Generator::_AddTypeDefinitions(std::string_view const& /*name*/, std::string_view const& text)
{
    IDL::Lang::TypeDefinition::Context loadcontext{*_program};
    IDL::Lang::TypeDefinition::LoadString(loadcontext, text.data());
}

void CreateTemplateFromNode(tree<TemplateFragment>&          tmpl,
                            tree<TemplateFragment>::iterator parent,
                            std::string_view const&          name,
                            XMLNode const&                   xml)
{
    auto textNode = xml.ToText();
    auto elemNode = xml.ToElement();

    TemplateFragment data;
    data.body = [&]() -> decltype(data.body) {
        if (textNode != nullptr)
        {
            auto textval = Str::Convert(textNode->Value());
            if (textval.length() > 0)
            {
                return Binding::Expression::Create(textval, Str::Create(L"zz"), Str::Create(L"zz"), L'_');
            }
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

    for (auto node = xml.FirstChild(); node != nullptr; node = node->NextSibling())
    {
        CreateTemplateFromNode(tmpl, it, name, *node);
    }
}

template <typename TMap, typename TKey> auto FindInMapOrDefault(TMap const& map, TKey const& key)
{
    auto it = map.find(key);
    if (it == map.end()) return decltype(it->second){};
    return it->second;
}

void ExpandTemplate(tree<Str::Type>&                 codetree,
                    tree<Str::Type>::iterator        root,
                    tree<TemplateFragment> const&    tmpl,
                    tree<TemplateFragment>::iterator tmplrootit,
                    Binding::BindingContext&         context,
                    Binding::IBindable const&        data)
{

    auto it = codetree.addchild(root, [&]() {
        if (tmplrootit->body != nullptr)
        {
            return context.EvaluateExpression(data, *tmplrootit->body)->String();
        }
        return std::wstring();
    }());

    auto filterExpr = FindInMapOrDefault(tmplrootit->attributes, "Filter");
    if (!filterExpr.empty())
    {
        auto pos    = ((std::wstring_view)filterExpr).find(L':');
        auto prefix = ((std::wstring_view)filterExpr).substr(0, pos);
        auto suffix = ((std::wstring_view)filterExpr).substr(pos);

        //        auto                   paramName = splitintotwo(filterExpr, ":");
        Binding::Expression expr;
        expr.AddString(Str::Create(prefix));
        if (suffix != Str::Value(context.EvaluateExpression(data, expr)->String()))
        {
            return;
        }
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
            if (tmplrootit->body != nullptr)
            {
                return context.EvaluateExpression(data, *tmplit->body)->String();
            }
            return std::wstring();
        }());

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
        for (auto const& cdata : context.GetRange(data, bexpr))
        {
            auto scope = context.ContextScope(cdata);
            if (!first && !joinStr.empty())
            {
                codetree.addchild(it, Str::Create(joinStr));
            }
            first = false;
            ExpandTemplate(codetree, it, tmpl, tmplit, context, cdata);
        }
    }
}

tree<Str::Type> ExpandTemplate(tree<TemplateFragment> const& tmpl, Binding::BindingContext& context, Binding::IBindable const& data)
{
    tree<Str::Type> codetree;
    for (auto it = tmpl.rootbegin(); it != tmpl.rootend(); ++it)
    {
        ExpandTemplate(codetree, codetree.rootbegin(), tmpl, it, context, data);
    }

    return codetree;
}

Template CreateTemplate(XMLElement const& element, std::string_view const& name)
{
    Template templ;
    CreateTemplateFromNode(templ.root, templ.root.rootbegin(), name, element);
    templ.fileName   = Binding::Expression::Create(Str::Convert(element.Attribute("file")), Str::Create(L"zz"), Str::Create(L"zz"), L'_');
    templ.dataSource = Str::Convert(element.Attribute("datasource"));
    return templ;
}

static std::wstring AddCDataBegin(std::wstring const& tmplview)
{
    std::wstringstream    sstr;
    size_t                index = 0;
    std::wregex           re(L"[ \t]*//(<[^/>]+>)\\s*[\r\n]");
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
    std::wregex           re(L"[ \t]*//(</[^>]+>)\\s*[\r\n]");
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
    if (text.length() == 0)
    {
        return;
    }
    auto fullTemplateContents = Str::Create(L"<ModelGenerator><![CDATA[" + Str::Value(Str::Convert(text)) + L"]]></ModelGenerator>");
    auto modified             = AddCDataEnd(AddCDataBegin(fullTemplateContents));

    tinyxml2::XMLDocument doc(false);
    auto                  rc = doc.Parse(wstring_to_string(modified).c_str());
    if (rc != XMLError::XML_SUCCESS)
    {
        throw std::logic_error(("Error Loading Template: \n\t%s\n" + std::string(doc.ErrorStr())).c_str());
    }

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

    if (name.find("typedef") != std::string_view ::npos)
    {
        _AddTypeDefinitions(name, text);
    }
    else if (name.find("template") != std::string_view ::npos)
    {
        _AddTemplate(name, text);
    }
}

void Generator::LoadBuilltinTemplates()
{
    for (const auto& res : LOAD_RESOURCE_COLLECTION(templates))
    {
        _AddContent(wstring_to_string(res.name()), res.string());
    }
}

void Generator::LoadTemplate(std::filesystem::path const& templateFilePath)
{
    if (std::filesystem::is_directory(templateFilePath))
    {
        for (auto const& f : std::filesystem::directory_iterator(templateFilePath))
        {
            LoadTemplate(f);
        }
        return;
    }

    _AddContent(templateFilePath.filename().string(), readfile(templateFilePath));
}

static GeneratedCode GenerateCode(Template const& tmpl, Binding::IBindable const& data)
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

    for (auto& tmpl : _templates)
    {
        if (!Str::IsEmpty(tmpl.dataSource) && !_program->TryLookup<IDL::DataSource>(Str::Create(tmpl.dataSource)).has_value())
        {
            continue;
        }

        auto code     = GenerateCode(tmpl, *_program);
        auto filepath = outDir / code.filename;
        if (!isDryRun)
        {
            std::ofstream file(filepath);
            if (!file.is_open())
            {
                throw std::runtime_error("Cannot Write to file : " + filepath.string());
            }
            WriteCodeToStream(file, code.content, code.content.rootbegin(), code.content.rootend());
            file.close();
        }

        outfiles.push_back(std::move(filepath));
    }

    return outfiles;
}
