#version 450 core

in vec2 TexCoord;

layout (binding=0)             // (from ARB_shading_language_420pack)
uniform sampler2D gPosition;  // texture unit binding 0
layout (binding=1)
uniform sampler2D gNormal;  // texture unit binding 1
layout (binding=2)
uniform sampler2D gAlbedo;  // texture unit binding 2

layout (location=0)
out vec4 o_color;    // output fragment data location 0

void main()
{
    //TODO: get these values from outside
    vec3 lightPos    = vec3(0, 4, 0);
    vec3 lightColor = vec3(0.7, 0.75, 0.78);
    float ambientStrength = 0.1;
    float specularStrength = 0.5;
    float shininess = 32;

    vec3 FragPosition = texture(gPosition, TexCoord).xyz;
    vec3 Normal       = texture(gNormal, TexCoord).xyz;
    vec3 objectColor = texture(gAlbedo, TexCoord).rgb;
    
    
    vec3 ambient = ambientStrength * lightColor;
     // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    vec3 viewDir = normalize(-FragPosition); // the viewer is always at (0,0,0) in view-space, so viewDir is (0,0,0) - Position => -Position
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor; 
    
    vec3 result = (ambient + diffuse + specular) * objectColor;
    o_color = vec4(result, 1);
}
