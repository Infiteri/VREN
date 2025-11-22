#pragma once

#include <GLFW/glfw3.h>

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
        glfwSwapInterval(1);
        glfwMakeContextCurrent(handle);
    }

    bool ShouldClose() { return glfwWindowShouldClose(handle); }

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
