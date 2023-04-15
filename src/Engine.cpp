#include "Engine.hpp"
#include "ObjParser.hpp"
#include "meow_hash_x64_aesni.h"

#define STB_IMAGE_IMPLEMENTATION
#include "STB_Image/stb_image.h"

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
    m_models[m_modelCount].obj = CreateCube(&m_arenaAllocator);
    m_models[m_modelCount].hash = 1;
    m_modelCount++;
    m_models[m_modelCount].obj = CreateSphere(30, 25, &m_arenaAllocator);
    m_models[m_modelCount].hash = 2;
    m_modelCount++;
    ObjModelPush("ts_bot912.obj");
    // ObjModelPush("vortigaunt.obj");
    ObjModelPush("barbarian.obj");

    m_graphics.m_models = m_models;
    m_graphics.m_modelCount = m_modelCount;
    m_gameObjects = (GameObject *)MyMalloc(&m_arenaAllocator,
        sizeof(GameObject) * 100000);

    m_graphics.lightStorage.lights = (Light*)MyMalloc(&m_arenaAllocator, sizeof(Light) * 1000);
    m_graphics.lightStorage.shadowMaps = (unsigned int*)MyMalloc(&m_arenaAllocator, sizeof(unsigned int) * 1000);

    //TODO transition to an instance based model 'model'
    for (int i = 0; i < (int)m_modelCount; i++)
        m_graphics.InitModel(&m_models[i]);
    InitSkyDome();
    m_sceneUsedMemory = m_arenaAllocator.usedSize;
#if 0
    LoadScene("Sample Scene.scene");
#else
    initSphericalManyGameObjects(1000);
    m_sceneName = initStringChar("Sample Scene", 255, &m_arenaAllocator);
    
    // Some light for testing
    {
        Light* dir = m_graphics.lightStorage.CreateLight(LightType::DIRECTIONAL);

        dir->lightInfo.constant = 1.0f;
        dir->lightInfo.linear = 0.09f;
        dir->lightInfo.quadratic = 0.032f;
        dir->lightInfo.position = {0.0f, 0.0f, 0.0f};
        dir->lightInfo.direction = { 0.0f, -0.5f, -0.0f };
        dir->lightInfo.ambient = {0.3, 0.3, 0.3};
        dir->lightInfo.diffuse = {0.6, 0.6, 0.6};
        dir->lightInfo.specular = {0.1, 0.1, 0.1};
     
        // Light* spot = m_graphics.lightStorage.CreateLight(LightType::SPOT);
        // spot->lightInfo.constant = 1.0f;
        // spot->lightInfo.linear = 0.09f;
        // spot->lightInfo.quadratic = 0.032f;
        // spot->lightInfo.direction = {0.0f, 0.0f, 1.0f};
        // spot->lightInfo.position = {0.0f, 0.0f, -5.0f};
        // spot->lightInfo.ambient = {0.3, 0.3, 0.3};
        // spot->lightInfo.diffuse = {0.6, 0.6, 0.6};
        // spot->lightInfo.specular = {0.1, 0.1, 0.1};
        // spot->lightInfo.cutOff = 0.5f;
        // spot->lightInfo.outerCutOff = 0.1f;
        
        // Light* point = m_graphics.lightStorage.CreateLight(LightType::POINT);
        // point->lightInfo.constant = 1.0f;
        // point->lightInfo.linear = 0.1f;
        // point->lightInfo.quadratic = 0.1f;
        // point->lightInfo.direction = {0.0f, 0.0f, 1.0f};
        // point->lightInfo.position = {0.0f, 0.0f, -5.0f};
        // point->lightInfo.ambient = {0.3, 0.3, 0.3};
        // point->lightInfo.diffuse = {0.6, 0.6, 0.6};
        // point->lightInfo.specular = {0.1, 0.1, 0.1};
        // point->lightInfo.cutOff = 0.5f;
        // point->lightInfo.outerCutOff = 0.1f;
    }


#endif
    m_input = {};
    m_dc = GetDC(m_platform.m_window);
    UpdateGame = m_platform.LoadGameLibrary("UpdateGame", "game.dll",
        m_gameLibrary, &m_lastTime, nullptr);
    m_graphics.InitLights();
    InitPhysics();
}

void Engine::ObjModelPush(const char *path)
{
    if (ParseModel(&m_models[m_modelCount++].obj, path))
    {
        m_modelCount--;
#if _DEBUG 
        __debugbreak();
#endif
        return;
    }
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

void Engine::initSphericalManyGameObjects(int count) //TODO: remove
{
    m_gameObjectCount = count;
    for (int i = 0; i < (int)m_gameObjectCount; i++)
    {
        m_gameObjects[i].parent = nullptr;
        m_gameObjects[i].model = &m_models[i% m_modelCount];
        if (m_gameObjects[i].model == &m_models[0])
            m_gameObjects[i].boxExtents = { 0.5, 0.5, 0.5 };
        else if (m_gameObjects[i].model == &m_models[1])
            m_gameObjects[i].radius = 1;
        m_gameObjects[i].scale.x = m_gameObjects[i].scale.y = m_gameObjects[i].scale.z = 1;
        m_gameObjects[i].orientation.a = 1;
        char tmp[255];
        int size = snprintf(tmp, 255, "Entity%d", i);
        m_gameObjects[i].name = initStringChar(tmp, size, &m_arenaAllocator);
        m_gameObjects[i].name.capacity = 255;
    }

    int countX = (int)sqrtf(count);
    int countY = count / countX;
    float longitudeStep = M_PI * 2 / countX;
    float latitudeStep = M_PI / countY;

    int index = 0;
    float scale = 50;
    for (int i = 0; i < countX; i++)
    {
        for(int j = 0; j < countY; j++)
        {
            m_gameObjects[index++].position =
                {
                    cosf(longitudeStep * j) * sinf(i * latitudeStep),
                    sinf(longitudeStep * j) * sinf(i * latitudeStep) + 50,
                    cosf(i * latitudeStep - M_PI)
                };
            m_gameObjects[index - 1].position =
                m_gameObjects[index - 1].position * scale;
        }
    }
    m_gameObjects[0].position =
    {
        0, -10, 0
    };
    m_gameObjects[0].scale =
    {
        1000, 1, 1000
    };

}

void Engine::ProcessInputs()
{
    m_time = Platform::GetTimer();

    m_platform.MessageProcessing(&m_input);
    glViewport(0, 0, m_platform.m_windowDimension.width,
                     m_platform.m_windowDimension.height);
    m_editorCamera.SetProjection(projectionType::PERSPECTIVE);
}

void Engine::UpdateEditorCamera()
{
    if (m_editorCameraEnabled)
    {
        const f32 dt32 = (f32)m_deltaTime;
        static Float3 cameraRotation;
        cameraRotation += {(f32)m_input.mouseYDelta* dt32, (f32)m_input.mouseXDelta* dt32, 0};
        m_editorCamera.orientation = Quaternion::FromEuler(-cameraRotation.x, -cameraRotation.y, cameraRotation.z);

        Float3 inputDirection(0, 0, 0);
        if (m_input.W || m_input.Up)    inputDirection.z += -1;
        if (m_input.S || m_input.Down)  inputDirection.z +=  1;
        if (m_input.A || m_input.Left)  inputDirection.x += -1;
        if (m_input.D || m_input.Right) inputDirection.x +=  1;
        inputDirection = (Mat4::GetRotationY(-cameraRotation.y) * Mat4::GetRotationX(-cameraRotation.x) * inputDirection).GetXYZF3();
        inputDirection.Normalize();
        inputDirection = inputDirection * 200.f;
        m_editorCamera.position += m_editorCameraSpeed * dt32 + inputDirection * (dt32 * dt32 * 0.5f);
        m_editorCameraSpeed += inputDirection * dt32 * 0.5f;
        m_editorCameraSpeed *= exp(dt32 * -3.f); // Drag
    }
    m_editorCamera.m_parameters.aspect = m_platform.m_windowDimension.width / (f32)m_platform.m_windowDimension.height;
}

void Engine::UpdateModelMatrices()
{
    int batchCount = 100000;
    m_modelMatrices = (RedFoxMaths::Mat4 *)MyMalloc(&m_tempAllocator,
        sizeof(RedFoxMaths::Mat4) * batchCount * (m_modelCount));
    m_modelCountIndex = (u64 *)MyMalloc(&m_tempAllocator,
        sizeof(u64) * m_modelCount);
    memset(m_modelCountIndex, 0, sizeof(u64) * m_modelCount);
    for(int index = 0;index < m_gameObjectCount; index++)
    {
        if (m_gameObjects[index].model)
        {
            u64 modelIndex = m_gameObjects[index].model - m_models;
            u64 countIndex = m_modelCountIndex[modelIndex];
            m_modelMatrices[countIndex + (batchCount * modelIndex)] =
                m_gameObjects[index].GetWorldMatrix().GetTransposedMatrix();
            m_modelCountIndex[modelIndex]++;
        }
    }
}

void Engine::Update()
{
    ProcessInputs();
    UpdateGame = m_platform.LoadGameLibrary("UpdateGame", "game.dll",
        m_gameLibrary, &m_lastTime, UpdateGame);

    UpdateEditorCamera();

    UpdateLights(&m_graphics.lightStorage);
    UpdatePhysics();
    //TODO we'll need to think how we pass the resources,
    // and gameplay structures and objects to this update function
    UpdateGame(m_deltaTime, m_input, m_gameObjects, m_gameObjectCount, m_time);
    UpdateModelMatrices();
    UpdateIMGUI();
    m_skyDome.sunPosition.x = cosf(m_time / 500);
    m_skyDome.sunPosition.y = sinf(m_time / 500);
    m_input.mouseXDelta = m_input.mouseYDelta = 0;
}

void Engine::Draw()
{
    Camera *currentCamera = &m_editorCamera; //TODO game camera
    m_graphics.SetViewProjectionMatrix(currentCamera->GetVP());
    m_graphics.Draw(m_modelMatrices, m_modelCountIndex, m_platform.m_windowDimension, m_skyDome, m_time);
    if (!SwapBuffers(m_dc))
        m_platform.FatalError("Failed to swap OpenGL buffers!");
    m_deltaTime = (Platform::GetTimer() - m_time);
    m_tempAllocator.usedSize = 0;
}

u32 Engine::LoadTextureFromFilePath(const char *filePath, bool resident, bool repeat)
{
    int width, height, comp;
    MyString file = OpenAndReadEntireFile(filePath, &m_tempAllocator);
    char* data = (char*)stbi_load_from_memory((u8*)file.data, file.size, &width, &height, &comp, 4);
    GLuint texture = m_graphics.InitTexture(data, width, height, resident, repeat);
    stbi_image_free(data);
    return (texture);
}

void Engine::InitSkyDome()
{
    m_skyDome.sunPosition = { 0, 1, 0 };
    m_skyDome.model = RedFoxMaths::Mat4::GetScale({ 5000, 5000, 5000 });

    m_skyDome.topTint = LoadTextureFromFilePath("Textures/topSkyTint.png", false, true);
    m_skyDome.botTint = LoadTextureFromFilePath("Textures/botSkyTint.png", false, false);
    m_skyDome.sun     = LoadTextureFromFilePath("Textures/sun.png", false, false);
    m_skyDome.moon    = LoadTextureFromFilePath("Textures/moon.png", false, false);
    m_skyDome.clouds  = LoadTextureFromFilePath("Textures/clouds.png", false, false);
}

Engine::~Engine()
{
    for (int i = 0; i < (int)m_modelCount; i++)
        DeInitObj(&m_models[i].obj);
    m_defaultFont->ContainerAtlas->Clear();
    ImGui::DestroyContext();
    m_cudaContextManager->release();
}