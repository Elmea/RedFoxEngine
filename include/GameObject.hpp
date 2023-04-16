#pragma once
#include "ObjParser.hpp"
#include "RedfoxMaths.hpp"
#include "Model.hpp"

namespace RedFoxEngine
{
    struct GameObject
    {
        MyString name;
        int parent;
        Model *model;

        RedFoxMaths::Float3 position;
        RedFoxMaths::Float3 scale;
        RedFoxMaths::Quaternion orientation;
        
        float radius;
        RedFoxMaths::Float3 boxExtents;

        RedFoxMaths::Mat4 GetLocalMatrix();
        // int *GetChildren(GameObject *objects, int objectCount, Memory *temp);
        // GameObject **GetChildren(GameObject *gameObjects, int modelCount,
            // Memory *temp);
        

        // int GetChildrenCount(GameObject *objects, int objectCount);
    };
}
