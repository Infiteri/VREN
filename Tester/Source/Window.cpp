
#include "Window.h"

void Window::Init(int width, int height, const char *title)
{
    if (!glfwInit())
        return;

    handle = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!handle)
    {
        glfwTerminate();
        return;
    }

    glfwSwapInterval(1);
    glfwMakeContextCurrent(handle);

    glfwGetWindowSize(handle, &Width, &Height);
    lastFrameTime = glfwGetTime();
    DeltaTime = 0.0f;
}

bool Window::ShouldClose() { return glfwWindowShouldClose(handle); }

void Window::Update()
{
    double currentTime = glfwGetTime();
    DeltaTime = float(currentTime - lastFrameTime);
    lastFrameTime = currentTime;

    glfwSwapBuffers(handle);
    glfwPollEvents();
    glfwGetWindowSize(handle, &Width, &Height);
}

void Window::Shutdown()
{
    glfwDestroyWindow(handle);
    glfwTerminate();
}
