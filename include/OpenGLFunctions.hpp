#pragma once
#include "glcorearb.h" // download from https://www.khronos.org/registry/OpenGL/api/GL/glcorearb.h
#include "wglext.h"    // download from https://www.khronos.org/registry/OpenGL/api/GL/wglext.h

#define GL_FUNCTIONS(X)                                                       \
    X(PFNGLCREATEBUFFERSPROC, glCreateBuffers)                                \
    X(PFNGLNAMEDFRAMEBUFFERTEXTUREPROC, glNamedFramebufferTexture)            \
    X(PFNGLCREATEFRAMEBUFFERSPROC, glCreateFramebuffers)                      \
    X(PFNGLDELETEFRAMEBUFFERSPROC, glDeleteFramebuffers)                      \
    X(PFNGLBLITFRAMEBUFFERPROC, glBlitFramebuffer)                            \
    X(PFNGLBINDFRAMEBUFFERPROC, glBindFramebuffer)                            \
    X(PFNGLFRAMEBUFFERTEXTURE2DPROC, glFramebufferTexture2D)                  \
    X(PFNGLCREATERENDERBUFFERSPROC, glCreateRenderbuffers)                    \
    X(PFNGLBINDRENDERBUFFERPROC, glBindRenderbuffer)                          \
    X(PFNGLNAMEDRENDERBUFFERSTORAGEPROC, glNamedRenderbufferStorage)          \
    X(PFNGLNAMEDFRAMEBUFFERRENDERBUFFERPROC, glNamedFramebufferRenderbuffer)  \
    X(PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC, glNamedFramebufferDrawBuffers)    \
    X(PFNGLCHECKNAMEDFRAMEBUFFERSTATUSPROC, glCheckNamedFramebufferStatus)    \
    X(PFNGLNAMEDBUFFERSTORAGEPROC, glNamedBufferStorage)                      \
    X(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray)                            \
    X(PFNGLCREATEVERTEXARRAYSPROC, glCreateVertexArrays)                      \
    X(PFNGLVERTEXARRAYATTRIBBINDINGPROC, glVertexArrayAttribBinding)          \
    X(PFNGLVERTEXARRAYVERTEXBUFFERPROC, glVertexArrayVertexBuffer)            \
    X(PFNGLVERTEXARRAYATTRIBFORMATPROC, glVertexArrayAttribFormat)            \
    X(PFNGLVERTEXARRAYELEMENTBUFFERPROC, glVertexArrayElementBuffer)          \
    X(PFNGLENABLEVERTEXARRAYATTRIBPROC, glEnableVertexArrayAttrib)            \
    X(PFNGLCREATESHADERPROGRAMVPROC, glCreateShaderProgramv)                  \
    X(PFNGLGETPROGRAMIVPROC, glGetProgramiv)                                  \
    X(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog)                        \
    X(PFNGLGENPROGRAMPIPELINESPROC, glGenProgramPipelines)                    \
    X(PFNGLUSEPROGRAMSTAGESPROC, glUseProgramStages)                          \
    X(PFNGLBINDPROGRAMPIPELINEPROC, glBindProgramPipeline)                    \
    X(PFNGLPROGRAMUNIFORMMATRIX2FVPROC, glProgramUniformMatrix2fv)            \
    X(PFNGLPROGRAMUNIFORMMATRIX3FVPROC, glProgramUniformMatrix3fv)            \
    X(PFNGLPROGRAMUNIFORMMATRIX3FVPROC, glProgramUniformMatrix4fv)            \
    X(PFNGLPROGRAMUNIFORM1IPROC, glProgramUniform1i)                          \
    X(PFNGLBINDTEXTUREUNITPROC, glBindTextureUnit)                            \
    X(PFNGLCREATETEXTURESPROC, glCreateTextures)                              \
    X(PFNGLTEXTUREPARAMETERIPROC, glTextureParameteri)                        \
    X(PFNGLTEXTURESTORAGE2DPROC, glTextureStorage2D)                          \
    X(PFNGLTEXTURESUBIMAGE2DPROC, glTextureSubImage2D)                        \
    X(PFNGLDEBUGMESSAGECALLBACKPROC, glDebugMessageCallback)                  \
    X(PFNGLBLENDEQUATIONPROC, glBlendEquation)                                \
    X(PFNGLBLENDFUNCSEPARATEPROC, glBlendFuncSeparate)                        \
    X(PFNGLDRAWELEMENTSINSTANCEDPROC, glDrawElementsInstanced)                \
    X(PFNGLVERTEXARRAYBINDINGDIVISORPROC, glVertexArrayBindingDivisor)        \
    X(PFNGLBLITNAMEDFRAMEBUFFERPROC, glBlitNamedFramebuffer)                  \
    X(PFNGLNAMEDBUFFERSUBDATAPROC, glNamedBufferSubData)