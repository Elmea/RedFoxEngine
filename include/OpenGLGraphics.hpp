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
    GLuint m_vshader, m_fshader;
    GLuint m_gvshader, m_gfshader;
    GLuint m_rboIMGUI;
    GLuint m_quadVAO;
    GLuint m_gpipeline;
    GLuint m_pipeline;
    GLuint m_gBuffer;
    GLuint m_gPosition, m_gNormal, m_gAlbedoSpec;
    GLuint m_imguiFramebuffer;

    u32    m_spotLightCount;
    GLuint m_spotLightBuffer;
    u32    m_dirLightCount;
    GLuint m_dirLightBuffer;
    u32    m_pointLightCount;
    GLuint m_pointLightBuffer;

    void SetDirLightBuffer(Light* pointLight, int lightCount);
    void SetPointLightBuffer(Light* pointLight, int lightCount);
    void SetSpotLightBuffer(Light* pointLight, int lightCount);

public:
    Model *m_models = nullptr;
    u32    m_modelCount;
    GLuint m_imguiTexture;
    void DrawGBuffer(GameObject *objects, int gameObjectCount, Memory *temp);
    void DrawQuad(WindowDimension dimension);
    void DrawModel(Model *model);
    void DrawModelInstances(Model *model, int instanceCount);
    void InitQuad();
    void InitModel(Model *model);
    void InitLights();
    void BindLights();
    Light*GetDirLightBuffer(int *lightCount);
    void ReleaseDirLightBuffer();
    Light*GetPointLightBuffer(int *lightCount);
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
