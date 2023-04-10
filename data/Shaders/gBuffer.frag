#version 450 core
#extension GL_ARB_bindless_texture : enable

layout(std430, binding=1) buffer Texture
{
    readonly uvec2 data[];
} sb_Texture;

struct Material
{
    vec3 ambient;
    float Opaqueness;
        
    vec3 diffuse;
    float Shininess;
        
    vec3 specular;
    int diffuseMap;

    vec3 emissive;
    int normalMap;
};

layout(std430, binding=3) buffer Materials
{
    readonly Material material[];
} mat;

in FS_IN
{
    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;
    flat unsigned int materialID;
} fs_in;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec3 gAlbedoSpec;

void main()
{
    gPosition = fs_in.Position;

    if (mat.material[fs_in.materialID].diffuseMap == -1)
        gAlbedoSpec = mat.material[fs_in.materialID].diffuse;
    else
    {
        sampler2D diffuse = sampler2D(sb_Texture.data[mat.material[fs_in.materialID].diffuseMap]);
        gAlbedoSpec = texture(diffuse, fs_in.TexCoords).xyz;
    }
    if (mat.material[fs_in.materialID].normalMap == -1)
    {
        gNormal.xyz = normalize(fs_in.Normal);
        gNormal.w = 0;
    }
    else
    {
        sampler2D normal = sampler2D(sb_Texture.data[mat.material[fs_in.materialID].normalMap]);
        gNormal.xyz = texture(normal, fs_in.TexCoords).xyz * fs_in.Normal;
        gNormal.w = 1;
    }
}
