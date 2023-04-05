#pragma once
#include "RedfoxMaths.hpp"


namespace RedFoxEngine
{
    struct ShadowParameters
    {
        unsigned int depthMapFBO = 0;
        const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
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
        RedFoxMaths::Float3 position;
        float cutOff;

        RedFoxMaths::Float3 direction;
        float outerCutOff;
        
        RedFoxMaths::Float3 ambient;
        float constant;
        
        RedFoxMaths::Float3 diffuse;
        float linear;
        
        RedFoxMaths::Float3 specular;
        float quadratic;

        float __padding;

        RedFoxMaths::Mat4 VP;
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