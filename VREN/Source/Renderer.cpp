#include "Renderer.h"
#include "Core/Logger.h"
#include <glad/glad.h>

namespace VREN
{
    void Renderer::Init()
    {
        gladLoadGL();
    }

    void Renderer::Render()
    {
    }
    
    void Renderer::ClearScreen(float r, float g, float b, float a)
    {
        glClearColor(r / 255, g / 255, b / 255, a / 255);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}