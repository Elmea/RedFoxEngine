/* date = February 25th 2023 4:19 am */
#pragma once

#ifdef _WIN32
#include "Win32Platform.hpp"
#endif

#include <GL/gl.h>
#include "OpenGLFunctions.hpp"
#include "ObjParser.hpp"
#include "imgui.h"

namespace RedFoxEngine
{

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

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
};

class Graphics
{
private:
    RedFoxMaths::Mat4 m_viewProjection;
    GLuint m_vshader, m_fshader;
    GLuint m_gvshader, m_gfshader;
    GLuint m_rboIMGUI;
    GLuint m_quadVAO;
    GLuint m_gpipeline;
    GLuint m_pipeline;
    GLuint m_gBuffer;
    GLuint m_imguiFramebuffer;
    // GLuint m_lightBuffer;
    u32    m_lightCount;
public:
    GLuint m_lightBuffer;
    GLuint m_imguiTexture;
    GLuint m_gPosition, m_gNormal, m_gAlbedoSpec;
    // void DrawGBuffer(GameObject *objects, int gameObjectCount, Memory *temp);
    void DrawGBuffer(GameObject *objects, int gameObjectCount,
         Memory *temp, Model *models, int modelCount);
    void DrawQuad(WindowDimension dimension);
    void DrawModel(Model *model);
    void DrawModelInstances(Model *model, int instanceCount);
    void InitQuad();
    void InitModel(Model *model);
    void InitLights();
    void InitTexture(ObjModel *model);
    u32 InitTexture(void *data,int height, int width);
    void InitFramebuffer();
    void InitShaders(Memory *tempArena);
    void InitGraphics(Memory *tempArena, WindowDimension dimension);
    void InitGeometryFramebuffer(WindowDimension dimension);
    void InitImGUIFramebuffer(WindowDimension dimension);
    void UpdateImGUIFrameBuffer(WindowDimension &dimension, WindowDimension content);
    void SetViewProjectionMatrix(RedFoxMaths::Mat4 vp);
};
} // namespace RedFoxEngine

extern PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
extern PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
extern PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;

#define X(type, name) extern type name;
GL_FUNCTIONS(X)
#undef X
