#include "Engine.hpp"
#include "meow_hash_x64_aesni.h"

#define MEMORY_IMPLEMENTATION
#include "MyMemory.hpp"

#define REDFOXMATHS_IMPLEMENTATION
#include "RedfoxMaths.hpp"

using namespace RedFoxEngine;

void Engine::ObjModelPush(const char *path)
{
    if (ParseModel(&m_models[m_modelCount++].obj, path))
        __debugbreak();
    u32 length = 0;
    while (path[length] != '\0')
        length++;
    m_models[m_modelCount - 1].hash = MeowU64From(MeowHash(MeowDefaultSeed,
        (u64)length, (void *)path), 0);
}

using namespace RedFoxMaths;

bool Engine::isRunning()
{
    return(m_platform.m_running);
}

void Engine::StartTime()
{
    m_startingTime = m_platform.GetTimer();
}

void Engine::LoadScene(const char *fileName)
{

    HANDLE file = CreateFile(fileName, GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,nullptr, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL, nullptr);
    ReadFile(file, &m_gameObjectCount, sizeof(u32), nullptr, nullptr);
    for(int i = 0; i < (int)m_gameObjectCount; i++)
    {
        GameObject *current = &m_gameObjects[i];
        int length = 0;
        ReadFile(file, &length, sizeof(int), nullptr, nullptr);
        current->name = (char *)MyMalloc(&m_arenaAllocator, length);
        current->name[length] = '\0';
        ReadFile(file, current->name, length, nullptr, nullptr);
        int parent;
        ReadFile(file, &parent, sizeof(int), nullptr, nullptr);
        if (parent == -1)
            current->parent = nullptr;
        else
            current->parent = &m_gameObjects[parent];
        u64 hash = 0;
        ReadFile(file, &hash, sizeof(u64), nullptr, nullptr);
        current->model = nullptr;
        if (hash != 0)
        {
            for (int modelIndex = 0;
            modelIndex < (int)m_modelCount;
            modelIndex++)
            {
                if (m_models[modelIndex].hash == hash)
                {
                    current->model = &m_models[modelIndex];
                    break;
                }
            }
        }
        ReadFile(file, &current->position,
            sizeof(current->position), nullptr, nullptr);
        ReadFile(file, &current->scale,
            sizeof(current->scale), nullptr, nullptr);
        ReadFile(file, &current->orientation,
            sizeof(current->orientation), nullptr, nullptr);
    }
    CloseHandle(file);
}

void Engine::SaveScene(const char *fileName)
{
    HANDLE file = CreateFile(fileName, GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, nullptr);

    WriteFile(file, &m_gameObjectCount, sizeof(u32), nullptr, nullptr);
    for(int i = 0; i < (int)m_gameObjectCount; i++)
    {
        GameObject *current = &m_gameObjects[i];
        int length = 0;
        while (current->name[length] != '\0')
            length++;
        WriteFile(file, &length, sizeof(int), nullptr, nullptr);
        WriteFile(file, current->name, length, nullptr, nullptr);
        int parent = (int)(current->parent - m_gameObjects);
        if (current->parent == nullptr)
            parent = -1;
        WriteFile(file, &parent, sizeof(int), nullptr, nullptr);
        WriteFile(file, &current->model->hash, sizeof(u64), nullptr, nullptr);

        WriteFile(file, &current->position,
            sizeof(current->position), nullptr, nullptr);
        WriteFile(file, &current->scale,
            sizeof(current->scale), nullptr, nullptr);
        WriteFile(file, &current->orientation,
            sizeof(current->orientation), nullptr, nullptr);
    }
    CloseHandle(file);
}

Engine::Engine(int width, int height) :
    m_platform(width, height),
    m_editorCamera(projectionType::PERSPECTIVE, width / (f32)height)
{
    m_arenaAllocator = InitVirtualMemory(1 * GigaByte);
    m_tempAllocator = InitVirtualMemory(1 * GigaByte);
    IncreaseTotalCapacity(&m_arenaAllocator, 1 * MegaByte);
    m_graphics.InitGraphics(&m_tempAllocator, m_platform.m_windowDimension);
    InitIMGUI();
    m_editorCamera.position = Float3(0.0f, 0.0f, 4.0f);

    m_models = (Model *)MyMalloc(&m_arenaAllocator, sizeof(Model) * 10);
    ObjModelPush("ts_bot912.obj");
    m_gameObjects = (GameObject *)MyMalloc(&m_arenaAllocator,
        sizeof(GameObject) * 100000);

    //TODO transition to an instance based model 'model'
    for (int i = 0; i < (int)m_modelCount; i++)
        m_graphics.InitModel(&m_models[i]);
    
    LoadScene("test.scene");
    m_input = {};
    m_dc = GetDC(m_platform.m_window);
    UpdateGame = m_platform.LoadGameLibrary("UpdateGame", "game.dll",
        m_gameLibrary, &m_lastTime, nullptr);
    m_graphics.InitQuad();
    StartTime();
}

void Engine::ProcessInputs()
{
    m_time = Platform::GetTimer();

    int mouseX = m_input.mouseXPosition;
    int mouseY = m_input.mouseYPosition;
    m_platform.MessageProcessing(&m_input);
//    m_platform.GetWindowDimension();
    glViewport(0, 0, m_platform.m_windowDimension.width,
                     m_platform.m_windowDimension.height);
    m_editorCamera.SetProjection(projectionType::PERSPECTIVE);
    if (GetFocus() != m_platform.m_window)
    {
        //TODO?: maybe don't reset mouse position on out of focus
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
    UpdateGame = m_platform.LoadGameLibrary("UpdateGame", "game.dll",
        m_gameLibrary, &m_lastTime, UpdateGame);
    static Float3 cameraRotation(0, 0, 0);
    Float3 inputDirection(0, 0, 0);

    if (m_input.mouseRClick)
        cameraRotation += {(f32)m_input.mouseYDelta* (f32)m_deltaTime,
                           (f32)m_input.mouseXDelta* (f32)m_deltaTime, 0};
    m_editorCamera.m_parameters.aspect = 
        m_platform.m_windowDimension.width / 
        (f32)m_platform.m_windowDimension.height;


    m_editorCamera.orientation = Quaternion::FromEuler(-cameraRotation.x,
                                                       -cameraRotation.y,
                                                        cameraRotation.z);

    static f32 time;
    time += m_deltaTime * 0.1f;
    //TODO we'll need to think how we pass the resources,
    // and gameplay structures and objects to this update function
    UpdateGame(m_deltaTime, m_input, m_gameObjects, m_gameObjectCount,
        time, cameraRotation, &m_editorCamera.position);
    m_graphics.SetViewProjectionMatrix(m_editorCamera.GetVP());
    m_gameObjects[0].GetChildren(m_gameObjects, m_gameObjectCount,
        &m_tempAllocator);
}

void Engine::Draw()
{
    glClearColor(0, 0, 0, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    m_graphics.DrawGBuffer(m_gameObjects, m_gameObjectCount, &m_tempAllocator);
    // m_graphics.DrawGBuffer(m_gameObjects, 1, &m_tempAllocator);
    m_graphics.DrawQuad();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    DrawIMGUI();
    // swap the buffers to show output
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
    m_defaultFont->ContainerAtlas->Clear();
    ImGui::DestroyContext();
}