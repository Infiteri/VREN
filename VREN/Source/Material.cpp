#include "Material.h"
#include "Core/Logger.h"
#include "Renderer.h"
#include "Texture.h"

namespace VREN
{
    Material::Material() {}

    Material::~Material() {}

    void Material::SetColor(const Color &c) { color = c; }

    void Material::SetColorTextureHandle(const TextureHandle &handle)
    {
        Texture::DestroyHandle(colorTexture);
        colorTexture = handle;
    }

    void Material::Use(std::shared_ptr<Shader> shader)
    {
        if (!shader)
            return;

        shader->Vec4(color, "uColor");

        if (colorTexture.Id != 0)
            Texture::UseHandle(colorTexture);
        else
            Texture::UseHandle(Renderer::GetDefaultTexture2DHandle());

        shader->Int(0, "uColorTexture");
    }
} // namespace VREN
