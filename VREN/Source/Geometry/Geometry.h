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
        Box
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
} // namespace VREN
