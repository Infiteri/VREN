#include "GPUScreen.h"
#include <glad/glad.h>

namespace VREN
{
    void GPUScreen::Init(const GPUScreenConfiguration &config)
    {
        // array
        {
            float screenQuadVertices[] = {
                -1.0f, 1.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f, 0.0f,

                -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};

            Array = std::make_unique<VertexArray>();
            Array->GenerateVertexBuffer(screenQuadVertices, sizeof(screenQuadVertices));
            Array->GetVertexBuffer()->AddLayout(0, 0, 2);
            Array->GetVertexBuffer()->AddLayout(1, 2, 2);
        }

        // shader
        {
            ScreenShader = std::make_shared<Shader>("Screen.glsl");
        }

        // buffer
        {
            FramebufferConfiguration config;
            config.Width = config.Width;
            config.Height = config.Height;
            config.Passes.push_back({.Type = RPTextureType::Rgb});   // color
            config.Passes.push_back({.Type = RPTextureType::Depth}); // depth
            Buffer = std::make_unique<Framebuffer>(config);
        }
    }

    void GPUScreen::Begin()
    {
        Buffer->Bind();
        glEnable(GL_DEPTH_TEST);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void GPUScreen::End()
    {
        Buffer->Unbind();
        glDisable(GL_DEPTH_TEST);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void GPUScreen::Resize(float width, float height)
    {
        if (Buffer)
            Buffer->Resize(width, height);
    }
}