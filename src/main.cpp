#include "imgui.h"
#include "Engine.hpp"

int main()
{
    RedFoxEngine::Engine engine(1920, 1080);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    
    ImGui::StyleColorsDark();
    int width, height;
    unsigned char* pixels = NULL;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    GLuint imGuiTexture = 0;
    engine.m_graphics.InitTexture(pixels, height, width, imGuiTexture);

    io.Fonts->SetTexID((void *)&imGuiTexture);

    while (engine.m_platform.m_running)
    {
        engine.ProcessInputs();
        io.DeltaTime = engine.m_deltaTime;
        io.DisplaySize.x = engine.m_platform.windowDimension.width;
        io.DisplaySize.y = engine.m_platform.windowDimension.height;        
        io.AddMousePosEvent(engine.GetInputs().mouseXPosition, engine.GetInputs().mouseYPosition);
        ImGui::NewFrame();
        ImGui::Text("Hello, world!");
        engine.Update();
        engine.Draw();
        ImGui::EndFrame();
    }
    ImGui::DestroyContext();
    return (0);
}
