#pragma once
#include "engine_math.hpp"


namespace RedFoxEngine
{
    struct ShadowParameters
    {
        unsigned int depthMapFBO = 0;
        const unsigned int SHADOW_WIDTH = 5120, SHADOW_HEIGHT = 5120;
        unsigned int depthMap = 0;
        RedFoxMaths::Mat4 projection;
    };

    enum LightType
    {
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

        DirLight()
        {
            shadowParameters.projection = RedFoxMaths::Mat4::GetOrthographicMatrix(-50, 50, -50, 50, 0.1, 100);

            if (shadowParameters.NextFreeTextureSlot == 0)
                shadowParameters.NextFreeTextureSlot = GL_TEXTURE15;

            shadowParameters.textureSlot = shadowParameters.NextFreeTextureSlot;
            shadowParameters.NextFreeTextureSlot--;

            glCreateFramebuffers(1, &shadowParameters.depthMapFBO);

            glGenTextures(1, &shadowParameters.depthMap);
            glBindTexture(GL_TEXTURE_2D, shadowParameters.depthMap);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                shadowParameters.SHADOW_WIDTH, shadowParameters.SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glBindFramebuffer(GL_FRAMEBUFFER, shadowParameters.depthMapFBO);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowParameters.depthMap, 0);
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

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

        SpotLight()
        {
             shadowParameters.projection = RedFoxMaths::Mat4::GetPerspectiveMatrix(80, widht / height, 0.1, 25);

            if (shadowParameters.NextFreeTextureSlot == 0)
                shadowParameters.NextFreeTextureSlot = GL_TEXTURE15;

            shadowParameters.textureSlot = shadowParameters.NextFreeTextureSlot;
            shadowParameters.NextFreeTextureSlot--;

            glCreateFramebuffers(1, &shadowParameters.depthMapFBO);

            glGenTextures(1, &shadowParameters.depthMap);
            glBindTexture(GL_TEXTURE_2D, shadowParameters.depthMap);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                shadowParameters.SHADOW_WIDTH, shadowParameters.SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glBindFramebuffer(GL_FRAMEBUFFER, shadowParameters.depthMapFBO);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowParameters.depthMap, 0);
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
    };
    */

    struct Light
    {
        vec3 position;
        vec3 direction;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;

        float cutOff;
        float outerCutOff;
        float constant;
        float linear;
        float quadratic;
        float __padding;

        ShadowParameters shadowParameters;

        Light::Light(LightType lightType);
    };
}