#pragma once
#include "RedfoxMaths.hpp"


namespace RedFoxEngine
{
    struct ShadowParameters
    {
        int index;
        unsigned int depthMapFBO = 0;
        unsigned int SHADOW_WIDTH = 5120, SHADOW_HEIGHT = 5120;
        unsigned int depthMap = 0;
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

        RedFoxMaths::Mat4 VP;
        ShadowParameters shadowParameters;

        void operator=(LightInfo& light);
    };

    class Light
    {
        LightType type;
        RedFoxMaths::Mat4 projection;

        void SetProjection(LightType type);
    public:
        LightInfo lightInfo;

        Light::Light(LightType lightType, int index);
        void operator=(Light& light);
        void SetType(LightType type);
        LightType GetType();
        RedFoxMaths::Mat4 GetProjection();
    };
}