#include "Engine.hpp"

using namespace RedFoxEngine;

void Engine::DrawSceneNodes(int* id, bool is_child, GameObject* gameObj)
{
    int childrenCount = 0;
    GameObject** children = gameObj->GetChildren(m_gameObjects, m_gameObjectCount, &m_tempAllocator, &childrenCount);

    ImGuiTreeNodeFlags flags;
    if (is_child)
    {
        flags = ImGuiTreeNodeFlags_Bullet;
    }
    else
    {
        if (childrenCount == 0)
            flags = ImGuiTreeNodeFlags_Leaf;
        else
            flags = ImGuiTreeNodeFlags_OpenOnArrow;
    }
    if (gameObj == m_selectedObject)
        flags |= ImGuiTreeNodeFlags_Selected;

    flags |= ImGuiTreeNodeFlags_SpanFullWidth |
        ImGuiTreeNodeFlags_OpenOnDoubleClick;

    bool nodeOpen = ImGui::TreeNodeEx(gameObj->name, flags, gameObj->name);
    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
    {
        m_selectedObject = gameObj;
    }

    if (nodeOpen)
    {
        if (ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("_TREENODE", gameObj, sizeof(GameObject));
            ImGui::Text("This is a drag and drop source");
            ImGui::EndDragDropSource();
        }
        

        if (children != nullptr)
        {
            for (int i = 0; children[i] != nullptr; i++)
            {
                *id += i + 1;
                DrawSceneNodes(id, true, children[i]);
            }
        }

        ImGui::TreePop();
    }
}

void Engine::DrawIMGUI()
{
    ImGui_ImplWin32_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    ImGuiDockNodeFlags dockingFlags = ImGuiDockNodeFlags_PassthruCentralNode |
                                      ImGuiDockNodeFlags_NoWindowMenuButton |
                                      ImGuiDockNodeFlags_NoCloseButton |
                                      ImGuiDockNodeFlags_NoDockingInCentralNode;

    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), dockingFlags);

    if (ImGui::Begin("Scene Graph"))
    {
        ImGuiTreeNodeFlags rootNodeFlags = ImGuiTreeNodeFlags_Framed |
            ImGuiTreeNodeFlags_Leaf |
            ImGuiTreeNodeFlags_DefaultOpen |
            ImGuiTreeNodeFlags_SpanFullWidth;

        if (ImGui::TreeNodeEx("_TREENODE", rootNodeFlags, "Sample Scene"))
        {
            ImGui::Unindent();
            for (int i = 0; i < m_gameObjectCount; i++)
                if (m_gameObjects[i].parent == nullptr)
                    DrawSceneNodes(&i, false, &m_gameObjects[i]);
            ImGui::TreePop();
        }

        ImGui::End();
    }

    if (ImGui::Begin("Properties"))
    {
        if (m_selectedObject != nullptr)
        {
            if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow))
            {
                //TODO(a.perche) : Drag speed according to user param.
                //TODO(a.perche) : Fix position and rotation drag overriding pointers value.
                ImGuiTableFlags tableFlags = ImGuiTableFlags_RowBg |
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

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (m_ImGuiIO->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}