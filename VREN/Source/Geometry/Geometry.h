#pragma once

#include "Base.h"
#include "Math/Vector.h"
#include <vector>

namespace VREN
{
    struct Vertex3D
    {
        Vector3 Position;
        Vector2 UV;
    };

    enum class GeometryType
    {
        None = 0,
        Box,
        Plane
    };

    class VREN_API Geometry
    {
    public:
        Geometry() = default;
        ~Geometry() = default;

        inline GeometryType GetType() { return type; };

    public:
        std::vector<Vertex3D> Vertices;
        std::vector<u32> Indices;

    protected:
        GeometryType type;
    };

    class VREN_API BoxGeometry : public Geometry
    {
    public:
        float Width, Height, Depth;
        BoxGeometry(float w, float h, float d);
    };

    class VREN_API PlaneGeometry : public Geometry
    {
    public:
        float Width, Height;

        PlaneGeometry(float w, float h);
    };
} // namespace VREN
