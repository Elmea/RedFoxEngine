#define MEMORY_IMPLEMENTATION
#include "MyMemory.hpp"

#include "Engine.hpp"

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
    m_gui.io = &GetIO();
    m_gui.io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    m_gui.io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;

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
    m_gui.defaultFont = m_gui.io->Fonts->AddFontFromFileTTF("D-DIN.otf", 14);
#pragma endregion

    ImGui_ImplWin32_Init(m_platform.m_window);
    ImGui_ImplOpenGL3_Init("#version 450");

    m_gui.gizmoType = ImGuizmo::OPERATION::TRANSLATE;
    m_gui.gizmoMode = ImGuizmo::MODE::LOCAL;

    m_gui.icons[0] = (void*)(u64)LoadTextureFromFilePath("Textures/new_scene.png", false, false);
    m_gui.icons[1] = (void*)(u64)LoadTextureFromFilePath("Textures/save_scene.png", false, false);
    m_gui.icons[2] = (void*)(u64)LoadTextureFromFilePath("Textures/pause.png", false, false);
    m_gui.icons[3] = (void*)(u64)LoadTextureFromFilePath("Textures/resume.png", false, false);
    m_gui.icons[4] = (void*)(u64)LoadTextureFromFilePath("Textures/translate.png", false, false);
    m_gui.icons[5] = (void*)(u64)LoadTextureFromFilePath("Textures/rotate.png", false, false);
    m_gui.icons[6] = (void*)(u64)LoadTextureFromFilePath("Textures/scale.png", false, false);
    m_gui.icons[7] = (void*)(u64)LoadTextureFromFilePath("Textures/new_entity.png", false, false);
    m_gui.icons[8] = (void*)(u64)LoadTextureFromFilePath("Textures/new_cube.png", false, false);
    m_gui.icons[9] = (void*)(u64)LoadTextureFromFilePath("Textures/new_sphere.png", false, false);
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

    // TODO(a.perche): Project name here
    SetCursorPosX((GetWindowWidth() - CalcTextSize("Project Name").x) / 2.f);
    Text("Project Name");

    SameLine();
    SetCursorPosX(GetWindowWidth() - buttonHeight - 5.f);
    if (Button("X", ImVec2(buttonHeight, titleBarHeight)))
        m_platform.m_running = 0;

    SameLine();
    SetCursorPosX(GetWindowWidth() - (buttonHeight * 2.f) - 10.f);
    if (Button("[__]", ImVec2(buttonHeight, titleBarHeight)))
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
    
    if (ImageButton("NEW SCENE", m_gui.icons[0], ImVec2(buttonHeight, buttonHeight)))
    {
        m_scene.gameObjectCount = 0;
        m_memoryManager.m_memory.arena.usedSize = m_memoryManager.m_sceneUsedMemory;
        m_scene.m_name = initStringChar("Sample Scene", 255, &m_memoryManager.m_memory.arena);
    }

    SameLine();
    SetCursorPosX(GetItemRectMin().x + GetItemRectSize().x + 10.f);
    if (ImageButton("SAVE SCENE", m_gui.icons[1], ImVec2(buttonHeight, buttonHeight)))
    {
        strncat_s((char*)m_scene.m_name.data, 7, ".scene", m_scene.m_name.capacity);
        SaveScene(m_scene.m_name.data, m_scene);
    }

    SameLine();
    SetCursorPosX(GetItemRectMin().x + GetItemRectSize().x + 32.f);
    if (ImageButton("PAUSE", m_scene.isPaused ? m_gui.icons[3] : m_gui.icons[2], ImVec2(buttonHeight, buttonHeight)))
        m_scene.isPaused = !m_scene.isPaused;

    SameLine();
    SetCursorPosX(GetItemRectMin().x + GetItemRectSize().x + 32.f);
    if (ImageButton("TRANSLATE", m_gui.icons[4], ImVec2(buttonHeight, buttonHeight), ImVec2(0,0), ImVec2(1,1),
        m_gui.gizmoType == ImGuizmo::OPERATION::TRANSLATE ? RF_LIGHTGRAYBLUE : RF_DARKORANGE))
        m_gui.gizmoType = ImGuizmo::OPERATION::TRANSLATE;

    SameLine();
    SetCursorPosX(GetItemRectMin().x + GetItemRectSize().x + 10.f);
    if (ImageButton("ROTATE", m_gui.icons[5], ImVec2(buttonHeight, buttonHeight), ImVec2(0, 0), ImVec2(1, 1),
        m_gui.gizmoType == ImGuizmo::OPERATION::ROTATE ? RF_LIGHTGRAYBLUE : RF_DARKORANGE))
        m_gui.gizmoType = ImGuizmo::OPERATION::ROTATE;

    SameLine();
    SetCursorPosX(GetItemRectMin().x + GetItemRectSize().x + 10.f);
    if (ImageButton("SCALE", m_gui.icons[6], ImVec2(buttonHeight, buttonHeight), ImVec2(0, 0), ImVec2(1, 1),
        m_gui.gizmoType == ImGuizmo::OPERATION::SCALE ? RF_LIGHTGRAYBLUE : RF_DARKORANGE))
        m_gui.gizmoType = ImGuizmo::OPERATION::SCALE;
    
    SameLine();
    SetCursorPosX(GetItemRectMin().x + GetItemRectSize().x + 32.f);
    if (ImageButton("ADD ENTITY", m_gui.icons[7], ImVec2(buttonHeight, buttonHeight)))
    {
        GameObject* newGameObject = &m_scene.gameObjects[m_scene.gameObjectCount++];
        *newGameObject = { };
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
    if (ImageButton("ADD CUBE", m_gui.icons[8], ImVec2(buttonHeight, buttonHeight)))
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
    }

    SameLine();
    SetCursorPosX(GetItemRectMin().x + GetItemRectSize().x + 10.f);
    if (ImageButton("ADD SPHERE", m_gui.icons[9], ImVec2(buttonHeight, buttonHeight)))
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
    }

    ImGui::SameLine();
    if (ImGui::Button("ADD UI", ImVec2(0, buttonHeight)))
    {
        GameUI* newGameUI = &m_scene.gameUIs[m_scene.gameUICount++];
        *newGameUI = {};
        char tmp[255];
        int size = snprintf(tmp, 255, "New UI #%d", m_scene.gameUICount - 1);

        newGameUI->name = initStringChar(tmp, size, &m_memoryManager.m_memory.arena);

        newGameUI->name.capacity = 255;
        newGameUI->scale = { 1, 1 };
        newGameUI->text.data = "hello world";
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
    if (index == m_gui.selectedObject && m_gui.selectedObject != 0)
        flags |= ImGuiTreeNodeFlags_Selected;
    flags |= ImGuiTreeNodeFlags_SpanFullWidth;

    bool nodeOpen = TreeNodeEx((char*)m_scene.gameObjects[index].name.data, flags, "%s", (char*)m_scene.gameObjects[index].name.data);
    if ((IsItemClicked() && !IsItemToggledOpen()) ||
        IsItemFocused())
    {
        m_gui.selectedObject = index;
        m_gui.selectedUI = 0;
    }

    // TODO(a.perche): Double click to focus object or press F (lerp position)
    /*
    if (IsMouseDoubleClicked(ImGuiMouseButton_Left) && IsItemHovered())
    {
        m_editorCamera.position = m_selectedObject->position;
        const RedFoxMaths::Float3 up(0, 1, 0);
        m_editorCamera.SetViewLookAt(m_selectedObject->position, up);
    }
    */

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
    if (index == m_gui.selectedUI && m_gui.selectedUI != 0)
        flags |= ImGuiTreeNodeFlags_Selected;
    flags |= ImGuiTreeNodeFlags_SpanFullWidth;

    bool nodeOpen = TreeNodeEx((char*)m_scene.gameUIs[index].name.data, flags, "%s", (char*)m_scene.gameUIs[index].name.data);
    if ((IsItemClicked() && !IsItemToggledOpen()) || 
        IsItemFocused())
    {
        m_gui.selectedUI = index;
        m_gui.selectedObject = 0;
    }

    // TODO(a.perche): Double click to focus object or press F (lerp position)
    /*
    if (IsMouseDoubleClicked(ImGuiMouseButton_Left) && IsItemHovered())
    {
        m_editorCamera.position = m_selectedObject->position;
        const RedFoxMaths::Float3 up(0, 1, 0);
        m_editorCamera.SetViewLookAt(m_selectedObject->position, up);
    }
    */

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

void Engine::UpdateIMGUI()
{
    int mousePickNodeIndexTmp = -1;

    ImGuiDockNodeFlags dockingFlags =
        ImGuiDockNodeFlags_NoWindowMenuButton |
        ImGuiDockNodeFlags_NoCloseButton;

    ImGui_ImplWin32_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    NewFrame();

    ImGuizmo::SetOrthographic(false);
    ImGuizmo::BeginFrame();
    
    // TODO(a.perche) : Build dockspace at runtime
    DrawDockSpace(GetMainViewport(), dockingFlags, (const ImGuiWindowClass*)0);

    PushFont(m_gui.defaultFont);

    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
    if (Begin("Editor", (bool*)0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar))
    {
        WindowDimension &dimension = m_platform.m_windowDimension;
        ImVec2 content = GetContentRegionAvail();
        ImVec2 windowPos = GetWindowPos();

        if (content.x != 0 && content.y != 0)
        {
            if (content.x != dimension.width || content.y != dimension.height)
                m_graphics.UpdateImGUIFrameBuffer(dimension, {(int)content.x, (int)content.y});
            void *framebuffer = (void*)((u64)m_graphics.m_imguiTexture);
            Image(framebuffer,
                ImVec2(dimension.width, dimension.height), ImVec2(0, 1), ImVec2(1, 0));
        }

        ImVec2 vPos = GetWindowPos();
        ImVec2 vMin = GetWindowContentRegionMin() + vPos;
        ImVec2 vMax = GetWindowContentRegionMax() + vPos;
        ImVec2 mousePos = GetMousePos();
        RedFoxMaths::Float2 mousePosEditor = {
            mousePos.x * dimension.width / content.x - vMin.x,
            mousePos.y * dimension.height / content.y - vMin.y
        };
        
        if (m_time.delta)
        {
            m_gui.fps[m_gui.currentFrame++] = (1.0f / m_time.delta);
            if (m_gui.currentFrame >= (int)(1 / m_time.delta))
            {
                m_gui.currentFrame = 0;
                for (int i = 0; i < (int)(1 / m_time.delta); i += 2)
                    m_gui.averageFps = (m_gui.fps[i] + m_gui.averageFps) / 2.0f;
            }
        }
        
        SetCursorPos(ImVec2(vMax.x - vPos.x - 115, vMin.y - vPos.y));
        PushStyleColor(ImGuiCol_WindowBg, RF_DARKGRAY);
        BeginChild("Fps counter", ImVec2(115, vMin.y - vPos.y));
        PushStyleColor(ImGuiCol_Text, RF_ORANGE);
        m_gui.averageFps = RedFoxMaths::Misc::Clamp(m_gui.averageFps, 0, 1000);
        float deltaTime = RedFoxMaths::Misc::Clamp(m_time.delta, 0, 1);
        Text(" %.f FPS | %.2f ms", m_gui.averageFps, deltaTime * 1000);
        PopStyleColor();
        EndChild();
        PopStyleColor();
        SetItemAllowOverlap();

        SetCursorPos(GetWindowContentRegionMin());
        if (ArrowButton("Editor settings button", ImGuiDir_Down))
            m_gui.editorMenuOpen = !m_gui.editorMenuOpen;
        if (m_gui.editorMenuOpen == true)
        {
            ImVec2 menuPos = vMin + ImVec2(0, 20);
            ImVec2 menuSize = ImVec2(200, 175);
            ImVec2 menuAbsSize = menuPos + menuSize;
            SetNextWindowPos(menuPos);
            BeginChild("Editor settings", menuSize);
            if (IsMouseClicked(ImGuiMouseButton_Left) &&
                !(mousePos.x + 1 > menuPos.x && mousePos.x < menuAbsSize.x &&
                    mousePos.y > menuPos.y && mousePos.y < menuAbsSize.y))
            {
                m_gui.editorMenuOpen = false;
            }
            if (m_editorCameraEnabled)
                m_gui.editorMenuOpen = false;
            SeparatorText("Camera");
            Text("Speed"); SameLine();
            SetNextItemWidth(-FLT_MIN);
            SliderFloat("CameraSpeed", &m_editorCameraSpeed, 1, 4);

            SeparatorText("Grid");
            Text("Translation snap"); SameLine();
            SetNextItemWidth(-FLT_MIN);
            DragInt("TSnap", &m_gui.translateSnap, 10, 1, 1000, "%d", ImGuiSliderFlags_AlwaysClamp);
            Text("Rotation snap"); SameLine();
            SetNextItemWidth(-FLT_MIN);
            DragInt("RSnap", &m_gui.rotateSnap, 10, 1, 180, "%d", ImGuiSliderFlags_AlwaysClamp);
            Text("Scale snap"); SameLine();
            SetNextItemWidth(-FLT_MIN);
            DragInt("SSnap", &m_gui.scaleSnap, 10, 1, 1000, "%d", ImGuiSliderFlags_AlwaysClamp);

            SeparatorText("Editor UI");
            Text("Drag speed"); SameLine();
            SetNextItemWidth(-FLT_MIN);
            DragFloat("Drag speed", &m_gui.dragSpeed, 10, 1, 1000, "%.3f", ImGuiSliderFlags_AlwaysClamp);

            EndChild();
        }
        SetItemAllowOverlap();

        if (m_gui.selectedObject != 0 && !m_editorCameraEnabled && m_scene.isPaused)
        {
            ImGuizmo::SetDrawlist();
            GetCurrentWindow();
            ImGuizmo::SetRect(windowPos.x, windowPos.y, content.x, content.y);
            RedFoxMaths::Mat4 cameraProjection = m_editorCamera.m_projection.GetTransposedMatrix();
            RedFoxMaths::Mat4 cameraView = m_editorCamera.GetViewMatrix().GetTransposedMatrix();
            RedFoxMaths::Mat4 transformMat = m_scene.GetWorldMatrix(m_gui.selectedObject).GetTransposedMatrix();
            RedFoxMaths::Mat4 deltaMat = { };
            float* cameraViewPtr = (float*)cameraView.AsPtr();
            float* cameraProjectionPtr = (float*)cameraProjection.AsPtr();
            float* transformMatPtr = (float*)transformMat.AsPtr();
            float* deltaMatPtr = (float*)deltaMat.AsPtr();
            float snap3[3] = { 0, 0, 0 };
            if (m_input.W)
                m_gui.gizmoType = ImGuizmo::OPERATION::TRANSLATE;
            else if (m_input.R)
                m_gui.gizmoType = ImGuizmo::OPERATION::ROTATE;
            else if (m_input.E)
                m_gui.gizmoType = ImGuizmo::OPERATION::SCALE;

            switch (m_gui.gizmoType)
            {
            case ImGuizmo::OPERATION::TRANSLATE:
                snap3[0] = (float)m_gui.translateSnap;
                snap3[1] = (float)m_gui.translateSnap;
                snap3[2] = (float)m_gui.translateSnap;
                break;
            case ImGuizmo::OPERATION::ROTATE:
                snap3[0] = (float)m_gui.rotateSnap;
                snap3[1] = (float)m_gui.rotateSnap;
                snap3[2] = (float)m_gui.rotateSnap;
                break;
            case ImGuizmo::OPERATION::SCALE:
                snap3[0] = (float)m_gui.scaleSnap;
                snap3[1] = (float)m_gui.scaleSnap;
                snap3[2] = (float)m_gui.scaleSnap;
                break;
            default:
                break;
            }

            bool snapping = m_input.LControl;
            m_gui.manipulatingGizmo = ImGuizmo::Manipulate(cameraViewPtr, cameraProjectionPtr,
                m_gui.gizmoType, m_gui.gizmoMode, transformMatPtr, deltaMatPtr,
                snapping ? snap3 : nullptr);

            if (ImGuizmo::IsUsing())
            {
                using namespace RedFoxMaths;
                Float3 translation, rotation, scale;
                Float3 scaleUnit = { 1, 1, 1 };
                ImGuizmo::DecomposeMatrixToComponents(deltaMat.AsPtr(),
                    (float*)&translation.x, (float*)&rotation.x, (float*)&scale.x);

                m_scene.gameObjects[m_gui.selectedObject].orientation =
                    Quaternion::Hamilton(Quaternion::FromEuler(rotation * DEG2RAD),
                        m_scene.gameObjects[m_gui.selectedObject].orientation);
                if (m_gui.gizmoType != ImGuizmo::OPERATION::ROTATE)
                {
                    m_scene.gameObjects[m_gui.selectedObject].position += translation;
                    m_scene.gameObjects[m_gui.selectedObject].scale += scale - scaleUnit;
                }
                m_scene.gameObjects[m_gui.selectedObject].scale.x = RedFoxMaths::Misc::Clamp(m_scene.gameObjects[m_gui.selectedObject].scale.x, 1, 10000);
                m_scene.gameObjects[m_gui.selectedObject].scale.y = RedFoxMaths::Misc::Clamp(m_scene.gameObjects[m_gui.selectedObject].scale.y, 1, 10000);
                m_scene.gameObjects[m_gui.selectedObject].scale.z = RedFoxMaths::Misc::Clamp(m_scene.gameObjects[m_gui.selectedObject].scale.z, 1, 10000);
            }
        }

        if (mousePosEditor.x > 0 && mousePosEditor.x < content.x &&
            mousePosEditor.y > 0 && mousePosEditor.y < content.y)
        {
            RedFoxMaths::Float3 ray_ndc = {
                (2.0f * mousePosEditor.x) / content.x - 1.0f,
                1.0f - (2.0f * mousePosEditor.y) / content.y,
                1
            };
            RedFoxMaths::Float4 ray_clip = { ray_ndc.x, ray_ndc.y, -1, 1 };
            RedFoxMaths::Float4 ray_eye = m_editorCamera.m_projection.GetInverseMatrix() * ray_clip;
            ray_eye = { ray_eye.x, ray_eye.y, -1, 0 };
            RedFoxMaths::Float4 ray_world = m_editorCamera.GetViewMatrix().GetInverseMatrix() * ray_eye;
            ray_world.Normalize(); 

            if (IsMouseClicked(ImGuiMouseButton_Left) && !m_gui.manipulatingGizmo)
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
                        if (hitIndex < m_scene.gameObjectCount && hitIndex > 0 && hitIndex != m_gui.selectedObject)
                        {
                            m_gui.selectedObject = hitIndex;
                            mousePickNodeIndexTmp = hitIndex;
                        }
                    }
                }
            }

            if (m_input.mouseRClick)
            {
                m_input.lockMouse = m_editorCameraEnabled = true;
            }
            else
            {
                m_editorCameraVelocity = { 0.f, 0.f, 0.f };
                m_input.lockMouse = m_editorCameraEnabled = false;
            }
        }
      
        if (m_input.Escape)
        {
            m_gui.selectedObject = 0;
        }
    }
    End();
    PopStyleVar();

    SameLine();
    if (Begin("Scene Graph", (bool*)0, ImGuiWindowFlags_NoCollapse))
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
                if (m_input.Enter)
                    CloseCurrentPopup();

                SameLine();
                InputText(" ", (char*)m_scene.m_name.data, m_scene.m_name.capacity);
                EndPopup();
            }

            if (IsMouseDoubleClicked(ImGuiMouseButton_Left) && IsItemHovered() && !m_gui.sceneGraphScrollButtonHovered)
            {
                OpenPopup("RenameScenePopup");
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
            
            float sceneGraphWidth = GetContentRegionAvail().x;
            int buttonWidth = 50;
            if (buttonWidth < sceneGraphWidth)
            {
                char tempString[32] = {};
                snprintf(tempString, 32, "%d", m_gui.sceneGraphScrollStrength);
                SameLine(sceneGraphWidth - (float)buttonWidth / 2);
                if (Button(tempString, ImVec2(buttonWidth, 0)))
                {
                    m_gui.sceneGraphScrollStrength *= 10;
                    if (m_gui.sceneGraphScrollStrength > 1000)
                        m_gui.sceneGraphScrollStrength = 1;
                }
            }
            m_gui.sceneGraphScrollButtonHovered = IsItemHovered();
        }
        TreePop();

        if (m_scene.gameObjectCount > 1)
        {
            ImGuiWindowFlags sceneGraphFlags =
                ImGuiWindowFlags_AlwaysHorizontalScrollbar |
                ImGuiWindowFlags_AlwaysVerticalScrollbar |
                ImGuiWindowFlags_NoMove;

            BeginChild("SceneGraphNodes", ImVec2(0, 0), true, sceneGraphFlags);
            if (m_gui.nodeIndex < 1)
                m_gui.nodeIndex = 1;
            else if (m_gui.nodeIndex > (int)m_scene.gameObjectCount - 1)
                m_gui.nodeIndex = m_scene.gameObjectCount - 1;

            int maxItems = (int)GetMainViewport()->Size.y / 16;
            for (int i = 0; i + m_gui.nodeIndex < (int)m_scene.gameObjectCount && i < maxItems; i++)
            {
                if (mousePickNodeIndexTmp != -1)
                {
                    m_gui.nodeIndex = mousePickNodeIndexTmp;
                    SetScrollY(mousePickNodeIndexTmp - m_gui.nodeIndex);
                    mousePickNodeIndexTmp = -1;
                }

                if (i == 0 && m_gui.nodeIndex > 1 && GetScrollY() == 0)
                {
                    SetScrollY(1);
                    m_gui.nodeIndex -= m_gui.sceneGraphScrollStrength;
                }

                float scrollMax = GetScrollMaxY();
                if (i == maxItems - 1 && m_gui.nodeIndex + i < (int)m_scene.gameObjectCount - 1 &&
                    scrollMax == GetScrollY() && scrollMax != 0)
                {
                    SetScrollY(scrollMax - 1);
                    m_gui.nodeIndex += m_gui.sceneGraphScrollStrength;
                }

                if (m_gui.nodeIndex + i < 1)
                    m_gui.nodeIndex = 1;
                else if (m_gui.nodeIndex + i > (int)m_scene.gameObjectCount - 1)
                    m_gui.nodeIndex = m_scene.gameObjectCount - i - 1;

                if (m_scene.gameObjects[i + m_gui.nodeIndex].parent == 0)
                    DrawSceneNodes(false, i + m_gui.nodeIndex);
            }
            EndChild();
        }
    }
    End();

    
    SameLine();
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
                if (m_input.Enter)
                    CloseCurrentPopup();

                SameLine();
                InputText(" ", (char*)m_scene.m_name.data, m_scene.m_name.capacity);
                EndPopup();
            }

            if (IsMouseDoubleClicked(ImGuiMouseButton_Left) && IsItemHovered() && !m_gui.sceneGraphScrollButtonHovered)
            {
                OpenPopup("RenameScenePopup");
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
                snprintf(tempString, 32, "%d", m_gui.sceneGraphScrollStrength);
                SameLine(sceneGraphWidth - (float)buttonWidth / 2);
                if (Button(tempString, ImVec2(buttonWidth, 0)))
                {
                    m_gui.sceneGraphScrollStrength *= 10;
                    if (m_gui.sceneGraphScrollStrength > 1000)
                        m_gui.sceneGraphScrollStrength = 1;
                }
            }
            m_gui.sceneGraphScrollButtonHovered = IsItemHovered();
        }
        TreePop();

        if (m_scene.gameUICount > 1)
        {
            ImGuiWindowFlags sceneGraphFlags =
                ImGuiWindowFlags_AlwaysHorizontalScrollbar |
                ImGuiWindowFlags_AlwaysVerticalScrollbar |
                ImGuiWindowFlags_NoMove;

            BeginChild("SceneGraphNodes", ImVec2(0, 0), true, sceneGraphFlags);
            if (m_gui.uiIndex < 1)
                m_gui.uiIndex = 1;
            else if (m_gui.uiIndex > (int)m_scene.gameUICount - 1)
                m_gui.uiIndex = m_scene.gameUICount - 1;

            int maxItems = (int)GetMainViewport()->Size.y / 16;
            for (int i = 0; i + m_gui.uiIndex < (int)m_scene.gameUICount && i < maxItems; i++)
            {
                if (mousePickNodeIndexTmp != -1)
                {
                    m_gui.uiIndex = mousePickNodeIndexTmp;
                    SetScrollY(mousePickNodeIndexTmp - m_gui.uiIndex);
                    mousePickNodeIndexTmp = -1;
                }

                if (i == 0 && m_gui.uiIndex > 1 && GetScrollY() == 0)
                {
                    SetScrollY(1);
                    m_gui.uiIndex -= m_gui.sceneGraphScrollStrength;
                }

                float scrollMax = GetScrollMaxY();
                if (i == maxItems - 1 && m_gui.uiIndex + i < (int)m_scene.gameUICount - 1 &&
                    scrollMax == GetScrollY() && scrollMax != 0)
                {
                    SetScrollY(scrollMax - 1);
                    m_gui.uiIndex += m_gui.sceneGraphScrollStrength;
                }

                if (m_gui.uiIndex + i < 1)
                    m_gui.uiIndex = 1;
                else if (m_gui.uiIndex + i > (int)m_scene.gameUICount - 1)
                    m_gui.uiIndex = m_scene.gameUICount - i - 1;

                if (m_scene.gameUIs[i + m_gui.uiIndex].parent == 0)
                    DrawSceneNodesUI(false, i + m_gui.uiIndex);
            }
            EndChild();
        }
    }
    End();
    

    if (Begin("Properties", (bool*)0, ImGuiWindowFlags_NoCollapse))
    {
        ImGuiTreeNodeFlags propertiesFlags =
            ImGuiTreeNodeFlags_DefaultOpen |
            ImGuiTreeNodeFlags_OpenOnArrow |
            ImGuiTreeNodeFlags_OpenOnDoubleClick;

        ImGuiTableFlags tableFlags =
            ImGuiTableFlags_RowBg |
            ImGuiTableFlags_SizingStretchSame |
            ImGuiTableFlags_Resizable |
            ImGuiTableFlags_BordersOuter;

        if (m_gui.selectedObject != 0)
        {
            if (CollapsingHeader("Transform", propertiesFlags))
            {
                //TODO(a.perche) : Drag speed according to user param.
                ImGuiTableFlags tableFlags =
                    ImGuiTableFlags_RowBg |
                    ImGuiTableFlags_SizingStretchSame |
                    ImGuiTableFlags_Resizable |
                    ImGuiTableFlags_BordersOuter;

                if (BeginTable("TransformTable", 2, tableFlags))
                {
                    TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);
                    TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);
                    TableNextRow();
                    TableSetColumnIndex(0);
                    Text("Position");
                    TableSetColumnIndex(1);
                    SetNextItemWidth(-FLT_MIN);
                    DragFloat3("TransformPosition", &m_scene.gameObjects[m_gui.selectedObject].position.x, m_gui.dragSpeed, - 32767.f, 32767.f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
                    TableNextRow();
                    TableSetColumnIndex(0);
                    Text("Rotation");
                    TableSetColumnIndex(1);
                    SetNextItemWidth(-FLT_MIN);

                    static RedFoxMaths::Float3 rotation;
                    if (DragFloat3("TransformRotation", &rotation.x, m_gui.dragSpeed, -360.f, 360.f, "%.3f", ImGuiSliderFlags_AlwaysClamp))
                    {
                        /*
                        rotation.x = RedFoxMaths::Misc::Clamp(rotation.x, -360, 360);
                        rotation.y = RedFoxMaths::Misc::Clamp(rotation.y, -360, 360);
                        rotation.z = RedFoxMaths::Misc::Clamp(rotation.z, -360, 360);
                        */
                        rotation *= DEG2RAD;
                        m_scene.gameObjects[m_gui.selectedObject].orientation =
                            RedFoxMaths::Quaternion::SLerp(m_scene.gameObjects[m_gui.selectedObject].orientation,
                                RedFoxMaths::Quaternion::FromEuler(rotation), 0.5);
                        m_scene.gameObjects[m_gui.selectedObject].orientation.Normalize();
                        rotation *= RAD2DEG;
                    }

                    TableNextRow();
                    TableSetColumnIndex(0);
                    Text("Scale");
                    TableSetColumnIndex(1);
                    SetNextItemWidth(-FLT_MIN);
                    DragFloat3("TransformScale", &m_scene.gameObjects[m_gui.selectedObject].scale.x, m_gui.dragSpeed, 0.00001f, 32767.f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
                    EndTable();
                }
            }
            if (CollapsingHeader("Render", propertiesFlags))
            {
                SeparatorText("Model");

                SeparatorText("Material");
                if (BeginTable("MaterialTable", 2, tableFlags))
                {
                    TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);
                    TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);
                    TableNextRow();
                    int modelId = m_scene.gameObjects[m_gui.selectedObject].modelIndex;
                    TableSetColumnIndex(0);
                    Text("Color");
                    TableSetColumnIndex(1);
                    SetNextItemWidth(-FLT_MIN);
                    ColorPicker3("MaterialColor",
                        &m_models[modelId].obj.materials.material->diffuse.x,
                        ImGuiColorEditFlags_PickerHueWheel);
                    EndTable();
                }
            }
        }
  
        if (m_gui.selectedUI != 0)
        {
            if (ImGui::CollapsingHeader("Transform", propertiesFlags))
            {
                //TODO(a.perche) : Drag speed according to user param.
                ImGuiTableFlags tableFlags =
                    ImGuiTableFlags_RowBg |
                    ImGuiTableFlags_SizingStretchSame |
                    ImGuiTableFlags_Resizable |
                    ImGuiTableFlags_BordersOuter;

                if (ImGui::BeginTable("TransformTable", 2, tableFlags))
                {
                    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);

                    ImGui::Text("Position");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    DragFloat2("TransformPosition", &m_scene.gameUIs[m_gui.selectedUI].screenPosition.x, m_gui.dragSpeed, -150.f, 150.f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);

                    ImGui::Text("Scale");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    DragFloat2("TransformScale", &m_scene.gameUIs[m_gui.selectedUI].scale.x, m_gui.dragSpeed, 0, 32767.f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
                    ImGui::EndTable();
                }
            }
            if (ImGui::CollapsingHeader("Text", propertiesFlags))
                ImGui::InputText((char*)&m_scene.gameUIs[m_gui.selectedUI].name, (char*)&m_scene.gameUIs[m_gui.selectedUI].text, 256, 0, 0, 0);
        }
        End();
        PopFont();
    }
}