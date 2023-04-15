#pragma once
#include "RedfoxMaths.hpp"


namespace RedFoxEngine
{
    struct ShadowParameters
    {
        int index;
        static const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
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
        unsigned int depthMapFBO = 0;
        LightInfo lightInfo;

        Light::Light(LightType lightType, int index);
        void operator=(Light& light);
        void SetType(LightType type);
        LightType GetType();
        RedFoxMaths::Mat4 GetProjection();
    };
}