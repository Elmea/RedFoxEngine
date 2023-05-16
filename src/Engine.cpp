#include "Engine.hpp"
#include "ObjParser.hpp"
#include "meow_hash_x64_aesni.h"

#define STB_IMAGE_IMPLEMENTATION
#include "STB_Image/stb_image.h"

#define MEMORY_IMPLEMENTATION
#include "MyMemory.hpp"

#define REDFOXMATHS_IMPLEMENTATION
#include "RedfoxMaths.hpp"

#include <timeapi.h>

using namespace RedFoxEngine;
using namespace RedFoxMaths;

Engine::Engine(int width, int height) :
    m_scene(width, height),
    m_editorCamera(projectionType::PERSPECTIVE, width / (f32)height),
    m_platform(width, height)
{
    m_graphics.InitGraphics(&m_memoryManager, m_platform.m_windowDimension);
    m_graphics.InitPostProcess(&m_memoryManager.m_memory.arena);
    InitIMGUI();
    m_editorCamera.position = Float3(0.0f, 0.0f, 4.0f);
    m_editorCameraSpeed = 1;
    m_models = (Model *)m_memoryManager.PersistentAllocation(sizeof(Model) * m_maxModel);

    m_modelsName = (MyString*)m_memoryManager.PersistentAllocation(sizeof(MyString) * m_maxModel);
    for (int i = 0; i < 100; i++)
          m_modelsName->capacity   = 64;
    m_graphics.lightStorage.lights = (Light*)m_memoryManager.PersistentAllocation(sizeof(Light) * 1000);
    m_graphics.lightStorage.shadowMaps = (unsigned int*)m_memoryManager.PersistentAllocation(sizeof(unsigned int) * 1000);

    m_models[m_modelCount].obj = CreateCube(&m_memoryManager.m_memory.arena);
    m_models[m_modelCount].hash = 1;
    m_modelsName[0] = initStringChar("Cube", 4, &m_memoryManager.m_memory.arena);
    m_modelCount++;
    m_models[m_modelCount].obj = CreateSphere(30, 25, &m_memoryManager.m_memory.arena);
    m_models[m_modelCount].hash = 2;
    m_modelsName[1] = initStringChar("Sphere", 6, &m_memoryManager.m_memory.arena);
    m_modelCount++;

    m_graphics.m_models = m_models;
    m_graphics.m_modelCount = m_modelCount;
    for (int i = 0; i < (int)m_modelCount; i++)
        m_graphics.InitModel(&m_models[i]);

    m_physx.InitPhysics();
    InitSkyDome();
    m_soundManager.Init(&m_memoryManager.m_memory.arena);
    m_soundManager.SetMasterVolume(1);
    
    m_testMusic = m_soundManager.CreateSound("music.ogg", &m_memoryManager.m_memory.arena);
    if (m_testMusic)
    {
        m_testMusic->SetVolume(1);
        m_testMusic->SetLoop(true);
        m_testMusic->position = {0.f, 0.f, 0.f};
        m_testMusic->Play3D();
    }
    m_scene.gameObjectBehaviours = (Behaviour*)m_memoryManager.PersistentAllocation(sizeof(Behaviour) * 100);
    m_scene.gameUIBehaviours = (Behaviour*)m_memoryManager.PersistentAllocation(sizeof(Behaviour) * 100);
    m_scene.gameUIs = (GameUI*)m_memoryManager.PersistentAllocation(sizeof(GameUI) * 100);
    m_scene.gameObjects = (GameObject *)m_memoryManager.PersistentAllocation(sizeof(GameObject) * 100000);
    m_memoryManager.m_sceneUsedMemory = m_memoryManager.m_memory.arena.usedSize;

    //TODO transition to an instance based model 'model'
#if 1
    LoadScene("Sample Scene");
    Light* dir = m_graphics.lightStorage.CreateLight(LightType::DIRECTIONAL);
    dir->lightInfo.constant = 1.0f;
    dir->lightInfo.linear = 0.09f;
    dir->lightInfo.quadratic = 0.032f;
    dir->lightInfo.position = {0.0f, 75.0f, 0.0f};
    dir->lightInfo.direction = { 0.3f, -0.8f, -0.5f };
    dir->lightInfo.ambient = {0.3f, 0.3f, 0.3f};
    dir->lightInfo.diffuse = {0.6f, 0.6f, 0.6f};
    dir->lightInfo.specular = {0.1f, 0.1f, 0.1f};
#else
    initSphericalManyGameObjects(5000);
    m_scene.m_name = initStringChar("Sample Scene", 255, &m_memoryManager.m_memory.arena);

    // Some light for testing
    {
        Light* dir = m_graphics.lightStorage.CreateLight(LightType::DIRECTIONAL);
        dir->lightInfo.constant = 1.0f;
        dir->lightInfo.linear = 0.09f;
        dir->lightInfo.quadratic = 0.032f;
        dir->lightInfo.position = {0.0f, 75.0f, 0.0f};
        dir->lightInfo.direction = { 0.3f, -0.8f, -0.5f };
        dir->lightInfo.ambient = {0.3f, 0.3f, 0.3f};
        dir->lightInfo.diffuse = {0.6f, 0.6f, 0.6f};
        dir->lightInfo.specular = {0.1f, 0.1f, 0.1f};
    }
    /*
    // Post process tests
    {

        float edge[4][4] = {
            { 1.f, 1.f, 1.f, 0.f },
            { 1.f, -8.f, 1.f, 0.f },
            { 1.f, 1.f, 1.f, 0.f },
            { 0.f, 0.f, 0.f, 1.f }
        };

        float blur[4][4] = {
            { 1.f / 16.f, 2.f / 16.f, 1.f / 16.f, 0.f },
            { 2.f / 16.f, 4.f / 16.f, 2.f / 16.f, 0.f },
            { 1.f / 16.f, 2.f / 16.f, 1.f / 16.f, 0.f },
            { 0.f, 0.f, 0.f, 1.f }
        };

        RedFoxMaths::Mat4 kernelMat = edge;
        RedFoxMaths::Mat4 secondKernelMat = blur;
        m_graphics.AddKernel(kernelMat);
        // m_graphics.AddKernel(secondKernelMat);

        m_graphics.AddPostProcessShader(&m_memoryManager.m_memory.temp, "greyScale.frag");
        m_graphics.AddPostProcessShader(&m_memoryManager.m_memory.temp, "invertColor.frag");
    }*/

#endif
    m_input = {};
    //TODO: ask user for what game to load ? or maybe save the game dll
    // path into the scene data ? maybe both
    m_game = m_platform.LoadGameLibrary("UpdateGame", "game.dll", m_game);
    m_graphics.InitLights();
    m_graphics.InitQuad();
    m_graphics.InitFont();
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

    u64 len = strlen(path);
    m_modelsName[m_modelCount - 1] = initStringChar(path, len, &m_memoryManager.m_memory.arena);
    m_modelsName[m_modelCount - 1].capacity = len;
}

bool Engine::isRunning()
{
    return(m_platform.m_running);
}

void Engine::initSphericalManyGameObjects(int count) //TODO: remove
{
    //Init behaviours buffer
    for (int i = 0; i < 100; i++)
    {
        m_scene.gameObjectBehaviours[i].name = initString(64, &m_memoryManager.m_memory.arena);
        m_scene.gameObjectBehaviours[i].function = nullptr;
        m_scene.gameUIBehaviours[i].name = initString(64, &m_memoryManager.m_memory.arena);
        m_scene.gameUIBehaviours[i].function = nullptr;
    }
    //Init GameUI
    m_scene.gameUIs[0] = {};
    m_scene.gameUIs[0].name = initStringChar("Root", 255, &m_memoryManager.m_memory.arena);
    m_scene.gameUIs[0].name.capacity = 255;
    m_scene.gameUIs[0].screenPosition = { 0, 0 };
    m_scene.gameUICount++;
    for (int i = 1; i < 100; i++)
    {
        m_scene.gameUIs[i].parent = 0;
        m_scene.gameUIs[i].behaviourIndex = -1;
        m_scene.gameUIs[i].isPressed = false;
        m_scene.gameUIs[i].isHovered = false;
    }

    //Init GameObject
    m_scene.gameObjects[0] = {};
    m_scene.gameObjects[0].name = initStringChar("Root", 255, &m_memoryManager.m_memory.arena);
    m_scene.gameObjects[0].name.capacity = 255;
    m_scene.gameObjects[0].position = { 0, 0, 0 };
    m_scene.gameObjects[0].orientation = { 1, 0, 0, 0 };
    m_scene.gameObjects[0].scale = { 1, 1, 1 };
    m_scene.gameObjects->modelIndex = -1;

    m_scene.gameObjectCount++;
    m_scene.gameObjectCount = count;

    for (int i = 1; i < (int)m_scene.gameObjectCount; i++)
    {
        m_scene.gameObjects[i].parent = 0;
        m_scene.gameObjects[i].modelIndex = i % m_modelCount;
        if (m_scene.gameObjects[i].modelIndex == 0)
            m_scene.gameObjects[i].scale = { 0.5, 0.5, 0.5 };
        else if (m_scene.gameObjects[i].modelIndex == 1)
            m_scene.gameObjects[i].scale = {1, 1, 1};
        m_scene.gameObjects[i].behaviourIndex = -1;
        m_scene.gameObjects[i].scale.x = m_scene.gameObjects[i].scale.y = m_scene.gameObjects[i].scale.z = 1;
        m_scene.gameObjects[i].orientation.a = 1;
        char tmp[255];
        int size = snprintf(tmp, 255, "Entity%d", i);
        m_scene.gameObjects[i].name = initStringChar(tmp, size, &m_memoryManager.m_memory.arena);
        m_scene.gameObjects[i].name.capacity = 255;
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
            m_scene.gameObjects[index++].position =
                {
                    cosf(longitudeStep * j) * sinf(i * latitudeStep),
                    sinf(longitudeStep * j) * sinf(i * latitudeStep) + 1,
                    cosf(i * latitudeStep - M_PI)
                };
            m_scene.gameObjects[index - 1].position =
                m_scene.gameObjects[index - 1].position * scale;
        }
    }
    m_scene.gameObjects[1].modelIndex = 0;
    m_scene.gameObjects[1].position = { 0, -10, 0 };
    m_scene.gameObjects[1].scale = { 1, 1, 1 };
}

void Engine::ProcessInputs()
{
    m_time.current = Platform::GetTimer();

    m_platform.MessageProcessing(&m_input);
    glViewport(0, 0, m_platform.m_windowDimension.width,
                     m_platform.m_windowDimension.height);
    m_editorCamera.SetProjection(projectionType::PERSPECTIVE);
}

void Engine::UpdateEditorCamera()
{
    if (m_editorCameraEnabled && m_scene.isPaused)
    {
        const f32 dt32 = (f32)m_time.delta;
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
        m_editorCamera.position += m_editorCameraVelocity * dt32 + inputDirection * (dt32 * dt32 * 0.5f);
        m_editorCameraVelocity += inputDirection * m_editorCameraSpeed * dt32 * 0.5f;
        m_editorCameraVelocity *= exp(dt32 * -3.f); // Drag
    }
    m_editorCamera.m_parameters.aspect = m_platform.m_windowDimension.width / (f32)m_platform.m_windowDimension.height;
}

static Material materialFromObjMaterial(ObjMaterial modelMaterial, Float3 color)
{
    Material material = {};
    Float3 zero = {};
    if (memcmp(&color, &zero, sizeof(Float3)))
        material.diffuse = color;
    else
        material.diffuse = {modelMaterial.diffuse.x,
                                modelMaterial.diffuse.y,
                                modelMaterial.diffuse.z};
    material.Shininess = modelMaterial.Shininess;
    if (modelMaterial.hasTexture == 0)
        material.diffuseMap = -1;
    else
        material.diffuseMap = modelMaterial.diffuseMap.index0;
    if (modelMaterial.hasNormal == 0)
        material.normalMap = -1;
    else
        material.normalMap = modelMaterial.normalMap.index0;
    return (material);
}

void Engine::UpdateModelMatrices()
{
    ResourcesManager *m = &m_memoryManager;
    Mat4 *modelMatrices = (RedFoxMaths::Mat4 *)m->TemporaryAllocation(
        sizeof(RedFoxMaths::Mat4) * m_scene.gameObjectCount);
    m_scene.m_modelCountIndex = (u64 *)m->TemporaryAllocation(sizeof(u64)
                                                         * (m_modelCount + 1));
    memset(m_scene.m_modelCountIndex, 0, sizeof(u64) * (m_modelCount + 1));
    int totalIndex = 0;
    
    Material *materials = (Material *)m->TemporaryAllocation(sizeof(Material) * m_scene.gameObjectCount);
    for (int modelIndex = 0; modelIndex < (int)m_modelCount; modelIndex++)
    {
        for(int index = 0;index < (int)m_scene.gameObjectCount; index++)
        {
            if (m_scene.gameObjects[index].modelIndex == modelIndex)
            {
                Float3    color    = m_scene.gameObjects[index].Color;
                Model    *model    = &m_models[modelIndex];
                for (int i = 0; i < (int)model->obj.materials.count; i++)
                {
                    ObjMaterial objMaterial = model->obj.materials.material[i];
                    materials[totalIndex] = materialFromObjMaterial(objMaterial, color);
                }

                modelMatrices[totalIndex] =
                    m_scene.GetWorldMatrix(index).GetTransposedMatrix();
                m_scene.m_modelCountIndex[modelIndex]++;
                totalIndex++;
            }
        }
    }
    m_graphics.PushMaterial(materials, totalIndex);
    m_graphics.PushModelMatrices(modelMatrices, totalIndex);
    m_graphics.m_models = m_models;
    m_graphics.m_modelCount = m_modelCount;
}

void Engine::UpdateSkyDome()
{
    m_scene.skyDome.model.mat16[3]  = m_editorCamera.position.x;
    m_scene.skyDome.model.mat16[7]  = m_editorCamera.position.y;
    m_scene.skyDome.model.mat16[11] = m_editorCamera.position.z;
    m_scene.skyDome.sunPosition.x = cosf(m_time.current / 500);
    m_scene.skyDome.sunPosition.y = sinf(m_time.current / 500);
}

void Engine::UpdateBehaviours()
{
    for (int i = 1; i < (int)m_scene.gameObjectCount; i++)
    {
        Behaviour* gameObjectBehavior = &m_scene.gameObjectBehaviours[m_scene.gameObjects[i].behaviourIndex];
        if (!m_scene.isPaused && gameObjectBehavior->function != nullptr)
            gameObjectBehavior->function(&m_scene.gameObjects[i], m_time.delta, &m_scene, &m_input);
    }
    for (int i = 1; i < (int)m_scene.gameUICount; i++)
    {
        Behaviour* gameUIBehavior = &m_scene.gameUIBehaviours[m_scene.gameUIs[i].behaviourIndex];
        if (!m_scene.isPaused && gameUIBehavior->function != nullptr)
            gameUIBehavior->function(&m_scene.gameObjects[i], m_time.delta, &m_scene, &m_input);
    }
}

void Engine::Update()
{
    Camera *currentCamera;
    if (m_scene.isPaused)
        currentCamera = &m_editorCamera;
    else
        currentCamera = &m_scene.m_gameCamera;
    ProcessInputs();
    m_game = m_platform.LoadGameLibrary("UpdateGame", "game.dll", m_game);
    UpdateEditorCamera();
    UpdateSkyDome();
    m_soundManager.UpdateListener(m_editorCamera.position, m_editorCamera.orientation.ToEuler());
    UpdateLights(&m_graphics.lightStorage);
    m_physx.UpdatePhysics(1.0 / 60.0, &m_scene, m_memoryManager);
    m_game.update(&m_scene, &m_physx, m_input, 1.0 / 60.0);
    if (!m_scene.isPaused)
    {
        UpdateBehaviours();
    }
    UpdateModelMatrices();
    UpdateIMGUI();
    m_input.mouseXDelta = m_input.mouseYDelta = 0;
    if (m_time.delta < 1.0 / 100.0)
    {
        timeBeginPeriod(1);
        Sleep(10);
        timeEndPeriod(1);
    }
}

void Engine::Draw()
{
    Camera *currentCamera;
    if (m_scene.isPaused)
        currentCamera = &m_editorCamera;
    else
        currentCamera = &m_scene.m_gameCamera;

    m_graphics.BindKernelBuffer(&m_memoryManager.m_memory.temp);
    m_graphics.SetViewProjectionMatrix(currentCamera->GetVP());
    m_graphics.Draw(&m_scene, m_platform.m_windowDimension, m_time.current, m_time.delta, currentCamera->position);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    m_platform.SwapFramebuffers();
    m_time.delta = (Platform::GetTimer() - m_time.current);
    m_memoryManager.m_memory.temp.usedSize = 0;
}

u32 Engine::LoadTextureFromFilePath(const char *filePath, bool resident, bool repeat, bool flip)
{
    int width, height, comp;
    MyString file = OpenAndReadEntireFile(filePath, &m_memoryManager.m_memory.temp);
    return (LoadTextureFromMemory((u8 *)file.data, file.size, resident, repeat, flip));
}

u32 Engine::LoadTextureFromMemory(u8* memory, int size, bool resident, bool repeat, bool flip)
{
    int width, height, comp;
    stbi_set_flip_vertically_on_load(flip);
    char *data = (char *)stbi_load_from_memory((u8 *)memory, size, &width, &height, &comp, 4);
    GLuint texture = m_graphics.InitTexture(data, width, height, resident, repeat);
    stbi_image_free(data);
    return (texture);
}

void Engine::InitSkyDome()
{
    m_scene.skyDome.sunPosition = { 0, 1, 0 };
    float skyDrawDistance = m_editorCamera.m_parameters._far / 1.5;
    m_scene.skyDome.model = RedFoxMaths::Mat4::GetScale({ skyDrawDistance,
        skyDrawDistance, skyDrawDistance });

    m_scene.skyDome.topTint = LoadTextureFromFilePath("topSkyTint.png", false, true);
    m_scene.skyDome.botTint = LoadTextureFromFilePath("botSkyTint.png");
    m_scene.skyDome.sun     = LoadTextureFromFilePath("sun.png");
    m_scene.skyDome.moon    = LoadTextureFromFilePath("moon.png");
    m_scene.skyDome.clouds  = LoadTextureFromFilePath("clouds.png");
}

Engine::~Engine()
{
    for (int i = 0; i < (int)m_modelCount; i++)
        DeInitObj(&m_models[i].obj);
}