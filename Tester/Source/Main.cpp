#include "Camera/OrthographicCamera.h"
#include "GLFW/glfw3.h"
#include "Math/Transform.h"
#include "Renderer.h"
#include "Window.h"
#include <memory>

using namespace VREN;

static Window window;
static std::shared_ptr<OrthographicCamera> camera = std::make_shared<OrthographicCamera>(1280, 720);

static void OnResize(GLFWwindow *window, int w, int h) { Renderer::ResizeViewport(w, h); }

int main()
{
    window.Init(1280, 720, "heheh");
    glfwSetWindowSizeCallback(window.handle, OnResize);

    camera->GetPosition().z = 5;
    camera->UpdateView();
    Renderer::Init();
    Renderer::SetActiveCamera(camera);
    Renderer::ResizeViewport(window.Width, window.Height);

    while (!window.ShouldClose())
    {
        window.Update();

        Renderer::BeginFrame();
        Renderer::ClearScreen(255, 255, 255, 255);

        Renderer::SubmitCircle({.Position = {0, 0, 0}}, {0, 0, 255, 255}, 100);

        Renderer::Render();
        Renderer::EndFrame();
    }

    window.Shutdown();

    return 0;
}
