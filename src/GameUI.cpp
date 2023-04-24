#include "GameUI.hpp"

#define MEMORY_IMPLEMENTATION
#include "MyMemory.hpp"

namespace RedFoxEngine
{



    GameUI** GameUI::GetChildren(GameUI* objects, int objectCount, Memory* temp)
    {
        GameUI** result = (GameUI**)MyMalloc(temp, sizeof(GameUI**));
        int count = 0;
        for (int i = 0; i < objectCount; i++)
        {
            if (objects[i].parent == this)
                result[count++] = &objects[i];
        }
        result[count] = nullptr;
        return(result);
    }

    int GameUI::GetChildrenCount(GameUI* objects, int objectCount)
    {
        int count = 0;
        for (int i = 0; i < objectCount; i++)
        {
            if (objects[i].parent == this)
                count++;
        }
        return (count);
    }
}
