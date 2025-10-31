#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "Camera/PerspectiveCamera.h"
#include "Core/Logger.h"
#include "Math/Math.h"
#include <memory>
#include <vector>
#include <random>

std::shared_ptr<VREN::PerspectiveCamera> camera = std::make_shared<VREN::PerspectiveCamera>();

bool keys[1024] = {false};
double lastMouseX = 0.0, lastMouseY = 0.0;
double deltaX = 0.0, deltaY = 0.0;
bool firstMouse = true;

static void OnResize(GLFWwindow *_, int w, int h)
{
    VREN::Renderer::ResizeViewport(w, h);
}

static void OnKeyCallback(GLFWwindow *_, int key, int scancode, int action, int mods)
{
    if (key < 0 || key >= 1024)
        return;

    if (action == GLFW_PRESS)
        keys[key] = true;
    else if (action == GLFW_RELEASE)
        keys[key] = false;
}

static void OnMouseMove(GLFWwindow *_, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastMouseX = xpos;
        lastMouseY = ypos;
        firstMouse = false;
    }

    deltaX = xpos - lastMouseX;
    deltaY = lastMouseY - ypos;
    lastMouseX = xpos;
    lastMouseY = ypos;
}

void ProcessInput(float dt)
{
    float speed = 5.5f * dt;
    auto &pos = camera->GetPosition();

    if (keys[GLFW_KEY_W])
        pos.z -= speed;
    if (keys[GLFW_KEY_S])
        pos.z += speed;
    if (keys[GLFW_KEY_A])
        pos.x -= speed;
    if (keys[GLFW_KEY_D])
        pos.x += speed;
    if (keys[GLFW_KEY_SPACE])
        pos.y += speed;
    if (keys[GLFW_KEY_LEFT_SHIFT])
        pos.y -= speed;

    float mouseSensitivity = 0.001f;
    auto &rot = camera->GetRotation();

    rot.x += float(deltaY) * mouseSensitivity;
    rot.y += float(deltaX) * mouseSensitivity;

    if (rot.x > 89.0f * DEG_TO_RAD)
        rot.x = 89.0f* DEG_TO_RAD;
    if (rot.x < -89.0f* DEG_TO_RAD)
        rot.x = -89.0f* DEG_TO_RAD;


    camera->UpdateView();

    deltaX = 0.0;
    deltaY = 0.0;
}

std::mt19937 rng(std::random_device{}());
float RandFloat(float min, float max)
{
    std::uniform_real_distribution<float> dist(min, max);
    return dist(rng);
}
int RandInt(int min, int max)
{
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

struct CubeInstance
{
    VREN::Transform t;
    VREN::Material mat;
};

struct Window
{
    GLFWwindow *handle;
    int Width, Height;

    void Init(int width, int height, const char *title)
    {
        if (!glfwInit())
            return;

        Width = width;
        Height = height;
        handle = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!handle)
        {
            glfwTerminate();
            return;
        }

        glfwSetWindowSizeCallback(handle, OnResize);
        glfwSetKeyCallback(handle, OnKeyCallback);
        glfwSetCursorPosCallback(handle, OnMouseMove);
        glfwMakeContextCurrent(handle);
        glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

int main()
{
    Window window;
    window.Init(1920, 1080, "My Window");

    VREN::Renderer::Init();
    window.Update();
    VREN::Renderer::ResizeViewport(window.Width, window.Height);
    VREN::Renderer::SetActiveCamera(camera);

    const int cubeCount = 500;
    const float maxDistance = 50.0f;
    std::vector<CubeInstance> cubes;

    for (int i = 0; i < cubeCount; ++i)
    {
        VREN::Transform t;
        t.Position = {RandFloat(-maxDistance, maxDistance), RandFloat(-maxDistance, maxDistance), RandFloat(-maxDistance, maxDistance)};
        t.Rotation = {RandFloat(0, 360), RandFloat(0, 360), RandFloat(0, 360)};
        t.Scale = {RandFloat(0.5f, 3.0f), RandFloat(0.5f, 3.0f), RandFloat(0.5f, 3.0f)};

        VREN::Material mat;
        mat.Color.r = RandInt(0, 255);
        mat.Color.g = RandInt(0, 255);
        mat.Color.b = RandInt(0, 255);
        mat.Color.a = 255;

        cubes.push_back({t, mat});
    }

    double lastTime = glfwGetTime();

    while (!window.ShouldClose())
    {
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);
        lastTime = currentTime;

        window.Update();
        ProcessInput(deltaTime);

        VREN::Renderer::BeginFrame();
        VREN::Renderer::ClearScreen(255, 255, 255, 255);
        VREN::Renderer::RenderCube({}, {.Color = {0, 0, 0, 255}});

        for (auto &c : cubes)
            VREN::Renderer::RenderCube(c.t, c.mat);

        VREN::Renderer::EndFrame();
    }

    window.Shutdown();
    return 0;
}
