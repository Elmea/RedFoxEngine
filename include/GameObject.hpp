#pragma once
#include "ObjParser.hpp"
#include "RedfoxMaths.hpp"
#include "Model.hpp"

namespace RedFoxEngine
{
    struct GameObject
    {
        MyString name;
        GameObject* parent;
        Model *model;
        RedFoxMaths::Float3 position;
        RedFoxMaths::Float3 scale;
        RedFoxMaths::Quaternion orientation;

        RedFoxMaths::Mat4 GetLocalMatrix();
        RedFoxMaths::Mat4 GetWorldMatrix();
        GameObject **GetChildren(GameObject *gameObjects, int modelCount,
            Memory *temp);
        int GetChildrenCount(GameObject *objects, int objectCount);
    };
}
