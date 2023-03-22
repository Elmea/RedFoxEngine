/* date = February 22nd 2023 2:44 pm */

#pragma once
#ifdef _WIN32
#include "Win32Platform.hpp"
#else
// TODO(V. Caraulan): linux mac or whatever
#endif

#include <imgui.h>
#include <imgui_internal.h>
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#include "ObjParser.hpp"
#include "OpenGLGraphics.hpp"
#include "GameObject.hpp"
#include "Camera.hpp"

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
private:
 
    void DrawIMGUI();
    void DrawSceneNodes(bool is_child, GameObject* model);
    Input GetInputs();
    void ObjModelPush(const char *objPath);
    void InitIMGUI();
    void SetViewProjectionMatrix(RedFoxMaths::Mat4 viewProjection);
    void StartTime();
    void LoadScene(const char *fileName);
    void SaveScene(const char *fileName);
public:
    Engine(int width, int height);
    ~Engine();
    void ProcessInputs();
    void Update();
    void Draw();
    bool isRunning();
};
} // namespace RedFoxEngine
