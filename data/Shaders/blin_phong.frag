#version 450 core

in vec2 TexCoord;

layout (binding=0)            // (from ARB_shading_language_420pack)
uniform sampler2D gPosition;  // texture unit binding 0
layout (binding=1)
uniform sampler2D gNormal;    // texture unit binding 1
layout (binding=2)
uniform sampler2D gAlbedo;    // texture unit binding 2

layout (binding=3)
uniform sampler2D shadowMap;

layout (location=0)
out vec4 o_color;  // output fragment data location 0

struct Material {
    vec3  ambient;
    vec3  diffuse;
    vec3  specular;
    vec3  emissive;
    float shininess;
}; 

struct ShadowParameters
{
    unsigned int depthMapFBO;
    int SHADOW_WIDTH, SHADOW_HEIGHT;
    unsigned int depthMap;
};

struct Light {
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

    float _padding;

    mat4 lightVp;
    ShadowParameters shadowParameters;
    int index;
};

vec3 CalcDirLight  (Light   light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight (Light  light, vec3 normal, vec3 fragPos, vec3 viewDir);

layout(std430, binding = 0) buffer PointLightBlock {
    Light pointLight[];
} u_pointLightBlock;

layout(std430, binding = 1) buffer DirLightBlock {
    Light   dirLight[];
} u_dirLightBlock;

layout(std430, binding = 2) buffer SpotLightBlock {
    Light  spotLight[];
} u_spotLightBlock;

const float shininessFloat = 32;
const float specularFloat = 32;

void main()
{
    vec3 FragPosition = texture(gPosition, TexCoord).xyz;
    vec3 Normal       = texture(gNormal, TexCoord).xyz;

    vec3 result = vec3(0, 0, 0);
    
    for (int i = 0; i < u_dirLightBlock.dirLight.length(); i++)
        result += CalcDirLight(u_dirLightBlock.dirLight[i], Normal, vec3(0, 0, 0));
    
    for (int i = 0; i < u_pointLightBlock.pointLight.length(); i++)
        result += CalcPointLight(u_pointLightBlock.pointLight[i], Normal, FragPosition, vec3(0, 0, 0));
    
    for (int i = 0; i < u_spotLightBlock.spotLight.length(); i++)
        result += CalcSpotLight(u_spotLightBlock.spotLight[i], Normal, FragPosition, vec3(0, 0, 0));
    
    o_color = vec4(result, 1);
}

// Adapted code from a previous project
float ShadowCalculation(Light light)
{
    vec3 FragPosition = texture(gPosition, TexCoord).xyz;
    vec3 Normal       = texture(gNormal, TexCoord).xyz;
    vec4 fragPosLightSpace = light.lightVp * vec4(FragPosition, 1);

    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = -max(0.00025 * (1.0 - dot(Normal, vec3(fragPosLightSpace))), 0.000025);  

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth + bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    return shadow;
}  


vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir)
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

    float shadow = ShadowCalculation(light);
    return (ambient + (1 - shadow) * (diffuse + specular));
}

vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
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

    // float shadow = ShadowCalculation(light, ShadowIndex);
    
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 Color = texture(gAlbedo, TexCoord).rgb;

    vec3 lightDir     = normalize(light.position - fragPos);
    float diff        = max(dot(normal, lightDir), 0.0);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(viewDir, halfwayDir), 0.0), shininessFloat * 4);
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 ; //  / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); // Commented due to an issue that this calculation return nan.
    float theta       = dot(lightDir, normalize(-light.direction)); 
    float epsilon     = light.cutOff - light.outerCutOff;
    float intensity   = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient = light.ambient * Color;
    vec3 diffuse = light.diffuse * diff * Color;
    vec3 specular = light.specular * spec * specularFloat;
    ambient  *= attenuation * intensity;
    diffuse  *= attenuation * intensity;
    specular *= attenuation * intensity;

    float shadow = ShadowCalculation(light);
    return (ambient + (1 - shadow) * (diffuse + specular));
}
