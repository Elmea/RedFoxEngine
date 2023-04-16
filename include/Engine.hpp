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
    int selectedObject;
    ImGuiIO* ImGuiIO;
    ImFont* defaultFont;
    ImGuizmo::OPERATION gizmoType;
    ImGuizmo::MODE gizmoMode;
};

struct TimeManager
{
    f64 current;
    f64 delta;
};

struct SceneNode
{
    
};

struct SceneGraph
{
    SceneNode *root;    
};

struct Scene
{
    RedFoxEngine::GameObject *gameObjects = nullptr;
    u32 gameObjectCount = 0;
    RedFoxEngine::SkyDome skyDome;
    SceneGraph graph;
    RedFoxMaths::Mat4 GetWorldMatrix(int gameObjectindex)
    {
        RedFoxEngine::GameObject *current = &gameObjects[gameObjectindex]; 
        if (current->parent)
            return current->GetLocalMatrix() * GetWorldMatrix(gameObjects[current->parent].parent);
        return gameObjects[gameObjectindex].GetLocalMatrix();
    };
    int *GetChildren(int gameObjectIndex, Memory *temp)
    {
        int *result = (int *)MyMalloc(temp, sizeof(int));
        int count = 0;
        for (int i = 0; i < (int)gameObjectCount; i++)
        {
            if (gameObjects[i].parent == gameObjectIndex)
                result[count++] = i;
        }
        result[count] = 0;
        return(result);
    }
    int GetChildrenCount(int gameObjectIndex)
    {
        int count = 0;
        for (int i = 0; i < (int)gameObjectCount; i++)
        {
            if (gameObjects[i].parent == gameObjectIndex)
                count++;
        }
        return (count);
    }
    
};

class Engine
{
private:
    ResourcesManager m_memoryManager = {};

    //Models/Scene/SceneGraph class
    MyString m_sceneName;
    RedFoxMaths::Mat4 *m_modelMatrices;

    //GameState??/Scene/SceneGraph
    Scene scene; 
    //GameState
    Input m_input = {};

    u64 *m_modelCountIndex;
    Model *m_models = nullptr;
    u64 m_modelCount = 0;

    Camera m_editorCamera;// TODO: figure it out

    //Game dll, library
    GameLibrary m_game = {};

    //Perfect where it is
    Platform m_platform = {};
    Graphics m_graphics = {};

    TimeManager m_time = {};
    RedFoxMaths::Float3 m_editorCameraSpeed;
    bool m_editorCameraEnabled;
    
    ImGUI m_gui = {};
    Physx m_physx {};

public:
    bool isGame = false;
private:    
    void InitSkyDome();
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
