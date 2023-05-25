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

static void ReadStringFromFile(HANDLE file, MyString *string, RedFoxEngine::ResourcesManager *m)
{
    string->data = (char *)m->PersistentAllocation(string->capacity);
    ReadFile(file, (void *)string->data, string->size, nullptr, nullptr);
    ((char *)string->data)[string->size] = 0;
}

static void ReadGameObjectFromFile(HANDLE file, RedFoxEngine::GameObject *current, RedFoxEngine::Model *m_models, int m_modelCount, RedFoxEngine::ResourcesManager *m)
{
    
    int size = sizeof(RedFoxEngine::GameObject);
    ReadFile(file, &current->transform, size, nullptr, nullptr);
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
    ReadStringFromFile(file, &current->name, m);
}

static BEHAVIOUR(DefaultBehaviour) { }
static UIBEHAVIOUR(DefaultUIBehaviour) { }

void RedFoxEngine::Engine::LoadScene(const char *fileName)
{
    m_scene.isInit = false;
    m_scene.gameObjectCount = 0;
    m_scene.gameUICount = 0;
    m_scene.gameUIBehaviourCount = 0;
    m_scene.gameObjectBehaviourCount = 0;
    m_memoryManager.m_memory.arena.usedSize = m_memoryManager.m_sceneUsedMemory;

    //Init GameUI
    m_scene.gameUIs[0] = {};
    m_scene.gameUIs[0].name = initStringChar("Root", 255, &m_memoryManager.m_memory.arena);
    m_scene.gameUIs[0].name.capacity = 255;
    m_scene.gameUIs[0].screenPosition = { 0, 0 };
    m_scene.gameUICount = 1;
    for (int i = 1; i < 100; i++)
    {
        m_scene.gameUIs[i].parent = 0;
        m_scene.gameUIs[i].behaviourIndex = 0;
        m_scene.gameUIs[i].isPressed = false;
        m_scene.gameUIs[i].isHovered = false;
    }

    //Init GameObject
    m_scene.gameObjects[0] = {};
    m_scene.gameObjects[0].name = initStringChar("Root", 255, &m_memoryManager.m_memory.arena);
    m_scene.gameObjects[0].name.capacity = 255;
    m_scene.gameObjects[0].position = { 0, 0, 0 };
    m_scene.gameObjects[0].orientation = { 1, 0, 0, 0 };
    m_scene.gameObjects[0].scale = { 1, 1, 1 };
    m_scene.gameObjects->modelIndex = -1;
    m_scene.gameObjectCount = 1;

    SkyDome tmp = m_scene.skyDome;

    HANDLE file = CreateFile(fileName, GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,nullptr, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL, nullptr);

    ReadFile(file, &m_scene, sizeof(Scene) - (sizeof(void *) * 7), nullptr, nullptr);
    ReadStringFromFile(file, &m_scene.m_name, &m_memoryManager);
    ReadFile(file, &m_scene.gameObjectCount, sizeof(u32), nullptr, nullptr);
    for(int i = 0; i < (int)m_scene.gameObjectCount; i++)
    {
        ReadGameObjectFromFile(file, &m_scene.gameObjects[i], m_models, m_modelCount, &m_memoryManager);
    }
    CloseHandle(file);

    //Init behaviours buffer
    for (int i = 0; i < 100; i++)
    {
        m_scene.gameObjectBehaviours[i].name = initStringChar("None", 64, &m_memoryManager.m_memory.arena);
        m_scene.gameObjectBehaviours[i].function = DefaultBehaviour;
        m_scene.gameUIBehaviours[i].name = initStringChar("None", 64, &m_memoryManager.m_memory.arena);
        m_scene.gameUIBehaviours[i].function = DefaultUIBehaviour;
    }
    m_scene.gameObjectBehaviourCount = 1;
    m_scene.gameUIBehaviourCount = 1;
    m_scene.skyDome = tmp;
    m_physx.InitScene(&m_scene, 1, 0);
}

static void WriteStringToFile(HANDLE file, MyString string)
{
    WriteFile(file, string.data, string.size, nullptr, nullptr);
}

static void WriteGameObjectToFile(HANDLE file, RedFoxEngine::GameObject *current, RedFoxEngine::Model *m_models)
{
    int size = sizeof(RedFoxEngine::GameObject);
    WriteFile(file, &current->transform, size, nullptr, nullptr);
    WriteFile(file, &m_models[current->modelIndex].hash, sizeof(u64), nullptr, nullptr);
    WriteStringToFile(file, current->name);
}

static void WriteGameUIToFile(HANDLE file, RedFoxEngine::GameUI* current, RedFoxEngine::Model* m_models)
{
    int size = sizeof(RedFoxEngine::GameObject);
    WriteFile(file, &current->screenPosition, size, nullptr, nullptr);
    WriteStringToFile(file, current->text);
    WriteStringToFile(file, current->name);
}

void RedFoxEngine::Engine::SaveScene(const char *fileName, Scene scene)
{
    HANDLE file = CreateFile(fileName, GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, nullptr);

    WriteFile(file, &m_scene, sizeof(Scene) - (sizeof(void *) * 7), nullptr, nullptr);
    WriteStringToFile(file, scene.m_name);    
    WriteFile(file, &m_scene.gameObjectCount, sizeof(u32), nullptr, nullptr);
    for(int i = 0; i < (int)m_scene.gameObjectCount; i++)
    {
        GameObject *current = &m_scene.gameObjects[i];
        WriteGameObjectToFile(file, current, m_models);
    }

    WriteFile(file, &m_scene.gameUICount, sizeof(u32), nullptr, nullptr);
    for (int i = 0; i < (int)m_scene.gameUICount; i++)
    {
        GameUI *current = &m_scene.gameUIs[i];
        WriteGameUIToFile(file, current, m_models);
    }
    CloseHandle(file);
}