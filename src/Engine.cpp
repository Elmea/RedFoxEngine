#include "Engine.hpp"

#define MEMORY_IMPLEMENTATION
#include "MyMemory.hpp"

#define REDFOXMATHS_IMPLEMENTATION
#include "RedfoxMaths.hpp"

using namespace RedFoxEngine;

void Engine::ObjModelPush(const char *path)
{
    if (ParseModel(&m_models[m_modelCount++].obj, path))
        __debugbreak();
}

using namespace RedFoxMaths;

void Engine::InitIMGUI()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    m_ImGuiIO = &ImGui::GetIO(); (void)m_ImGuiIO;
    m_ImGuiIO->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    m_ImGuiIO->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    // TODO: Necessary backend to support multi-viewport
    // m_ImGuiIO->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    ImGui_ImplWin32_Init(m_platform.m_window);
    ImGui_ImplOpenGL3_Init("#version 450");
}

bool Engine::isRunning()
{
    return(m_platform.m_running);
}

void Engine::StartTime()
{
    m_startingTime = m_platform.GetTimer();
}

Engine::Engine(int width, int height) :
    m_editorCamera(projectionType::PERSPECTIVE, width / (f32)height)
{
    memset(this, 0, sizeof(Engine)); // TODO formalize this in a C++ way

    new (&m_platform) Platform(width, height);

    m_arenaAllocator = InitVirtualMemory(1 * GigaByte);
    m_tempAllocator = InitVirtualMemory(1 * GigaByte);
    IncreaseTotalCapacity(&m_arenaAllocator, 1 * MegaByte);
    m_platform.GetWindowDimension();
    m_graphics.InitGraphics(&m_tempAllocator);
    InitIMGUI();

    m_editorCamera = Camera(projectionType::PERSPECTIVE, width / (f32)height);
    m_editorCamera.position = Float3(0.0f, 0.0f, 4.0f);

    {//TODO save/load scene graph after creating this data inside the engine editor
        m_models = (Model*)MyMalloc(&m_arenaAllocator, sizeof(Model) * 1000);
        ObjModelPush("ts_bot912.obj");
        m_gameObjects = (GameObject*)MyMalloc(&m_arenaAllocator, sizeof(GameObject) * 1000);

        for (int i = 0; i < (int)m_modelCount; i++)
        {
            m_graphics.InitModel(&m_models[i]);
        }
        m_gameObjectCount = 3;
        for (int i = 0; i < (int)m_gameObjectCount; i++)
        {
            m_gameObjects[i].parent = nullptr;
            m_gameObjects[i].model = m_models;
        }

        m_gameObjects[1].parent = &m_gameObjects[0];
        m_gameObjects[1].position = {};
        m_gameObjects[1].scale = 0.5;
        m_gameObjects[2].parent = nullptr;
        m_gameObjects[2].position = { 2, 1, 0 };
        m_gameObjects[2].scale = 0.5;
        
        m_gameObjects[0].name = "Entity";
        m_gameObjects[1].name = "Another entity";
        m_gameObjects[2].name = "Perfect entity";

    }
    m_input = {};
    m_dc = GetDC(m_platform.m_window);
    UpdateGame = m_platform.LoadGameLibrary("UpdateGame", "game.dll", m_gameLibrary, &m_lastTime, nullptr);
    StartTime();
}

void Engine::ProcessInputs()
{
    m_time = Platform::GetTimer();

    int mouseX = m_input.mouseXPosition;
    int mouseY = m_input.mouseYPosition;
    m_platform.MessageProcessing(&m_input); // TODO(V. Caraulan): make a Input struct that handles all the inputs
    m_platform.GetWindowDimension();
    glViewport(0, 0, m_platform.m_windowDimension.width, m_platform.m_windowDimension.height);
    m_editorCamera.SetProjection(projectionType::PERSPECTIVE);
    if (GetFocus() != m_platform.m_window)
    {
        m_input.mouseXDelta = m_input.mouseXPosition = 0;
        m_input.mouseYDelta = m_input.mouseYPosition = 0;
    }
    else
    {
        m_input.mouseXDelta = m_input.mouseXPosition - mouseX;
        m_input.mouseYDelta = m_input.mouseYPosition - mouseY;
    }
}

Input Engine::GetInputs()
{
    return (m_input);
}

void Engine::Update()
{
    UpdateGame = m_platform.LoadGameLibrary("UpdateGame", "game.dll", m_gameLibrary, &m_lastTime, UpdateGame);
    static Float3 cameraRotation(0, 0, 0);
    Float3 inputDirection(0, 0, 0);

    if (m_input.mouseRClick)
        cameraRotation += {(f32)m_input.mouseYDelta* (f32)m_deltaTime, (f32)m_input.mouseXDelta* (f32)m_deltaTime, 0};
    m_editorCamera.m_parameters.aspect = (float)m_platform.m_windowDimension.width / (float)m_platform.m_windowDimension.height;


    m_editorCamera.orientation = Quaternion::FromEuler(-cameraRotation.x, -cameraRotation.y, cameraRotation.z);

    static f32 time;
    time += m_deltaTime * 0.1f;
    //TODO well need to think how we pass the resources, and gameplay structures and objects to this update function
    UpdateGame(m_deltaTime, m_input, m_gameObjects, m_gameObjectCount, time, cameraRotation, &m_editorCamera.position);
    m_graphics.SetViewProjectionMatrix(m_editorCamera.GetVP());
    m_gameObjects[0].GetChildren(m_gameObjects, m_gameObjectCount, &m_tempAllocator);
}

void Engine::Draw()
{
    m_graphics.Draw(m_gameObjects, m_gameObjectCount);
    // swap the buffers to show output
    DrawIMGUI();
    if (!SwapBuffers(m_dc))
        m_platform.FatalError("Failed to swap OpenGL buffers!");
    u64 endTime = RedFoxEngine::Platform::GetTimer();
    m_deltaTime = (endTime - m_time) / (f64)Platform::GetTimerFrequency();
    m_tempAllocator.usedSize = 0;
}

Engine::~Engine()
{
    for (int i = 0; i < (int)m_modelCount; i++)
        DeInitObj(&m_models[i].obj);
    ImGui::DestroyContext();
}