#include "Renderer.h"
#include "Core/Logger.h"
#include "Shader.h"
#include <glad/glad.h>

namespace VREN
{
    static Renderer::State state;

    void Renderer::Init()
    {
        VREN_LOG(Info, "Starting...");
        VREN_LOG(Info, "Loaded Glad context...");
        gladLoadGL();

        VREN_LOG(Info, "Creating GPU Screen...");
        {
            GPUScreenConfiguration conf;
            conf.Width = 1280;
            conf.Height = 720;
            state.Screen.Init(conf);
        }

        VREN_LOG(Info, "Finished Renderer Initalization...");
        ResizeViewport(1280, 720);
    }

    void Renderer::BeginFrame()
    {
        state.Screen.Begin();
    }

    void Renderer::Render()
    {
    }

    void Renderer::EndFrame()
    {
        state.Screen.End();

        // todo: Somewhere else?
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
}