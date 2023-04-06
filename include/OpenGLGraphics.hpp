/* date = February 25th 2023 4:19 am */
#pragma once

#ifdef _WIN32
#include "Win32Platform.hpp"
#endif

#include <GL/gl.h>
#include "OpenGLFunctions.hpp"
#include "ObjParser.hpp"
#include "imgui.h"
#include "Light.hpp"

namespace RedFoxEngine
{

class Graphics
{
private:
    RedFoxMaths::Mat4 m_viewProjection;
    GLuint m_vshader, m_fshader, m_pipeline;
    GLuint m_gvshader, m_gfshader, m_gpipeline;
    GLuint m_ngvshader, m_ngfshader, m_ngpipeline;
    GLuint m_rboIMGUI;
    GLuint m_quadVAO;
    GLuint m_gBuffer;
    GLuint m_booleanBuffer;
    GLuint m_imguiFramebuffer;
    u32    m_spotLightCount;
    GLuint m_spotLightBuffer;
    u32    m_dirLightCount;
    GLuint m_dirLightBuffer;
    u32    m_pointLightCount;
    GLuint m_pointLightBuffer;
    GLuint m_gPosition, m_gNormal, m_gAlbedoSpec, m_gTangent;
public:
    Model *m_models = nullptr;
    u32    m_modelCount;
    GLuint m_imguiTexture;
    void DrawGBuffer(GameObject *objects, int gameObjectCount, Memory *temp);
    void DrawQuad(WindowDimension dimension);
    void DrawModel(Model *model);
    // void DrawModelInstances(Model *model, int instanceCount);
    void DrawModelInstances(Model *model, RedFoxMaths::Mat4 *modelMatrices,
        int instanceCount);
    void InitQuad();
    void InitModel(Model *model);
    void InitNormalMappedModel(Model *model, Memory *temp);
    void InitLights();
    void BindLights();
    DirLight *GetDirLightBuffer(int *lightCount);
    void SetPointLightBuffer(PointLight* pointLight, int lightCount);
    void ReleaseDirLightBuffer();
    PointLight *GetPointLightBuffer(int *lightCount);
    void ReleasePointLightBuffer();
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
