#pragma once

#include "Base.h"
#include "GPUScreen.h"

namespace VREN
{
    struct Viewport
    {
        float Width;
        float Height;
        inline float GetAspect() const { return Width / Height; };
    };

    class VREN_API Renderer
    {
    public:
        static void Init();
        static void BeginFrame();
        static void Render();
        static void EndFrame();

        static void ResizeViewport(float newWidth, float newHeight);
        static const Viewport &GetViewport();

        /// @brief Will clear the screen with a certain color (ranging from 0-255)
        static void ClearScreen(float r = 255, float g = 255, float b = 255, float a = 255);

    public:
        struct State
        {
            GPUScreen Screen;
            Viewport VP;
        };
    };
}