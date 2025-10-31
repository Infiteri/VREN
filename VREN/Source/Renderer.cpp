#include "Renderer.h"
#include "Core/Logger.h"
#include "Shader.h"
#include "Camera/PerspectiveCamera.h"
#include <glad/glad.h>

namespace VREN
{
    static Renderer::State state;
    static std::shared_ptr<Shader> ObjectShader;

    void Renderer::Init()
    {
        VREN_LOG(Info, "Starting Renderer...");
        gladLoadGL();

        GPUScreenConfiguration conf;
        conf.Width = 1280;
        conf.Height = 720;
        state.Screen.Init(conf);

        state.CubeBatch = std::make_shared<CubeBatchRenderer>();
        state.CubeBatch->Init();

        ObjectShader = std::make_shared<Shader>("Obj.glsl");

        glEnable(GL_DEPTH_TEST);
        ResizeViewport(1280, 720);
        VREN_LOG(Info, "Renderer initialized (Cube).");
    }

    void Renderer::BeginFrame()
    {
        state.Screen.Begin();
        state.CubeBatch->Begin();
    }

    void Renderer::Render()
    {
        ObjectShader->Use();

        if (state.ActiveCamera)
        {
            ObjectShader->Mat4(state.ActiveCamera->GetProjection(), "uProj");
            ObjectShader->Mat4(Matrix4::Invert(state.ActiveCamera->GetView()), "uView");
        }
    }

    void Renderer::EndFrame()
    {
        state.CubeBatch->End(std::static_pointer_cast<PerspectiveCamera>(state.ActiveCamera), ObjectShader);
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

    void Renderer::RenderCube(const Transform &t, const Material &mat)
    {
        state.CubeBatch->Submit(t, mat);
    }
}
