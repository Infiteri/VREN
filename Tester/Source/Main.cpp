#include "Camera/PerspectiveCamera.h"
#include "Color.h"
#include "Core/Logger.h"
#include "Math/Transform.h"
#include "Math/Vector.h"
#include "Mesh.h"
#include "Renderer.h"
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <vector>

std::shared_ptr<VREN::PerspectiveCamera> camera = std::make_shared<VREN::PerspectiveCamera>();

static std::vector<VREN::Vector3> positions;
static std::vector<VREN::Color> colors;
static int cubes = 30000;

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
    float speed = 2.5f * dt;
    auto &pos = camera->GetPosition();

    if (keys[GLFW_KEY_W])
        pos.z -= speed; // forward
    if (keys[GLFW_KEY_S])
        pos.z += speed; // backward
    if (keys[GLFW_KEY_A])
        pos.x -= speed; // left
    if (keys[GLFW_KEY_D])
        pos.x += speed; // right
    if (keys[GLFW_KEY_SPACE])
        pos.y += speed; // up
    if (keys[GLFW_KEY_LEFT_SHIFT])
        pos.y -= speed; // down

    camera->UpdateView();
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
        glfwSwapInterval(1);

        glfwSetWindowSizeCallback(handle, OnResize);
        glfwSetKeyCallback(handle, OnKeyCallback);

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

int main()
{
    Window window;
    window.Init(1920, 1080, "My Window");

    VREN::Renderer::Init();
    window.Update();
    VREN::Renderer::ResizeViewport(window.Width, window.Height);
    VREN::Renderer::SetActiveCamera(camera);

    positions.resize(cubes);
    colors.resize(cubes);

    std::srand((unsigned int)std::time(nullptr));

    for (int i = 0; i < cubes; i++)
    {
        positions[i] = {(std::rand() % 4000 - 2000) / 100.0f, (std::rand() % 4000 - 2000) / 100.0f,
                        (std::rand() % 4000 - 2000) / 100.0f};

        colors[i] = VREN::Color(float(std::rand() % 256), float(std::rand() % 256),
                                float(std::rand() % 256), 255.0f);
    }

    double lastTime = glfwGetTime();

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

        for (int i = 0; i < cubes; i++)
        {
            VREN::Renderer::SubmitCube(VREN::Transform({.Position = positions[i]}), colors[i]);
        }

        VREN::Renderer::EndFrame();
    }

    window.Shutdown();
    return 0;
}
