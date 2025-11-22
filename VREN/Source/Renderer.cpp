#include "Renderer.h"
#include "Camera/PerspectiveCamera.h"
#include "Core/Logger.h"
#include "Math/Transform.h"
#include "Mesh.h"
#include "Shader.h"
#include <cstdlib>
#include <ctime>
#include <glad/glad.h>
#include <memory>

namespace VREN
{
    static Renderer::State state;

    void Renderer::Init()
    {
        VREN_LOG(Info, "Starting Renderer...");
        gladLoadGL();

        GPUScreenConfiguration conf;
        conf.Width = 1280;
        conf.Height = 720;
        state.Screen.Init(conf);

        state.ObjectShader = std::make_shared<Shader>("Assets/Obj.glsl");
        state.BatchShader = std::make_shared<Shader>("Assets/BatchShader.glsl");

        glEnable(GL_DEPTH_TEST);
        ResizeViewport(1280, 720);

        state.CubeRenderer.Init();

        VREN_LOG(Info, "Renderer initialized.");
    }

    std::shared_ptr<Shader> Renderer::GetObjectShader() { return state.ObjectShader; }

    void Renderer::BeginFrame()
    {
        state.Screen.Begin();

        state.CubeRenderer.Begin();
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
        state.CubeRenderer.End(std::dynamic_pointer_cast<PerspectiveCamera>(state.ActiveCamera),
                               state.BatchShader);
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

    void Renderer::SubmitCube(const Transform &t, const Color &c)
    {
        state.CubeRenderer.Submit(t, c);
    }
} // namespace VREN
