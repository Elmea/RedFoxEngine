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
        int modelIndex;
        int materialIndex;
  
        RedFoxMaths::Float3 position;
        RedFoxMaths::Float3 scale;
        RedFoxMaths::Quaternion orientation;
        
        float radius;
        RedFoxMaths::Float3 boxExtents;

        RedFoxMaths::Mat4 GetLocalMatrix();
    };
}
