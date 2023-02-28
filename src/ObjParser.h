#ifndef PARSER_H
#define PARSER_H

#include <immintrin.h>
#include <direct.h>

#include "engine_math.h"
#include "MyMemory.h"
#include <stdio.h>

typedef struct ObjImageData
{
    unsigned char *data;
	size_t         hash64;
    int width, height, nrChannels;
    size_t imageSize;
    MyString       path;
} ImageData;

typedef struct ObjTexture
{
    u32       index0;
} Texture;

typedef struct ObjMaterial
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 emissive;
	f32  Shininess;
	f32  Opaqueness;

	u8 hasTexture  : 1;
	u8 hasNormal   : 1;
	u8 hasSpecular : 1;

	Texture diffuseMap;
	Texture normalMap;
	MyString name;
}            Material;

typedef struct ObjImages
{
	ImageData *data;
	u64 	   count;
    HANDLE     thread;
}            ObjImages;

typedef struct ObjMaterials
{
    Material *material;
    u64      count;
}            ObjMaterials;

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

typedef struct ObjMesh{
    u32          indexCount;
    u32          indexStart;
    u32          materialIndex;
} ObjMesh;

typedef struct ObjParsingObject {
	vec3 *position;
	s64  PosCount;
	vec3 *normal;
	s64  NormalCount;
	vec2 *textureUV;
	s64  TextureUVCount;
	ObjVertexIndex *verticesIndices;
} ObjParsingObject;

typedef struct ObjModel {
	ObjVertex *vertices;
	u64    vertexCount;
	u32    *indices;
	u64    IndexCount;

	size_t meshCount;
	ObjMesh   *meshes;

	ObjMaterials materials;
	ObjImages    images;
    Memory meshMem;
    Memory imageMem;
    Memory vertexMem;
    Memory indexMem;
} ObjModel;

int ParseModel(ObjModel *result, char *path);
static void FatalError(const char* message, char *file, int line);
ObjModel CreateCube();
void DeInitObj(ObjModel *obj);
void DeInitGraphicsObj(ObjModel *obj);

#endif //PARSER_H