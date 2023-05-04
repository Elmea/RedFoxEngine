#pragma once
#include "Camera.hpp"
#include "GameObject.hpp"
#include "GameUI.hpp"
#include "GameUIBehaviour.hpp"

namespace RedFoxEngine
{

struct SceneNode
{
    
};

struct SceneGraph
{
    SceneNode *root;    
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

    SkyDome skyDome;
    Camera m_gameCamera;
    SceneGraph graph;
    int m_width, m_height;
    bool isPaused = true;
    
    RedFoxMaths::Mat4 *m_modelMatrices;
    u64 *m_modelCountIndex;

    GameObject *gameObjects = nullptr;
    GameUI* gameUIs = nullptr;
    GameUIBehaviour* gameUIBehaviours = nullptr;

    Scene(int width, int height):m_gameCamera(projectionType::PERSPECTIVE,
        width / (f32)height){};
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

};

}
