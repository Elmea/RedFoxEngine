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

    // Information for the GPU
    struct LightInfo
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

        void operator=(LightInfo& light);
    };

    struct Light
    {
        LightType type;

        LightInfo lightInfo;

        Light::Light(LightType lightType);
        void operator=(Light& light);
    };
}