#pragma once
#include "engine_math.hpp"


namespace RedFoxEngine
{
    struct ShadowParameters
    {
        unsigned int depthMapFBO = 0;
        const unsigned int SHADOW_WIDTH = 5120, SHADOW_HEIGHT = 5120;
        unsigned int depthMap = 0;
        // RedFoxMaths::Mat4 projection;
    };

    enum LightType
    {
        NONE,
        DIRECTIONAL,
        POINT,
        SPOT
    };

    /*
    struct DirLight {
        vec3 direction;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;

        ShadowParameters shadowParameters;
    };

    struct PointLight {
        vec3 position;
        float constant;
        vec3 ambient;
        float linear;
        vec3 diffuse;
        float quadratic;
        vec3 specular;
        float __padding;

        ShadowParameters shadowParameters;
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

        ShadowParameters shadowParameters;
    };
    */

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

        float __padding;

        ShadowParameters shadowParameters;

        LightType type;

        Light::Light(LightType lightType);
        void operator=(Light& light);
    };
}