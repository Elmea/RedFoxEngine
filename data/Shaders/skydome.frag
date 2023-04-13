#version 450 core

in vec3 pos;
in vec3 sunNorm;

layout (binding = 0) uniform sampler2D topSkyTint;
layout (binding = 1) uniform sampler2D botSkyTint;
layout (binding = 2) uniform sampler2D sun;
layout (binding = 3) uniform sampler2D moon;
layout (binding = 4) uniform sampler2D clouds;

layout (location = 0) uniform float time;
out vec4 color;  // output fragment data location 0

// Noise generation
float Hash(float n)
{
    return fract( (1.0 + sin(n))  *  415.92653);
}

float Noise3d(vec3 x)
{
    float xhash = Hash(round(400 * x.x)  *  37.0);
    float yhash = Hash(round(400 * x.y)  *  57.0);
    float zhash = Hash(round(400 * x.z)  *  67.0);
    return fract(xhash + yhash + zhash);
}

void main()
{
    vec3 posNorm = normalize(pos);
    float dist = dot(sunNorm, posNorm);
    
    vec3 color_top_sun = texture(topSkyTint,  vec2((sunNorm.y + 1.0) / 2.0, max(0.01, -posNorm.y))).rgb;
    vec3 color_bot_sun = texture(botSkyTint,  vec2((sunNorm.y + 1.0) / 2.0, max(0.01, -posNorm.y))).rgb;
    color = vec4(mix(color_bot_sun.xyz, color_top_sun, dist * 0.5 + 0.5), 1);

    // Computing uv for the clouds textures (spherical projection)
    float u = 0.5 + atan(posNorm.z, posNorm.x) / (2 * 3.14159265);
    float v = 1 - (-0.5 + asin(posNorm.y) / 3.14159265);
    vec3 cloudColor = vec3(min(3.0/2.0, 1.0)) * (sunNorm.y > 0 ? 0.95 : 0.95+sunNorm.y * 1.8);
    float transparency = texture(clouds, vec2(u + time, v)).r;
    
    // Sun
    float radius = length(posNorm-sunNorm);
    if(radius < 0.05)
    {
        float time = clamp(sunNorm.y, 0.01f, 1);
        radius = radius / 0.05;
        if (radius < 1.0 - 0.001)
        {
            vec4 sun_color = texture(sun, vec2(radius, time));
            color = vec4(mix(color.xyz, sun_color.rgb, sun_color.a), 1);
        }
    }

    // Moon
    float radius_moon = length(posNorm + sunNorm);
    if(radius_moon < 0.03){
        vec3 n1 = normalize(cross(-sunNorm, vec3(0, 1, 0)));
        vec3 n2 = normalize(cross(-sunNorm, n1));
        float x = dot(posNorm, n1);
        float y = dot(posNorm, n2);
        float scale = 23.57 * 0.5;
        float compensation = 1.4;
        color = vec4(mix(color.xyz, texture(moon, vec2(x, y) * scale * compensation + vec2(0.5)).rgb, clamp(-sunNorm.y * 3, 0, 1)), 1);
    }

    color = vec4(mix(color.xyz, cloudColor, clamp(transparency, 0, 1)),1);
}