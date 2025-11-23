#pragma once

#include "Base.h"

namespace VREN
{
    // Will be loaded from some kind of texture utils. maybe in renderer
    struct TextureHandle
    {
        u32 Id = 0;
    };

    /// @brief Basic texture wrapping, maps to OpenGL texture wrapps
    enum class TextureWrap
    {
        Repeat,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder
    };

    enum class TextureFilter
    {
        Nearest,
        Linear
    };

    namespace Texture
    {
        VREN_API TextureHandle CreateTexture2DHandle(int width, int height, u8 *pixels,
                                                     u32 format = 3,
                                                     TextureWrap wrap = TextureWrap::Repeat,
                                                     TextureFilter filter = TextureFilter::Nearest);

        VREN_API TextureHandle CreateTexture2DHandle(const char *imagePath,
                                                     TextureWrap wrap = TextureWrap::Repeat,
                                                     TextureFilter filter = TextureFilter::Nearest);

        void UseHandle(const TextureHandle &handle, int level = 0);
        void DestroyHandle(const TextureHandle &handle);
    } // namespace Texture
} // namespace VREN
