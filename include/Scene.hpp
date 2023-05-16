#pragma once
#include "Camera.hpp"
#include "GameObject.hpp"
#include "GameUI.hpp"
#include "GameBehaviour.hpp"

namespace RedFoxEngine
{

enum SceneNodeType
{
    SN_NONE,
    SN_GameObject,
    SN_Camera,
    SN_Light,
  // SN_GameUI,
};

struct SceneNode
{
    SceneNode *previous;
    SceneNode *next;
    SceneNodeType type;
    void          *data;
};

struct TimeManager
{
    f64 current;
    f64 delta;
};

struct SkyDome
{
    RedFoxMaths::Float3 sunPosition;
    u32 topTint, botTint, sun, moon, clouds;
    RedFoxMaths::Mat4 model;
};

class Scene
{
public:
    MyString m_name;
    u32 gameObjectCount = 0;
    u32 gameUICount = 0;
    u32 gameUIBehaviourCount = 0;
    u32 gameObjectBehaviourCount = 0;

    SkyDome skyDome;
    Camera m_gameCamera;
    SceneNode *first;
    SceneNode *last;
    int sceneNodeCount;
    int m_width, m_height;
    bool isPaused = true;
    
    RedFoxMaths::Mat4 *m_modelMatrices;
    u64 *m_modelCountIndex;

    GameObject *gameObjects = nullptr;
    GameUI* gameUIs = nullptr;
    Behaviour* gameUIBehaviours = nullptr;
    Behaviour* gameObjectBehaviours = nullptr;

    Scene(int width, int height):m_gameCamera(projectionType::PERSPECTIVE,
        width / (f32)height){}

    void InitScene(Memory *persistent)
    {
        first = (SceneNode *)MyMalloc(persistent, sizeof(SceneNode) * 100000);
        sceneNodeCount = 0;
        *first = {};
        first->type = SN_NONE;
        first->next = &first[sceneNodeCount + 1];        
        last = first->next;
        *last = {};
        last->previous = first;
        sceneNodeCount++;
    }
    
    RedFoxMaths::Mat4 GetWorldMatrix(int gameObjectindex)
    {
        GameObject *current = &gameObjects[gameObjectindex];
        if (current->parent)
        {
            return current->GetLocalMatrix() * GetWorldMatrix(current->parent);
        }
        return gameObjects[gameObjectindex].GetLocalMatrix();
    };

    int *GetChildren(int gameObjectIndex, Memory *temp)
    {
        int *result = (int *)MyMalloc(temp, sizeof(int));
        int count = 0;
        for (int i = 0; i < (int)gameObjectCount; i++)
        {
            if (gameObjects[i].parent == gameObjectIndex)
                result[count++] = i;
        }
        result[count] = 0;
        return(result);
    }
    
    int GetChildrenCount(int gameObjectIndex)
    {
        int count = 0;
        for (int i = 0; i < (int)gameObjectCount; i++)
        {
            if (gameObjects[i].parent == gameObjectIndex)
                count++;
        }
        return (count);
    }

    int* GetChildrenUI(int gameUIIndex, Memory* temp)
    {
        int* result = (int*)MyMalloc(temp, sizeof(int));
        int count = 0;
        for (int i = 0; i < (int)gameObjectCount; i++)
        {
            if (gameObjects[i].parent == gameUIIndex)
                result[count++] = i;
        }
        result[count] = 0;
        return(result);
    }

    int GetChildrenCountUI(int gameUIIndex)
    {
        int count = 0;
        for (int i = 0; i < (int)gameObjectCount; i++)
        {
            if (gameObjects[i].parent == gameUIIndex)
                count++;
        }
        return (count);
    }

    int AddUIBehavior(const char* name, functionBehaviour func)
    {
        this->gameUIBehaviours[this->gameUIBehaviourCount].name = assignString(this->gameUIBehaviours[this->gameUIBehaviourCount].name, name);
        this->gameUIBehaviours[this->gameUIBehaviourCount].function = func;
        this->gameUIBehaviourCount++;
        return this->gameUIBehaviourCount - 1;
    }

    int AddGameObjectBehavior(const char* name, functionBehaviour func)
    {
        this->gameObjectBehaviours[this->gameObjectBehaviourCount].name = assignString(this->gameObjectBehaviours[this->gameObjectBehaviourCount].name, name);
        this->gameObjectBehaviours[this->gameObjectBehaviourCount].function = func;
        this->gameObjectBehaviourCount++;
        return this->gameObjectBehaviourCount - 1;
    }
};

}
