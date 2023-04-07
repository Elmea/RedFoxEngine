#version 450 core

layout (binding=0)             // (from ARB_shading_language_420pack)
uniform sampler2D diffuseMap;  // texture unit binding 0
layout (binding=1)
uniform sampler2D normalMap;

in FS_IN
{
    vec3 Position;
    vec2 TexCoords;
} fs_in;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec3 gAlbedoSpec;

void main()
{
    gPosition = fs_in.Position;
    gNormal.xyz = texture(normalMap, fs_in.TexCoords).xyz;
    gNormal.w = 1;
    gAlbedoSpec = texture(diffuseMap, fs_in.TexCoords).xyz;
}
