#version 450 core

in VS_IN
{
    vec3 FragPosition;
    vec3 Normal;
    vec2 TexCoord;
} vs_in;

in vec3 lightPos;

layout (binding=0)             // (from ARB_shading_language_420pack)
uniform sampler2D diffuseMap;  // texture unit binding 0

layout (location=0)
out vec4 o_color;              // output fragment data location 0

void main()
{
    //TODO modulable. Right now it's hard coded for prototype
    // Goal : defered shading
    vec3 lightColor = vec3(0.7, 0.75, 0.78);
    vec3 objectColor = texture(diffuseMap, vs_in.TexCoord).xyz;
    
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    
     // diffuse 
    vec3 norm = normalize(vs_in.Normal);
    vec3 lightDir = normalize(lightPos - vs_in.FragPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(-vs_in.FragPosition); // the viewer is always at (0,0,0) in view-space, so viewDir is (0,0,0) - Position => -Position
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor; 
    
    vec3 result = (ambient + diffuse + specular) * objectColor;
//    o_color = texture(diffuseMap, vs_in.TexCoord);
    o_color = vec4(result, 1);
}
