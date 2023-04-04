#version 450 core

layout (binding=0)             // (from ARB_shading_language_420pack)
uniform sampler2D diffuseMap;  // texture unit binding 0
layout (binding=1)
uniform sampler2D normalMap;

in FS_IN
{
    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;
    // mat3 TBN; 
} fs_in;

layout (std140, binding = 1) uniform normalMapped
{
    int isNormalMapped;
} is;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedoSpec;

void main()
{
    gPosition = fs_in.Position;
    if (is.isNormalMapped == 0)
        gNormal = normalize(fs_in.Normal);
    // else
    //    gNormal = vec3(0, 1, 0);
    gAlbedoSpec = texture(diffuseMap, fs_in.TexCoords).xyz;
    if (gAlbedoSpec == vec3(0, 0, 0))
        gAlbedoSpec = vec3(1, 1, 1);
}
