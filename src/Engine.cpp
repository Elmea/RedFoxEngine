#include "Engine.hpp"

#define MEMORY_IMPLEMENTATION
#include "MyMemory.hpp"

#define REDFOXMATHS_IMPLEMENTATION
#include "RedfoxMaths.hpp"

#include <imgui.h>
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"

using namespace RedFoxEngine;

void Engine::ObjModelPush(const char *path)
{
    int errorState = 0;
    if ((errorState = ParseModel(&m_models[m_modelCount++].obj, path)))
        __debugbreak();
}

using namespace RedFoxMaths;

typedef void (*_updategame)(float deltaTime, RedFoxEngine::Input input, RedFoxEngine::Model *models, u32 modelCount, f32 time, Float3 cameraRotation, Float3 *cameraPosition);
_updategame UpdateGame;

void updateGame(float deltaTime, RedFoxEngine::Input input, RedFoxEngine::Model *models, u32 modelCount, f32 time, Float3 cameraRotation, Float3 *cameraPosition)
{
//NOTE We use an empty function in case our library loading fails, so we don't crash
}

void LoadGame(HINSTANCE &gameLibrary, LPFILETIME LastWriteTime)
{
    FILETIME temp = *LastWriteTime;

    HANDLE File = CreateFileA("game.dll", GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    GetFileTime(File, NULL, NULL, LastWriteTime);
    CloseHandle(File);
    if (CompareFileTime(&temp, LastWriteTime) != 0)
    {
        if (gameLibrary)
            FreeLibrary(gameLibrary);
        CopyFileA("game.dll", "gameCopy.dll", false);
        gameLibrary = LoadLibraryA("gameCopy.dll");
        if (gameLibrary)
            UpdateGame = (_updategame)GetProcAddress(gameLibrary, "UpdateGame");
        if (UpdateGame == NULL)
            UpdateGame = &updateGame;
    }
}

Engine::Engine(int width, int height)
{
    memset(this, 0, sizeof(Engine)); // TODO formalize this in a C++ way
    m_arenaAllocator = InitVirtualMemory(1 * GigaByte);
    IncreaseTotalCapacity(&m_arenaAllocator, 1 * MegaByte);

    new (&m_platform) Platform(width, height);

    m_platform.GetWindowDimension();
    m_graphics.InitGraphics();

    m_models = (Model *)MyMalloc(&m_arenaAllocator, sizeof(Model) * 100);
    m_modelCount = 0;
    ObjModelPush("ts_bot912.obj");
   
    for (int i = 0; i < (int)m_modelCount; i++)
    {
        m_graphics.InitModel(&m_models[i]);
        m_models[i].parent = nullptr;
        // Tmp code for scene graph
        if (i > 0)
        {
            m_models[i].parent = &m_models[i - 1];
        }
    }
    m_models[1] = m_models[0];
    m_models[1].position.x += 2.f;
    m_modelCount++;
    QueryPerformanceFrequency((LARGE_INTEGER *)&m_frequency);
    QueryPerformanceCounter((LARGE_INTEGER *)&m_startingTime);
    m_input = {};
    m_dc = GetDC(m_platform.m_window);
    UpdateGame = updateGame;
    LoadGame(m_gameLibrary, &m_lastTime);
}

void Engine::ProcessInputs()
{
    m_time = Platform::GetTimer();

    int mouseX = m_input.mouseXPosition;
    int mouseY = m_input.mouseYPosition;
    m_platform.MessageProcessing(&m_input); // TODO(V. Caraulan): make a Input struct that handles all the inputs
    m_platform.GetWindowDimension();        // TODO(V. Caraulan): do this in message processing
    if (GetFocus() != m_platform.m_window)
    {
        m_input.mouseXDelta = m_input.mouseXPosition = 0;
        m_input.mouseYDelta = m_input.mouseYPosition = 0;
    }
    else
    {
        m_input.mouseXDelta = m_input.mouseXPosition - mouseX;
        m_input.mouseYDelta = m_input.mouseYPosition - mouseY;
    }
}

Input Engine::GetInputs()
{
    return (m_input);
}

void Engine::Update()
{
    LoadGame(m_gameLibrary, &m_lastTime);
    static Float3 cameraPosition(0, 0, -4);
    static Float3 cameraRotation(0, 0, 0);
    Float3 inputDirection(0, 0, 0);

    cameraRotation += {(f32)m_input.mouseYDelta * (f32)m_deltaTime, (f32)m_input.mouseXDelta * (f32)m_deltaTime, 0};
    float aspect = (float)m_platform.windowDimension.width / (float)m_platform.windowDimension.height;

    Mat4 projection = Mat4::GetPerspectiveMatrix(aspect, 90, 0.01f, 100);
    Mat4 view = (Mat4::GetRotationY(-cameraRotation.y) * Mat4::GetRotationX(-cameraRotation.x)).GetTransposedMatrix() *
                Mat4::GetTranslation(cameraPosition);

    static f32 time;
    time += m_deltaTime * 0.1f;
    //TODO well need to think how we pass the resources, and gameplay structures and objects to this update function
    UpdateGame(m_deltaTime, m_input, m_models, m_modelCount, time, cameraRotation, &cameraPosition);
    m_graphics.SetViewProjectionMatrix(projection * view);
}

void Engine::Draw()
{
    m_graphics.Draw(m_models, m_modelCount);
    // swap the buffers to show output
    ImGui_ImplWin32_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Test window");
    ImGui::Text("Hello, world!");
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (!SwapBuffers(m_dc))
        m_platform.FatalError("Failed to swap OpenGL buffers!");
    u64 endTime = RedFoxEngine::Platform::GetTimer();
    m_deltaTime = ((endTime - m_time) / (f64)m_frequency);
}

Engine::~Engine()
{
    for (int i = 0; i < (int)m_modelCount; i++)
        DeInitObj(&m_models[i].obj);
}