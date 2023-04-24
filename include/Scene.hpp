#pragma once
#include "Camera.hpp"
#include "OpenGLGraphics.hpp"
#include "GameObject.hpp"

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


class Scene
{
public:
    MyString m_name;
    u32 gameObjectCount = 0;
    SkyDome skyDome;
    Camera m_gameCamera;
    SceneGraph graph;
    int m_width, m_height;
    bool isPaused = true;
    RedFoxMaths::Mat4 *m_modelMatrices;
    GameObject *gameObjects = nullptr;
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
};

}
