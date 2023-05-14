#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in uint aMaterialID;

layout(std430, binding = 3) buffer MatrixBlock 
{
    readonly mat4 worldMatrix[];
};

struct Material
{
    vec3 diffuse;
    float Shininess;

    float Opaqueness;
    int diffuseMap;
    int normalMap;
    int _padding;
};

layout(std430, binding = 5) buffer Materials
{
    readonly Material material[];
} mat;

layout (location = 0) uniform mat4 vp;

layout (location = 0) out VS_OUT
{
    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;
    flat uint materialID;
    vec3 Color;
} vs_out;

out gl_PerVertex { vec4 gl_Position; };

void main()
{
    vec4 worldPosition = worldMatrix[gl_InstanceID] * vec4(aPos, 1);
    vs_out.Position = worldPosition.xyz;
    vs_out.TexCoords = aTexCoord;
    vs_out.Normal   = normalize((transpose(inverse(worldMatrix[gl_InstanceID])) *
                                 vec4(aNormal, 1)).rgb);
    vs_out.materialID = aMaterialID;
    vs_out.Color = mat.material[gl_InstanceID].diffuse;
    gl_Position = vp * worldPosition;
}
