#pragma once

#include "ObjParser.hpp"
#include "RedfoxMaths.hpp"

namespace RedFoxEngine
{
    struct Model
    {
        u32      textureOffset;
        u32      materialOffset;
        u32      vertexOffset;
        u32      indexOffset;
        u32      indexCount;
        u32      meshCount;
        u64      hash;
    };
}