#pragma once

#include "ObjParser.hpp"
#include "RedfoxMaths.hpp"

namespace RedFoxEngine
{
    struct Model
    {
        u32 vao;
        ObjModel obj;
        u64 hash;
    };
}