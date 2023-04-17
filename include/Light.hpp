#pragma once
#include "RedfoxMaths.hpp"


namespace RedFoxEngine
{
    struct ShadowParameters
    {
        unsigned int depthMapFBO = 0;
        static const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
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
        int index;
        int padding0;
        int padding1;
        int padding2;

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