#include "Engine.hpp"

int main()
{
    RedFoxEngine::Engine engine(1920, 1040);

    while (engine.isRunning())
    {
        engine.ProcessInputs();
        engine.Update();
        engine.Draw();
    }
    return (0);
}
