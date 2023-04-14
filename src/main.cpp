#include "Engine.hpp"

int main()
{
    RedFoxEngine::Engine engine(1920, 1080);

    while (engine.isRunning())
    {
        engine.ProcessInputs();
        engine.Update();
        engine.Draw();
    }
    
    return (0);
}