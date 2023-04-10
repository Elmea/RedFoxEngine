#version 450 core

in vec2 TexCoord;

layout (binding=0)            // (from ARB_shading_language_420pack)
uniform sampler2D gPosition;  // texture unit binding 0
layout (binding=1)
uniform sampler2D gNormal;
layout (binding=2)
uniform sampler2D gAlbedo;

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
    readonly PointLight pointLight[];
} u_pointLightBlock;

layout(std430, binding = 1) buffer DirLightBlock {
    readonly DirLight   dirLight[];
} u_dirLightBlock;

layout(std430, binding = 2) buffer SpotLightBlock {
    readonly SpotLight  spotLight[];
} u_spotLightBlock;

const float shininessFloat = 32;
const float specularFloat = 32;

void main()
{
    vec3 FragPosition = texture(gPosition, TexCoord).xyz;
    vec3 Normal       = texture(gNormal, TexCoord).xyz;
    float isNormalMapped = texture(gNormal, TexCoord).w;
    
    mat3 TBN = mat3(1.0f);
    if (isNormalMapped != 0)
    {
        Normal = normalize(Normal * 2.0 - 1.0);
        vec3 q1 = dFdx(vec3(gl_FragCoord.xy, 0.0));
        vec3 q2 = dFdy(vec3(gl_FragCoord.xy, 0.0));
        vec3 T = normalize(q1 * q2.y - q2 * q1.y);
        vec3 B = normalize(cross(Normal, T));
        TBN = transpose(mat3(T, B, Normal));
        FragPosition = TBN * FragPosition;
    }
    vec3 result = vec3(0, 0, 0);
    for (int i = 0; i < u_dirLightBlock.dirLight.length(); i++)
        result += CalcDirLight(u_dirLightBlock.dirLight[i], Normal, vec3(0, 0, 0));
    for (int i = 0; i < u_pointLightBlock.pointLight.length(); i++)
    {
        PointLight light = u_pointLightBlock.pointLight[i];
        light.position = TBN * light.position;
        result += CalcPointLight(light, Normal, FragPosition, vec3(0, 0, 0));
    }
    for (int i = 0; i < u_spotLightBlock.spotLight.length(); i++)
    {
        SpotLight light = u_spotLightBlock.spotLight[i];
        light.position = TBN * light.position;
        result += CalcSpotLight(light, Normal, FragPosition, vec3(0, 0, 0));
    }
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
