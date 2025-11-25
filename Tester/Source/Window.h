
#pragma once

#include <GLFW/glfw3.h>

struct Window
{
    GLFWwindow *handle;
    int Width, Height;
    float DeltaTime;

    void Init(int width, int height, const char *title);
    bool ShouldClose();
    void Update();
    void Shutdown();

private:
    double lastFrameTime;
};
