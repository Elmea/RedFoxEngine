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

static void ReadStringFromFile(HANDLE file, MyString *string, RedFoxEngine::ResourcesManager m)
{
    ReadFile(file, string, sizeof(MyString), nullptr, nullptr);
    string->data = (char *)m.PersistentAllocation(string->capacity);
    ReadFile(file, (void *)string->data, string->size, nullptr, nullptr);
}

static void ReadGameObjectFromFile(HANDLE file, RedFoxEngine::GameObject *current, RedFoxEngine::Model *m_models, int m_modelCount, RedFoxEngine::ResourcesManager m)
{
        ReadFile(file, &current->name, sizeof(MyString), nullptr, nullptr);
        current->name.data = (char *)m.PersistentAllocation(255);
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
        ReadFile(file, &current->orientation,
            sizeof(current->orientation), nullptr, nullptr);
}

void RedFoxEngine::Engine::LoadScene(const char *fileName)
{
    HANDLE file = CreateFile(fileName, GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,nullptr, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL, nullptr);

    printf("%lld\n", sizeof(MyString) + sizeof(SkyDome) + sizeof(Camera) + sizeof(int) + sizeof(u32) + sizeof(int) + sizeof(bool)); 
    ReadFile(file, &m_scene, 309, nullptr, nullptr);
    ReadStringFromFile(file, &m_scene.m_name, m_memoryManager);
    ReadFile(file, &m_scene.gameObjectCount, sizeof(u32), nullptr, nullptr);
    for(int i = 0; i < (int)m_scene.gameObjectCount; i++)
    {
        ReadGameObjectFromFile(file, &m_scene.gameObjects[i], m_models, m_modelCount, m_memoryManager);
    }
    CloseHandle(file);
}

static void WriteStringToFile(HANDLE file, MyString string)
{
    WriteFile(file, &string, sizeof(MyString), nullptr, nullptr);
    WriteFile(file, string.data, string.size, nullptr, nullptr);
}

static void WriteGameObjectToFile(HANDLE file, RedFoxEngine::GameObject *current, RedFoxEngine::Model *m_models)
{
    WriteStringToFile(file, current->name);
    WriteFile(file, &current->parent, sizeof(int), nullptr, nullptr);
    WriteFile(file, &m_models[current->modelIndex].hash, sizeof(u64), nullptr, nullptr);

    WriteFile(file, &current->position,
        sizeof(current->position), nullptr, nullptr);
    WriteFile(file, &current->scale,
        sizeof(current->scale), nullptr, nullptr);
    WriteFile(file, &current->orientation,
        sizeof(current->orientation), nullptr, nullptr);
}

void RedFoxEngine::Engine::SaveScene(const char *fileName, Scene scene)
{
    HANDLE file = CreateFile(fileName, GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, nullptr);

    WriteFile(file, &scene, 309, nullptr, nullptr);
    WriteStringToFile(file, scene.m_name);    
    WriteFile(file, &m_scene.gameObjectCount, sizeof(u32), nullptr, nullptr);
    for(int i = 0; i < (int)m_scene.gameObjectCount; i++)
    {
        GameObject *current = &m_scene.gameObjects[i];
        WriteGameObjectToFile(file, current, m_models);
    }
    CloseHandle(file);
}

