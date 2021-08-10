#pragma once
#include <numeric>
#include <stdexcept>
#include <stdint.h>
#include <string>
#include <vector>

namespace ModelFormat::WavefrontObj
{

using Id                         = std::string;
constexpr uint16_t InvalidTexel  = std::numeric_limits<uint16_t>::max();
constexpr uint16_t InvalidNormal = std::numeric_limits<uint16_t>::max();

struct FaceElem
{
    uint16_t vertex;
    uint16_t texel;
    uint16_t normal;
};

inline void debugnewline()
{}

inline FaceElem CreateFaceElem(int v, int t, int n)
{
    return FaceElem{static_cast<uint16_t>(v), static_cast<uint16_t>(t), static_cast<uint16_t>(n)};
}

struct FaceElems
{
    std::vector<uint16_t> vertices;
    std::vector<uint16_t> texels;
    std::vector<uint16_t> normals;
};

template <typename T> inline std::vector<T> ListCreate(T& obj)
{
    return std::vector<T>(std::move(obj));
}

template <typename T> inline std::vector<T> ListAdd(std::vector<T>& l, T& obj)
{
    l.push_back(std::move(obj));
    return std::move(l);
}

inline FaceElems ListAdd(FaceElems& elems, FaceElem& e)
{
    elems.vertices.push_back(static_cast<uint16_t>(e.vertex - 1u));
    if (e.texel != InvalidTexel) { elems.texels.push_back(static_cast<uint16_t>(e.texel - 1u)); }
    if (e.normal != InvalidNormal) { elems.normals.push_back(static_cast<uint16_t>(e.normal - 1u)); }
    return std::move(elems);
}

inline FaceElems ListCreate(FaceElem& e)
{
    FaceElems elems;
    return ListAdd(elems, e);
}

struct Context
{
    struct Vertex
    {
        double x, y, z;
    };
    struct Texel
    {
        double u, v;
    };
    struct Normal
    {
        double x, y, z;
    };
    struct Triangle
    {
        uint16_t v1, v2, v3;
    };

    bool Debug() { return false; }
    void NotifyError(int /*line*/, int /*col*/, std::string const& /*msg*/) {}

    void CreateVertex(double x, double y, double z) { vertices.emplace_back(Vertex{x, y, z}); }
    void CreateTexel(double u, double v) { texels.emplace_back(Texel{u, v}); }
    void CreateNormal(double x, double y, double z) { normals.emplace_back(Normal{x, y, z}); }
    void CreateFace(FaceElems& elemsIn)
    {
        FaceElems elems;
        std::swap(elems, elemsIn);
        if (elems.vertices.size() != 3) { throw std::invalid_argument("Polygon faces unsupported"); }
        triangles.emplace_back(Triangle{elems.vertices[0], elems.vertices[1], elems.vertices[2]});
    }

    void SetCurrentObjectName(std::string& /*str*/) {}
    void SetCurrentGroupName(std::string& /*str*/) {}
    void SetMaterialLibFile(std::string& /*str*/) {}
    void SetMaterialName(std::string& /*str*/) {}

    std::vector<Triangle> triangles;
    std::vector<Vertex>   vertices;
    std::vector<Texel>    texels;
    std::vector<Normal>   normals;
};

}    // namespace ModelFormat::WavefrontObj
