#define MEMORY_IMPLEMENTATION
#include "MyMemory.hpp"
#include "Icons.hpp"
#include "Engine.hpp"
#include <unordered_map>

#include "Fonts.hpp"

using namespace RedFoxEngine;
using namespace ImGui;

#pragma region RedFox_Style_Color_Palette_Declaration
#define RF_BLACK         ImVec4(0.00f, 0.00f, 0.00f, 1.00f)
#define RF_WHITE         ImVec4(1.00f, 1.00f, 1.00f, 1.00f)
#define RF_DARKGRAY      ImVec4(0.19f, 0.19f, 0.19f, 1.00f)
#define RF_GRAY          ImVec4(0.31f, 0.31f, 0.31f, 1.00f)
#define RF_LIGHTGRAY     ImVec4(0.51f, 0.51f, 0.51f, 1.00f)
#define RF_DARKGRAYBLUE  ImVec4(0.28f, 0.30f, 0.37f, 1.00f)
#define RF_GRAYBLUE      ImVec4(0.35f, 0.39f, 0.49f, 1.00f)
#define RF_LIGHTGRAYBLUE ImVec4(0.47f, 0.56f, 0.95f, 1.00f)
#define RF_DARKORANGE    ImVec4(0.81f, 0.41f, 0.19f, 1.00f)
#define RF_ORANGE        ImVec4(0.89f, 0.49f, 0.21f, 1.00f)
#define RF_LIGHTORANGE   ImVec4(1.00f, 0.57f, 0.27f, 1.00f)
#pragma endregion

void Engine::InitIMGUI()
{
    IMGUI_CHECKVERSION();

    CreateContext();
    m_imgui.io = &GetIO();
    m_imgui.io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    m_imgui.io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;

#pragma region RedFox_Style_Definition
    ImGuiStyle& style = GetStyle();
    style.WindowPadding = ImVec2(4.f, 4.f);
    style.FramePadding = ImVec2(4.f, 2.f);
    style.CellPadding = ImVec2(4.f, 2.f);
    style.ItemSpacing = ImVec2(8.f, 4.f);
    style.ItemInnerSpacing = ImVec2(4.f, 4.f);
    style.IndentSpacing = 24.f;
    style.ScrollbarSize = 14.f;
    style.GrabMinSize = 16.f;
    style.WindowBorderSize = 0.f;
    style.ChildBorderSize = 0.f;
    style.PopupBorderSize = 0.f;
    style.FrameBorderSize = 0.f;
    style.TabBorderSize = 0.f;
    style.Colors[ImGuiCol_Text] = RF_WHITE;
    style.Colors[ImGuiCol_TextDisabled] = RF_GRAY;
    style.Colors[ImGuiCol_WindowBg] = RF_DARKGRAY;
    style.Colors[ImGuiCol_ChildBg] = RF_DARKGRAY;
    style.Colors[ImGuiCol_PopupBg] = RF_BLACK;
    style.Colors[ImGuiCol_Border] = RF_BLACK;
    style.Colors[ImGuiCol_BorderShadow] = ImVec4();
    style.Colors[ImGuiCol_FrameBg] = RF_DARKGRAYBLUE;
    style.Colors[ImGuiCol_FrameBgHovered] = RF_GRAYBLUE;
    style.Colors[ImGuiCol_FrameBgActive] = RF_LIGHTGRAYBLUE;
    style.Colors[ImGuiCol_TitleBg] = RF_DARKGRAY;
    style.Colors[ImGuiCol_TitleBgActive] = RF_GRAY;
    style.Colors[ImGuiCol_TitleBgCollapsed] = RF_DARKGRAY;
    style.Colors[ImGuiCol_MenuBarBg] = RF_DARKORANGE;
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.2f, 0.2f, 0.2f, 0.53f);
    style.Colors[ImGuiCol_ScrollbarGrab] = RF_GRAY;
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = RF_LIGHTGRAY;
    style.Colors[ImGuiCol_ScrollbarGrabActive] = RF_LIGHTGRAY;
    style.Colors[ImGuiCol_CheckMark] = RF_ORANGE;
    style.Colors[ImGuiCol_SliderGrab] = RF_ORANGE;
    style.Colors[ImGuiCol_SliderGrabActive] = RF_ORANGE;
    style.Colors[ImGuiCol_Button] = RF_DARKORANGE;
    style.Colors[ImGuiCol_ButtonHovered] = RF_ORANGE;
    style.Colors[ImGuiCol_ButtonActive] = RF_LIGHTORANGE;
    style.Colors[ImGuiCol_Header] = RF_DARKORANGE;
    style.Colors[ImGuiCol_HeaderHovered] = RF_ORANGE;
    style.Colors[ImGuiCol_HeaderActive] = RF_ORANGE;
    style.Colors[ImGuiCol_Separator] = RF_BLACK;
    style.Colors[ImGuiCol_SeparatorHovered] = RF_BLACK;
    style.Colors[ImGuiCol_SeparatorActive] = RF_BLACK;
    style.Colors[ImGuiCol_ResizeGrip] = RF_BLACK;
    style.Colors[ImGuiCol_ResizeGripHovered] = RF_BLACK;
    style.Colors[ImGuiCol_ResizeGripActive] = RF_BLACK;
    style.Colors[ImGuiCol_Tab] = RF_DARKORANGE;
    style.Colors[ImGuiCol_TabHovered] = RF_ORANGE;
    style.Colors[ImGuiCol_TabActive] = RF_ORANGE;
    style.Colors[ImGuiCol_TabUnfocused] = RF_DARKORANGE;
    style.Colors[ImGuiCol_TabUnfocusedActive] = RF_ORANGE;
    style.Colors[ImGuiCol_DockingPreview] = RF_ORANGE;
    style.Colors[ImGuiCol_DockingEmptyBg] = RF_DARKGRAY;
    style.Colors[ImGuiCol_PlotLines] = RF_WHITE;
    style.Colors[ImGuiCol_PlotLinesHovered] = RF_WHITE;
    style.Colors[ImGuiCol_PlotHistogram] = RF_ORANGE;
    style.Colors[ImGuiCol_PlotHistogramHovered] = RF_ORANGE;
    style.Colors[ImGuiCol_TableHeaderBg] = RF_DARKGRAY;
    style.Colors[ImGuiCol_TableBorderStrong] = RF_GRAY;
    style.Colors[ImGuiCol_TableBorderLight] = RF_GRAY;
    style.Colors[ImGuiCol_TableRowBg] = RF_DARKGRAY;
    style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.f, 1.f, 1.f, 0.06f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    style.Colors[ImGuiCol_DragDropTarget] = RF_ORANGE;
    style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.f);
#pragma endregion

    m_imgui.defaultFont = m_imgui.io->Fonts->AddFontFromMemoryTTF(ddinFont, sizeof(ddinFont), 14);

    ImGui_ImplWin32_Init(m_platform.m_window);
    ImGui_ImplOpenGL3_Init("#version 330");

    m_imgui.gizmoType = ImGuizmo::OPERATION::TRANSLATE;
    m_imgui.gizmoMode = ImGuizmo::MODE::LOCAL;

    m_imgui.icons[0] = (void*)(u64)LoadTextureFromMemory(new_scene_icon , sizeof(new_scene_icon));
    m_imgui.icons[1] = (void*)(u64)LoadTextureFromMemory(save_scene_icon, sizeof(save_scene_icon));
    m_imgui.icons[2] = (void*)(u64)LoadTextureFromMemory(pause_icon     , sizeof(pause_icon));
    m_imgui.icons[3] = (void*)(u64)LoadTextureFromMemory(resume_icon    , sizeof(resume_icon));
    m_imgui.icons[4] = (void*)(u64)LoadTextureFromMemory(translate_icon , sizeof(translate_icon));
    m_imgui.icons[5] = (void*)(u64)LoadTextureFromMemory(rotate_icon    , sizeof(rotate_icon));
    m_imgui.icons[6] = (void*)(u64)LoadTextureFromMemory(scale_icon     , sizeof(scale_icon));
    m_imgui.icons[7] = (void*)(u64)LoadTextureFromMemory(new_entity_icon, sizeof(new_entity_icon));
    m_imgui.icons[8] = (void*)(u64)LoadTextureFromMemory(new_cube_icon  , sizeof(new_cube_icon));
    m_imgui.icons[9] = (void*)(u64)LoadTextureFromMemory(new_sphere_icon, sizeof(new_sphere_icon));
}

void Engine::DrawTopBar(const ImGuiViewport* viewport, float titleBarHeight, float toolbarSize, float totalHeight, float buttonHeight)
{
    const ImGuiWindowFlags window_flags = 0
        | ImGuiWindowFlags_NoDocking
        | ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoScrollbar
        | ImGuiWindowFlags_NoSavedSettings;

    SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y));
    SetNextWindowSize(ImVec2(viewport->Size.x, titleBarHeight));
    SetNextWindowViewport(viewport->ID);

    PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15f, 0.15, 0.15f, 1.f));
    Begin("Topbar", (bool*)0, window_flags | ImGuiWindowFlags_NoBringToFrontOnFocus);
    PopStyleColor();

    char* windowTitle = "RedFox Engine";
    SetCursorPosX((GetWindowWidth() - CalcTextSize(windowTitle).x) / 2.f);
    Text(windowTitle);

    SameLine();
    SetCursorPosX(GetWindowWidth() - buttonHeight - 5.f);
    if (ButtonEx("X", ImVec2(buttonHeight, titleBarHeight), ImGuiButtonFlags_FlattenChildren))
        m_platform.m_running = 0;

    SameLine();
    SetCursorPosX(GetWindowWidth() - (buttonHeight * 2.f) - 10.f);
    if (ButtonEx("[__]", ImVec2(buttonHeight, titleBarHeight), ImGuiButtonFlags_FlattenChildren))
        m_platform.Maximize();

    End();

    SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + titleBarHeight + 8.f));
    SetNextWindowSize(ImVec2(viewport->Size.x, toolbarSize));
    SetNextWindowViewport(viewport->ID);

    PushStyleColor(ImGuiCol_Border, (const ImVec4)RF_GRAY);
    PushStyleVar(ImGuiStyleVar_WindowBorderSize, 4.f);
    PushStyleVar(ImGuiStyleVar_WindowRounding, 4.f);
    Begin("Toolbar", (bool*)0, window_flags);
    PopStyleVar(2);
    PopStyleColor();

    PushStyleVar(ImGuiStyleVar_FrameRounding, 4.f);

    if (ImageButton("NEW SCENE", m_imgui.icons[0], ImVec2(buttonHeight, buttonHeight)))
    {
        m_scene.gameObjectCount = 1;
        m_imgui.selectedObject = 0;
        m_memoryManager.m_memory.arena.usedSize = m_memoryManager.m_sceneUsedMemory;
        m_scene.m_name = initStringChar("Sample Scene", 255, &m_memoryManager.m_memory.arena);
    }

    SameLine();
    SetCursorPosX(GetItemRectMin().x + GetItemRectSize().x + 10.f);
    if (ImageButton("SAVE SCENE", m_imgui.icons[1], ImVec2(buttonHeight, buttonHeight))
        || (IsKeyDown(ImGuiKey_S) && IsKeyDown(ImGuiKey_LeftCtrl)) ) //TODO Add security on CTRL+S to only add SaveScene when there is a change
    {
        SaveScene(m_scene.m_name.data, m_scene);
    }

    SameLine();
    SetCursorPosX(GetItemRectMin().x + GetItemRectSize().x + 10.f);
    if (Button("LOAD SCENE", ImVec2(100, 25)))
    {
        LoadScene(m_scene.m_name.data);
    }

    SameLine();
    SetCursorPosX(GetItemRectMin().x + GetItemRectSize().x + 32.f);
    if (ImageButton("PAUSE", m_scene.isPaused ? m_imgui.icons[3] : m_imgui.icons[2], ImVec2(buttonHeight, buttonHeight)))
        m_scene.isPaused = !m_scene.isPaused;

    SameLine();
    SetCursorPosX(GetItemRectMin().x + GetItemRectSize().x + 32.f);
    if (ImageButton("TRANSLATE", m_imgui.icons[4], ImVec2(buttonHeight, buttonHeight), ImVec2(0, 0), ImVec2(1, 1),
        m_imgui.gizmoType == ImGuizmo::OPERATION::TRANSLATE ? RF_LIGHTGRAYBLUE : RF_DARKORANGE))
        m_imgui.gizmoType = ImGuizmo::OPERATION::TRANSLATE;

    SameLine();
    SetCursorPosX(GetItemRectMin().x + GetItemRectSize().x + 10.f);
    if (ImageButton("ROTATE", m_imgui.icons[5], ImVec2(buttonHeight, buttonHeight), ImVec2(0, 0), ImVec2(1, 1),
        m_imgui.gizmoType == ImGuizmo::OPERATION::ROTATE ? RF_LIGHTGRAYBLUE : RF_DARKORANGE))
        m_imgui.gizmoType = ImGuizmo::OPERATION::ROTATE;

    SameLine();
    SetCursorPosX(GetItemRectMin().x + GetItemRectSize().x + 10.f);
    if (ImageButton("SCALE", m_imgui.icons[6], ImVec2(buttonHeight, buttonHeight), ImVec2(0, 0), ImVec2(1, 1),
        m_imgui.gizmoType == ImGuizmo::OPERATION::SCALE ? RF_LIGHTGRAYBLUE : RF_DARKORANGE))
        m_imgui.gizmoType = ImGuizmo::OPERATION::SCALE;

    SameLine();
    bool isLocal = m_imgui.gizmoMode == ImGuizmo::MODE::LOCAL;
    SetCursorPosX(GetItemRectMin().x + GetItemRectSize().x + 10.f);
    if (ButtonEx(isLocal ? "LOCAL" : "WORLD", ImVec2(0, buttonHeight)))
        m_imgui.gizmoMode = isLocal ? ImGuizmo::MODE::WORLD : ImGuizmo::MODE::LOCAL;

    SameLine();
    SetCursorPosX(GetItemRectMin().x + GetItemRectSize().x + 32.f);
    if (ImageButton("ADD ENTITY", m_imgui.icons[7], ImVec2(buttonHeight, buttonHeight)))
    {
        GameObject* newGameObject = &m_scene.gameObjects[m_scene.gameObjectCount++];
        *newGameObject = { 0 };
        char tmp[255];
        int size = snprintf(tmp, 255, "New entity #%d", m_scene.gameObjectCount - 1);
        newGameObject->name = initStringChar(tmp, size, &m_memoryManager.m_memory.arena);
        newGameObject->name.capacity = 255;
        newGameObject->orientation = { 1,0,0,0 };
        newGameObject->scale = { 1,1,1 };
        newGameObject->modelIndex = -1;
    }

    SameLine();
    SetCursorPosX(GetItemRectMin().x + GetItemRectSize().x + 10.f);
    if (ImageButton("ADD CUBE", m_imgui.icons[8], ImVec2(buttonHeight, buttonHeight)))
    {
        GameObject* newGameObject = &m_scene.gameObjects[m_scene.gameObjectCount++];
        *newGameObject = { };
        char tmp[255];
        int size = snprintf(tmp, 255, "New cube #%d", m_scene.gameObjectCount - 1);
        newGameObject->name = initStringChar(tmp, size, &m_memoryManager.m_memory.arena);
        newGameObject->name.capacity = 255;
        newGameObject->orientation = { 1,0,0,0 };
        newGameObject->scale = { 1,1,1 };
        newGameObject->modelIndex = 0;
        //m_physx.CreateCubeCollider(newGameObject);
    }

    SameLine();
    SetCursorPosX(GetItemRectMin().x + GetItemRectSize().x + 10.f);
    if (ImageButton("ADD SPHERE", m_imgui.icons[9], ImVec2(buttonHeight, buttonHeight)))
    {
        GameObject* newGameObject = &m_scene.gameObjects[m_scene.gameObjectCount++];
        *newGameObject = { };
        char tmp[255];
        int size = snprintf(tmp, 255, "New sphere #%d", m_scene.gameObjectCount - 1);
        newGameObject->name = initStringChar(tmp, size, &m_memoryManager.m_memory.arena);
        newGameObject->name.capacity = 255;
        newGameObject->orientation = { 1,0,0,0 };
        newGameObject->scale = { 1,1,1 };
        newGameObject->modelIndex = 1;
        //m_physx.CreateSphereCollider(newGameObject);
    }

    SameLine();
    if (Button("ADD UI", ImVec2(0, buttonHeight)))
    {
        GameUI* newGameUI = &m_scene.gameUIs[m_scene.gameUICount++];
        *newGameUI = {};
        char tmp[255];
        int size = snprintf(tmp, 255, "New UI #%d", m_scene.gameUICount - 1);
        newGameUI->name = initStringChar(tmp, size, &m_memoryManager.m_memory.arena);
        newGameUI->name.capacity = 255;
        newGameUI->text = initStringChar("", 254, &m_memoryManager.m_memory.arena);
        newGameUI->size = {100, 100};
    }
    PopStyleVar();
    End();
}

int Engine::DrawDockSpace(const ImGuiViewport* viewport, ImGuiDockNodeFlags dockspace_flags, const ImGuiWindowClass* window_class)
{
    if (viewport == NULL)
        viewport = GetMainViewport();

    const ImGuiWindow* window = GetCurrentWindow();
    const float titleBarHeight = window->TitleBarHeight();
    const float toolbarSize = 44.f;
    const float totalHeight = titleBarHeight + toolbarSize;
    const float buttonHeight = 32.f;


    SetNextWindowPos(ImVec2(viewport->WorkPos.x, viewport->WorkPos.y + totalHeight + 8.f));
    SetNextWindowSize(ImVec2(viewport->WorkSize.x, viewport->WorkSize.y - totalHeight - 8.f));
    SetNextWindowViewport(viewport->ID);

    ImGuiWindowFlags host_window_flags = 0;
    host_window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus;

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        host_window_flags |= ImGuiWindowFlags_NoBackground;

    char label[32];
    ImFormatString(label, 32, "DockSpaceViewport_%08X", viewport->ID);

    PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    Begin(label, NULL, host_window_flags);
    PopStyleVar(3);

    ImGuiID dockspace_id = GetID("DockSpace");
    DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags, window_class);
    End();

    DrawTopBar(viewport, titleBarHeight, toolbarSize, totalHeight, buttonHeight);

    return dockspace_id;
}

void Engine::DrawSceneNodes(bool is_child, int index)
{
    int childrenCount = m_scene.GetChildrenCount(index);

    ImGuiTreeNodeFlags flags;
    if (is_child && childrenCount == 0)
        flags = ImGuiTreeNodeFlags_Bullet;
    else
        flags = (childrenCount == 0) ? ImGuiTreeNodeFlags_Leaf : ImGuiTreeNodeFlags_OpenOnArrow;
    if (index == m_imgui.selectedObject && m_imgui.selectedObject != 0)
        flags |= ImGuiTreeNodeFlags_Selected;
    flags |= ImGuiTreeNodeFlags_SpanFullWidth;

    char* gameObjectName = (char*)m_scene.gameObjects[index].name.data;
    bool nodeOpen = TreeNodeEx(gameObjectName[0] == '\0' ? " " : gameObjectName, flags, "%s", gameObjectName);
    if ((IsItemClicked() && !IsItemToggledOpen()) || IsItemFocused())
    {
        m_imgui.selectedObject = index;
        m_imgui.selectedUI = 0;
    }

    if (BeginDragDropSource())
    {
        SetDragDropPayload("_SCENENODE", &index, sizeof(index));
        Text("Moving %s", m_scene.gameObjects[index].name.data);
        EndDragDropSource();
    }

    if (BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = AcceptDragDropPayload("_SCENENODE"))
        {
            if (payload->IsDelivery())
            {
                int movedIndex = *(int*)payload->Data;
                m_scene.gameObjects[movedIndex].parent = index;
            }
            EndDragDropTarget();
        }
    }

    if (nodeOpen)
    {
        if (childrenCount)
        {
            int* children = m_scene.GetChildren(index, &m_memoryManager.m_memory.temp);
            for (int i = 0; i < childrenCount; i++)
                DrawSceneNodes(true, children[i]);
        }
        TreePop();
    }
}

void Engine::DrawSceneNodesUI(bool is_child, int index)
{
    int childrenCount = m_scene.GetChildrenCount(index);

    ImGuiTreeNodeFlags flags;
    if (is_child && childrenCount == 0)
        flags = ImGuiTreeNodeFlags_Bullet;
    else
        flags = (childrenCount == 0) ? ImGuiTreeNodeFlags_Leaf : ImGuiTreeNodeFlags_OpenOnArrow;
    if (index == m_imgui.selectedUI && m_imgui.selectedUI != 0)
        flags |= ImGuiTreeNodeFlags_Selected;
    flags |= ImGuiTreeNodeFlags_SpanFullWidth;

    bool nodeOpen = TreeNodeEx((char*)m_scene.gameUIs[index].name.data, flags, "%s",
        (char*)m_scene.gameUIs[index].name.data);
    if ((IsItemClicked() && !IsItemToggledOpen()) || IsItemFocused())
    {
        m_imgui.selectedUI = index;
        m_imgui.selectedObject = 0;
    }

    if (BeginDragDropSource())
    {
        SetDragDropPayload("_SCENENODE", &index, sizeof(index));
        Text("Moving %s", m_scene.gameUIs[index].name.data);
        EndDragDropSource();
    }

    if (BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = AcceptDragDropPayload("_SCENENODE"))
        {
            if (payload->IsDelivery())
            {
                int movedIndex = *(int*)payload->Data;
                m_scene.gameUIs[movedIndex].parent = index;
            }
            EndDragDropTarget();
        }
    }

    if (nodeOpen)
    {
        if (childrenCount)
        {
            int* children = m_scene.GetChildren(index, &m_memoryManager.m_memory.temp);
            for (int i = 0; i < childrenCount; i++)
                DrawSceneNodesUI(true, children[i]);
        }
        TreePop();
    }
}

void Engine::DrawEditor()
{
    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
    if (Begin("Editor", (bool*)0, m_imgui.windowFlags))
    {
        WindowDimension& dimension = m_platform.m_windowDimension;
        ImVec2 content = GetContentRegionAvail();
        ImVec2 windowPos = GetWindowPos();

        if (content.x != 0 && content.y != 0)
        {
            if (content.x != dimension.width || content.y != dimension.height)
                m_graphics.UpdateImGUIFrameBuffer(dimension, { (int)content.x, (int)content.y });
            void* framebuffer = (void*)((u64)m_graphics.m_imguiTexture);
            Image(framebuffer,
                ImVec2(dimension.width, dimension.height), ImVec2(0, 1), ImVec2(1, 0));
        }

        ImVec2 vPos = GetWindowPos();
        ImVec2 vMin = GetWindowContentRegionMin() + vPos;
        ImVec2 vMax = GetWindowContentRegionMax() + vPos;
        ImVec2 mousePos = GetMousePos();
        m_imgui.mousePosEditor = {
            mousePos.x * dimension.width / content.x - vMin.x,
            mousePos.y * dimension.height / content.y - vMin.y
        };

        RedFoxMaths::Float2 uiPos, convertedPos, uiSize;
        for (int i = 1; i < m_scene.gameUICount; i++)
        {

            uiPos = m_scene.gameUIs[i].screenPosition;
            uiSize = m_scene.gameUIs[i].size;

            convertedPos = {
                vMax.x * (uiPos.x / 100),
                dimension.height - dimension.height * (uiPos.y / 100) - uiSize.y
            };

            if (m_imgui.mousePosEditor.x > convertedPos.x &&
                m_imgui.mousePosEditor.x <= convertedPos.x + uiSize.x &&
                m_imgui.mousePosEditor.y > convertedPos.y &&
                m_imgui.mousePosEditor.y <= convertedPos.y + uiSize.y)
            {
                m_scene.gameUIs[i].isHovered = true;
                m_scene.gameUIs[i].isPressed = IsMouseClicked(ImGuiMouseButton_Left);
            }
            else
            {
                m_scene.gameUIs[i].isHovered = false;
            }
        }

        static float averageFps;
        if (m_time.delta)
            m_imgui.fps[m_imgui.currentFrame++ % 255] = (1.0f / m_time.delta);
        if (m_imgui.fpsUpdate >= 0.5)
        {
            averageFps = m_imgui.fps[0];
            for (int i = 1; i < 255; i++)
                averageFps = (averageFps + m_imgui.fps[i]) / 2.0f;
            // averageFps = 1.0 / m_imguiaverageFps;
            m_imgui.fpsUpdate = 0;
        }
        m_imgui.fpsUpdate += m_time.delta;
        SetCursorPos(ImVec2(vMax.x - vPos.x - 115, vMin.y - vPos.y));
        PushStyleColor(ImGuiCol_WindowBg, RF_DARKGRAY);
        BeginChild("Fps counter", ImVec2(115, vMin.y - vPos.y));
        PushStyleColor(ImGuiCol_Text, RF_ORANGE);
        float deltaTime = RedFoxMaths::Misc::Clamp(m_time.delta, 0, 1);
        Text(" %.f FPS | %.2f ms", averageFps, deltaTime * 1000);
        PopStyleColor();
        EndChild();
        PopStyleColor();
        SetItemAllowOverlap();

        SetCursorPos(GetWindowContentRegionMin());
        if (ArrowButton("Editor settings button", ImGuiDir_Down))
            m_imgui.editorMenuOpen = !m_imgui.editorMenuOpen;
        if (m_imgui.editorMenuOpen == true)
        {
            ImVec2 menuPos = vMin + ImVec2(0, 20);
            ImVec2 menuSize = ImVec2(200, 250);
            ImVec2 menuAbsSize = menuPos + menuSize;
            SetNextWindowPos(menuPos);
            BeginChild("Editor settings", menuSize);
            if (IsMouseClicked(ImGuiMouseButton_Left) &&
                !(mousePos.x + 1 > menuPos.x && mousePos.x < menuAbsSize.x &&
                    mousePos.y > menuPos.y && mousePos.y < menuAbsSize.y))
            {
                m_imgui.editorMenuOpen = false;
            }
            if (m_editorCameraEnabled)
                m_imgui.editorMenuOpen = false;
            SeparatorText("Camera");
            Text("Speed"); SameLine();
            SetNextItemWidth(-FLT_MIN);
            SliderFloat("CameraSpeed", &m_editorCameraSpeed, 1, 4);

            SeparatorText("Grid");
            Text("Translation snap"); SameLine();
            SetNextItemWidth(-FLT_MIN);
            DragInt("TSnap", &m_imgui.translateSnap, 10, 1, 1000, "%d", ImGuiSliderFlags_AlwaysClamp);
            Text("Rotation snap"); SameLine();
            SetNextItemWidth(-FLT_MIN);
            DragInt("RSnap", &m_imgui.rotateSnap, 10, 1, 180, "%d", ImGuiSliderFlags_AlwaysClamp);
            Text("Scale snap"); SameLine();
            SetNextItemWidth(-FLT_MIN);
            DragInt("SSnap", &m_imgui.scaleSnap, 10, 1, 1000, "%d", ImGuiSliderFlags_AlwaysClamp);

            SeparatorText("Editor UI");
            Text("Drag speed"); SameLine();
            SetNextItemWidth(-FLT_MIN);
            DragFloat("Drag speed", &m_imgui.dragSpeed, 10, 1, 1000, "%.3f", ImGuiSliderFlags_AlwaysClamp);

            EndChild();
        }
        SetItemAllowOverlap();

        if (m_imgui.selectedObject != 0 && m_scene.isPaused && !m_imgui.lockEditor)
        {
            ImGuizmo::SetDrawlist();
            GetCurrentWindow();
            ImGuizmo::SetRect(windowPos.x, windowPos.y, content.x, content.y);
            RedFoxMaths::Mat4 cameraProjection = m_editorCamera.m_projection.GetTransposedMatrix();
            RedFoxMaths::Mat4 cameraView = m_editorCamera.GetViewMatrix().GetTransposedMatrix();
            RedFoxMaths::Mat4 transformMat = m_scene.GetWorldMatrix(m_imgui.selectedObject).GetTransposedMatrix();
            RedFoxMaths::Mat4 deltaMat = { };
            float* cameraViewPtr = (float*)cameraView.AsPtr();
            float* cameraProjectionPtr = (float*)cameraProjection.AsPtr();
            float* transformMatPtr = (float*)transformMat.AsPtr();
            float* deltaMatPtr = (float*)deltaMat.AsPtr();
            float snap3[3] = { 0, 0, 0 };
            if (IsKeyPressed(ImGuiKey_W))
                m_imgui.gizmoType = ImGuizmo::OPERATION::TRANSLATE;
            else if (IsKeyPressed(ImGuiKey_R))
                m_imgui.gizmoType = ImGuizmo::OPERATION::ROTATE;
            else if (IsKeyPressed(ImGuiKey_E))
                m_imgui.gizmoType = ImGuizmo::OPERATION::SCALE;

            switch (m_imgui.gizmoType)
            {
            case ImGuizmo::OPERATION::TRANSLATE:
                snap3[0] = (float)m_imgui.translateSnap;
                snap3[1] = (float)m_imgui.translateSnap;
                snap3[2] = (float)m_imgui.translateSnap;
                break;
            case ImGuizmo::OPERATION::ROTATE:
                snap3[0] = (float)m_imgui.rotateSnap;
                snap3[1] = (float)m_imgui.rotateSnap;
                snap3[2] = (float)m_imgui.rotateSnap;
                break;
            case ImGuizmo::OPERATION::SCALE:
                snap3[0] = (float)m_imgui.scaleSnap;
                snap3[1] = (float)m_imgui.scaleSnap;
                snap3[2] = (float)m_imgui.scaleSnap;
                break;
            default:
                break;
            }

            bool snapping = IsKeyDown(ImGuiKey_LeftCtrl);
            m_imgui.manipulatingGizmo = ImGuizmo::Manipulate(cameraViewPtr, cameraProjectionPtr,
                m_imgui.gizmoType, m_imgui.gizmoMode, transformMatPtr, deltaMatPtr,
                snapping ? snap3 : nullptr);

            if (ImGuizmo::IsUsing())
            {
                using namespace RedFoxMaths;
                Float3 translation, rotation, scale;
                Float3 scaleUnit = { 1, 1, 1 };
                ImGuizmo::DecomposeMatrixToComponents(deltaMat.AsPtr(),
                    (float*)&translation.x, (float*)&rotation.x, (float*)&scale.x);

                m_scene.gameObjects[m_imgui.selectedObject].orientation =
                    Quaternion::Hamilton(Quaternion::FromEuler(rotation * DEG2RAD),
                        m_scene.gameObjects[m_imgui.selectedObject].orientation);
                if (m_imgui.gizmoType != ImGuizmo::OPERATION::ROTATE)
                {
                    m_scene.gameObjects[m_imgui.selectedObject].position += translation;
                    m_scene.gameObjects[m_imgui.selectedObject].scale += scale - scaleUnit;
                }
                m_scene.gameObjects[m_imgui.selectedObject].scale.x = RedFoxMaths::Misc::Clamp(m_scene.gameObjects[m_imgui.selectedObject].scale.x, 1, 10000);
                m_scene.gameObjects[m_imgui.selectedObject].scale.y = RedFoxMaths::Misc::Clamp(m_scene.gameObjects[m_imgui.selectedObject].scale.y, 1, 10000);
                m_scene.gameObjects[m_imgui.selectedObject].scale.z = RedFoxMaths::Misc::Clamp(m_scene.gameObjects[m_imgui.selectedObject].scale.z, 1, 10000);
                m_physx.SetTransform(m_imgui.selectedObject, m_scene.GetWorldTransform(m_imgui.selectedObject));
            }
        }

        if (m_imgui.mousePosEditor.x > 0 && m_imgui.mousePosEditor.x < content.x &&
            m_imgui.mousePosEditor.y > 0 && m_imgui.mousePosEditor.y < content.y)
        {
            RedFoxMaths::Float3 ray_ndc = {
                (2.0f * m_imgui.mousePosEditor.x) / content.x - 1.0f,
                1.0f - (2.0f * m_imgui.mousePosEditor.y) / content.y,
                1
            };
            RedFoxMaths::Float4 ray_clip = { ray_ndc.x, ray_ndc.y, -1, 1 };
            RedFoxMaths::Float4 ray_eye = m_editorCamera.m_projection.GetInverseMatrix() * ray_clip;
            ray_eye = { ray_eye.x, ray_eye.y, -1, 0 };
            RedFoxMaths::Float4 ray_world = m_editorCamera.GetViewMatrix().GetInverseMatrix() * ray_eye;
            ray_world.Normalize();

            if (IsMouseClicked(ImGuiMouseButton_Left) && !m_imgui.manipulatingGizmo && !m_imgui.lockEditor)
            {
                RedFoxMaths::Mat4 view = m_editorCamera.GetViewMatrix().GetInverseMatrix();
                physx::PxVec3 origin = { view.mat[0][3], view.mat[1][3], view.mat[2][3] };
                physx::PxVec3 unitDir = { ray_world.x, ray_world.y, ray_world.z };
                physx::PxRaycastBuffer hitCalls;
                if (m_physx.m_scene->raycast(origin, unitDir, m_editorCamera.m_parameters._far, hitCalls, physx::PxHitFlag::eANY_HIT))
                {
                    physx::PxRaycastHit hit = hitCalls.getAnyHit(0);
                    if (hit.actor)
                    {
                        int hitIndex = hit.actor->getInternalActorIndex();
                        if (hitIndex < (int)m_scene.gameObjectCount && hitIndex > 0)
                        {
                            m_imgui.selectedObject = hitIndex;
                            m_imgui.mousePickNodeIndex = hitIndex;
                        }
                    }
                }
            }

            if (IsMouseDown(ImGuiMouseButton_Right))
            {
                m_input.lockMouse = m_editorCameraEnabled = true;
            }
            else
            {
                m_editorCameraVelocity = { 0.f, 0.f, 0.f };
                m_input.lockMouse = m_editorCameraEnabled = false;
            }
        }

        if (IsKeyPressed(ImGuiKey_Escape))
            m_imgui.selectedObject = 0;
    }
    End();
    PopStyleVar();
}

void Engine::DrawUIGraph()
{
    if (Begin("UI Graph", (bool*)0, ImGuiWindowFlags_NoCollapse))
    {
        ImGuiTreeNodeFlags rootNodeFlags =
            ImGuiTreeNodeFlags_Framed |
            ImGuiTreeNodeFlags_Leaf |
            ImGuiTreeNodeFlags_AllowItemOverlap |
            ImGuiTreeNodeFlags_DefaultOpen |
            ImGuiTreeNodeFlags_SpanFullWidth;

        if (TreeNodeEx("_TREENODE", rootNodeFlags, " %s", m_scene.m_name.data))
        {
            if (BeginPopupContextItem("RenameScenePopup"))
            {
                if (IsKeyPressed(ImGuiKey_Enter))
                    CloseCurrentPopup();

                SameLine();
                InputText(" ", (char*)m_scene.m_name.data, m_scene.m_name.capacity);
                EndPopup();
            }

            if (IsMouseDoubleClicked(ImGuiMouseButton_Left) && IsItemHovered() && !m_imgui.sceneGraphScrollButtonHovered)
            {
                OpenPopup("RenameScenePopup");
            }

            if (BeginPopupContextItem("RenameUIPopup"))
            {
                if (IsKeyPressed(ImGuiKey_Enter))
                    CloseCurrentPopup();

                SameLine();
                InputText(" ", (char*)m_scene.gameUIs[m_imgui.selectedUI].name.data, m_scene.gameUIs[m_imgui.selectedObject].name.capacity);
                EndPopup();
            }

            if (IsKeyPressed(ImGuiKey_F2, false))
            {
                OpenPopup("RenameUIPopup");
            }

            if (BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = AcceptDragDropPayload("_SCENENODE"))
                {
                    if (payload->IsDelivery())
                    {
                        int* movedGameobject = (int*)payload->Data;
                        m_scene.gameUIs[*movedGameobject].parent = 0;
                    }
                    EndDragDropTarget();
                }
            }

            float sceneGraphWidth = GetContentRegionAvail().x;
            int buttonWidth = 50;
            if (buttonWidth < sceneGraphWidth)
            {
                char tempString[32] = {};
                snprintf(tempString, 32, "%d", m_imgui.sceneGraphScrollStrength);
                SameLine(sceneGraphWidth - (float)buttonWidth / 2);
                if (Button(tempString, ImVec2(buttonWidth, 0)))
                {
                    m_imgui.sceneGraphScrollStrength *= 10;
                    if (m_imgui.sceneGraphScrollStrength > 1000)
                        m_imgui.sceneGraphScrollStrength = 1;
                }
            }
            m_imgui.sceneGraphScrollButtonHovered = IsItemHovered();
        }
        TreePop();

        if (m_scene.gameUICount > 1)
        {
            BeginChild("SceneGraphNodes", ImVec2(0, 0), true, m_imgui.sceneGraphFlags);
            if (m_imgui.uiIndex < 1)
                m_imgui.uiIndex = 1;
            else if (m_imgui.uiIndex > (int)m_scene.gameUICount - 1)
                m_imgui.uiIndex = m_scene.gameUICount - 1;

            int maxItems = (int)GetMainViewport()->Size.y / 16;
            for (int i = 0; i + m_imgui.uiIndex < (int)m_scene.gameUICount && i < maxItems; i++)
            {
                if (m_imgui.mousePickNodeIndex != -1)
                {
                    m_imgui.uiIndex = m_imgui.mousePickNodeIndex;
                    SetScrollY(m_imgui.mousePickNodeIndex - m_imgui.uiIndex);
                    m_imgui.mousePickNodeIndex = -1;
                }

                if (i == 0 && m_imgui.uiIndex > 1 && GetScrollY() == 0)
                {
                    SetScrollY(1);
                    m_imgui.uiIndex -= m_imgui.sceneGraphScrollStrength;
                }

                float scrollMax = GetScrollMaxY();
                if (i == maxItems - 1 && m_imgui.uiIndex + i < (int)m_scene.gameUICount - 1 &&
                    scrollMax == GetScrollY() && scrollMax != 0)
                {
                    SetScrollY(scrollMax - 1);
                    m_imgui.uiIndex += m_imgui.sceneGraphScrollStrength;
                }

                if (m_imgui.uiIndex + i < 1)
                    m_imgui.uiIndex = 1;
                else if (m_imgui.uiIndex + i > (int)m_scene.gameUICount - 1)
                    m_imgui.uiIndex = m_scene.gameUICount - i - 1;

                if (m_scene.gameUIs[i + m_imgui.uiIndex].parent == 0)
                    DrawSceneNodesUI(false, i + m_imgui.uiIndex);
            }
            EndChild();
        }
    }
    End();
}

void Engine::DrawSceneGraph()
{
    if (Begin("Scene Graph", (bool*)0, ImGuiWindowFlags_NoCollapse))
    {

        if (TreeNodeEx("_TREENODE", m_imgui.rootNodeFlags, " %s", m_scene.m_name.data))
        {
            if (BeginPopupContextItem("RenameScenePopup"))
            {
                if (IsKeyPressed(ImGuiKey_Enter))
                    CloseCurrentPopup();

                SameLine();
                InputText(" ", (char*)m_scene.m_name.data, m_scene.m_name.capacity);
                EndPopup();
            }

            if (IsMouseDoubleClicked(ImGuiMouseButton_Left) && IsItemHovered() && !m_imgui.sceneGraphScrollButtonHovered)
            {
                OpenPopup("RenameScenePopup");
            }

            if (IsKeyPressed(ImGuiKey_D) && IsKeyPressed(ImGuiKey_LeftCtrl))
            {
                GameObject* newGameObject = &m_scene.gameObjects[m_scene.gameObjectCount++];
                *newGameObject = m_scene.gameObjects[m_imgui.selectedObject];

                char tmp[255];
                int size = snprintf(tmp, 255, "New entity #%d", m_scene.gameObjectCount - 1);
                newGameObject->name = initStringChar(tmp, size, &m_memoryManager.m_memory.arena);

            }

            if (BeginDragDropTarget())
            {             
                
                if (const ImGuiPayload* payload = AcceptDragDropPayload("_SCENENODE"))
                {
                    if (payload->IsDelivery())
                    {
                        int* movedGameobject = (int*)payload->Data;
                        m_scene.gameObjects[*movedGameobject].parent = 0;
                    }
                    EndDragDropTarget();
                }
            }

            if (BeginPopupContextItem("RenameItemPopup"))
            {
                if (IsKeyPressed(ImGuiKey_Enter))
                    CloseCurrentPopup();

                SameLine();
                InputText(" ", (char*)m_scene.gameObjects[m_imgui.selectedObject].name.data, m_scene.gameObjects[m_imgui.selectedObject].name.capacity);
                EndPopup();
            }

            if (IsKeyPressed(ImGuiKey_F2, false) && m_imgui.selectedObject > 0)
            {
                OpenPopup("RenameItemPopup");
            }

            if (IsKeyPressed(ImGuiKey_Delete, false))
            {
                if (m_scene.gameObjectCount - 1 > 1 && m_imgui.selectedObject != 0)
                {
                    int* children = m_scene.GetChildren(m_imgui.selectedObject, &m_memoryManager.m_memory.temp);
                    if (children != nullptr)
                    {
                        int childrenCount = m_scene.GetChildrenCount(m_imgui.selectedObject);
                        for (int i = 0; i < childrenCount; i++)
                            m_scene.gameObjects[*children + i].parent = 0;
                    }
                    m_scene.gameObjects[m_imgui.selectedObject] = m_scene.gameObjects[m_scene.gameObjectCount - 1];
                }
                if (m_scene.gameObjectCount > 1)
                    m_scene.gameObjectCount--;

                m_imgui.selectedObject = 0;
            }

            float sceneGraphWidth = GetContentRegionAvail().x;
            int buttonWidth = 50;
            if (buttonWidth < sceneGraphWidth)
            {
                char tempString[32] = {};
                snprintf(tempString, 32, "%d", m_imgui.sceneGraphScrollStrength);
                SameLine(sceneGraphWidth - (float)buttonWidth / 2);
                if (Button(tempString, ImVec2(buttonWidth, 0)))
                {
                    m_imgui.sceneGraphScrollStrength *= 10;
                    if (m_imgui.sceneGraphScrollStrength > 1000)
                        m_imgui.sceneGraphScrollStrength = 1;
                }
            }
            m_imgui.sceneGraphScrollButtonHovered = IsItemHovered();
        }
        TreePop();

        if (m_scene.gameObjectCount > 1)
        {
            BeginChild("SceneGraphNodes", ImVec2(0, 0), true, m_imgui.sceneGraphFlags);
            if (m_imgui.nodeIndex < 1)
                m_imgui.nodeIndex = 1;
            else if (m_imgui.nodeIndex > (int)m_scene.gameObjectCount - 1)
                m_imgui.nodeIndex = m_scene.gameObjectCount - 1;

            int maxItems = (int)GetMainViewport()->Size.y / 16;
            for (int i = 0; i + m_imgui.nodeIndex < (int)m_scene.gameObjectCount && i < maxItems; i++)
            {
                if (m_imgui.mousePickNodeIndex != -1)
                {
                    m_imgui.nodeIndex = m_imgui.mousePickNodeIndex;
                    SetScrollY(m_imgui.mousePickNodeIndex - m_imgui.nodeIndex);
                    m_imgui.mousePickNodeIndex = -1;
                }

                if (i == 0 && m_imgui.nodeIndex > 1 && GetScrollY() == 0)
                {
                    SetScrollY(1);
                    m_imgui.nodeIndex -= m_imgui.sceneGraphScrollStrength;
                }

                float scrollMax = GetScrollMaxY();
                if (i == maxItems - 1 && m_imgui.nodeIndex + i < (int)m_scene.gameObjectCount - 1 &&
                    scrollMax == GetScrollY() && scrollMax != 0)
                {
                    SetScrollY(scrollMax - 1);
                    m_imgui.nodeIndex += m_imgui.sceneGraphScrollStrength;
                }

                if (m_imgui.nodeIndex + i < 1)
                    m_imgui.nodeIndex = 1;
                else if (m_imgui.nodeIndex + i > (int)m_scene.gameObjectCount - 1)
                    m_imgui.nodeIndex = m_scene.gameObjectCount - i - 1;

                if (m_scene.gameObjects[i + m_imgui.nodeIndex].parent == 0)
                    DrawSceneNodes(false, i + m_imgui.nodeIndex);
            }
            EndChild();
        }
    }
    End();
}

void Engine::DrawAssetsBrowser()
{
    if (Begin("Assets Browser", (bool*)0, m_imgui.windowFlags))
    {
        if (BeginTable("AssetsTable", 2, m_imgui.tableFlags))
        {
            const ImVec2 popupDim(400, 70);
            ImVec2 windowPos = ImGui::GetIO().DisplaySize;

            TableNextRow();
            TableSetColumnIndex(0);
            Text("Models (%d/%d)", m_modelCount, m_maxModel);
            SameLine();

            if (Button("Import model"))
            {
                OpenPopup("Importing a model...");
            }
            SetNextWindowPos((windowPos - popupDim) / 2);
            SetNextWindowSize(popupDim);
            if (BeginPopupModal("Importing a model..."))
            {
                m_imgui.lockEditor = true;
                SetItemDefaultFocus();
                static MyString path = initStringChar("", 256, &m_memoryManager.m_memory.arena);
                Text("Path:");
                SameLine();
                SetNextItemWidth(-FLT_MIN);
                InputText("Path", (char*)path.data, path.capacity);
                if (Button("Import"))
                {
                    if (m_platform.FileExist(path.data))
                    {
                        ObjModelPush(path.data);
                        m_graphics.InitModel(&m_models[m_modelCount - 1]);
                        m_graphics.m_models = m_models;
                        m_graphics.m_modelCount = m_modelCount;
                        m_imgui.lockEditor = false;
                        CloseCurrentPopup();
                    }
                    assignString(path, "");
                }
                SameLine();
                if (Button("Cancel"))
                {
                    assignString(path, "");
                    m_imgui.lockEditor = false;
                    CloseCurrentPopup();
                }
                EndPopup();
            }

            TableSetColumnIndex(1);
            Text("Sounds (%d/%d)", m_soundManager.m_soundCount, m_soundManager.m_maxSounds);
            SameLine();

            if (Button("Import sound"))
            {
                OpenPopup("Importing a sound...");
            }
            SetNextWindowPos((windowPos - popupDim) / 2);
            SetNextWindowSize(popupDim);
            if (BeginPopupModal("Importing a sound..."))
            {
                m_imgui.lockEditor = true;
                SetItemDefaultFocus();
                static MyString path = initStringChar("", 256, &m_memoryManager.m_memory.arena);
                Text("Path:");
                SameLine();
                SetNextItemWidth(-FLT_MIN);
                InputText("Path", (char*)path.data, path.capacity);
                if (Button("Import"))
                {
                    if (m_platform.FileExist(path.data))
                    {
                        m_soundManager.CreateSound(path.data, &m_memoryManager.m_memory.arena);
                        m_imgui.lockEditor = false;
                        CloseCurrentPopup();
                    }
                    assignString(path, "");
                }
                SameLine();
                if (Button("Cancel"))
                {
                    assignString(path, "");
                    m_imgui.lockEditor = false;
                    CloseCurrentPopup();
                }
                EndPopup();
            }
            
            TableNextRow();
            TableSetColumnIndex(0);
            if (BeginListBox("ModelsList", ImVec2(-FLT_MIN, 5 * GetTextLineHeightWithSpacing())))
            {
                for (int i = 0; i < m_modelCount; i++)
                {
                    bool is_selected = m_imgui.selectedModelAsset == i;
                    if (Selectable(m_modelsName[i].data, is_selected))
                        m_imgui.selectedModelAsset = i;

                    if (is_selected)
                        SetItemDefaultFocus();
                }
                EndListBox();
            }

            TableSetColumnIndex(1);
            if (BeginListBox("SoundsList", ImVec2(-FLT_MIN, 5 * GetTextLineHeightWithSpacing())))
            {
                for (int i = 0; i < m_soundManager.m_soundCount; i++)
                {
                    bool is_selected = m_imgui.selectedSoundAsset == i;
                    if (Selectable(m_soundManager.m_soundsName[i].data, is_selected))
                        m_imgui.selectedSoundAsset = i;

                    if (is_selected)
                        SetItemDefaultFocus();
                }
                EndListBox();
            }
            EndTable();
        }
    }
    End();
}

void Engine::DrawProperties()
{
    if (Begin("Properties", (bool*)0, ImGuiWindowFlags_NoCollapse))
    {
        if (m_imgui.selectedObject != 0)
        {
            if (CollapsingHeader("Transform", m_imgui.propertiesFlags))
            {
                if (BeginTable("TransformTable", 2, m_imgui.tableFlags))
                {
                    TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);
                    TableNextRow();
                    TableSetColumnIndex(0);
                    Text("Position");
                    TableSetColumnIndex(1);
                    SetNextItemWidth(-FLT_MIN);
                    if (DragFloat3("TransformPosition", &m_scene.gameObjects[m_imgui.selectedObject].position.x, m_imgui.dragSpeed, -32767.f, 32767.f, "%.3f", ImGuiSliderFlags_AlwaysClamp))
                        m_physx.SetTransform(m_imgui.selectedObject, m_scene.gameObjects[m_imgui.selectedObject].transform);

                    TableNextRow();
                    TableSetColumnIndex(0);
                    Text("Rotation");
                    TableSetColumnIndex(1);
                    SetNextItemWidth(-FLT_MIN);
                    static RedFoxMaths::Float3 rotation;
                    if (DragFloat3("TransformRotation", &rotation.x, m_imgui.dragSpeed, -360.f, 360.f, "%.3f", ImGuiSliderFlags_AlwaysClamp))
                    {
                        rotation *= DEG2RAD;
                        m_scene.gameObjects[m_imgui.selectedObject].orientation =
                            RedFoxMaths::Quaternion::SLerp(m_scene.gameObjects[m_imgui.selectedObject].orientation,
                                RedFoxMaths::Quaternion::FromEuler(rotation), 0.5);
                        m_scene.gameObjects[m_imgui.selectedObject].orientation.Normalize();
                        m_physx.SetTransform(m_imgui.selectedObject, m_scene.gameObjects[m_imgui.selectedObject].transform);
                    }

                    TableNextRow();
                    TableSetColumnIndex(0);
                    Text("Scale");
                    TableSetColumnIndex(1);
                    SetNextItemWidth(-FLT_MIN);
                    DragFloat3("TransformScale", &m_scene.gameObjects[m_imgui.selectedObject].scale.x, m_imgui.dragSpeed, 0.00001f, 32767.f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
                    EndTable();
                }
            }

            if (CollapsingHeader("Behaviour", m_imgui.propertiesFlags))
            {
                Text("GameObject ID: %d", m_imgui.selectedObject);
                SetNextItemWidth(-FLT_MIN);
                
                int* curBehaviourIndex = &m_scene.gameObjects[m_imgui.selectedObject].behaviourIndex;
                if (ImGui::BeginCombo("BehaviourList", m_scene.gameObjectBehaviours[*curBehaviourIndex].name.data))
                {
                    for (int i = 0; i < m_scene.gameObjectBehaviourCount; i++)
                    {
                        bool is_selected = (*curBehaviourIndex == i);
                        if (ImGui::Selectable(m_scene.gameObjectBehaviours[i].name.data, is_selected))
                            *curBehaviourIndex = i;
                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }
            }

            if (CollapsingHeader("Render", m_imgui.propertiesFlags))
            {
                int* modelIndex = &m_scene.gameObjects[m_imgui.selectedObject].modelIndex;
                SeparatorText("Model");
                SetNextItemWidth(-FLT_MIN);
                if (BeginCombo(" ", (*modelIndex != -1) ? (char*)m_modelsName[*modelIndex].data : "None"))
                {
                    for (int i = 0; i < (int)m_modelCount; i++)
                    {
                        bool is_selected = (*modelIndex == i);
                        if (Selectable((char*)m_modelsName[i].data, is_selected))
                            *modelIndex = i;

                        if (is_selected)
                            SetItemDefaultFocus();
                    }
                    EndCombo();
                }

                SeparatorText("Material");
                if (*modelIndex != -1)
                {
                    if (BeginTable("MaterialTable", 2, m_imgui.tableFlags))
                    {
                        TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);
                        TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);
                        TableNextRow();
                        TableSetColumnIndex(0);
                        Text("Color");
                        TableSetColumnIndex(1);
                        SetNextItemWidth(-FLT_MIN);
                        ColorPicker3("MaterialColor",
                            &m_scene.gameObjects[m_imgui.selectedObject].Color.x,
                            ImGuiColorEditFlags_PickerHueWheel);
                        EndTable();
                    }
                }
            }
        }

        if (m_imgui.selectedUI != 0)
        {
            if (CollapsingHeader("Transform", m_imgui.propertiesFlags))
            {
                if (BeginTable("TransformTable", 2, m_imgui.tableFlags))
                {
                    TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);
                    TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);
                    TableNextRow();
                    TableSetColumnIndex(0);

                    ImGui::Text("Position");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    DragFloat2("TransformPosition", &m_scene.gameUIs[m_imgui.selectedUI].screenPosition.x, m_imgui.dragSpeed, 0, 100.f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
                    ImGui::TableNextRow();
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("Text Offset");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    DragFloat2("TextOffset", &m_scene.gameUIs[m_imgui.selectedUI].textOffset.x, m_imgui.dragSpeed, 0, 100.f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);

                    ImGui::Text("Scale");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    DragFloat2("TransformScale", &m_scene.gameUIs[m_imgui.selectedUI].size.x, m_imgui.dragSpeed, 0, 32767.f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
                    ImGui::EndTable();
                }
            }
            if (CollapsingHeader("Text", m_imgui.propertiesFlags))
            {
                if (BeginTable("Attributes", 2, m_imgui.tableFlags))
                {
                    TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);
                    TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);
                    TableNextRow();
                    TableSetColumnIndex(0);

                    ImGui::Text("Text");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    ImGui::InputText("Text", (char*)m_scene.gameUIs[m_imgui.selectedUI].text.data, m_scene.gameUIs[m_imgui.selectedUI].text.capacity);
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::EndTable();
                }
            }

            SeparatorText("Behaviour");
            SetNextItemWidth(-FLT_MIN);

            int* curBehaviourIndex = &m_scene.gameUIs[m_imgui.selectedUI].behaviourIndex;
            if (ImGui::BeginCombo("BehaviourList", m_scene.gameUIBehaviours[*curBehaviourIndex].name.data))
            {
                for (int i = 0; i < m_scene.gameUIBehaviourCount; i++)
                {
                    bool is_selected = (*curBehaviourIndex == i);
                    if (ImGui::Selectable(m_scene.gameUIBehaviours[i].name.data, is_selected))
                        *curBehaviourIndex = i;
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }


            if (CollapsingHeader("Render", m_imgui.propertiesFlags))
            {
                SeparatorText("Colors");
                const char* colors[3] = { "selectedColor", "textColor", "hoverColor" };
                float* variable[3] = { &m_scene.gameUIs[m_imgui.selectedUI].selectedColor.x,
                                      &m_scene.gameUIs[m_imgui.selectedUI].textColor.x,
                                      &m_scene.gameUIs[m_imgui.selectedUI].hoverColor.x
                };
                static int colorIndex = 0;
                static const char* currentColorType = colors[0];


                SetNextItemWidth(-FLT_MIN);
                if (ImGui::BeginCombo("ColorList", currentColorType))
                {
                    for (int i = 0; i < IM_ARRAYSIZE(colors); i++)
                    {
                        bool is_selected = (currentColorType == colors[i]);
                        if (ImGui::Selectable(colors[i], is_selected))
                        {
                            currentColorType = colors[i];
                            colorIndex = i;
                        }
                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }


                if (BeginTable("TextTable", 2, m_imgui.tableFlags))
                {
                    TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);
                    TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);
                    TableNextRow();

                    TableSetColumnIndex(0);
                    SetNextItemWidth(-FLT_MIN);
                    SeparatorText("Button color");
                    ColorPicker3("SelectedColor",
                        variable[colorIndex],
                        ImGuiColorEditFlags_PickerHueWheel);
                    EndTable();

                }
            }
        }
    }
    End();
}

void Engine::DrawWorldProperties()
{
    if (Begin("World Properties", (bool*)0, ImGuiWindowFlags_NoCollapse))
    {
        if (CollapsingHeader("Shaders", m_imgui.propertiesFlags))
        {
            BeginTable("PostProcessTable", 2, m_imgui.tableFlags);
            TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);
            TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);
            for (int i = 0; i < m_graphics.m_postProcessShaders.size(); i++)
            {
                TableNextRow();
                TableSetColumnIndex(0);
                Text(m_graphics.m_postProcessShaders[i].name.c_str());
                TableSetColumnIndex(1);
                Checkbox("Is active", &m_graphics.m_postProcessShaders[i].active);
                Checkbox("Use kernels", &m_graphics.m_postProcessShaders[i].useKernels);

                for (int j = 0; j < m_graphics.m_postProcessShaders[i].kernels.size(); j++)
                {
                    TableNextRow();
                    TableSetColumnIndex(0);
                    Text("Kernel %d %s", i + 1, m_graphics.m_postProcessShaders[i].name.c_str());
                    TableSetColumnIndex(1);
                    SetNextItemWidth(-FLT_MIN);
                    DragFloat3("KernelRow1" + i + j, &m_graphics.m_postProcessShaders[i].kernels[j].kernel.mat16[0], m_imgui.dragSpeed, -32767.f, 32767.f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
                    SetNextItemWidth(-FLT_MIN);
                    DragFloat3("KernelRow2" + i + j, &m_graphics.m_postProcessShaders[i].kernels[j].kernel.mat16[4], m_imgui.dragSpeed, -32767.f, 32767.f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
                    SetNextItemWidth(-FLT_MIN);
                    DragFloat3("KernelRow3" + i + j, &m_graphics.m_postProcessShaders[i].kernels[j].kernel.mat16[8], m_imgui.dragSpeed, -32767.f, 32767.f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
                    Checkbox("Is active" + i + j, &m_graphics.m_postProcessShaders[i].kernels[j].active);
                
                    m_graphics.m_postProcessShaders[i].EditKernel(i, m_graphics.m_postProcessShaders[i].kernels[j].kernel);
                }
                
                if (Button("Add empty kernel") && m_graphics.m_postProcessShaders[i].kernels.size() < MAX_KERNEL)
                {
                    float mat[4][4] = { 0 }; mat[1][1] = 1;
                    Kernel* k = m_graphics.m_postProcessShaders[i].AddKernel(RedFoxMaths::Mat4(mat));
                }
            }
            EndTable();
            if (Button("Import", ImVec2(GetContentRegionAvail().x, 20)) && m_graphics.m_postProcessShaders.size() < m_graphics.m_maxPostProcessShader)
            {
                OpenPopup("Importing a shader...");
            }
            const ImVec2 popupDim(400, 70);
            ImVec2 windowPos = ImGui::GetIO().DisplaySize;
            SetNextWindowPos((windowPos - popupDim) / 2);
            SetNextWindowSize(popupDim);
            if (BeginPopupModal("Importing a shader..."))
            {
                SetItemDefaultFocus();
                static MyString path = initStringChar("", 256, &m_memoryManager.m_memory.arena);
                Text("Path:");
                SameLine();
                SetNextItemWidth(-FLT_MIN);
                InputText("Path", (char*)path.data, path.capacity);
                if (Button("Import"))
                {
                    m_graphics.AddPostProcessShader(&m_memoryManager.m_memory, path.data);
                    assignString(path, "");
                    CloseCurrentPopup();
                }
                SameLine();
                if (Button("Cancel"))
                {
                    assignString(path, "");
                    CloseCurrentPopup();
                }
                EndPopup();
            }
        }
        
        if (CollapsingHeader("General Kernels", m_imgui.propertiesFlags))
        {
            BeginTable("PostProcessTable", 2, m_imgui.tableFlags);
            TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);
            TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);
            for (int i = 0; i < m_graphics.m_kernelCount; i++)
            {
                TableNextRow();
                TableSetColumnIndex(0);
                Text("Kernel %d", i + 1);
                TableSetColumnIndex(1);
                SetNextItemWidth(-FLT_MIN);
                DragFloat3("KernelRow1" + i, &m_graphics.m_kernels[i].kernel.mat16[0], m_imgui.dragSpeed, -32767.f, 32767.f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
                SetNextItemWidth(-FLT_MIN);
                DragFloat3("KernelRow2" + i, &m_graphics.m_kernels[i].kernel.mat16[4], m_imgui.dragSpeed, -32767.f, 32767.f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
                SetNextItemWidth(-FLT_MIN);
                DragFloat3("KernelRow3" + i, &m_graphics.m_kernels[i].kernel.mat16[8], m_imgui.dragSpeed, -32767.f, 32767.f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
                Checkbox("Is active" + i, &m_graphics.m_kernels[i].active);
                
                m_graphics.EditKernel(i, m_graphics.m_kernels[i].kernel);
            }
            EndTable();
            if (Button("Add empty kernel", ImVec2(GetContentRegionAvail().x, 20)) && m_graphics.m_kernelCount < MAX_KERNEL)
            {
                float mat[4][4] = { 0 }; mat[1][1] = 1;
                Kernel* k = m_graphics.AddKernel(RedFoxMaths::Mat4(mat));
            }
        }
    }
    End();
}

void Engine::UpdateIMGUI()
{
    ImGui_ImplWin32_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    NewFrame();

    ImGuizmo::SetOrthographic(false);
    ImGuizmo::BeginFrame();

    DrawDockSpace(GetMainViewport(), m_imgui.dockingFlags, (const ImGuiWindowClass*)0);

    PushFont(m_imgui.defaultFont);
    DrawEditor();
    DrawUIGraph();
    DrawSceneGraph();
    DrawWorldProperties();
    DrawProperties();
    DrawAssetsBrowser();
    PopFont();
}