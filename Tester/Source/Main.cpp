#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "Core/Logger.h"

struct Window
{
    GLFWwindow* handle;

    void Init(int width, int height, const char* title)
    {
        if (!glfwInit())
            return;

        handle = glfwCreateWindow(width, height, title, 0, 0);
        if (!handle)
        {
            glfwTerminate();
            return;
        }

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
    while (!window.ShouldClose())
    {
        window.Update();

        VREN::Renderer::ClearScreen(255,255,255,255);
    }

    window.Shutdown();
    return 0;
}
