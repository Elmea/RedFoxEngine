
#pragma once
#ifdef _WIN32
#include "Win32Platform.hpp"
#else
#endif

#include "Scene.hpp"
#include "Physics.hpp"
#include "ObjParser.hpp"
#include "OpenGLGraphics.hpp"
#include "GameObject.hpp"
#include "Camera.hpp"
#include "ResourceManager.hpp"
#include "SoundManager.hpp"
#include "GameBehaviour.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "ImGuizmo.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "imgui_impl_win32.h"

namespace RedFoxEngine
{


struct ImGUI
{
    bool captureMouse;
    bool lockEditor;
    bool manipulatingGizmo;
    bool editorMenuOpen;
    bool sceneGraphScrollButtonHovered;
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
    ImVec2 windowSize;
    ImVec2 mousePosEditor;
    ImVec2 centerEditorViewport;
    float fps[255];
    ImGuizmo::OPERATION gizmoType;
    ImGuizmo::MODE gizmoMode;
    ImGuiIO* io;
    ImFont* defaultFont;
    ImTextureID icons[11];

    const ImVec2 popupDim = { 400, 70 };

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

    ImGuiTreeNodeFlags propertiesFlags =
        ImGuiTreeNodeFlags_DefaultOpen |
        ImGuiTreeNodeFlags_OpenOnArrow |
        ImGuiTreeNodeFlags_OpenOnDoubleClick;

    ImGuiTableFlags tableFlags =
        ImGuiTableFlags_RowBg |
        ImGuiTableFlags_SizingStretchSame |
        ImGuiTableFlags_Resizable |
        ImGuiTableFlags_BordersOuter;
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
    u64 m_maxModel = 100;

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
    void DrawAssetsBrowser();
    void DrawUIGraph();
    void DrawSceneGraph();
    void DrawProperties();
    void DrawWorldProperties();
    void DrawLightsProperties();
    void UpdateIMGUI();
    void UpdateEditorCamera();
    void UpdateModelMatrices();
    void UpdateBehaviours();
    void UpdateUIBehaviours();
    void ObjModelPush(const char *objPath);
    void InitIMGUI();
    void LoadScene(const char *fileName);
    void SaveScene(const char *fileName, Scene scene);
    void UpdateLights(LightStorage* lightStorage);
    u32 LoadTextureFromFilePath(const char *filePath, bool resident = false, bool repeat = false, bool flip = false);
    u32 LoadTextureFromMemory(u8* memory, int size, bool resident = false, bool repeat = false, bool flip = false);

public:
    Engine(int width, int height);
    ~Engine();
    void ProcessInputs();
    void Update();
    void Draw();
    bool isRunning();
};

} // namespace RedFoxEngine
