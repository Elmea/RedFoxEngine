#include "Engine.hpp"

using namespace RedFoxEngine;

#pragma region RedFox_Style_Color_Palette_Declaration
#define RF_BLACK         ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
#define RF_WHITE         ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
#define RF_DARKGRAY      ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
#define RF_GRAY          ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
#define RF_LIGHTGRAY     ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
#define RF_DARKGRAYBLUE  ImVec4(0.28f, 0.30f, 0.37f, 1.00f);
#define RF_GRAYBLUE      ImVec4(0.35f, 0.39f, 0.49f, 1.00f);
#define RF_LIGHTGRAYBLUE ImVec4(0.47f, 0.56f, 0.95f, 1.00f);
#define RF_DARKORANGE    ImVec4(0.81f, 0.41f, 0.19f, 1.00f);
#define RF_ORANGE        ImVec4(0.89f, 0.49f, 0.21f, 1.00f);
#define RF_LIGHTORANGE   ImVec4(1.00f, 0.57f, 0.27f, 1.00f);
#pragma endregion

void Engine::InitIMGUI()
{
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    m_ImGuiIO = &ImGui::GetIO(); (void)m_ImGuiIO;
    m_ImGuiIO->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    m_ImGuiIO->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    // TODO: Necessary backend for Win32 API windowing
    //m_ImGuiIO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

#pragma region RedFox_Style_Definition
    ImGuiStyle& style = ImGui::GetStyle();
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
    style.Colors[ImGuiCol_ChildBg] = RF_BLACK;
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
    defaultFont = m_ImGuiIO->Fonts->AddFontFromFileTTF("D-DIN.otf", 14);
#pragma endregion

    ImGui_ImplWin32_Init(m_platform.m_window);
    ImGui_ImplOpenGL3_Init("#version 450");
}

void Engine::DrawSceneNodes(bool is_child, GameObject* gameObj)
{
    int childrenCount = gameObj->GetChildrenCount(m_gameObjects, m_gameObjectCount);

    ImGuiTreeNodeFlags flags;
    if (is_child && childrenCount == 0)
        flags = ImGuiTreeNodeFlags_Bullet;
    else
        flags = (childrenCount == 0) ? ImGuiTreeNodeFlags_Leaf : ImGuiTreeNodeFlags_OpenOnArrow;
    if (gameObj == m_selectedObject) 
        flags |= ImGuiTreeNodeFlags_Selected;
    flags |= ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick;

    bool nodeOpen = ImGui::TreeNodeEx(gameObj->name, flags, "%s", gameObj->name);
    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
    {
        m_selectedObject = gameObj;
    }

    if (ImGui::BeginDragDropSource())
    {
        ImGui::SetDragDropPayload("_SCENENODE", &gameObj, sizeof(&gameObj));
        ImGui::Text("Moving %s", gameObj->name);
        ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_SCENENODE"))
        {
            if (payload->IsDelivery())
            {
                GameObject* movedGameobject = *(GameObject**)payload->Data;
                movedGameobject->parent = gameObj;
            }
            ImGui::EndDragDropTarget();
        }
    }
    
    if (nodeOpen)
    {
        if (childrenCount)
        {
            GameObject **children = gameObj->GetChildren(m_gameObjects, m_gameObjectCount, &m_tempAllocator);
            for (int i = 0; i < childrenCount; i++)
                DrawSceneNodes(true, children[i]);
        }
        ImGui::TreePop();
    }
}

void Engine::DrawIMGUI()
{
    ImGuiDockNodeFlags dockingFlags =
        ImGuiDockNodeFlags_PassthruCentralNode |
        ImGuiDockNodeFlags_NoWindowMenuButton |
        ImGuiDockNodeFlags_NoCloseButton |
        ImGuiDockNodeFlags_NoDockingInCentralNode;

    ImGui_ImplWin32_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
    
    // TODO(a.perche) : Build dockspace at runtime
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), dockingFlags);

    ImGui::PushFont(defaultFont);
    if (0 && ImGui::Begin("Scene Graph", (bool*)0, ImGuiWindowFlags_NoCollapse))
    {
        ImGuiTreeNodeFlags rootNodeFlags = 
            ImGuiTreeNodeFlags_Framed |
            ImGuiTreeNodeFlags_Leaf |
            ImGuiTreeNodeFlags_DefaultOpen |
            ImGuiTreeNodeFlags_SpanFullWidth;

        if (ImGui::TreeNodeEx("_TREENODE", rootNodeFlags, "Sample Scene"))
        {
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_SCENENODE"))
                {
                    if (payload->IsDelivery())
                    {
                        GameObject* movedGameobject = *(GameObject**)payload->Data;
                        movedGameobject->parent = nullptr;
                    }
                    ImGui::EndDragDropTarget();
                }
            }

            ImGui::Unindent();
            for (int i = 0; i < (int)m_gameObjectCount; i++)
                if (m_gameObjects[i].parent == nullptr)
                    DrawSceneNodes(false, &m_gameObjects[i]);
            ImGui::TreePop();
        }
    ImGui::End();
    }
    ImGui::PopFont();

    ImGui::PushFont(defaultFont);
    if (ImGui::Begin("Properties", (bool*)0, ImGuiWindowFlags_NoCollapse))
    {
        ImGui::Text("%f", m_deltaTime);
        ImGuiTreeNodeFlags propertiesFlags = 
            ImGuiTreeNodeFlags_DefaultOpen |
            ImGuiTreeNodeFlags_OpenOnArrow | 
            ImGuiTreeNodeFlags_OpenOnDoubleClick;

        if (m_selectedObject != nullptr)
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
                    ImGui::DragFloat3("TransformPosition", &m_selectedObject->position.x, 0.001f, -32767.f, 32767.f);
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("Rotation");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    ImGui::DragFloat4("TransformRotation", &m_selectedObject->orientation.a, 0.001f, -32767.f, 32767.f);
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("Scale");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    ImGui::DragFloat("TransformScale", &m_selectedObject->scale, 0.001f, -32767.f, 32767.f);
                    ImGui::EndTable();
                }
            }
        }
    }
    ImGui::End();
    ImGui::PopFont();
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (m_ImGuiIO->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}