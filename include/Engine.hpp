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

#include "Scene.hpp"
#include "Physics.hpp"
#include "ObjParser.hpp"
#include "OpenGLGraphics.hpp"
#include "GameObject.hpp"
#include "Camera.hpp"
#include "ResourceManager.hpp"
#include "SoundManager.hpp"

namespace RedFoxEngine
{

struct ImGUI
{
    bool manipulatingGizmo = false;
    bool editorMenuOpen = false;
    bool sceneGraphScrollButtonHovered = false;
    int selectedObject;
    int selectedUI;
    int selectedModelAsset;
    int selectedSoundAsset;
    int nodeIndex = 1;
    int uiIndex;
    int sceneGraphScrollStrength = 1;
    int currentFrame;
    int translateSnap = 1;
    int rotateSnap = 45;
    int scaleSnap = 1;
    int mousePickNodeIndex = -1;
    float fpsUpdate;
    float dragSpeed = 1.f;
    RedFoxMaths::Float2 mousePosEditor = RedFoxMaths::Float2();
    float fps[255];
    ImGuizmo::OPERATION gizmoType;
    ImGuizmo::MODE gizmoMode;
    ImGuiIO* io;
    ImFont* defaultFont;
    ImTextureID icons[10];

    const ImGuiDockNodeFlags dockingFlags =
        ImGuiDockNodeFlags_NoWindowMenuButton |
        ImGuiDockNodeFlags_NoCloseButton;

    const ImGuiWindowFlags windowFlags =
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoScrollbar;

    const ImGuiWindowFlags sceneGraphFlags =
        ImGuiWindowFlags_AlwaysHorizontalScrollbar |
        ImGuiWindowFlags_AlwaysVerticalScrollbar |
        ImGuiWindowFlags_NoMove;

    const ImGuiTreeNodeFlags rootNodeFlags =
        ImGuiTreeNodeFlags_Framed |
        ImGuiTreeNodeFlags_Leaf |
        ImGuiTreeNodeFlags_AllowItemOverlap |
        ImGuiTreeNodeFlags_DefaultOpen |
        ImGuiTreeNodeFlags_SpanFullWidth;
};

class Engine
{
private:
    ResourcesManager m_memoryManager = {};

    Scene m_scene;
    //GameState
    Input m_input = {};
    Model *m_models = nullptr;
    MyString* m_modelsName;
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
    ImGUI m_imgui = {};
    Physx m_physx {};
    SoundManager m_soundManager;
    Sound* m_testMusic;

public:
    bool isGame = false;
private:    
    void InitSkyDome();
    void UpdateSkyDome();
    void DrawIMGUI();
    void DrawTopBar(const ImGuiViewport* viewport, float titleBarHeight, float toolbarSize, float totalHeight, float buttonHeight);
    int  DrawDockSpace(const ImGuiViewport* viewport, ImGuiDockNodeFlags dockspace_flags, const ImGuiWindowClass* window_class);
    void DrawSceneNodes(bool is_child, int index);
    void DrawSceneNodesUI(bool is_child, int index);
    void DrawEditor();
    void DrawUIGraph();
    void DrawSceneGraph();
    void DrawProperties();
    void UpdateIMGUI();
    void UpdateEditorCamera();
    void UpdateModelMatrices();
    void ObjModelPush(const char *objPath);
    void InitIMGUI();
    void LoadScene(const char *fileName);
    void SaveScene(const char *fileName, Scene scene);
    void UpdateLights(LightStorage* lightStorage);
    void initSphericalManyGameObjects(int count); //TODO: remove
    u32 LoadTextureFromFilePath(const char *filePath, bool resident, bool repeat, bool flip);
    u32 LoadTextureFromMemory(u8* memory, int size, bool resident, bool repeat, bool flip);

public:
    Engine(int width, int height);
    ~Engine();
    void ProcessInputs();
    void Update();
    void Draw();
    bool isRunning();
};

} // namespace RedFoxEngine
