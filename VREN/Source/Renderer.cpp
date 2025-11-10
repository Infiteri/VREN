#include "Renderer.h"
#include "Core/Logger.h"
#include "Shader.h"
#include "Mesh.h"
#include "Camera/PerspectiveCamera.h"
#include <glad/glad.h>

namespace VREN
{
    static Renderer::State state;
    static std::shared_ptr<Shader> TestShader;
    static std::shared_ptr<VertexArray> TestVAO;
    static Mesh mesh;

    void Renderer::Init()
    {
        VREN_LOG(Info, "Starting Renderer...");
        gladLoadGL();

        GPUScreenConfiguration conf;
        conf.Width = 1280;
        conf.Height = 720;
        state.Screen.Init(conf);

        TestShader = std::make_shared<Shader>("Assets/Obj.glsl");

        mesh.Init();

        glEnable(GL_DEPTH_TEST);
        ResizeViewport(1280, 720);
        VREN_LOG(Info, "Renderer initialized (Cube).");
    }

    void Renderer::BeginFrame()
    {
        state.Screen.Begin();
    }

    void Renderer::Render()
    {
        TestShader->Use();

        if (state.ActiveCamera)
        {
            TestShader->Mat4(state.ActiveCamera->GetProjection(), "uProj");
            TestShader->Mat4(state.ActiveCamera->GetView(), "uView");
        }

        mesh.Render();
    }

    void Renderer::EndFrame()
    {
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

    const Viewport &Renderer::GetViewport()
    {
        return state.VP;
    }

    void Renderer::ClearScreen(float r, float g, float b, float a)
    {
        glClearColor(r / 255, g / 255, b / 255, a / 255);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Renderer::SetActiveCamera(std::shared_ptr<Camera> camera)
    {
        state.ActiveCamera = camera;
    }
}
