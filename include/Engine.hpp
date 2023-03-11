/* date = February 22nd 2023 2:44 pm */

#pragma once

#ifdef _WIN32
#include "Win32Platform.hpp"
#else
// TODO(V. Caraulan): linux mac or whatever
#endif

#include "ObjParser.hpp"
#include "OpenGLGraphics.hpp"
#include "GameObject.hpp"

namespace RedFoxEngine
{

class Engine
{
  private:
    u64 m_startingTime;
    Memory m_arenaAllocator;
    Memory m_tempAllocator;
    Model *m_models;
    u64 m_modelCount;
    GameObject *m_gameObjects;
    u64 m_gameObjectCount;
    u64 m_frequency;
    Input m_input;
    HDC m_dc;
    HINSTANCE m_gameLibrary;
    FILETIME m_lastTime;
  public:
    Platform m_platform; // TODO(V. Caraulan): make private
    Graphics m_graphics;
    u64 m_time;
    f64 m_deltaTime;
    _updategame *UpdateGame;

  private:
  public:
    Engine(int width, int height);
    ~Engine();
    void ProcessInputs();
    Input GetInputs();
    void Update();
    void Draw();
    void DrawIMGUI();
    void DrawSceneNodes(int index, bool is_child, GameObject* model);
    void ObjModelPush(const char *objPath);

    void InitIMGUI();
    void SetViewProjectionMatrix(RedFoxMaths::Mat4 viewProjection);
};
} // namespace RedFoxEngine
