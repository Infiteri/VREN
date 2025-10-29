#pragma once

#include "Base.h"
#include <vector>

namespace VREN
{
    /// @brief Render pass texture type
    enum class RPTextureType
    {
        Depth,
        Rgb,
        Rgba8
    };

    struct FBRenderPass
    {
        RPTextureType Type;
        u32 Index, Id;
    };

    struct FramebufferConfiguration
    {
        float Width, Height;

        std::vector<FBRenderPass> Passes;
    };

    class VREN_API Framebuffer
    {
    public:
        Framebuffer(const FramebufferConfiguration &cfg);
        ~Framebuffer();

        void Bind();
        void Unbind();
        void Create();
        void Resize(float w, float h);

        FBRenderPass *GetRenderPass(int index);

        inline u32 GetID() const { return id; };

    private:
        u32 id;
        FramebufferConfiguration state;

    private:
        void DestroyForResize();
    };
} // namespace Core