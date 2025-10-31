#pragma once

#include "Math/Transform.h"
#include "Math/Matrix.h"
#include "Shader.h"
#include "Camera/PerspectiveCamera.h"
#include "Material.h"
#include "Buffer/VertexArray.h"
#include <memory>

namespace VREN
{
    struct GPUMaterial
    {
        float r, g, b, a;
    };

    class BatchRenderer
    {
    public:
        virtual ~BatchRenderer() = default;

        virtual void Init() = 0;
        void Begin();
        void Submit(const Transform &t, const Material &mat);
        void End(std::shared_ptr<PerspectiveCamera> cam, std::shared_ptr<Shader> shader);
        void Flush(std::shared_ptr<PerspectiveCamera> cam, std::shared_ptr<Shader> shader);
        void Shutdown();

    protected:
        struct InstanceData
        {
            Matrix4 model;
            GPUMaterial material;
        };

        static const int MaxInstances = 10000;

        InstanceData instanceData[MaxInstances];
        int instanceCount = 0;

        std::unique_ptr<VertexArray> geometryVAO;
        unsigned int instanceVBO = 0;
        int indexCount = 0;
    };

    class CubeBatchRenderer : public BatchRenderer
    {
    public:
        void Init() override;
    };
}
