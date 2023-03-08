#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_opengl3.h"
#include "Engine.hpp"

int main()
{
    RedFoxEngine::Engine engine(1920, 1080);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplWin32_Init(engine.m_platform.m_window);
    ImGui_ImplOpenGL3_Init("#version 450");

    while (engine.m_platform.m_running)
    {
        engine.ProcessInputs();
        engine.Update();
        engine.Draw();
    }
    ImGui::DestroyContext();
    return (0);
}
