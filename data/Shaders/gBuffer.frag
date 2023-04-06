#version 450 core

layout (binding=0)             // (from ARB_shading_language_420pack)
uniform sampler2D diffuseMap;  // texture unit binding 0

in FS_IN
{
    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedoSpec;

void main()
{
    gPosition = fs_in.Position;
    gNormal = normalize(fs_in.Normal);
    gAlbedoSpec = texture(diffuseMap, fs_in.TexCoords).xyz;
    if (gAlbedoSpec == vec3(0, 0, 0)) //TODO handle materials
        gAlbedoSpec = vec3(1, 1, 1);
}
