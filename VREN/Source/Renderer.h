#pragma once

#include "Base.h"

namespace VREN
{
    class VREN_API Renderer
    {
    public:
        static void Init();
        static void Render();

        /// @brief Will clear the screen with a certain color (ranging from 0-255)
        static void ClearScreen(float r = 255, float g = 255, float b = 255, float a = 255);
    };
}