#pragma once

#include "ObjParser.hpp"
#include "RedfoxMaths.hpp"

namespace RedFoxEngine
{
    struct Mesh
    {
        u32 indexCount;
        u32 indexStart;
        u32 materialIndex;
    };
    struct Meshes
    {
        Mesh *data;
        u32 count;
    };
    struct Model
    {
        // u32 vao;
        u32 vertexOffset;
        u32 indexOffset;
        u32 indexCount;
        Meshes mesh;
        ObjModel obj;
        u64 hash;
    };
}