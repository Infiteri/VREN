#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "Core/Logger.h"

static void OnResize(GLFWwindow *_, int w, int h)
{
    VREN::Renderer::ResizeViewport(w, h);
}

struct Window
{
    GLFWwindow *handle;

    int Width, Height;

    void Init(int width, int height, const char *title)
    {
        if (!glfwInit())
            return;

        handle = glfwCreateWindow(width, height, title, 0, 0);
        if (!handle)
        {
            glfwTerminate();
            return;
        }

        glfwSetWindowSizeCallback(handle, OnResize);

        glfwMakeContextCurrent(handle);
    }

    bool ShouldClose()
    {
        return glfwWindowShouldClose(handle);
    }

    void Update()
    {
        glfwSwapBuffers(handle);
        glfwPollEvents();

        glfwGetWindowSize(handle, &Width, &Height);
    }

    void Shutdown()
    {
        glfwDestroyWindow(handle);
        glfwTerminate();
    }
};

int main()
{
    Window window;
    window.Init(1920, 1080, "My Window");

    VREN::Renderer::Init();
    window.Update();
    VREN::Renderer::ResizeViewport(window.Width, window.Height);

    while (!window.ShouldClose())
    {
        window.Update();

        VREN::Renderer::BeginFrame();
        VREN::Renderer::ClearScreen(255, 255, 255, 255);
        VREN::Renderer::Render();
        VREN::Renderer::EndFrame();
    }

    window.Shutdown();
    return 0;
}
