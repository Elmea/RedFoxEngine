#include "Engine.hpp"
#include "meow_hash_x64_aesni.h"

#define MEMORY_IMPLEMENTATION
#include "MyMemory.hpp"

#define REDFOXMATHS_IMPLEMENTATION
#include "RedfoxMaths.hpp"

using namespace RedFoxEngine;

using namespace RedFoxMaths;

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

    m_models = (Model *)MyMalloc(&m_arenaAllocator, sizeof(Model) * 100);
    ObjModelPush("ts_bot912.obj");
    // ObjModelPush("bunny.obj");
    m_graphics.m_models = m_models;
    m_graphics.m_modelCount = m_modelCount;
    m_gameObjects = (GameObject *)MyMalloc(&m_arenaAllocator,
        sizeof(GameObject) * 100000);

    //TODO transition to an instance based model 'model'
    for (int i = 0; i < (int)m_modelCount; i++)
        m_graphics.InitModel(&m_models[i]);
#if 0
    LoadScene("test.scene");
#else
    initSphericalManyGameObjects(1000);
    m_sceneName = (char*)initStringChar("Scene", 5, &m_arenaAllocator).data;
#endif
    m_input = {};
    m_dc = GetDC(m_platform.m_window);
    UpdateGame = m_platform.LoadGameLibrary("UpdateGame", "game.dll",
        m_gameLibrary, &m_lastTime, nullptr);
    m_graphics.InitQuad();
    m_graphics.InitLights();
    StartTime();
}

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
    
    size_t fileNameLen = strlen(fileName);
    m_sceneName = (char*)MyMalloc(&m_arenaAllocator, fileNameLen);
    memcpy((char*)m_sceneName, fileName, fileNameLen);

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

/*
    Scene file reference (in progress)

    struct GameObject
    {
        s32 nameOfGameObjectSize;
        char nameOfGameObject[nameOfGameObjectSize];
        int parent; - -l if no parent, >= 0 if parent exists
        u64 modelHash; hash of the filename string of the model
        float position[3];
        float scale;
        float orientation[4];
    }

    - Start Of File
    struct
    {
        s32 gameObjectCount;
        GameObject arrayOfGameObjects[gameObjectCount];
    }
*/

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

void Engine::initSphericalManyGameObjects(int count) //TODO: remove
{
    m_gameObjectCount = count;
    for (int i = 0; i < (int)m_gameObjectCount; i++)
    {
        m_gameObjects[i].parent = nullptr;
        if (i % 2 == 0)
            m_gameObjects[i].model = &m_models[0];
        else
            m_gameObjects[i].model = nullptr;
            // m_gameObjects[i].model = &m_models[1];
        m_gameObjects[i].scale = 1;
        m_gameObjects[i].orientation.a = 1;
        m_gameObjects[i].name = (char *)MyMalloc(&m_arenaAllocator, sizeof(char) * 13);
        snprintf(m_gameObjects[i].name, 13, "Entity%d", i);
        if (i < 3 && i != 0)
        m_gameObjects[i].parent = &m_gameObjects[0];
    }
    //TODO transition to an instance based model 'model'

    m_gameObjects[1].parent = &m_gameObjects[0];
    m_gameObjects[1].position = {};
    m_gameObjects[1].scale = 0.5;
    m_gameObjects[2].position = {2, 1, 0};
    m_gameObjects[2].scale = 0.5;

    int countX = (int)sqrtf(count);
    int countY = count / countX;
    float longitudeStep = M_PI * 2 / countX;
    float latitudeStep = M_PI / countY;

    int index = 0;
    float scale = 10;
    for (int i = 0; i < countX; i++)
    {
        for(int j = 0; j < countY; j++)
        {
            m_gameObjects[index++].position = {cosf(longitudeStep * j) * sinf(i * latitudeStep),
            sinf(longitudeStep * j) * sinf(i * latitudeStep), cosf(i * latitudeStep - M_PI)};
            m_gameObjects[index - 1].position = m_gameObjects[index - 1].position * scale;
            //m_gameObjects[index - 1].position.y *= 2;
        }
    }
}

void Engine::ProcessInputs()
{
    m_time = Platform::GetTimer();

    m_platform.MessageProcessing(&m_input);
//    m_platform.GetWindowDimension();
    glViewport(0, 0, m_platform.m_windowDimension.width,
                     m_platform.m_windowDimension.height);
    m_editorCamera.SetProjection(projectionType::PERSPECTIVE);
}

Input Engine::GetInputs()
{
    return (m_input);
}

void Engine::Update()
{
    DragWindow();
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
    int lightCount = 0;
    Light *light = (Light *)m_graphics.GetLightBuffer(&lightCount);

    for(int i = 0; i < lightCount; i++)
    {
        light[i].constant  = 1.0f;
        light[i].linear    = 0.09f;
        light[i].quadratic = 0.032f;
        light[i].ambient = {{0.01, 0.01, 0.01}};
        light[i].specular = {{0.1, 0.1, 0.1}};
        light[i].position = {{sinf(time) * i * 2,
                              sinf((time / 3) * i * 3),
                              cosf((time / 6) * 2) * i * 3}};
        float intensity = 2.f;
        light[i].diffuse = {{sinf(time * 0.5 * i) * intensity,
                             sinf(time * 0.6 * i) * intensity,
                             cosf(time * 0.3 * i) * intensity}};
    }
    m_graphics.ReleaseLightBuffer();
    UpdateGame(m_deltaTime, m_input, m_gameObjects, m_gameObjectCount,
        time, cameraRotation, &m_editorCamera.position);
    m_graphics.SetViewProjectionMatrix(m_editorCamera.GetVP());
    m_gameObjects[0].GetChildren(m_gameObjects, m_gameObjectCount,
        &m_tempAllocator);
    m_input.mouseXDelta = m_input.mouseYDelta = 0;
}

void Engine::Draw()
{
    m_graphics.DrawGBuffer(m_gameObjects, m_gameObjectCount, &m_tempAllocator);
    m_graphics.DrawQuad(m_platform.m_windowDimension);
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