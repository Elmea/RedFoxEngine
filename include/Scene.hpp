#pragma once
#include "Camera.hpp"
#include "GameObject.hpp"
#include "GameUI.hpp"
#include "GameBehaviour.hpp"
#include "SoundManager.hpp"
#include "Transform.hpp"

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
    int sceneNodeCount;
    int m_width, m_height;
    bool isPaused = true;
    bool isInit = false;

    u64 *m_modelCountIndex;

    GameObject *gameObjects = nullptr;
    GameUI* gameUIs = nullptr;
    UIBehaviour* gameUIBehaviours = nullptr;
    Behaviour* gameObjectBehaviours = nullptr;
    SoundManager* soundManager;

    Scene(int width, int height):m_gameCamera(projectionType::PERSPECTIVE,
        width / (f32)height){}

    RedFoxMaths::Mat4 GetWorldMatrix(int gameObjectindex)
    {
        GameObject *current = &gameObjects[gameObjectindex];
        if (current->parent)
        {
            return current->GetLocalMatrix() * GetWorldMatrix(current->parent);
        }
        return gameObjects[gameObjectindex].GetLocalMatrix();
    };

    RedFoxEngine::Transform GetWorldTransformFromLocal(Transform transform, int gameObjectIndex)
    {
        GameObject* current = &gameObjects[gameObjectIndex];
        if (current->parent)
        {
            return current->transform + GetWorldTransform(current->parent).Inverse();
        }
        return gameObjects[gameObjectIndex].transform;
    }

    RedFoxEngine::Transform GetWorldTransform(int gameObjectindex)
    {
        GameObject* current = &gameObjects[gameObjectindex];
        if (current->parent)
        {
            return current->transform * GetWorldTransform(current->parent);
        }
        return gameObjects[gameObjectindex].transform;
    };

    int *GetChildren(int gameObjectIndex, Memory *temp)
    {
        int *result = (int *)MyMalloc(temp, sizeof(int) * gameObjectCount);
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

    int AddUIBehaviour(const char* name, uiBehaviour func)
    {
        this->gameUIBehaviours[this->gameUIBehaviourCount].name = assignString(this->gameUIBehaviours[this->gameUIBehaviourCount].name, name);
        this->gameUIBehaviours[this->gameUIBehaviourCount].function = func;
        this->gameUIBehaviourCount++;
        return this->gameUIBehaviourCount - 1;
    }

    int AddGameObjectBehaviour(const char* name, functionBehaviour func)
    {
        this->gameObjectBehaviours[this->gameObjectBehaviourCount].name = assignString(this->gameObjectBehaviours[this->gameObjectBehaviourCount].name, name);
        this->gameObjectBehaviours[this->gameObjectBehaviourCount].function = func;
        this->gameObjectBehaviourCount++;
        return this->gameObjectBehaviourCount - 1;
    }
};

}
