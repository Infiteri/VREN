
#pragma once
#include "Camera/Camera.h"
#include "Color.h"
#include "Math/Matrix.h"
#include <memory>

namespace VREN
{
    class VertexArray;
    class PerspectiveCamera;
    class Shader;
    class Transform;

    struct InstanceData
    {
        Matrix4 model;
        float color[4];
    };

    class BatchRenderer
    {
    public:
        static const int MaxInstances = 10001;

        void Begin();
        void Submit(const Transform &t, const Color &color);
        void End(std::shared_ptr<Camera> cam, std::shared_ptr<Shader> shader);
        void Shutdown();

    protected:
        InstanceData instanceData[MaxInstances];
        int instanceCount = 0;
        unsigned int instanceVBO = 0;

        std::unique_ptr<VertexArray> geometryVAO;
        int indexCount = 0;
    };

    class CubeBatchRenderer : public BatchRenderer
    {
    public:
        void Init();
    };
} // namespace VREN
