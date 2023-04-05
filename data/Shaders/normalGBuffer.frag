#version 450 core

layout (binding=0)             // (from ARB_shading_language_420pack)
uniform sampler2D diffuseMap;  // texture unit binding 0
layout (binding=1)
uniform sampler2D normalMap;

in FS_IN
{
    vec3 Position;
    vec3 Normal;
    vec3 Tangent;
    vec2 TexCoords;
} fs_in;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedoSpec;
layout (location = 3) out vec3 gTangent;

void main()
{
    gPosition = fs_in.Position;
    gNormal = normalize(fs_in.Normal);
    gTangent = normalize(fs_in.Tangent);
    gAlbedoSpec = texture(diffuseMap, fs_in.TexCoords).xyz;
    if (gAlbedoSpec == vec3(0, 0, 0))
        gAlbedoSpec = vec3(1, 1, 1);
}
