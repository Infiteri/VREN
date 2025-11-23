#pragma once

#include "Base.h"
#include "Buffer/VertexArray.h"
#include "Geometry/Geometry.h"
#include "Material.h"
#include "Math/Transform.h"
#include "Texture.h"
#include <memory>

namespace VREN
{
    class VREN_API Mesh
    {
    public:
        Mesh();
        ~Mesh();

        void Init();
        void Render();

        void SetGeometry(std::shared_ptr<Geometry> newGeometry);
        void SetMaterial(const Material &mat);

        inline Material &GetMaterial() { return material; };
        inline Transform &GetTrasnform() { return transform; };

    private:
        std::unique_ptr<VertexArray> array;
        std::shared_ptr<Geometry> geometry;
        Material material;
        Transform transform;

    private:
        void DestroyAndInitArray();

        // todo: Dont really like this flag, make some changes in the DestroyAndInitArray function
        // so that it will destroy and init an empty array if no geometry, idc rn tho
        bool isInit = false;
    };
} // namespace VREN
