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
    if (ParseModel(&m_models[m_modelCount++].obj, path))
        __debugbreak();
}

using namespace RedFoxMaths;

UPDATEGAME(updateStub)
{
//NOTE We use an empty function in case our library loading fails, so we don't crash
}

void Engine::InitIMGUI()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplWin32_Init(m_platform.m_window);
    ImGui_ImplOpenGL3_Init("#version 450");
}

Engine::Engine(int width, int height)
{
    memset(this, 0, sizeof(Engine)); // TODO formalize this in a C++ way
    new (&m_platform) Platform(width, height);

    m_arenaAllocator = InitVirtualMemory(1 * GigaByte);
    m_tempAllocator  = InitVirtualMemory(1 * GigaByte);
    IncreaseTotalCapacity(&m_arenaAllocator, 1 * MegaByte);
    m_platform.GetWindowDimension();
    m_graphics.InitGraphics();
    InitIMGUI();
    m_models = (Model *)MyMalloc(&m_arenaAllocator, sizeof(Model) * 100);
    ObjModelPush("ts_bot912.obj");
   
    for (int i = 0; i < (int)m_modelCount; i++)
    {
        m_graphics.InitModel(&m_models[i]);
        m_models[i].parent = nullptr;
    }

    m_models[1] = m_models[0];
    m_models[1].parent = &m_models[0];
    m_models[1].position = {};
    m_models[1].scale = 0.5;
    m_modelCount++;
    m_models[2] = m_models[0];
    m_models[2].position = {};
    m_models[2].scale = 0.5;
    m_modelCount++;
    QueryPerformanceFrequency((LARGE_INTEGER *)&m_frequency);
    QueryPerformanceCounter((LARGE_INTEGER *)&m_startingTime);
    m_input = {};
    m_dc = GetDC(m_platform.m_window);
    UpdateGame = m_platform.LoadGameLibrary("UpdateGame", "game.dll", m_gameLibrary, &m_lastTime, updateStub);
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
    UpdateGame = m_platform.LoadGameLibrary("UpdateGame", "game.dll", m_gameLibrary, &m_lastTime, UpdateGame);
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
    m_models[0].GetChildren(m_models, m_modelCount, &m_tempAllocator);
}

void Engine::DrawSceneNodes(int index, bool is_child, Model* model)
{
    //ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet;
    if (ImGui::TreeNodeEx((void*)(intptr_t)index, is_child ? ImGuiTreeNodeFlags_Bullet : ImGuiTreeNodeFlags_None, "Model %d", index))
    {
        if (ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("_TREENODE", model, sizeof(Model));
            ImGui::Text("This is a drag and drop source");
            ImGui::EndDragDropSource();
        }

        Model** children = model->GetChildren(m_models, m_modelCount, &m_tempAllocator);
        if (children)
        {
            for (int i = 0; sizeof(children) / sizeof(Model) || children[i] != nullptr; i++)
            {
                DrawSceneNodes(index + i + 1, true, children[i]);
            }
        }
        else if (m_models[index + 1].parent == nullptr)
        {
            DrawSceneNodes(index + 1, false, &m_models[index + 1]);
        }

        ImGui::TreePop();
    }
}


void Engine::DrawIMGUI()
{
    ImGui_ImplWin32_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Scene Graph");
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::TreeNodeEx("_TREENODE", ImGuiTreeNodeFlags_Leaf, "Root")) // TODO: Name of the scene here
    {
        DrawSceneNodes(0, (m_models[0].parent != nullptr), &m_models[0]);
    }
    ImGui::TreePop();
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Engine::Draw()
{
    m_graphics.Draw(m_models, m_modelCount);
    // swap the buffers to show output
    DrawIMGUI();
    if (!SwapBuffers(m_dc))
        m_platform.FatalError("Failed to swap OpenGL buffers!");
    u64 endTime = RedFoxEngine::Platform::GetTimer();
    m_deltaTime = ((endTime - m_time) / (f64)m_frequency);
    m_tempAllocator.usedSize = 0;
}

Engine::~Engine()
{
    for (int i = 0; i < (int)m_modelCount; i++)
        DeInitObj(&m_models[i].obj);
    ImGui::DestroyContext();
}