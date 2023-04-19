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

#include "ResourceManager.hpp"
#include "Scene.hpp"
#include "Physics.hpp"
#include "ObjParser.hpp"
#include "OpenGLGraphics.hpp"
#include "GameObject.hpp"
#include "Camera.hpp"

namespace RedFoxEngine
{

struct ImGUI
{
    //Editor ui
    bool editorMenuOpen = false;
    bool sceneGraphScrollButtonHovered = false;
    int selectedObject;
    int nodeIndex = 1;
    int sceneGraphScrollStrength = 1;
    int currentFrame = 0;
    ImTextureID icons[10];
    float fps[255];
    float averageFps;
    ImGuizmo::OPERATION gizmoType;
    ImGuizmo::MODE gizmoMode;
    ImGuiIO* io;
    ImFont* defaultFont;
};

class Engine
{
private:
    ResourcesManager m_memoryManager = {};

    Scene m_scene;
    //GameState
    Input m_input = {};
    u64 *m_modelCountIndex;
    Model *m_models = nullptr;
    u64 m_modelCount = 0;

    Camera m_editorCamera;
    //Game dll, library
    GameLibrary m_game = {};

    //Perfect where it is
    Platform m_platform = {};
    Graphics m_graphics = {};

    TimeManager m_time = {};
    float m_editorCameraSpeed;
    RedFoxMaths::Float3 m_editorCameraVelocity;
    bool m_editorCameraEnabled = false;
    
    ImGUI m_gui = {};
    Physx m_physx {};

public:
    bool isGame = false;
private:    
    void InitSkyDome();
    void UpdateSkyDome();
    void DrawIMGUI();
    void DrawTopBar(const ImGuiViewport* viewport, float titleBarHeight, float toolbarSize, float totalHeight, float buttonHeight);
    int  DrawDockSpace(const ImGuiViewport* viewport, ImGuiDockNodeFlags dockspace_flags, const ImGuiWindowClass* window_class);
    void DrawSceneNodes(bool is_child, int index);
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
