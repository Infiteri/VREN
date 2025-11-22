#pragma once

#include "Base.h"
#include "Color.h"
#include "Shader.h"
#include <memory>

namespace VREN
{
    class VREN_API Material
    {
    public:
        Material();
        ~Material();

        void Use(std::shared_ptr<Shader> shader);

        inline Color &GetColor() { return color; };

        void SetColor(const Color &c);

    private:
        Color color;
    };
} // namespace VREN
