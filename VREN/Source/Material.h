#pragma once

#include "Base.h"
#include "Color.h"
#include "Shader.h"
#include "Texture.h"
#include <memory>

namespace VREN
{
    class VREN_API Material
    {
    public:
        Material();
        ~Material();

        void Use(std::shared_ptr<Shader> shader);

        void SetColor(const Color &c);
        void SetColorTextureHandle(const TextureHandle &handle);

        inline Color &GetColor() { return color; };
        inline TextureHandle &GetColorTextureHandle() { return colorTexture; };

    private:
        Color color;
        TextureHandle colorTexture;
    };
} // namespace VREN
