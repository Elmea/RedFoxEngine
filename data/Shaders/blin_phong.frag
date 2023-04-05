#version 450 core

in vec2 TexCoord;

layout (binding=0)            // (from ARB_shading_language_420pack)
uniform sampler2D gPosition;  // texture unit binding 0
layout (binding=1)
uniform sampler2D gNormal;    // texture unit binding 1
layout (binding=2)
uniform sampler2D gAlbedo;    // texture unit binding 2
layout (binding=3)
uniform sampler2D gTangent;

layout (location=0)
out vec4 o_color;  // output fragment data location 0

struct Material {
    vec3  ambient;
    vec3  diffuse;
    vec3  specular;
    vec3  emissive;
    float shininess;
}; 

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    float constant;
    vec3 ambient;
    float linear;
    vec3 diffuse;
    float quadratic;
    vec3 specular;
    float _padding;
};

struct SpotLight {
    vec3 position;
    float cutOff;
    vec3 direction;
    float outerCutOff;
    vec3 ambient;
    float constant;
    vec3 diffuse;
    float linear;
    vec3 specular;
    float quadratic;
};

vec3 CalcDirLight  (DirLight   light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight (SpotLight  light, vec3 normal, vec3 fragPos, vec3 viewDir);

layout(std430, binding = 0) buffer PointLightBlock {
    PointLight pointLight[];
} u_pointLightBlock;

layout(std430, binding = 1) buffer DirLightBlock {
    DirLight   dirLight[];
} u_dirLightBlock;

layout(std430, binding = 2) buffer SpotLightBlock {
    SpotLight  spotLight[];
} u_spotLightBlock;

const float shininessFloat = 32;
const float specularFloat = 32;

void main()
{
    vec3 FragPosition = texture(gPosition, TexCoord).xyz;
    vec3 Normal       = texture(gNormal, TexCoord).xyz;
    vec3 Tangent      = texture(gTangent, TexCoord).xyz;
    vec3 BiTangent    = cross(Normal, Tangent);

    mat3 TBN = transpose(mat3(Tangent, BiTangent, Normal));

    vec3 result = vec3(0, 0, 0);
    for (int i = 0; i < u_dirLightBlock.dirLight.length(); i++)
        result += CalcDirLight(u_dirLightBlock.dirLight[i], Normal, vec3(0, 0, 0));
    for (int i = 0; i < u_pointLightBlock.pointLight.length(); i++)
        result += CalcPointLight(u_pointLightBlock.pointLight[i], Normal, FragPosition, vec3(0, 0, 0));
    for (int i = 0; i < u_spotLightBlock.spotLight.length(); i++)
        result += CalcSpotLight(u_spotLightBlock.spotLight[i], Normal, FragPosition, vec3(0, 0, 0));
    o_color = vec4(result, 1);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(viewDir, halfwayDir), 0.0), shininessFloat * 4);

    vec3 Color = texture(gAlbedo, TexCoord).rgb;

    vec3 ambient = light.ambient * Color;
    vec3 diffuse = light.diffuse * diff * Color;
    vec3 specular = light.specular * spec * specularFloat;
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 Color = vec3(texture(gAlbedo, TexCoord));
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(viewDir, halfwayDir), 0.0), shininessFloat * 4);
    
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    vec3 ambient = light.ambient * Color;
    vec3 diffuse = light.diffuse * diff * Color;
    vec3 specular = light.specular * spec * specularFloat;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 Color = texture(gAlbedo, TexCoord).rgb;

    vec3 lightDir     = normalize(light.position - fragPos);
    float diff        = max(dot(normal, lightDir), 0.0);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(viewDir, halfwayDir), 0.0), shininessFloat * 4);
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    float theta       = dot(lightDir, normalize(-light.direction)); 
    float epsilon     = light.cutOff - light.outerCutOff;
    float intensity   = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient = light.ambient * Color;
    vec3 diffuse = light.diffuse * diff * Color;
    vec3 specular = light.specular * spec * specularFloat;
    ambient  *= attenuation * intensity;
    diffuse  *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}
