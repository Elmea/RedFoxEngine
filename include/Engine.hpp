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

#include <vector> // TODO: Remove

namespace RedFoxEngine
{

class Engine
{
private:
    u64 m_startingTime = 0;
    Memory m_arenaAllocator = {};
    Memory m_tempAllocator = {};
    Model *m_models = nullptr;
    u64 m_modelCount = 0;
    GameObject *m_gameObjects = nullptr;
    u32 m_gameObjectCount = 0;
    GameObject *m_selectedObject = nullptr;
    Input m_input = {};
    HDC m_dc = 0;
    HINSTANCE m_gameLibrary = 0;
    FILETIME m_lastTime = {};
    Platform m_platform = {};
    Graphics m_graphics = {};
    u64 m_time = 0;
    f64 m_deltaTime = 0;
    _updategame *UpdateGame = nullptr;
    Camera m_editorCamera;
    ImGuiIO* m_ImGuiIO = nullptr;
    ImFont* m_defaultFont = nullptr;
    MyString m_sceneName;
    ImGuizmo::OPERATION m_GizmoType;
    ImGuizmo::MODE m_GizmoMode;

    RedFoxMaths::Float3 m_editorCameraSpeed;
    bool m_editorCameraEnabled;

    physx::PxDefaultAllocator m_allocator;
    physx::PxDefaultErrorCallback m_errorCallback;
    physx::PxFoundation* m_foundation = nullptr;
    physx::PxPhysics* m_physics = nullptr;
    physx::PxDefaultCpuDispatcher* m_dispatcher = nullptr;
    physx::PxScene* m_scene = nullptr;
    physx::PxMaterial* m_material = nullptr;
    physx::PxPvd* m_pvd = nullptr;
   
    bool isCube(GameObject& object);
    bool isSphere(GameObject& object);
    void createCubeCollider(const physx::PxTransform& t, physx::PxU32 size, physx::PxReal halfExtent);
    void createSphereCollider(const physx::PxTransform& t, physx::PxReal radius);

private:
    
    void InitPhysics();
    void UpdatePhysics();
    
    void DrawTopBar(const ImGuiViewport* viewport, float titleBarHeight, float toolbarSize, float totalHeight, float buttonHeight);
    int DrawDockSpace(const ImGuiViewport* viewport, ImGuiDockNodeFlags dockspace_flags, const ImGuiWindowClass* window_class);
    void DrawSceneNodes(bool is_child, GameObject* model);
    void DrawIMGUI();
    //void DrawGizmo(float* cameraView, float* cameraProjection, float* matrix, float camDistance, bool editTransformDecomposition);
    void UpdateEditorCamera();
    void ObjModelPush(const char *objPath);
    void InitIMGUI();
    void SetViewProjectionMatrix(RedFoxMaths::Mat4 viewProjection);
    void StartTime();
    void LoadScene(const char *fileName);
    void SaveScene(const char *fileName);
    void UpdateLights(float time);
    void initSphericalManyGameObjects(int count); //TODO: remove
public:
    Engine(int width, int height);
    ~Engine();
    void ProcessInputs();
    void Update();
    void Draw();
    bool isRunning();
};

} // namespace RedFoxEngine
