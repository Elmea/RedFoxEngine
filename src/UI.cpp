#define MEMORY_IMPLEMENTATION
#include "MyMemory.hpp"

#include "Engine.hpp"

using namespace RedFoxEngine;

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

    ImGui::CreateContext();
    m_ImGuiIO = &ImGui::GetIO(); (void)m_ImGuiIO;
    m_ImGuiIO->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    m_ImGuiIO->ConfigFlags |= ImGuiConfigFlags_DockingEnable;

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
    m_defaultFont = m_ImGuiIO->Fonts->AddFontFromFileTTF("D-DIN.otf", 14);
#pragma endregion

    ImGui_ImplWin32_Init(m_platform.m_window);
    ImGui_ImplOpenGL3_Init("#version 450");

    m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
    m_GizmoMode = ImGuizmo::MODE::LOCAL;
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
    
    ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y));
    ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, titleBarHeight));
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15f, 0.15, 0.15f, 1.f));
    ImGui::Begin("Topbar", (bool*)0, window_flags | ImGuiWindowFlags_NoBringToFrontOnFocus);
    ImGui::PopStyleColor();

    // TODO(a.perche): Project name here
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("Project Name").x) / 2.f);
    ImGui::Text("Project Name");

    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - buttonHeight - 5.f);
    if (ImGui::Button("X", ImVec2(buttonHeight, titleBarHeight)))
        m_platform.m_running = 0;

    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - (buttonHeight * 2.f) - 10.f);
    if (ImGui::Button("[__]", ImVec2(buttonHeight, titleBarHeight)))
        m_platform.Maximize();

    ImGui::End();  
    
    ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + titleBarHeight + 8.f));
    ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, toolbarSize));
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleColor(ImGuiCol_Border, (const ImVec4)RF_GRAY);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 4.f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 4.f);
    ImGui::Begin("Toolbar", (bool*)0, window_flags); 
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();
    
    if (ImGui::BeginPopupContextItem("MainMenu"))
    {
        if (ImGui::Selectable("Save scene"))
            SaveScene(strcat((char*)m_sceneName.data, ".scene"));
        ImGui::EndPopup();
    }

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.f);
    if (ImGui::Button("LOGO HERE", ImVec2(0, buttonHeight)))
        ImGui::OpenPopup("MainMenu");

    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetItemRectMin().x + ImGui::GetItemRectSize().x + 32.f);
    ImGui::Button("PLAY", ImVec2(0, buttonHeight));

    ImGui::SameLine();
    ImGui::Button("BUILD", ImVec2(0, buttonHeight));
    
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetItemRectMin().x + ImGui::GetItemRectSize().x + 64.f);
    if (ImGui::Button("ADD ENTITY", ImVec2(0, buttonHeight)))
    {
        m_gameObjectCount++;
        GameObject* newGameObject = &m_gameObjects[m_gameObjectCount - 1];
        newGameObject->name = (char*)MyMalloc(&m_arenaAllocator, 20);
        sprintf(newGameObject->name, "New entity #%d\0", m_gameObjectCount - 1);
    }

    ImGui::PopStyleVar();
    ImGui::End();
}

int Engine::DrawDockSpace(const ImGuiViewport* viewport, ImGuiDockNodeFlags dockspace_flags, const ImGuiWindowClass* window_class)
{
    if (viewport == NULL)
        viewport = ImGui::GetMainViewport();
    
    const ImGuiWindow* window = ImGui::GetCurrentWindow();
    const float titleBarHeight = window->TitleBarHeight();
    const float toolbarSize = 46.f;
    const float totalHeight = titleBarHeight + toolbarSize;
    const float buttonHeight = toolbarSize - 8.f;

    
    ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x, viewport->WorkPos.y + totalHeight + 8.f));
    ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x, viewport->WorkSize.y - totalHeight - 8.f));
    ImGui::SetNextWindowViewport(viewport->ID);
    
    ImGuiWindowFlags host_window_flags = 0;
    host_window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | 
                         ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
                         ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBringToFrontOnFocus |
                         ImGuiWindowFlags_NoNavFocus;

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        host_window_flags |= ImGuiWindowFlags_NoBackground;

    char label[32];
    ImFormatString(label, IM_ARRAYSIZE(label), "DockSpaceViewport_%08X", viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin(label, NULL, host_window_flags);
    ImGui::PopStyleVar(3);

    ImGuiID dockspace_id = ImGui::GetID("DockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags, window_class);
    ImGui::End();

    DrawTopBar(viewport, titleBarHeight, toolbarSize, totalHeight, buttonHeight);

    return dockspace_id;
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
    flags |= ImGuiTreeNodeFlags_SpanFullWidth;

    bool nodeOpen = ImGui::TreeNodeEx(gameObj->name, flags, "%s", gameObj->name);
    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
    {
        m_selectedObject = gameObj;
    }

    //TODO(a.perche): Fix maths
    /*
    if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
    {
        m_editorCamera.position = m_selectedObject->position;
        const RedFoxMaths::Float3 up(0, 1, 0);
        m_editorCamera.SetViewLookAt(m_selectedObject->position, up);
    }
    */

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
            GameObject** children = gameObj->GetChildren(m_gameObjects,
                m_gameObjectCount, &m_tempAllocator);
            for (int i = 0; i < childrenCount; i++)
                DrawSceneNodes(true, children[i]);
        }
        ImGui::TreePop();
    }
}

void Engine::DrawIMGUI()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    ImGuiDockNodeFlags dockingFlags =
        ImGuiDockNodeFlags_NoWindowMenuButton |
        ImGuiDockNodeFlags_NoCloseButton;

    ImGui_ImplWin32_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    ImGuizmo::SetOrthographic(false);
    ImGuizmo::BeginFrame();
    
    // TODO(a.perche) : Build dockspace at runtime
    DrawDockSpace(ImGui::GetMainViewport(), dockingFlags, (const ImGuiWindowClass*)0);

    ImGui::PushFont(m_defaultFont);
    static int index = 0;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
    if (ImGui::Begin("Editor", (bool*)0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar))
    {
        WindowDimension &dimension = m_platform.m_windowDimension;
        ImVec2 content = ImGui::GetContentRegionAvail();
        ImVec2 windowPos = ImGui::GetWindowPos();

        if (content.x != 0 && content.y != 0)
        {
            if (content.x != dimension.width || content.y != dimension.height)
                m_graphics.UpdateImGUIFrameBuffer(dimension, {(int)content.x, (int)content.y});
            void *framebuffer = (void*)((u64)m_graphics.m_imguiTexture);
            ImGui::Image(framebuffer,
                ImVec2(dimension.width, dimension.height), ImVec2(0, 1), ImVec2(1, 0));
        }

        if (m_selectedObject != nullptr)
        {
            ImGuizmo::SetDrawlist();
            ImGui::GetCurrentWindow();
            ImGuizmo::SetRect(windowPos.x, windowPos.y, content.x, content.y);

            RedFoxMaths::Mat4 cameraProjection = m_editorCamera.m_projection.GetTransposedMatrix();
            RedFoxMaths::Mat4 cameraView = m_editorCamera.GetViewMatrix().GetTransposedMatrix();
            RedFoxMaths::Mat4 transformMat = m_selectedObject->GetWorldMatrix().GetTransposedMatrix();
            RedFoxMaths::Mat4 deltaMat = { };

            bool snap = m_input.LControl;
            // if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
            //     snapValue = 45.0f;
            if (m_input.Q) // TODO: What are the unity or unreal buttons for this
                m_GizmoType = ImGuizmo::OPERATION::SCALE;
            else if (m_input.W)
                m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
            else if (m_input.R)
                m_GizmoType = ImGuizmo::OPERATION::ROTATE;
            float snapValues[3] = { 0.5f, 0.5f, 0.5f };


            ImGuizmo::Manipulate((float*)cameraView.AsPtr(), (float*)cameraProjection.AsPtr(),
                m_GizmoType, m_GizmoMode, (float*)transformMat.AsPtr(), (float*)deltaMat.AsPtr(), snap ? &snapValues[0] : nullptr);


            if (ImGuizmo::IsUsing())
            {
                using namespace RedFoxMaths;
                Float3 translation, rotation, scale;
                Float3 scaleUnit = {1, 1, 1};
                ImGuizmo::DecomposeMatrixToComponents(deltaMat.AsPtr(),
                    (float*)&translation.x, (float*)&rotation.x, (float*)&scale.x);

                m_selectedObject->orientation = 
                    Quaternion::Hamilton(Quaternion::FromEuler(rotation*DEG2RAD),
                    m_selectedObject->orientation);
                if(m_GizmoType != ImGuizmo::OPERATION::ROTATE)
                {
                    m_selectedObject->position += translation;
                    m_selectedObject->scale += scale - scaleUnit;
                }
                if (m_selectedObject->scale.x <= 0) //TODO: Float3 clamp inside RedFoxMaths
                    m_selectedObject->scale.x = 0.1;
                if (m_selectedObject->scale.y <= 0)
                    m_selectedObject->scale.y = 0.1;
                if (m_selectedObject->scale.z <= 0)
                    m_selectedObject->scale.z = 0.1;
            }
        }

        if (ImGui::IsMouseDown(ImGuiMouseButton_Right) && ImGui::IsItemHovered())
        {
            m_input.lockMouse = m_editorCameraEnabled = true;
        }
        else
        {
            m_editorCameraSpeed = { 0.f, 0.f, 0.f };
            m_input.lockMouse = m_editorCameraEnabled = false;
        }
    }
    ImGui::End();
    ImGui::PopStyleVar();

    ImGui::SameLine();
    if (ImGui::Begin("Scene Graph", (bool*)0, ImGuiWindowFlags_NoCollapse))
    {
        ImGuiTreeNodeFlags rootNodeFlags = 
            ImGuiTreeNodeFlags_Framed |
            ImGuiTreeNodeFlags_Leaf | 
            ImGuiTreeNodeFlags_AllowItemOverlap |
            ImGuiTreeNodeFlags_DefaultOpen |
            ImGuiTreeNodeFlags_SpanFullWidth;

        static int scrollStrength = 1;
        static int test = 0;
        static float fps[255];
        static float averageFps;
        if (m_deltaTime)
            fps[test++] = (1.0f / m_deltaTime);
        if (test >= (int)(1 / m_deltaTime))
        {
            test = 0;
            for (int i = 0; i < (int)(1 / m_deltaTime); i += 2)
            {
                averageFps = (fps[i] + averageFps) / 2.0f;
            }
        }
        if (ImGui::TreeNodeEx("_TREENODE", rootNodeFlags, "%s (%.f fps)(%.4f ms)", m_sceneName.data, averageFps, m_deltaTime * 1000))
        {
            static bool scrollButtonHovered = false;
            if (ImGui::BeginPopupContextItem("RenameScenePopup"))
            {
                if (m_input.Enter)
                    ImGui::CloseCurrentPopup();

                ImGui::SameLine();
                ImGui::InputText(" ", (char*)m_sceneName.data, m_sceneName.capacity);
                ImGui::EndPopup();
            }
            
            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered() && !scrollButtonHovered)
            {
                ImGui::OpenPopup("RenameScenePopup");
            }

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
            const int buttonWidth = 50;
            if (buttonWidth < ImGui::GetContentRegionAvail().x)
            {
                char tempString[32] = {};
                snprintf(tempString, 32, "%d", scrollStrength);
                ImGui::SameLine(ImGui::GetContentRegionAvail().x - (f32)buttonWidth / 2);
                if (ImGui::Button(tempString, ImVec2(buttonWidth, 0)))
                {
                    scrollStrength *= 10;
                    if (scrollStrength > 1000)
                        scrollStrength = 1;
                }
            }
            scrollButtonHovered = ImGui::IsItemHovered();
        }
        ImGui::TreePop();

        ImGuiWindowFlags sceneGraphFlags =
            ImGuiWindowFlags_AlwaysHorizontalScrollbar | 
            ImGuiWindowFlags_AlwaysVerticalScrollbar |
            ImGuiWindowFlags_NoMove;

        ImGui::BeginChild("SceneGraphNodes", ImVec2(0, 0), true, sceneGraphFlags);
        if (index < 0)
            index = 0;
        else if (index > (int)m_gameObjectCount - 1)
            index = m_gameObjectCount - 1;

        int maxItems = (int)ImGui::GetMainViewport()->Size.y / 16;
        for (int i = 0; i + index < (int)m_gameObjectCount && i < maxItems; i++)
        {
            if (i == 0 && index > 0 && ImGui::GetScrollY() == 0)
            {
                ImGui::SetScrollY(1);
                index -= scrollStrength;
            }

            float scrollMax = 0;
            if (i == maxItems - 1 && index + i < (int)m_gameObjectCount - 1 &&
                (scrollMax = ImGui::GetScrollMaxY()) == ImGui::GetScrollY() && scrollMax != 0)
            {
                ImGui::SetScrollY(scrollMax - 1);
                index += scrollStrength;
            }

            if (index + i < 0)
                index = i;
            else if (index + i > (int)m_gameObjectCount - 1)
                index = m_gameObjectCount - i - 1;

            if (m_gameObjects[i + index].parent == nullptr)
                DrawSceneNodes(false, &m_gameObjects[i + index]);
        }
        ImGui::EndChild();
    }
    ImGui::End();

    if (ImGui::Begin("Properties", (bool*)0, ImGuiWindowFlags_NoCollapse))
    {
        ImGuiTreeNodeFlags propertiesFlags = 
            ImGuiTreeNodeFlags_DefaultOpen |
            ImGuiTreeNodeFlags_OpenOnArrow | 
            ImGuiTreeNodeFlags_OpenOnDoubleClick;

        if (ImGui::CollapsingHeader("Camera", propertiesFlags))
        {
            ImGuiTableFlags tableFlags =
                ImGuiTableFlags_RowBg |
                ImGuiTableFlags_SizingStretchSame |
                ImGuiTableFlags_Resizable |
                ImGuiTableFlags_BordersOuter;

            if (ImGui::BeginTable("CameraTransformTable", 2, tableFlags))
            {
                ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Position");
                ImGui::TableSetColumnIndex(1);
                ImGui::SetNextItemWidth(-FLT_MIN);
                ImGui::DragFloat3("TransformPosition", &m_editorCamera.position.x, 1.0f, -32767.f, 32767.f);
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Rotation");
                ImGui::TableSetColumnIndex(1);
                ImGui::SetNextItemWidth(-FLT_MIN);

                static RedFoxMaths::Float3 rotation;
                if (ImGui::DragFloat3("TransformRotation", &rotation.x, 1.0f, -360, 360.f))
                {
                    if (rotation.x == 360)
                        rotation.x = -359;
                    if (rotation.x == -360)
                        rotation.x = 359;
                    if (rotation.y == 360)
                        rotation.y = -359;
                    if (rotation.y == -360)
                        rotation.y = 359;
                    if (rotation.z == 360)
                        rotation.z = -359;
                    if (rotation.z == -360)
                        rotation.z = 359;
                    rotation.x *= DEG2RAD;
                    rotation.y *= DEG2RAD;
                    rotation.z *= DEG2RAD;
                    //TODO: still pretty janky
                    m_editorCamera.orientation = RedFoxMaths::Quaternion::SLerp(m_editorCamera.orientation, RedFoxMaths::Quaternion::FromEuler(rotation * 2), 1);
                    m_editorCamera.orientation.Normalize();
                    rotation.x *= RAD2DEG;
                    rotation.y *= RAD2DEG;
                    rotation.z *= RAD2DEG;
                }
                ImGui::EndTable();
            }
        }

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
                    ImGui::DragFloat3("TransformPosition", &m_selectedObject->position.x, 1.f, -32767.f, 32767.f);
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("Rotation");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::SetNextItemWidth(-FLT_MIN);

                    static RedFoxMaths::Float3 rotation;
                    if (ImGui::DragFloat3("TransformRotation", &rotation.x, 1.f, -360.f, 360.f))
                    {
                        if (rotation.x == 360)
                            rotation.x = -359;
                        if (rotation.x == -360)
                            rotation.x = 359;
                        if (rotation.y == 360)
                            rotation.y = -359;
                        if (rotation.y == -360)
                            rotation.y = 359;
                        if (rotation.z == 360)
                            rotation.z = -359;
                        if (rotation.z == -360)
                            rotation.z = 359;
                        rotation.x *= DEG2RAD;
                        rotation.y *= DEG2RAD;
                        rotation.z *= DEG2RAD;
                        m_selectedObject->orientation = RedFoxMaths::Quaternion::SLerp(m_selectedObject->orientation, RedFoxMaths::Quaternion::FromEuler(rotation), 0.5);
                        m_selectedObject->orientation.Normalize();
                        rotation.x *= RAD2DEG;
                        rotation.y *= RAD2DEG;
                        rotation.z *= RAD2DEG;
                    }
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("Scale");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    ImGui::DragFloat3("TransformScale", &m_selectedObject->scale.x, 1.f, -32767.f, 32767.f);
                    ImGui::EndTable();
                }
            }
        }
    }
    ImGui::End();
    ImGui::PopFont();
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}