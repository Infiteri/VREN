#pragma once

#include "Base.h"
#include "Buffer/Framebuffer.h"
#include "Buffer/VertexArray.h"
#include "Shader.h"
#include <memory>

namespace VREN
{
    struct GPUScreenConfiguration
    {
        float Width, Height;
    };

    struct GPUScreen
    {
        std::unique_ptr<VertexArray> Array;
        std::unique_ptr<Framebuffer> Buffer;
        std::shared_ptr<Shader> ScreenShader;

        void Init(const GPUScreenConfiguration& config);
        void Begin();
        void End();
        void Resize(float width, float height);
    };
}