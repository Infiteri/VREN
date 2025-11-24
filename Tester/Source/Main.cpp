#include "Camera/OrthographicCamera.h"
#include "Camera/PerspectiveCamera.h"
#include "Color.h"
#include "GUI.h"
#include "Geometry/Geometry.h"
#include "Math/Vector.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Texture.h"
#include "Window.h"
#include "imgui.h"
#include <GLFW/glfw3.h>
#include <memory>

float width = 200, height = 200;

std::shared_ptr<VREN::OrthographicCamera> camera =
    std::make_shared<VREN::OrthographicCamera>(1280, 720);

static VREN::Mesh mesh;

// Track key states
bool keys[1024] = {false};

static void OnResize(GLFWwindow *_, int w, int h) { VREN::Renderer::ResizeViewport(w, h); }

static void OnKeyCallback(GLFWwindow *_, int key, int scancode, int action, int mods)
{
    if (key < 0 || key >= 1024)
        return;

    if (action == GLFW_PRESS)
        keys[key] = true;
    else if (action == GLFW_RELEASE)
        keys[key] = false;
}

void ProcessInput(float dt)
{
    float speed = 30.5f * dt;
    auto &pos = camera->GetPosition();

    if (keys[GLFW_KEY_LEFT_SHIFT])
        speed *= 30;

    if (keys[GLFW_KEY_W])
        pos.z += speed; // forward
    if (keys[GLFW_KEY_S])
        pos.z -= speed; // backward
    if (keys[GLFW_KEY_A])
        pos.x += speed; // left
    if (keys[GLFW_KEY_D])
        pos.x -= speed; // right
    if (keys[GLFW_KEY_SPACE])
        pos.y -= speed; // up
    if (keys[GLFW_KEY_LEFT_CONTROL])
        pos.y += speed; // down

    camera->UpdateView();
}

int main()
{
    Window window;
    window.Init(1920, 1080, "My Window");

    glfwSetWindowSizeCallback(window.handle, OnResize);
    glfwSetKeyCallback(window.handle, OnKeyCallback);

    VREN::Renderer::Init();
    window.Update();
    VREN::Renderer::SetActiveCamera(camera);
    VREN::Renderer::ResizeViewport(window.Width, window.Height);

    mesh.Init();
    mesh.GetTrasnform().Position.z = -5;
    mesh.SetGeometry(std::make_shared<VREN::PlaneGeometry>(250, 250));
    mesh.GetMaterial().SetColor({255, 255, 255, 255});

    mesh.GetMaterial().SetColorTextureHandle(
        VREN::Texture::CreateTexture2DHandle("Assets/negx.jpg"));

    double lastTime = glfwGetTime();

    ImGuiInit(&window);

    while (!window.ShouldClose())
    {
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);
        lastTime = currentTime;

        ProcessInput(deltaTime);

        window.Update();

        VREN::Renderer::BeginFrame();
        VREN::Renderer::ClearScreen(255, 255, 255, 255);
        VREN::Renderer::Render();

        mesh.Render();

        VREN::Renderer::SubmitPlane({.Position{-500, 0, 0}}, {255, 0, 0, 255}, {500, 50});

        ImGuiBegin();
        ImGui::Begin("A");

        bool changeW = ImGui::DragFloat("W", &width, 0.01, 0.0);
        bool changeH = ImGui::DragFloat("H", &height, 0.01, 0.0);

        if (changeW || changeH)
        {
            mesh.SetGeometry(std::make_shared<VREN::PlaneGeometry>(width, height));
        }

        ImGui::End();
        ImGuiEnd();

        VREN::Renderer::EndFrame();
    }

    window.Shutdown();
    return 0;
}
