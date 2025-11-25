#include "Renderer.h"
#include "Batch/BatchRenderer.h"
#include "Core/Logger.h"
#include "Math/Math.h"
#include "Math/Transform.h"
#include "Shader.h"
#include "Texture.h"
#include <cmath>
#include <glad/glad.h>
#include <memory>

#define CUBE_BTCH_IDX 0
#define PLNE_BTCH_IDX 1

namespace VREN
{
    static Renderer::State state;

    void Renderer::Init()
    {
        VREN_LOG(Info, "Starting Renderer...");
        gladLoadGL();

        state.Screen.Init({.Width = 1280, .Height = 720});
        state.ObjectShader = std::make_shared<Shader>("Assets/Obj.glsl");
        state.BatchShader = std::make_shared<Shader>("Assets/BatchShader.glsl");

        glEnable(GL_DEPTH_TEST);
        ResizeViewport(1280, 720);

        // setup batch Renderer
        {
            state.Batches.resize(2);
            state.Batches[CUBE_BTCH_IDX] = std::make_unique<CubeBatchRenderer>();
            state.Batches[PLNE_BTCH_IDX] = std::make_unique<PlaneBatchRenderer>();

            for (auto &b : state.Batches)
                b->Init();
        }

        {
            u8 pixels[] = {255, 255, 255, 255};
            state.DefaultTexture2D = Texture::CreateTexture2DHandle(1, 1, pixels);
        }

        VREN_LOG(Info, "Renderer initialized.");
    }

    std::shared_ptr<Shader> Renderer::GetObjectShader() { return state.ObjectShader; }

    void Renderer::BeginFrame()
    {
        state.Screen.Begin();

        for (auto &b : state.Batches)
            b->Begin();
    }

    void Renderer::Render()
    {
        state.ObjectShader->Use();

        if (state.ActiveCamera)
        {
            state.ObjectShader->Mat4(state.ActiveCamera->GetProjection(), "uProj");
            state.ObjectShader->Mat4(state.ActiveCamera->GetView(), "uView");
        }
    }

    void Renderer::EndFrame()
    {
        for (auto &b : state.Batches)
            b->End(state.ActiveCamera, state.BatchShader);

        state.Screen.End();

        FBRenderPass *pass = state.Screen.Buffer->GetRenderPass(0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, pass->Id);
        state.Screen.ScreenShader->Int(0, "uScreenTexture");
        state.Screen.ScreenShader->Use();
        state.Screen.Array->Bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void Renderer::ResizeViewport(float newWidth, float newHeight)
    {
        state.VP.Width = newWidth;
        state.VP.Height = newHeight;
        state.Screen.Resize(newWidth, newHeight);

        if (state.ActiveCamera)
            state.ActiveCamera->SetAspect(newWidth, newHeight);

        glViewport(0, 0, newWidth, newHeight);
    }

    const Viewport &Renderer::GetViewport() { return state.VP; }

    void Renderer::ClearScreen(float r, float g, float b, float a)
    {
        glClearColor(r / 255, g / 255, b / 255, a / 255);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Renderer::SetActiveCamera(std::shared_ptr<Camera> camera) { state.ActiveCamera = camera; }

    void Renderer::SubmitCube(const Transform &t, const Color &c, const Vector3 &size)
    {
        Transform t2 = t;
        if (size != Vector3(1, 1, 1))
            t2.Scale *= size;

        state.Batches[CUBE_BTCH_IDX]->Submit(t2, c);
    }

    void Renderer::SubmitPlane(const Transform &t, const Color &c, const Vector2 &size)
    {
        Transform t2 = t;
        if (size != Vector2(1, 1))
            t2.Scale *= Vector3(size.x, size.y, 1);

        state.Batches[PLNE_BTCH_IDX]->Submit(t2, c);
    }

    void Renderer::SubmitLine(const Vector2 &start, const Vector2 &end, const Color &color,
                              float thickness, bool centered)
    {
        Vector2 dir = end - start;
        float length = sqrt(dir.x * dir.x + dir.y * dir.y);
        float angle = atan2(dir.y, dir.x);

        Vector2 mid = (start + end) * 0.5f;

        // Create transform
        Transform t;
        t.Position = centered ? Vector3(mid.x, mid.y, 0) : Vector3(start.x, start.y, 0);

        t.Rotation = Vector3(0, 0, angle * 180.0f / 3.14159265f);

        Vector2 size(length, thickness);

        if (!centered)
        {
            float dx = (length * 0.5f) * cos(angle);
            float dy = (length * 0.5f) * sin(angle);
            t.Position.x += dx;
            t.Position.y += dy;
        }

        SubmitPlane(t, color, size);
    }

    void Renderer::SubmitLine(const Vector2 &pos, float angle, float length, const Color &color,
                              float thickness, bool centered)
    {
        Transform t;
        float rad = angle * DEG_TO_RAD;

        if (centered)
        {
            t.Position = Vector3(pos.x, pos.y, 0);
        }
        else
        {
            float dx = (length * 0.5f) * cos(rad);
            float dy = (length * 0.5f) * sin(rad);
            t.Position = Vector3(pos.x + dx, pos.y + dy, 0);
        }

        t.Rotation = Vector3(0, 0, rad * 180.0f / 3.14159265f);

        SubmitPlane(t, color, Vector2(length, thickness));
    }

    TextureHandle Renderer::GetDefaultTexture2DHandle() { return state.DefaultTexture2D; }
} // namespace VREN
