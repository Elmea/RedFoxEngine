#pragma once
#include "ObjParser.hpp"
#include "RedfoxMaths.hpp"
#include "Model.hpp"

namespace RedFoxEngine
{
    struct GameObject
    {
        char* name;
        GameObject* parent;
        Model *model;
        RedFoxMaths::Float3 position;
        f32 scale;
        RedFoxMaths::Quaternion orientation;

        RedFoxMaths::Mat4 GetWorldMatrix();
        GameObject **GetChildren(GameObject *gameObjects, int modelCount,
            Memory *temp);
        int GetChildrenCount(GameObject *objects, int objectCount);
    };
}
