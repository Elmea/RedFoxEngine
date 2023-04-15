/* date = February 22nd 2023 2:44 pm */

#pragma once
#ifdef _WIN32
#include "Win32Platform.hpp"
#else
// TODO(V. Caraulan): linux mac or whatever
#endif

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_internal.h>
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#include <ImGuizmo.h>

#include <PhysX/PxConfig.h>
#include <PhysX/PxPhysicsAPI.h>

#include "ObjParser.hpp"
#include "OpenGLGraphics.hpp"
#include "GameObject.hpp"
#include "Camera.hpp"

namespace RedFoxEngine
{
    
class Engine
{
private:
    //Memory Arena class
    Memory m_arenaAllocator = {};
    Memory m_tempAllocator = {};
    int m_sceneUsedMemory = 0;

    //Models/Scene/SceneGraph class
    MyString m_sceneName;
    RedFoxMaths::Mat4 *m_modelMatrices;
    u64 *m_modelCountIndex;
    Model *m_models = nullptr;
    u64 m_modelCount = 0;
    //GameState??/Scene/SceneGraph    
    GameObject *m_gameObjects = nullptr;
    u32 m_gameObjectCount = 0;
    SkyDome m_skyDome;
    

    //GameState
    Input m_input = {};

    HDC m_dc = 0;

    //Game dll, library
    HINSTANCE m_gameLibrary = 0;
    FILETIME m_lastTime = {};
    _updategame *UpdateGame = nullptr;

    //Perfect where it is
    Platform m_platform = {};
    Graphics m_graphics = {};

    f64 m_time = 0;    //Pass to gamestate as well ?
    f64 m_deltaTime = 0;

    //Editor ui
    GameObject *m_selectedObject = nullptr;
    Camera m_editorCamera;
    ImGuiIO* m_ImGuiIO = nullptr;
    ImFont* m_defaultFont = nullptr;
    ImGuizmo::OPERATION m_GizmoType;
    ImGuizmo::MODE m_GizmoMode;

    RedFoxMaths::Float3 m_editorCameraSpeed;
    bool m_editorCameraEnabled;
    
    //Physics
    physx::PxDefaultAllocator m_allocator;
    physx::PxDefaultErrorCallback m_errorCallback;
    physx::PxFoundation* m_foundation = nullptr;
    physx::PxPhysics* m_physics = nullptr;
    physx::PxDefaultCpuDispatcher* m_dispatcher = nullptr;
    physx::PxScene* m_scene = nullptr;
    physx::PxMaterial* m_material = nullptr;
    physx::PxPvd* m_pvd = nullptr;
    physx::PxCudaContextManager* m_cudaContextManager = nullptr;

public:
    bool isGame = false;
private:
    void InitPhysics();
    void CreateCubeCollider(const physx::PxTransform& t, physx::PxU32 size, physx::PxReal halfExtent);
    void CreateSphereCollider(const physx::PxTransform& t, physx::PxReal radius);
    void UpdatePhysics();
    
    void InitSkyDome();
    void DrawIMGUI();
    void DrawTopBar(const ImGuiViewport* viewport, float titleBarHeight, float toolbarSize, float totalHeight, float buttonHeight);
    int  DrawDockSpace(const ImGuiViewport* viewport, ImGuiDockNodeFlags dockspace_flags, const ImGuiWindowClass* window_class);
    void DrawSceneNodes(bool is_child, GameObject* model);
    void UpdateIMGUI();
    void UpdateEditorCamera();
    void UpdateModelMatrices();
    void ObjModelPush(const char *objPath);
    void InitIMGUI();
    void LoadScene(const char *fileName);
    void SaveScene(const char *fileName);
    void UpdateLights(LightStorage* lightStorage);
    void initSphericalManyGameObjects(int count); //TODO: remove
    u32 LoadTextureFromFilePath(const char *filePath, bool resident, bool repeat);
public:
    Engine(int width, int height);
    ~Engine();
    void ProcessInputs();
    void Update();
    void Draw();
    bool isRunning();
};

} // namespace RedFoxEngine
