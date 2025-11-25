#pragma once

#include "Base.h"
#include "Batch/BatchRenderer.h"
#include "Camera/Camera.h"
#include "Color.h"
#include "GPUScreen.h"
#include "Math/Transform.h"
#include "Math/Vector.h"
#include "Texture.h"
#include <vector>

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

        static std::shared_ptr<Shader> GetObjectShader();

        static void ResizeViewport(float newWidth, float newHeight);
        static const Viewport &GetViewport();

        /// @brief Will clear the screen with a certain color (ranging from 0-255)
        static void ClearScreen(float r = 255, float g = 255, float b = 255, float a = 255);

        static void SetActiveCamera(std::shared_ptr<Camera> camera);

        static void SubmitCube(const Transform &t, const Color &c, const Vector3 &size = {1, 1, 1});
        static void SubmitPlane(const Transform &t, const Color &c,
                                const Vector2 &size = {100, 100});
        static void SubmitLine(const Vector2 &start, const Vector2 &end, const Color &color,
                               float thickness = 1.0f, bool centered = true);
        static void SubmitLine(const Vector2 &pos, float angle, float length, const Color &color,
                               float thickness, bool centered);

        static TextureHandle GetDefaultTexture2DHandle();

    public:
        struct State
        {
            GPUScreen Screen;
            Viewport VP;

            std::shared_ptr<Camera> ActiveCamera;
            std::shared_ptr<Shader> ObjectShader, BatchShader;

            std::vector<std::unique_ptr<BatchRenderer>> Batches;

            TextureHandle DefaultTexture2D;
        };
    };
} // namespace VREN
