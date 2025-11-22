#include "Material.h"

namespace VREN
{
    Material::Material() {}

    Material::~Material() {}

    void Material::SetColor(const Color &c) { color = c; }

    void Material::Use(std::shared_ptr<Shader> shader)
    {
        if (!shader)
            return;

        shader->Vec4(color, "uColor");
    }
} // namespace VREN
