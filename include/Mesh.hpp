#pragma once

#include "ResourceManager.hpp"
#include "ObjParser.hpp"

class Mesh : RedFoxEngine::IResource
{
public:
    ObjVertex *vertices;
    u64 vertexCount;
    u32 *indices;
    u64 indexCount;
    
};
