#include "Camera/PerspectiveCamera.h"
#include "Color.h"
#include "Geometry/Geometry.h"
#include "Math/Vector.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Texture.h"
#include "Window.h"
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <vector>

std::shared_ptr<VREN::PerspectiveCamera> camera = std::make_shared<VREN::PerspectiveCamera>();

static std::vector<VREN::Vector3> positions;
static std::vector<VREN::Color> colors;
static int cubes = 300000;
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
    float speed = 3.5f * dt;
    auto &pos = camera->GetPosition();

    if (keys[GLFW_KEY_LEFT_SHIFT])
        speed *= 3;

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
    VREN::Renderer::ResizeViewport(window.Width, window.Height);
    VREN::Renderer::SetActiveCamera(camera);

    mesh.Init();
    mesh.SetGeometry(std::make_shared<VREN::BoxGeometry>(1, 1, 1));
    mesh.GetMaterial().SetColor({255, 255, 255, 255});

    u8 pixels[] = {0, 125, 0, 255};
    mesh.GetMaterial().SetColorTextureHandle(
        VREN::Texture::CreateTexture2DHandle("Assets/negx.jpg"));

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

        mesh.Render();

        VREN::Renderer::EndFrame();
    }

    window.Shutdown();
    return 0;
}
