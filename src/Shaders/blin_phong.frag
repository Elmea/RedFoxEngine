#version 450 core

in vec2 TexCoord;

layout (binding=0)            // (from ARB_shading_language_420pack)
uniform sampler2D gPosition;  // texture unit binding 0
layout (binding=1)
uniform sampler2D gNormal;    // texture unit binding 1
layout (binding=2)
uniform sampler2D gAlbedo;    // texture unit binding 2

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
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

struct Light
{
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

Material material;
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

layout(std430, binding = 0) buffer LightBlock {
    //TODO: split the lights in different types.    
    Light light[];
} u_lightBlock;

void main()
{
    material.shininess = 32;

    vec3 FragPosition = texture(gPosition, TexCoord).xyz;
    vec3 Normal       = texture(gNormal, TexCoord).xyz;

    vec3 result = vec3(0, 0, 0);
    for (int i = 0; i < u_lightBlock.light.length(); i++)
    {
        vec3 lightColor = u_lightBlock.light[i].diffuse;
        PointLight pl;
        pl.position = u_lightBlock.light[i].position;
        pl.constant  = u_lightBlock.light[i].constant;
        pl.linear    = u_lightBlock.light[i].linear;
        pl.quadratic = u_lightBlock.light[i].quadratic;
        pl.ambient = u_lightBlock.light[i].ambient;
        pl.diffuse = u_lightBlock.light[i].diffuse;
        pl.specular = u_lightBlock.light[i].specular;
        result   += CalcPointLight(pl, Normal, FragPosition, vec3(0, 0, 0));
    }
    o_color = vec4(result, 1);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(viewDir, halfwayDir), 0.0), material.shininess * 4);

    vec3 Color = texture(gAlbedo, TexCoord).rgb;

    vec3 ambient = light.ambient * Color;
    vec3 diffuse = light.diffuse * diff * Color;
    vec3 specular = light.specular * spec * material.specular;
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{

    vec3 Color = vec3(texture(gAlbedo, TexCoord));
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(viewDir, halfwayDir), 0.0), material.shininess * 4);
    
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    vec3 ambient = light.ambient * Color;
    vec3 diffuse = light.diffuse * diff * Color;
    vec3 specular = light.specular * spec * material.specular;
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
    float spec = pow(max(dot(viewDir, halfwayDir), 0.0), material.shininess * 4);
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    float theta       = dot(lightDir, normalize(-light.direction)); 
    float epsilon     = light.cutOff - light.outerCutOff;
    float intensity   = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient = light.ambient * Color;
    vec3 diffuse = light.diffuse * diff * Color;
    vec3 specular = light.specular * spec * material.specular;
    ambient  *= attenuation * intensity;
    diffuse  *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}
