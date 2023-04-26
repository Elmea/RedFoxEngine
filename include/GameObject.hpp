#pragma once
#include "ObjParser.hpp"
#include "RedfoxMaths.hpp"
#include "Model.hpp"

namespace RedFoxEngine
{
    struct GameObject
    {
        MyString name;
  
        RedFoxMaths::Float3 position;
        int parent;

        RedFoxMaths::Float3 scale;
        int modelIndex;

        RedFoxMaths::Quaternion orientation;
        float radius;
        RedFoxMaths::Float3 boxExtents;
        RedFoxMaths::Float3 Color;

        RedFoxMaths::Mat4 GetLocalMatrix();
    };
}
