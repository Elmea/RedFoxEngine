#include "Engine.hpp"

#define MEMORY_IMPLEMENTATION
#include "MyMemory.hpp"
/*
    Scene file reference (in progress)

    struct GameObject
    {
        u32 nameOfGameObjectSize;
        u32 nameOfGameObjectCapacity;
        char nameOfGameObject[nameOfGameObjectSize];
        int parent; - -l if no parent, >= 0 if parent exists
        u64 modelHash; hash of the filename string of the model
        float position[3];
        float scale[3];
        float orientation[4];
    }

    - Start Of File
    struct
    {
        s32 gameObjectCount;
        GameObject arrayOfGameObjects[gameObjectCount];
    }
*/

void RedFoxEngine::Engine::LoadScene(const char *fileName)
{
    
    HANDLE file = CreateFile(fileName, GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,nullptr, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL, nullptr);
    
    scene.m_name = initStringChar(fileName, 255, &m_memoryManager.m_memory.arena);

    ReadFile(file, &scene.gameObjectCount, sizeof(u32), nullptr, nullptr);
    for(int i = 0; i < (int)scene.gameObjectCount; i++)
    {
        GameObject *current = &scene.gameObjects[i];
        ReadFile(file, &current->name, sizeof(MyString), nullptr, nullptr);
        current->name.data = (char *)m_memoryManager.PersistentAllocation(255);
        ReadFile(file, (void*)current->name.data, current->name.size, nullptr, nullptr);
        int parent;
        ReadFile(file, &parent, sizeof(int), nullptr, nullptr);
        u64 hash = 0;
        ReadFile(file, &hash, sizeof(u64), nullptr, nullptr);
        current->modelIndex = -1;
        if (hash != 0)
        {
            for (int modelIndex = 0;
            modelIndex < (int)m_modelCount;
            modelIndex++)
            {
                if (m_models[modelIndex].hash == hash)
                {
                    current->modelIndex = modelIndex;
                    break;
                }
            }
        }
        ReadFile(file, &current->position,
            sizeof(current->position), nullptr, nullptr);
        ReadFile(file, &current->scale,
            sizeof(current->scale), nullptr, nullptr);
        // current->scale.y = current->scale.z = 1;
        ReadFile(file, &current->orientation,
            sizeof(current->orientation), nullptr, nullptr);
        // current->orientation.a = 1;
        // current->orientation.b = current->orientation.c = current->orientation.d = 0;
    }
    CloseHandle(file);

    
}

void RedFoxEngine::Engine::SaveScene(const char *fileName)
{
    HANDLE file = CreateFile(fileName, GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, nullptr);

    WriteFile(file, &scene.gameObjectCount, sizeof(u32), nullptr, nullptr);
    for(int i = 0; i < (int)scene.gameObjectCount; i++)
    {
        GameObject *current = &scene.gameObjects[i];

        WriteFile(file, &current->name, sizeof(MyString), nullptr, nullptr);
        WriteFile(file, current->name.data, current->name.size, nullptr, nullptr);
        WriteFile(file, &current->parent, sizeof(int), nullptr, nullptr);
        WriteFile(file, &m_models[current->modelIndex].hash, sizeof(u64), nullptr, nullptr);

        WriteFile(file, &current->position,
            sizeof(current->position), nullptr, nullptr);
        WriteFile(file, &current->scale,
            sizeof(current->scale), nullptr, nullptr);
        WriteFile(file, &current->orientation,
            sizeof(current->orientation), nullptr, nullptr);
    }
    CloseHandle(file);
}

