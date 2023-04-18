#version 450 core

in vec3 pos;
in vec3 sunPosNorm;

layout (binding = 0) uniform sampler2D topSkyTint;
layout (binding = 1) uniform sampler2D botSkyTint;
layout (binding = 2) uniform sampler2D sun;
layout (binding = 3) uniform sampler2D moon;
layout (binding = 4) uniform sampler2D clouds;

layout (location = 0) uniform float time;
out vec4 color;

void main()
{
    // Atmosphere
    vec3 posNorm = normalize(pos);
    float dist = dot(sunPosNorm, posNorm);
    vec2 uv = vec2((clamp(sunPosNorm.y, -0.99, 0.99) + 1.0) / 2.0);
    vec3 colorTopSun = texture(topSkyTint, uv).rgb;
    vec3 colorBotSun = texture(botSkyTint, uv).rgb;
    color = vec4(mix(colorBotSun.xyz, colorTopSun.xyz, dist * 0.5 + 0.5), 1);

    // Clouds
    float u = 0.5 + atan(posNorm.z, posNorm.x) / (2 * 3.14159265);
    float v = 1 - (-0.5 + asin(posNorm.y) / 3.14159265);
    vec3 cloudColor = vec3(min(3.0 /2.0, 1.0)) * (sunPosNorm.y > 0 ? 0.95 : 0.95 + sunPosNorm.y * 1.8);
    float transparency = texture(clouds, vec2(u + time, v)).r;
    
    // Sun
    float radius = length(posNorm - sunPosNorm);
    if(radius < 0.05)
    {
        float time = clamp(sunPosNorm.y, 0.05, 0.99);
        radius = radius / 0.1;
        if (radius < 0.999)
        {
            vec4 sun_color = texture(sun, vec2(radius, time));
            color = vec4(mix(color.xyz, sun_color.rgb, sun_color.a), 1);
        }
    }

    // Moon
    float radius_moon = length(posNorm + sunPosNorm);
    if(radius_moon < 0.03)
    {
        vec3 n1 = normalize(cross(-sunPosNorm, vec3(0, 1, 0)));
        vec3 n2 = normalize(cross(-sunPosNorm, n1));
        float x = dot(posNorm, n1);
        float y = dot(posNorm, n2);
        float scale = 11.785;
        float compensation = 1.4;
        color = vec4(mix(color.xyz, texture(moon, vec2(x, y) * scale * compensation + vec2(0.5)).rgb, clamp(-sunPosNorm.y * 3, 0, 1)), 1);
    }

    color = vec4(mix(color.xyz, cloudColor, clamp(transparency, 0, 1)), 1);
}