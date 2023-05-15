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
    X(PFNGLVERTEXARRAYATTRIBIFORMATPROC, glVertexArrayAttribIFormat)          \
    X(PFNGLVERTEXARRAYELEMENTBUFFERPROC, glVertexArrayElementBuffer)          \
    X(PFNGLENABLEVERTEXARRAYATTRIBPROC, glEnableVertexArrayAttrib)            \
    X(PFNGLCREATESHADERPROGRAMVPROC, glCreateShaderProgramv)                  \
    X(PFNGLGETPROGRAMIVPROC, glGetProgramiv)                                  \
    X(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog)                        \
    X(PFNGLGENPROGRAMPIPELINESPROC, glGenProgramPipelines)                    \
    X(PFNGLUSEPROGRAMSTAGESPROC, glUseProgramStages)                          \
    X(PFNGLBINDPROGRAMPIPELINEPROC, glBindProgramPipeline)                    \
    X(PFNGLPROGRAMUNIFORM3FPROC, glProgramUniform3f)                          \
    X(PFNGLPROGRAMUNIFORM3FVPROC, glProgramUniform3fv)                        \
    X(PFNGLPROGRAMUNIFORM4FVPROC, glProgramUniform4fv)                        \
    X(PFNGLPROGRAMUNIFORMMATRIX2FVPROC, glProgramUniformMatrix2fv)            \
    X(PFNGLPROGRAMUNIFORMMATRIX3FVPROC, glProgramUniformMatrix3fv)            \
    X(PFNGLPROGRAMUNIFORMMATRIX3FVPROC, glProgramUniformMatrix4fv)            \
    X(PFNGLPROGRAMUNIFORM1FPROC, glProgramUniform1f)                          \
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
    X(PFNGLDRAWELEMENTSBASEVERTEXPROC, glDrawElementsBaseVertex)              \
    X(PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC, glDrawElementsInstancedBaseVertexBaseInstance) \
    X(PFNGLVERTEXARRAYBINDINGDIVISORPROC, glVertexArrayBindingDivisor)        \
    X(PFNGLBLITNAMEDFRAMEBUFFERPROC, glBlitNamedFramebuffer)                  \
    X(PFNGLNAMEDBUFFERDATAPROC, glNamedBufferData)                            \
    X(PFNGLMAPNAMEDBUFFERRANGEPROC, glMapNamedBufferRange)                    \
    X(PFNGLUNMAPNAMEDBUFFERPROC, glUnmapNamedBuffer)                          \
    X(PFNGLGETPROGRAMRESOURCEINDEXPROC, glGetProgramResourceIndex)            \
    X(PFNGLSHADERSTORAGEBLOCKBINDINGPROC, glShaderStorageBlockBinding)        \
    X(PFNGLGETPROGRAMINTERFACEIVPROC, glGetProgramInterfaceiv)                \
    X(PFNGLBINDBUFFERBASEPROC, glBindBufferBase)                              \
    X(PFNGLBINDBUFFERRANGEPROC, glBindBufferRange)                            \
    X(PFNGLUNIFORMBLOCKBINDINGPROC, glUniformBlockBinding)                    \
    X(PFNGLCREATESAMPLERSPROC, glCreateSamplers)                              \
    X(PFNGLSAMPLERPARAMETERIPROC, glSamplerParameteri)                        \
    X(PFNGLGETTEXTUREHANDLEARBPROC, glGetTextureHandleARB)                    \
    X(PFNGLGETTEXTURESAMPLERHANDLEARBPROC, glGetTextureSamplerHandleARB)      \
    X(PFNGLMAKETEXTUREHANDLERESIDENTARBPROC, glMakeTextureHandleResidentARB)  \
    X(PFNGLBINDTEXTURESPROC, glBindTextures)                    \
    X(PFNGLNAMEDBUFFERSUBDATAPROC, glNamedBufferSubData)

// X(PFNGLPROC, gl)                  \ 