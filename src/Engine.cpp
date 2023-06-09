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

    m_graphics.InitModel(&m_models[0], CreateCube(&m_memoryManager.m_memory.arena));
    m_models[m_modelCount].hash = 1;
    m_modelsName[0] = initStringChar("Cube", 4, &m_memoryManager.m_memory.arena);
    m_modelCount++;
    m_graphics.InitModel(&m_models[1], CreateSphere(30, 25, &m_memoryManager.m_memory.arena));
    m_models[m_modelCount].hash = 2;
    m_modelsName[1] = initStringChar("Sphere", 6, &m_memoryManager.m_memory.arena);
    m_modelCount++;

    m_graphics.m_models = m_models;
    m_graphics.m_modelCount = m_modelCount;
    m_physx.InitPhysics();

    InitSkyDome();

    m_soundManager.Init(&m_memoryManager.m_memory.arena);
    m_soundManager.SetMasterVolume(1);

    m_scene.gameObjectBehaviours = (Behaviour*)m_memoryManager.PersistentAllocation(sizeof(Behaviour) * 100);
    m_scene.gameUIBehaviours = (UIBehaviour*)m_memoryManager.PersistentAllocation(sizeof(UIBehaviour) * 100);

    m_scene.gameUIs = (GameUI*)m_memoryManager.PersistentAllocation(sizeof(GameUI) * 100);
    m_scene.gameObjects = (GameObject *)m_memoryManager.PersistentAllocation(sizeof(GameObject) * 100000);
    m_scene.soundManager = &m_soundManager;
    m_memoryManager.m_sceneUsedMemory = m_memoryManager.m_memory.arena.usedSize;

    LoadScene("../assets/Scenes/Sample Scene");
    Light* dir = m_graphics.lightStorage.CreateLight(LightType::DIRECTIONAL);
    dir->lightInfo.constant = 1.0f;
    dir->lightInfo.linear = 0.09f;
    dir->lightInfo.quadratic = 0.032f;
    dir->lightInfo.position = {0.0f, 75.0f, 0.0f};
    dir->rotation = Quaternion::FromEuler(-PI/4.f, PI/6.f, 0.f );
    dir->lightInfo.ambient = {0.3f, 0.3f, 0.3f};
    dir->lightInfo.diffuse = {0.6f, 0.6f, 0.6f};
    dir->lightInfo.specular = {0.01f, 0.01f, 0.01f};
    m_input = {};
    m_game = m_platform.LoadGameLibrary("StartGame", "UpdateGame", "game.dll", m_game);
    m_graphics.InitLights();
    m_graphics.InitQuad();
    m_graphics.InitFont();
}

void Engine::ObjModelPush(const char *path)
{
    ObjModel temp = {};
    if (ParseModel(&temp, path))
    {
        m_modelCount--;
#if _DEBUG 
        __debugbreak();
#endif
        return;
    }
    else
    {
        m_graphics.InitModel(&m_models[m_modelCount], temp);
        m_modelCount++;
        m_graphics.m_models = m_models;
        m_graphics.m_modelCount = m_modelCount;
    }
    u32 length = 0;
    while (path[length] != '\0')
        length++;
    m_models[m_modelCount - 1].hash = MeowU64From(MeowHash(MeowDefaultSeed,
        (u64)length, (void *)path), 0);

    u64 len = strlen(path);
    m_modelsName[m_modelCount - 1] = initStringChar(path + 17, len - 17, &m_memoryManager.m_memory.arena);
    m_modelsName[m_modelCount - 1].capacity = len - 17;
}

bool Engine::isRunning()
{
    return(m_platform.m_running);
}


void Engine::ProcessInputs()
{
    m_time.current = Platform::GetTimer();

    m_platform.MessageProcessing(&m_input);
    glViewport(0, 0, m_platform.m_windowDimension.width,
                     m_platform.m_windowDimension.height);
    m_editorCamera.SetProjection(projectionType::PERSPECTIVE);
}

static bool pressed(Key key)
{
    return (key.isPressed || key.isHold);
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
        if (pressed(m_input.W) || pressed(m_input.Up))    inputDirection.z += -1;
        if (pressed(m_input.S) || pressed(m_input.Down))  inputDirection.z +=  1;
        if (pressed(m_input.A) || pressed(m_input.Left))  inputDirection.x += -1;
        if (pressed(m_input.D) || pressed(m_input.Right)) inputDirection.x +=  1;
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
    
    for (int modelIndex = 0; modelIndex < (int)m_modelCount; modelIndex++)
    {
        for(int index = 0;index < (int)m_scene.gameObjectCount; index++)
        {
            if (m_scene.gameObjects[index].modelIndex == modelIndex)
            {
                Model *model = &m_models[modelIndex];
                m_graphics.m_materials[m_graphics.m_materialCount + totalIndex]         = m_graphics.m_materials[model->materialOffset];
                m_graphics.m_materials[m_graphics.m_materialCount + totalIndex].diffuse = m_scene.gameObjects[index].Color;

                modelMatrices[totalIndex] = m_scene.GetWorldMatrix(index).GetTransposedMatrix();
                m_scene.m_modelCountIndex[modelIndex]++;
                totalIndex++;
            }
        }
    }
    m_graphics.PushMaterial(totalIndex);
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
        gameObjectBehavior->function(&m_scene.gameObjects[i], m_time.delta, &m_scene, &m_input, &m_physx, &m_platform.m_windowDimension);
    }
}

void Engine::UpdateUIBehaviours()
{
    for (int i = 1; i < (int)m_scene.gameUICount; i++)
    {
        UIBehaviour* gameUIBehavior = &m_scene.gameUIBehaviours[m_scene.gameUIs[i].behaviourIndex];
        gameUIBehavior->function(&m_scene.gameUIs[i], m_time.delta, &m_scene, &m_input);
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
    m_game = m_platform.LoadGameLibrary("StartGame", "UpdateGame", "game.dll", m_game);
    UpdateEditorCamera();
    UpdateSkyDome();
    m_soundManager.UpdateListener(m_editorCamera.position, m_editorCamera.orientation.ToEuler());
    UpdateLights(&m_graphics.lightStorage);
    m_physx.UpdatePhysics(1.0 / 60.0, &m_scene);
    m_game.update(&m_scene, &m_physx, &m_input, 1.0 / 60.0);
    if (!m_scene.isPaused)
        UpdateBehaviours();
    UpdateUIBehaviours();
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
    int tmp = m_memoryManager.m_memory.temp.usedSize;
    MyString file = OpenAndReadEntireFile(filePath, &m_memoryManager.m_memory.temp);
    m_memoryManager.m_memory.temp.usedSize = tmp;
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
    m_scene.skyDome.topTint = LoadTextureFromFilePath("Skydome/topSkyTint.png", false, true);
    m_scene.skyDome.botTint = LoadTextureFromFilePath("Skydome/botSkyTint.png");
    m_scene.skyDome.sun     = LoadTextureFromFilePath("Skydome/sun.png");
    m_scene.skyDome.moon    = LoadTextureFromFilePath("Skydome/moon.png");
    m_scene.skyDome.clouds  = LoadTextureFromFilePath("Skydome/clouds.png");
    m_scene.skyDome.sunPosition = { 0, 1, 0 };
    float skyDrawDistance = m_editorCamera.m_parameters._far / 1.5;
    m_scene.skyDome.model = RedFoxMaths::Mat4::GetScale({ skyDrawDistance,
        skyDrawDistance, skyDrawDistance });
}

Engine::~Engine()
{
}