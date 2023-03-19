#pragma once

#include "ObjParser.hpp"
#include "RedfoxMaths.hpp"

namespace RedFoxEngine
{
    struct Model
    {
        u32 vao;
        u32 vbm;
        ObjModel obj;
    };
}