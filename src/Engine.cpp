#include "Engine.hpp"
#include "GameObject.hpp"
#include "imgui.h"

#define MEMORY_IMPLEMENTATION
#include "MyMemory.hpp"

#define REDFOXMATHS_IMPLEMENTATION
#include "RedfoxMaths.hpp"

#include <vector>

using namespace RedFoxEngine;

void Engine::ObjModelPush(const char *path)
{
    if (ParseModel(&m_models[m_modelCount++].obj, path))
        __debugbreak();
}

using namespace RedFoxMaths;

void Engine::InitIMGUI()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
//    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    ImGui::StyleColorsDark();
    ImGui_ImplWin32_Init(m_platform.m_window);
    ImGui_ImplOpenGL3_Init("#version 450");

}

bool Engine::isRunning()
{
    return(m_platform.m_running);
}

void Engine::StartTime()
{
    m_startingTime = m_platform.GetTimer();
}

Engine::Engine(int width, int height) :
    m_editorCamera(projectionType::PERSPECTIVE, width / (f32)height)
{
    new (&m_platform) Platform(width, height);

    m_arenaAllocator = InitVirtualMemory(1 * GigaByte);
    m_tempAllocator  = InitVirtualMemory(1 * GigaByte);
    IncreaseTotalCapacity(&m_arenaAllocator, 1 * MegaByte);
    m_platform.GetWindowDimension();
    m_graphics.InitGraphics(&m_tempAllocator);
    InitIMGUI();
    m_editorCamera.position = Float3(0.0f, 0.0f, 4.0f);
    
    {//TODO save/load scene graph after creating this data inside the engine editor
            m_models = (Model *)MyMalloc(&m_arenaAllocator, sizeof(Model) * 1000);
            ObjModelPush("ts_bot912.obj");
            m_gameObjects = (GameObject *)MyMalloc(&m_arenaAllocator, sizeof(GameObject) * 10000);

            for (int i = 0; i < (int)m_modelCount; i++)
            {
                m_graphics.InitModel(&m_models[i]);
            }
            m_gameObjectCount = 10000;
            for (int i = 0; i < (int)m_gameObjectCount; i++)
            {
                m_gameObjects[i].parent = nullptr;
                m_gameObjects[i].model = m_models;
            }

            m_gameObjects[1] = m_gameObjects[0];
            m_gameObjects[1].parent = &m_gameObjects[0];
            m_gameObjects[1].position = {};
            m_gameObjects[1].scale = 0.5;
            m_gameObjects[2] = m_gameObjects[0];
            m_gameObjects[2].position = {2, 1, 0};
            m_gameObjects[2].scale = 0.5;
    
            float longitudeStep = M_PI * 2 / 500;
            float latitudeStep = M_PI / 20;

            int index = 0;
            float scale = 10;
            for (int i = 0; i < 20; i++)
            {
                for(int j = 0; j < 500; j++)
                {
                    m_gameObjects[index++].position = {cosf(longitudeStep * j) * sinf(i * latitudeStep), 
                        sinf(longitudeStep * j) * sinf(i * latitudeStep), cosf(i * latitudeStep - M_PI)};
                    m_gameObjects[index - 1].position = m_gameObjects[index - 1].position * scale;
                    //m_gameObjects[index - 1].position.y *= 2;
                }
            }
    }
    m_input = {};
    m_dc = GetDC(m_platform.m_window);
    UpdateGame = m_platform.LoadGameLibrary("UpdateGame", "game.dll", m_gameLibrary, &m_lastTime, nullptr);
    StartTime();
}

void Engine::ProcessInputs()
{
    m_time = Platform::GetTimer();

    int mouseX = m_input.mouseXPosition;
    int mouseY = m_input.mouseYPosition;
    m_platform.MessageProcessing(&m_input);
    m_platform.GetWindowDimension();
    glViewport(0, 0, m_platform.m_windowDimension.width, m_platform.m_windowDimension.height); //TODO wrap around
    m_editorCamera.SetProjection(projectionType::PERSPECTIVE);
    if (GetFocus() != m_platform.m_window)
    {
        //TODO?: maybe don't reset mouse position on out of focus
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
    static Float3 cameraRotation(0, 0, 0);
    Float3 inputDirection(0, 0, 0);

    if (m_input.mouseLClick)
        cameraRotation += {(f32)m_input.mouseYDelta * (f32)m_deltaTime, (f32)m_input.mouseXDelta * (f32)m_deltaTime, 0};
    m_editorCamera.m_parameters.aspect = (float)m_platform.m_windowDimension.width / (float)m_platform.m_windowDimension.height;
    

    m_editorCamera.orientation = Quaternion::FromEuler(-cameraRotation.x, -cameraRotation.y, cameraRotation.z);

    static f32 time;
    time += m_deltaTime * 0.1f;
    //TODO we'll need to think how we pass the resources, and gameplay structures and objects to this update function
    UpdateGame(m_deltaTime, m_input, m_gameObjects, m_gameObjectCount, time, cameraRotation, &m_editorCamera.position);
    m_graphics.SetViewProjectionMatrix(m_editorCamera.GetVP());
    m_gameObjects[0].GetChildren(m_gameObjects, m_gameObjectCount, &m_tempAllocator);
}

void Engine::DrawSceneNodes(int index, bool is_child, GameObject* model)
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

        GameObject** children = model->GetChildren(m_gameObjects, m_gameObjectCount, &m_tempAllocator);
        for (int i = 0; children && children[i] != nullptr; i++)
            DrawSceneNodes(index + i + 1, true, children[i]);
        if (children == nullptr && m_gameObjects[index + 1].parent == nullptr)
            DrawSceneNodes(index + 1, false, &m_gameObjects[index + 1]);

        ImGui::TreePop();
    }
}

void Engine::DrawIMGUI()
{
    ImGui_ImplWin32_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
    if (ImGui::Begin("Scene Graph"))
    {
        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::TreeNodeEx("_TREENODE", ImGuiTreeNodeFlags_Leaf, "Root")) // TODO: Name of the scene here
        {
            DrawSceneNodes(0, (m_gameObjects[0].parent != nullptr), &m_gameObjects[0]);
            ImGui::TreePop();
        }
    }
    ImGui::Text("%f", m_deltaTime * 1000);
    ImGui::End();
    ImGui::Render();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
//        HDC backup_current_context = GetDC(m_platform.m_window);
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
//        wglMakeCurrent(backup_current_context, m_platform.m_glContext);
    }
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void Engine::Draw()
{
    m_graphics.Draw(m_gameObjects, m_gameObjectCount);
    // swap the buffers to show output
    DrawIMGUI();
    if (!SwapBuffers(m_dc))
        m_platform.FatalError("Failed to swap OpenGL buffers!");
    u64 endTime = RedFoxEngine::Platform::GetTimer();
    m_deltaTime = (endTime - m_time) / (f64)Platform::GetTimerFrequency();
    m_tempAllocator.usedSize = 0;
}

Engine::~Engine()
{
    for (int i = 0; i < (int)m_modelCount; i++)
        DeInitObj(&m_models[i].obj);
    ImGui::DestroyContext();
}