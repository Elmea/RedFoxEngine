#pragma once
#include "GameObject.hpp"
#include "RedfoxMaths.hpp"


namespace RedFoxEngine
{
    class ShadowParameters
    {
        int index;
        unsigned int depthMap = 0;
        friend class Graphics;
        friend class Light;
        friend struct LightStorage;

    public:
        unsigned int SHADOW_WIDTH = 8192, SHADOW_HEIGHT = 8192;
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
        float cutOff = 1;

        //Calculated from the rotation, any change on this value will be override
        RedFoxMaths::Float3 direction;
        float outerCutOff = 0;
        
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
    private:
        LightType type;
        RedFoxMaths::Mat4 projection;
        void SetProjection(LightType type);
        unsigned int depthMapFBO = 0;
        friend class Graphics;
        friend class Engine;
        friend struct LightStorage;

    public:
        RedFoxMaths::Quaternion rotation;
        LightInfo lightInfo;
        Light(LightType lightType, int index);
        void operator=(Light& light);
        LightType GetType();
        RedFoxMaths::Mat4 GetProjection();
    };
}