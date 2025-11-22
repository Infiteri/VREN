#pragma once

#include "Base.h"

namespace VREN
{
    class VREN_API Color
    {
    public:
        float r, g, b, a;
        Color(float r = 255, float g = 255, float b = 255, float a = 255);
        Color Normalized() const;
    };
} // namespace VREN
