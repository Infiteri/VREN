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

VREN::Vector2 lineStart(0.0f, 0.0f);
VREN::Vector2 lineEnd(100.0f, 100.0f);

VREN::Color lineColor(255, 0, 0, 255);

float lineThickness = 5.0f;
bool lineCentered = false;

VREN::Vector2 angledPos(200.0f, 0.0f);
float angledAngle = 45.0f; // degrees
float angledLength = 150.0f;
VREN::Color angledColor(0, 0, 255, 255);
float angledThickness = 4.0f;
bool angledCentered = false;

std::shared_ptr<VREN::OrthographicCamera> camera =
    std::make_shared<VREN::OrthographicCamera>(1280, 720);

static Window window;
static VREN::Mesh mesh;

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
        pos.z += speed;
    if (keys[GLFW_KEY_S])
        pos.z -= speed;
    if (keys[GLFW_KEY_A])
        pos.x += speed;
    if (keys[GLFW_KEY_D])
        pos.x -= speed;
    if (keys[GLFW_KEY_SPACE])
        pos.y -= speed;
    if (keys[GLFW_KEY_LEFT_CONTROL])
        pos.y += speed;

    camera->UpdateView();
}

static void InitAll()
{
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

    ImGuiInit(&window);
}

static void RenderScene()
{
    VREN::Renderer::BeginFrame();
    VREN::Renderer::ClearScreen(255, 255, 255, 255);
    VREN::Renderer::Render();

    mesh.Render();

    // Normal line
    VREN::Renderer::SubmitLine(lineStart, lineEnd, lineColor, lineThickness, lineCentered);

    // Angle-based line
    VREN::Renderer::SubmitLine(angledPos, angledAngle, angledLength, angledColor, angledThickness,
                               angledCentered);
}

static void RenderGUI()
{
    ImGuiBegin();
    ImGui::Begin("A");

    bool changeW = ImGui::DragFloat("W", &width, 0.01, 0.0);
    bool changeH = ImGui::DragFloat("H", &height, 0.01, 0.0);

    if (changeW || changeH)
    {
        mesh.SetGeometry(std::make_shared<VREN::PlaneGeometry>(width, height));
    }

    ImGui::End();

    ImGui::Begin("Line Controls");

    // -----------------------
    // Line 1
    // -----------------------
    ImGui::Text("Line 1 (Start -> End)");
    ImGui::DragFloat2("Start", &lineStart.x, 0.1f);
    ImGui::DragFloat2("End", &lineEnd.x, 0.1f);

    ImGui::DragFloat("Thickness", &lineThickness, 0.1f, 0.1f, 500.0f);
    ImGui::Checkbox("Centered##1", &lineCentered);

    // integer color (convert to/from float)
    {
        float col[4] = {lineColor.r / 255.0f, lineColor.g / 255.0f, lineColor.b / 255.0f,
                        lineColor.a / 255.0f};

        if (ImGui::ColorEdit4("Color##1", col))
        {
            lineColor.r = int(col[0] * 255.0f);
            lineColor.g = int(col[1] * 255.0f);
            lineColor.b = int(col[2] * 255.0f);
            lineColor.a = int(col[3] * 255.0f);
        }
    }

    ImGui::Separator();

    // -----------------------
    // Line 2
    // -----------------------
    ImGui::Text("Line 2 (Angle + Length)");

    ImGui::DragFloat2("Position##2", &angledPos.x, 0.1f);
    ImGui::DragFloat("Angle (deg)", &angledAngle, 1.0f);
    ImGui::DragFloat("Length", &angledLength, 0.5f, 1.0f, 2000.0f);

    ImGui::DragFloat("Thickness##2", &angledThickness, 0.1f, 0.1f, 500.0f);
    ImGui::Checkbox("Centered##2", &angledCentered);

    // integer color (convert to/from float)
    {
        float col2[4] = {angledColor.r / 255.0f, angledColor.g / 255.0f, angledColor.b / 255.0f,
                         angledColor.a / 255.0f};

        if (ImGui::ColorEdit4("Color##2", col2))
        {
            angledColor.r = int(col2[0] * 255.0f);
            angledColor.g = int(col2[1] * 255.0f);
            angledColor.b = int(col2[2] * 255.0f);
            angledColor.a = int(col2[3] * 255.0f);
        }
    }

    ImGui::End();
    ImGuiEnd();
}

int main()
{
    InitAll();

    while (!window.ShouldClose())
    {
        window.Update();

        ProcessInput(window.DeltaTime);

        RenderScene();

        RenderGUI();

        VREN::Renderer::EndFrame();
    }

    window.Shutdown();
    return 0;
}
