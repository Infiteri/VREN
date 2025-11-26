
#include "Camera/OrthoMovement.h"
#include "Camera/OrthographicCamera.h"
#include "GLFW/glfw3.h"
#include "GUI.h"
#include "Math/Transform.h"
#include "Renderer.h"
#include "Window.h"
#include "imgui.h"
#include <iostream>
#include <memory>
#include <vector>

using namespace VREN;

struct Node
{
    Vector2 pos;
    Vector2 prevPos;
    Vector2 force;
    float mass;
    bool fixed;
};

struct Spring
{
    int a, b;
    float restLength;
    float k;
};

std::vector<Node> nodes;
std::vector<Spring> springs;

int GRID_W = 10;
int GRID_H = 10;
float SPACING = 10.8f;

float SPRING_STIFFNESS = 40.0f;
float DAMPING = 0.98f;
Vector2 GRAVITY = {0, 91.8};

void AddSpring(int a, int b)
{
    Spring s;
    s.a = a;
    s.b = b;
    s.restLength = (nodes[a].pos - nodes[b].pos).Length();
    s.k = SPRING_STIFFNESS;
    springs.push_back(s);
}

void BuildGrid()
{
    nodes.clear();
    springs.clear();

    for (int y = 0; y < GRID_H; y++)
    {
        for (int x = 0; x < GRID_W; x++)
        {
            Node n;
            n.pos = {x * SPACING, y * SPACING};
            n.prevPos = n.pos;
            n.force = {0, 0};
            n.mass = 1.0f;
            n.fixed = (y == GRID_H - 1);
            nodes.push_back(n);
        }
    }

    auto idx = [&](int x, int y) { return y * GRID_W + x; };

    for (int y = 0; y < GRID_H; y++)
    {
        for (int x = 0; x < GRID_W; x++)
        {
            int i = idx(x, y);
            if (x < GRID_W - 1)
                AddSpring(i, idx(x + 1, y));
            if (y < GRID_H - 1)
                AddSpring(i, idx(x, y + 1));
            if (x < GRID_W - 1 && y < GRID_H - 1)
                AddSpring(i, idx(x + 1, y + 1));
            if (x > 0 && y < GRID_H - 1)
                AddSpring(i, idx(x - 1, y + 1));
        }
    }
}

void Simulate(float dt)
{
    for (auto &n : nodes)
        n.force = {0, 0};
    for (auto &n : nodes)
        if (!n.fixed)
            n.force = n.force + GRAVITY * n.mass;

    for (auto &s : springs)
    {
        Node &A = nodes[s.a];
        Node &B = nodes[s.b];
        Vector2 d = B.pos - A.pos;
        float dist = d.Length();
        float stretch = dist - s.restLength;
        if (dist < 0.0001f)
            continue;
        Vector2 dir = d / dist;
        Vector2 force = dir * (stretch * s.k);
        if (!A.fixed)
            A.force = A.force + force;
        if (!B.fixed)
            B.force = B.force - force;
    }

    for (auto &n : nodes)
    {
        if (n.fixed)
            continue;
        Vector2 temp = n.pos;
        Vector2 velocity = (n.pos - n.prevPos) * DAMPING;
        n.pos = n.pos + velocity + n.force * (dt * dt);
        n.prevPos = temp;
    }
}

void Draw()
{
    for (auto &s : springs)
    {
        Vector2 a = nodes[s.a].pos;
        Vector2 b = nodes[s.b].pos;
        Renderer::SubmitLine({a.x, a.y}, {b.x, b.y}, {0, 255, 255, 255});
    }

    for (auto &n : nodes)
    {
        Transform t;
        t.Position = {n.pos.x, n.pos.y, 0};
        Renderer::SubmitCircle(t, {0, 0, 255, 255}, 0.1f);
    }
}

static Window window;
static std::shared_ptr<OrthographicCamera> camera = std::make_shared<OrthographicCamera>(1280, 720);

OrthoMovement movement(camera);

static void OnResize(GLFWwindow *win, int w, int h) { Renderer::ResizeViewport(w, h); }

static void UpdateCameraMovement();

int main()
{
    window.Init(1280, 720, "Spring Simulation");
    glfwSetWindowSizeCallback(window.handle, OnResize);
    camera->SetFar(10000.0f);

    camera->GetPosition().z = 20;
    camera->UpdateView();

    Renderer::Init();
    Renderer::SetActiveCamera(camera);

    ImGuiInit(&window);

    BuildGrid();

    float lastTime = glfwGetTime();

    while (!window.ShouldClose())
    {
        window.Update();
        float now = glfwGetTime();
        float dt = now - lastTime;
        lastTime = now;

        UpdateCameraMovement();

        Simulate(dt);

        Renderer::BeginFrame();
        Renderer::ClearScreen(0, 0, 0, 255);

        Draw();

        Renderer::Render();
        Renderer::EndFrame();

        ImGuiBegin();

        ImGui::Begin("Simulation Controls");
        ImGui::DragInt("Grid Width", &GRID_W, 1);
        ImGui::DragInt("Grid Height", &GRID_H, 1);
        ImGui::DragFloat("Spacing", &SPACING, 0.1f);
        ImGui::DragFloat("Spring Stiffness", &SPRING_STIFFNESS, 1.0f);
        ImGui::DragFloat("Damping", &DAMPING, 0.01f);
        ImGui::DragFloat("Gravity Y", &GRAVITY.y, 1.0f);
        if (ImGui::Button("Restart Simulation"))
            BuildGrid();

        if (ImGui::DragFloat("CamX", &camera->GetPosition().x, 0.01) ||
            ImGui::DragFloat("CamY", &camera->GetPosition().y, 0.01))
            camera->UpdateView();
        ImGui::End();

        ImGuiEnd();
    }

    window.Shutdown();
    return 0;
}

void UpdateCameraMovement()
{
    OrthoMovementFrameData data;
    data.Up = window.GetKey(GLFW_KEY_W);
    data.Down = window.GetKey(GLFW_KEY_S);
    data.Left = window.GetKey(GLFW_KEY_A);
    data.Right = window.GetKey(GLFW_KEY_D);
    data.DeltaTime = window.DeltaTime;
    data.FastMove = window.GetKey(GLFW_KEY_LEFT_SHIFT);
    data.FastSpeed = 1000.0f;
    data.NormalSpeed = 500.0f;

    data.ZoomScalar = window.GetKey(GLFW_KEY_LEFT_CONTROL) ? -1 : 1;
    data.UpdateZoom = window.GetKey(GLFW_KEY_R);

    data.NormalZoom = 0.05;

    movement.Update(data);
}
