#ifndef PARSER_H
#define PARSER_H

#include <direct.h>
#include <immintrin.h>
#include <stdio.h>

#include "MyMemory.hpp"
#include "engine_math.hpp"
#include "meow_hash_x64_aesni.h"

typedef struct ObjImageData
{
    u8 *data;
    u64 hash64;
    u64 imageSize;
    int width;
    int height;
    int nrChannels;
    MyString path;
} ObjImageData;

typedef struct ObjTexture
{
    u32 index0;
} ObjTexture;

typedef struct ObjMaterial
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 emissive;
    f32 Shininess;
    f32 Opaqueness;

    u8 hasTexture : 1;
    u8 hasNormal : 1;
    u8 hasSpecular : 1;

    ObjTexture diffuseMap;
    ObjTexture normalMap;
    MyString name;
} ObjMaterial;

typedef struct ObjImages
{
    ObjImageData *data;
    u64 count;
    HANDLE thread;
} ObjImages;

typedef struct ObjMaterials
{
    ObjMaterial *material;
    u64 count;
} ObjMaterials;

typedef struct ObjVertex
{
    vec3 position;
    vec3 normal;
    vec2 textureUV;
} ObjVertex;

typedef struct ObjVertexIndex
{
    u32 positionIndex;
    u32 normalIndex;
    u32 textureIndex;
} ObjVertexIndx;

typedef struct ObjMesh
{
    u32 indexCount;
    u32 indexStart;
    u32 materialIndex;
} ObjMesh;

typedef struct ObjParsingObject
{
    vec3 *position;
    s64 PosCount;
    vec3 *normal;
    s64 NormalCount;
    vec2 *textureUV;
    s64 TextureUVCount;
    ObjVertexIndex *vertexIndices;
} ObjParsingObject;

typedef struct ObjModel
{
    ObjVertex *vertices;
    u64 vertexCount;
    u32 *indices;
    u64 indexCount;

    size_t meshCount;
    ObjMesh *meshes;

    ObjMaterials materials;
    ObjImages images;
    Memory meshMem;
    Memory imageMem;
    Memory vertexMem;
    Memory indexMem;
} ObjModel;

int ParseModel(ObjModel *result, const char *path);
// void FatalError(const char* message, const char *file, int line);
ObjModel CreateCube();
void DeInitObj(ObjModel *obj);
void DeInitGraphicsObj(ObjModel *obj);

#endif // PARSER_H