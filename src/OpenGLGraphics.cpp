#include "OpenGLGraphics.hpp"
#include "glcorearb.h"
#include "imgui_impl_opengl3.h"

#define MEMORY_IMPLEMENTATION
#include "MyMemory.hpp"

#define STB_TRUETYPE_IMPLEMENTATION
#include <ResourceManager.hpp>

#include "imstb_truetype.h"

extern "C" 
{
  __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}

extern "C"
{
  __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

namespace RedFoxEngine
{
    void Graphics::InitGraphics(ResourcesManager *resources, WindowDimension p_dimension)
    {
        m = resources;
        dimension = p_dimension;

        m_font.fragmentPath        = initStringChar("Shaders/font.frag", 17, &m->m_memory.arena);
        m_font.vertexPath          = initStringChar("Shaders/font.vert", 17, &m->m_memory.arena);
        m_blinnPhong.fragmentPath  = initStringChar("Shaders/blinn_phong.frag.glsl", 29, &m->m_memory.arena);
        m_blinnPhong.vertexPath    = initStringChar("Shaders/blinn_phong.vert.glsl", 29, &m->m_memory.arena);
        m_shadow.fragmentPath      = initStringChar("Shaders/ShadowShader.frag", 25, &m->m_memory.arena);
        m_shadow.vertexPath        = initStringChar("Shaders/ShadowShader.vert", 25, &m->m_memory.arena);
        m_sky.fragmentPath         = initStringChar("Shaders/skydome.frag", 20, &m->m_memory.arena);
        m_sky.vertexPath           = initStringChar("Shaders/skydome.vert", 20, &m->m_memory.arena);
        m_postProcess.fragmentPath = initStringChar("Shaders/PostProcess.frag", 24, &m->m_memory.arena);
        m_postProcess.vertexPath   = initStringChar("Shaders/PostProcess.vert", 24, &m->m_memory.arena);
        m_materials = (Material*)m->PersistentAllocation(sizeof(Material) * 10000);
        InitShaders();
        // setup global GL state
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
        }

        InitImGUIFramebuffer(dimension);
        {
            glCreateBuffers(1, &m_matrixSSBO);
            glNamedBufferStorage(m_matrixSSBO,
                100000 * sizeof(RedFoxMaths::Mat4),
                nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);

            glCreateBuffers(1, &m_materialSSBO);
            glNamedBufferStorage(m_materialSSBO,
                100100 * sizeof(Material),
                nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT | GL_MAP_READ_BIT);

            glCreateBuffers(1, &m_textureSSBO);
            glNamedBufferStorage(m_textureSSBO, 10000 * sizeof(u64), nullptr, GL_DYNAMIC_STORAGE_BIT);

            glCreateBuffers(1, &m_shadowMapsSSBO);
            glNamedBufferStorage(m_shadowMapsSSBO, 1000 * sizeof(u64), nullptr, GL_DYNAMIC_STORAGE_BIT);
        }
        glCreateSamplers(1, &m_textureSampler);
        glSamplerParameteri(m_textureSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glSamplerParameteri(m_textureSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri(m_textureSampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glSamplerParameteri(m_textureSampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
        {
            glCreateBuffers(1, &m_vertexBufferObject);
            glNamedBufferStorage(m_vertexBufferObject, 1000000 * (sizeof(ObjVertex)), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
        }
        {
            glCreateBuffers(1, &m_ebo);
            glNamedBufferStorage(m_ebo, 2000000 * sizeof(u32), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
        }
        // vertex input
        {
            glCreateVertexArrays(1, &m_vertexArrayObject);

            int vbuf_index = 0;
            glVertexArrayVertexBuffer(m_vertexArrayObject, vbuf_index, m_vertexBufferObject, 0,
                sizeof(ObjVertex));

            glVertexArrayElementBuffer(m_vertexArrayObject, m_ebo);

            int a_pos = 0;
            glEnableVertexArrayAttrib(m_vertexArrayObject, a_pos);
            glVertexArrayAttribFormat(m_vertexArrayObject, a_pos, 3, GL_FLOAT, GL_FALSE,
                offsetof(struct ObjVertex, position));
            glVertexArrayAttribBinding(m_vertexArrayObject, a_pos, vbuf_index);

            int a_normal = 1;
            glEnableVertexArrayAttrib(m_vertexArrayObject, a_normal);
            glVertexArrayAttribFormat(m_vertexArrayObject, a_normal, 3, GL_FLOAT, GL_FALSE, offsetof(struct ObjVertex, normal));
            glVertexArrayAttribBinding(m_vertexArrayObject, a_normal, vbuf_index);

            int a_uv = 2;
            glEnableVertexArrayAttrib(m_vertexArrayObject, a_uv);
            glVertexArrayAttribFormat(m_vertexArrayObject, a_uv, 2, GL_FLOAT, GL_FALSE, offsetof(struct ObjVertex, textureUV));
            glVertexArrayAttribBinding(m_vertexArrayObject, a_uv, vbuf_index);
            int a_materialID = 3;
            glEnableVertexArrayAttrib(m_vertexArrayObject, a_materialID);
            glVertexArrayAttribIFormat(m_vertexArrayObject, a_materialID, 1, GL_UNSIGNED_INT, offsetof(struct ObjVertex, materialID));
            glVertexArrayAttribBinding(m_vertexArrayObject, a_materialID, vbuf_index);
            // glVertexArrayBindingDivisor(model->vao, a_materialID, 1);
        }

        //V-SYNC
        wglSwapIntervalEXT(1);

        WindowDimension sceneDimension;
        sceneDimension.height = 2160;
        sceneDimension.width = 3840;

        InitSceneFramebuffer(sceneDimension);
    }

    void Graphics::InitQuad()
    {
        float vertices[] = {
            // positions  // texture Coords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
             1.0f,  1.0f,  1.0f, 1.0f,
             1.0f, -1.0f,  1.0f, 0.0f,
        };
        {
            glCreateBuffers(1, &m_quadVBO);
            glNamedBufferStorage(m_quadVBO, sizeof(vertices), vertices, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
        }
        // vertex input
        {
            glCreateVertexArrays(1, &m_quadVAO);

            int vbuf_index = 0;
            glVertexArrayVertexBuffer(m_quadVAO, vbuf_index, m_quadVBO, 0,
                4 * sizeof(float));

            int a_pos = 0;
            glEnableVertexArrayAttrib(m_quadVAO, a_pos);
            glVertexArrayAttribFormat(m_quadVAO, a_pos, 2, GL_FLOAT, GL_FALSE, 0);
            glVertexArrayAttribBinding(m_quadVAO, a_pos, vbuf_index);

            int a_uv = 1;
            glEnableVertexArrayAttrib(m_quadVAO, a_uv);
            glVertexArrayAttribFormat(m_quadVAO, a_uv, 2, GL_FLOAT, GL_FALSE,
                sizeof(float) * 2);
            glVertexArrayAttribBinding(m_quadVAO, a_uv, vbuf_index);
        }
    }

    void Graphics::InitImGUIFramebuffer(WindowDimension dimension)
    {
        glCreateFramebuffers(1, &m_imguiFramebuffer);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_imguiTexture);
        glTextureParameteri(m_imguiTexture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureParameteri(m_imguiTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureStorage2D(m_imguiTexture, 1, GL_RGBA8, dimension.width,
            dimension.height);
        glNamedFramebufferTexture(m_imguiFramebuffer, GL_COLOR_ATTACHMENT0,
            m_imguiTexture, 0);
        unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
        glNamedFramebufferDrawBuffers(m_imguiFramebuffer, 1, attachments);
        GLuint rboIMGUI;

        glCreateRenderbuffers(1, &rboIMGUI);
        glNamedRenderbufferStorage(rboIMGUI, GL_DEPTH24_STENCIL8,
            dimension.width, dimension.height);
        glNamedFramebufferRenderbuffer(m_imguiFramebuffer, GL_DEPTH_ATTACHMENT,
            GL_RENDERBUFFER, rboIMGUI);

        if (glCheckNamedFramebufferStatus(m_imguiFramebuffer, GL_FRAMEBUFFER) !=
            GL_FRAMEBUFFER_COMPLETE)
            __debugbreak();
    }

    void Graphics::InitSceneFramebuffer(WindowDimension dimension)
    {
        glCreateFramebuffers(1, &m_sceneFramebuffer);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_sceneTexture);
        glTextureParameteri(m_sceneTexture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureParameteri(m_sceneTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        m_sceneTextureDimension = dimension;
        glTextureStorage2D(m_sceneTexture, 1, GL_RGBA8, m_sceneTextureDimension.width,
            m_sceneTextureDimension.height);

        glNamedFramebufferTexture(m_sceneFramebuffer, GL_COLOR_ATTACHMENT0,
            m_sceneTexture, 0);
        unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
        glNamedFramebufferDrawBuffers(m_sceneFramebuffer, 1, attachments);
        GLuint rbo;

        glCreateRenderbuffers(1, &rbo);
        glNamedRenderbufferStorage(rbo, GL_DEPTH24_STENCIL8,
            dimension.width, dimension.height);
        glNamedFramebufferRenderbuffer(m_sceneFramebuffer, GL_DEPTH_ATTACHMENT,
            GL_RENDERBUFFER, rbo);

        if (glCheckNamedFramebufferStatus(m_sceneFramebuffer, GL_FRAMEBUFFER) !=
            GL_FRAMEBUFFER_COMPLETE)
            __debugbreak();
    }

    /*If the dimensions of the window change, we need to resize*/
    void Graphics::UpdateImGUIFrameBuffer(WindowDimension& dimension,
        WindowDimension content)
    {
        dimension = content;
        glDeleteTextures(1, &m_imguiTexture);
        glDeleteFramebuffers(1, &m_imguiFramebuffer);
        InitImGUIFramebuffer(dimension);
    }

    void Graphics::InitModel(Model* model, ObjModel obj)
    {
        // vertex buffer containing triangle vertices
        model->vertexOffset = m_vertexCount;
        model->indexOffset = m_indexCount;
        model->indexCount = obj.indexCount;
        model->materialOffset = m_materialCount;
        int tempTexture = m_textures.textureCount;
        InitModelTextures(&obj.images);

        int temp = m_materialCount;
        for (int i = 0; i < (int)obj.materials.count; i++)
        {
            m_materials[i + temp].diffuse = { obj.materials.material[i].diffuse.x,
                                    obj.materials.material[i].diffuse.y,
                                    obj.materials.material[i].diffuse.z };
            m_materials[i + temp].Shininess = obj.materials.material[i].Shininess;
            if (obj.materials.material[i].hasTexture == 0)
                m_materials[i + temp].diffuseMap = -1;
            else
                m_materials[i + temp].diffuseMap = obj.materials.material[i].diffuseMap.index0 + tempTexture;
            if (obj.materials.material[i].hasNormal == 0)
                m_materials[i + temp].normalMap = -1;
            else
                m_materials[i + temp].normalMap = obj.materials.material[i].normalMap.index0 + tempTexture;
            m_materialCount++;
        }

        glNamedBufferSubData(m_materialSSBO, temp * sizeof(Material), (obj.materials.count) * sizeof(Material), m_materials);
        {
            // (GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data);
            glNamedBufferSubData(m_vertexBufferObject, m_vertexCount * sizeof(ObjVertex),
                obj.vertexCount * (sizeof(ObjVertex)), obj.vertices);
            m_vertexCount += obj.vertexCount;
        }
        {
            glNamedBufferSubData(m_ebo, m_indexCount * sizeof(u32),
                obj.indexCount * sizeof(u32), obj.indices);
            m_indexCount += obj.indexCount;
        }
        DeInitGraphicsObj(&obj);
    }

    void Graphics::InitFont()
    {
        u8* temp_bitmap = (u8 *)m->TemporaryAllocation(512 * 512);

        MyString file = OpenAndReadEntireFile("VictorMono-Bold.ttf", &m->m_memory.temp);
        stbtt_BakeFontBitmap((const unsigned char *)file.data, 0, 32.0, temp_bitmap, 512, 512, 32, 96, cdata); // no guarantee this fits!
        glGenTextures(1, &m_gFontTexture);
        glBindTexture(GL_TEXTURE_2D, m_gFontTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 512, 512, 0, GL_RED, GL_UNSIGNED_BYTE, temp_bitmap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    u32 Graphics::InitTexture(void* data, int width, int height, bool resident, bool repeat)
    {
        GLuint texture;
        glCreateTextures(GL_TEXTURE_2D, 1, &texture);
        glTextureStorage2D(texture, 1, GL_RGBA8, width, height);
        glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA,
            GL_UNSIGNED_BYTE, data);
        if (repeat)
        {
            glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }
        if (resident)
        {
            GLuint64 textureHandle = glGetTextureSamplerHandleARB(texture, m_textureSampler);
            glMakeTextureHandleResidentARB(textureHandle);
        }
        return (texture);
    }

    void Graphics::InitModelTextures(ObjImages* images)
    {
        Platform::WaitForThread(images->thread);

        for (int i = 0; i < (int)images->count; i++)
        {
            if (images->data[i].height && images->data[i].width)
            {
                m_textures.textures[m_textures.textureCount++] = InitTexture(images->data[i].data,
                    images->data[i].width, images->data[i].height, true, false);
            }
        }
    }

    void Graphics::SetViewProjectionMatrix(RedFoxMaths::Mat4 vp)
    {
        m_viewProjection = vp;
    }

    void Graphics::RenderText(GameUI ui)
    {
        if (ui.text.data == nullptr)
            return;
        glBindFramebuffer(GL_FRAMEBUFFER, m_sceneFramebuffer);
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBindProgramPipeline(m_font.pipeline);
        glBindVertexArray(m_quadVAO);
        float xOffset = (ui.screenPosition.x / 100) * dimension.width;
        float yOffset = (ui.screenPosition.y / 100) * dimension.height;
        RedFoxMaths::Mat4 mat = RedFoxMaths::Mat4::GetOrthographicMatrix(dimension.width, 0, dimension.height, 0, 1, -1);
        if (*ui.text.data)
        {
            RedFoxMaths::Float4 vertices[4] =
            {
                // positions  // texture Coords
                {xOffset, yOffset + ui.size.y,  0.0f, 1.0f},
                {xOffset, yOffset,  0.0f, 0.0f},
                {xOffset + ui.size.x, yOffset + ui.size.y,  1.0f, 1.0f},
                {xOffset + ui.size.x, yOffset,  1.0f, 0.0f},
            };
            glNamedBufferSubData(m_quadVBO, 0, sizeof(vertices), &vertices[0].x);
            if (ui.image)
                glBindTextureUnit(0, ui.image);
            else
                glBindTextureUnit(0, 0);

            if (ui.isHovered)
                glProgramUniform3fv(m_font.fragment, 2, 1, &ui.hoverColor.x);
            else
                glProgramUniform3fv(m_font.fragment, 2, 1, &ui.selectedColor.x);
            glProgramUniform1i(m_font.fragment, 1, 0);
            glProgramUniformMatrix4fv(m_font.vertex, 0, 1, GL_TRUE, mat.mat16);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
        char* text = (char*)ui.text.data;
        float xPos = xOffset + ui.textOffset.x;
        float yPos = -yOffset - ui.textOffset.y;
        while (*text) {
            if (*text >= 32)
            {
                stbtt_aligned_quad q;
                stbtt_GetBakedQuad(cdata, 512, 512, *text - 32, &xPos, &yPos, &q, 1);
                float vertices[] = {
                     q.x0, -q.y0, q.s0, q.t0,
                     q.x0, -q.y1, q.s0, q.t1,
                     q.x1, -q.y0, q.s1, q.t0,
                     q.x1, -q.y1, q.s1, q.t1,
                };
                // update content of VBO memory
                glNamedBufferSubData(m_quadVBO, 0, sizeof(vertices), vertices);
                glBindTextureUnit(0, m_gFontTexture);

                glProgramUniform3fv(m_font.fragment, 0, 1, &ui.textColor.x);
                glProgramUniform1i(m_font.fragment, 1, 1);
                // render quad 
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            }
            ++text;
        }
        float vertices[] = {
            // positions  // texture Coords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
             1.0f,  1.0f,  1.0f, 1.0f,
             1.0f, -1.0f,  1.0f, 0.0f,
        };
        glNamedBufferSubData(m_quadVBO, 0, sizeof(vertices), vertices);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Graphics::DrawSkyDome(SkyDome skyDome, float time)
    {
        glBindProgramPipeline(m_sky.pipeline);
        RedFoxMaths::Mat4 mvp = m_viewProjection * skyDome.model;

        glBindTextureUnit(0, skyDome.topTint);
        glBindTextureUnit(1, skyDome.botTint);
        glBindTextureUnit(2, skyDome.sun);
        glBindTextureUnit(3, skyDome.moon);
        glBindTextureUnit(4, skyDome.clouds);
        glProgramUniform3fv(m_sky.vertex, 0, 1, &skyDome.sunPosition.x);
        glProgramUniformMatrix4fv(m_sky.vertex, 1, 1, GL_TRUE, mvp.AsPtr());
        glProgramUniform1f(m_sky.fragment, 0, time);

        Model* sphere = &m_models[1];
        glDrawElementsInstancedBaseVertexBaseInstance(GL_TRIANGLES, sphere->indexCount,
            GL_UNSIGNED_INT, (void*)(sphere->indexOffset * sizeof(u32)), 1, sphere->vertexOffset, 0);
    }

    void Graphics::Draw(Scene *p_scene, WindowDimension p_windowDimension, float p_time, float p_delta, RedFoxMaths::Float3 position)
    {
        dimension = p_windowDimension;
        glBindVertexArray(m_vertexArrayObject);
        //
        GLuint64 textureHandles[128];
        for (int i = 0; i < (int)m_textures.textureCount; i++)
            textureHandles[i] = glGetTextureSamplerHandleARB(m_textures.textures[i], m_textureSampler);
        glNamedBufferSubData(m_textureSSBO, 0, sizeof(u64) * (m_textures.textureCount), textureHandles);
        //
        static float shadowMapUpdate;
        if (shadowMapUpdate > 0.016)
        {
            UpdateShaders();
            DrawShadowMaps(p_scene->m_modelCountIndex);
            shadowMapUpdate = 0;
        }
        shadowMapUpdate += p_delta;

        glBindFramebuffer(GL_FRAMEBUFFER, m_sceneFramebuffer);
        glCullFace(GL_BACK);
        glViewport(0, 0, m_sceneTextureDimension.width,
            m_sceneTextureDimension.height);
        glClearColor(0, 0, 0, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glDisable(GL_CULL_FACE);
        DrawSkyDome(p_scene->skyDome, p_time / 500);

        glEnable(GL_CULL_FACE);
        DrawGameObjects(p_scene->m_modelCountIndex, position);
        for (int i = 0; i < (int)p_scene->gameUICount; i++)
            RenderText(p_scene->gameUIs[i]);
        glViewport(0, 0, dimension.width, dimension.height);
        glBindFramebuffer(GL_FRAMEBUFFER, m_imguiFramebuffer);
        PostProcessingPass();
    }

    static void bindBuffer(int bindingPoint, GLuint buffer, int size)
    {
        if (size)
            glBindBufferRange(GL_SHADER_STORAGE_BUFFER, bindingPoint, buffer, 0, size);
        else
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, 0);
    }

    void Graphics::DrawGameObjects(u64* modelCountIndex, RedFoxMaths::Float3 view)
    {
        GLint u_matrix = 0; //location inside shader
        glProgramUniformMatrix4fv(m_blinnPhong.vertex, u_matrix, 1, GL_TRUE,
            m_viewProjection.AsPtr());
        glProgramUniform3f(m_blinnPhong.fragment, 0, view.x, view.y, view.z);

        glBindProgramPipeline(m_blinnPhong.pipeline);
        glBindVertexArray(m_vertexArrayObject);

        BindLights();
        bindBuffer(4, m_textureSSBO, m_textures.textureCount * sizeof(u64));
        bindBuffer(6, m_shadowMapsSSBO, lightStorage.lightCount * sizeof(u64));

        int totalCount = 0;
        for (int i = 0; i < (int)m_modelCount; i++)
        {
            if (modelCountIndex[i])
            {
                Model* model = &m_models[i];
                int instanceCount = modelCountIndex[i];
                glProgramUniform1i(m_blinnPhong.fragment, 1, 0);
                if (m_materials[model->materialOffset].diffuseMap != -1)
                {
                    glProgramUniform1i(m_blinnPhong.fragment, 1, model->materialOffset);
                    bindBuffer(5, m_materialSSBO, m_materialCount * sizeof(Material));
                }
                else
                    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 5, m_materialSSBO, (totalCount + m_materialCount) * sizeof(Material), instanceCount * sizeof(Material));
                glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 3, m_matrixSSBO, totalCount * sizeof(RedFoxMaths::Mat4), instanceCount * sizeof(RedFoxMaths::Mat4));
                glDrawElementsInstancedBaseVertexBaseInstance(GL_TRIANGLES, model->indexCount,
                    GL_UNSIGNED_INT, (void*)(model->indexOffset * sizeof(u32)), instanceCount, model->vertexOffset, 0);
            }
            totalCount += modelCountIndex[i];
        }
    }

    void Graphics::DrawShadowMaps(u64* modelCountIndex)
    {
        glCullFace(GL_FRONT);
        GLuint64 shadowMapsHandles[128];
        for (int lightIndex = 0; lightIndex < lightStorage.lightCount; lightIndex++)
        {
            int totalIndex = 0; int shadowDrawn = 0;
            if (lightStorage.lights[lightIndex].GetType() == LightType::NONE)
                continue;

            glBindFramebuffer(GL_FRAMEBUFFER, lightStorage.lights[lightIndex].depthMapFBO);
            glViewport(0, 0, lightStorage.lights[lightIndex].lightInfo.shadowParameters.SHADOW_WIDTH,
                lightStorage.lights[lightIndex].lightInfo.shadowParameters.SHADOW_HEIGHT);
            glClear(GL_DEPTH_BUFFER_BIT);

            GLint u_matrix = 0;
            glProgramUniformMatrix4fv(m_shadow.vertex, u_matrix, 1, GL_TRUE,
                lightStorage.lights[lightIndex].lightInfo.VP.AsPtr());

            for (int i = 0; i < (int)m_modelCount; i++)
            {
                u64 countIndex = modelCountIndex[i];
                if (countIndex)
                {
                    glBindProgramPipeline(m_shadow.pipeline);
                    glBindVertexArray(m_vertexArrayObject);
                    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 0, m_matrixSSBO, sizeof(RedFoxMaths::Mat4) * totalIndex, sizeof(RedFoxMaths::Mat4) * countIndex);
                    glDrawElementsInstancedBaseVertexBaseInstance(GL_TRIANGLES, m_models[i].indexCount,
                        GL_UNSIGNED_INT, (void*)(m_models[i].indexOffset * sizeof(u32)), countIndex, m_models[i].vertexOffset, 0);
                    totalIndex += countIndex;
                }
            }
            shadowMapsHandles[shadowDrawn] = glGetTextureHandleARB(lightStorage.lights[lightIndex].lightInfo.shadowParameters.depthMap);
        }
        glNamedBufferSubData(m_shadowMapsSSBO, 0, sizeof(u64) * (lightStorage.lightCount), shadowMapsHandles);
    }

    void Graphics::PushModelMatrices(RedFoxMaths::Mat4 *matrices, int count)
    {
        glNamedBufferSubData(m_matrixSSBO, 0, sizeof(RedFoxMaths::Mat4) * count, matrices);
    }

    void Graphics::PushMaterial(int count)
    {
        glNamedBufferSubData(m_materialSSBO, 0, sizeof(Material) * count + sizeof(Material) * m_materialCount, m_materials);
    }

    void Graphics::DrawModelShadowInstances(Model* model, int instanceCount)
    {
        // provide vertex input
        glBindProgramPipeline(m_shadow.pipeline);
        glBindVertexArray(m_vertexArrayObject);
        bindBuffer(0, m_matrixSSBO, sizeof(RedFoxMaths::Mat4) * instanceCount);
        glDrawElementsInstanced(GL_TRIANGLES, model->indexCount, GL_UNSIGNED_INT,
            0, instanceCount);
    }

    void Graphics::InitPostProcess(Memory* arena)
    {
        m_kernels = (Kernel*)MyMalloc(arena, sizeof(Kernel) * MAX_KERNEL);
        glCreateFramebuffers(1, &m_evenPostProcessFramebuffer);
        glCreateFramebuffers(1, &m_oddPostProcessFramebuffer);

        // Even
        {
            glCreateTextures(GL_TEXTURE_2D, 1, &m_evenPostProcessTexture);
            glTextureParameteri(m_evenPostProcessTexture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTextureParameteri(m_evenPostProcessTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTextureStorage2D(m_evenPostProcessTexture, 1, GL_RGBA8, m_sceneTextureDimension.width,
                m_sceneTextureDimension.height);

            glNamedFramebufferTexture(m_evenPostProcessFramebuffer, GL_COLOR_ATTACHMENT0,
                m_evenPostProcessTexture, 0);
            unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
            glNamedFramebufferDrawBuffers(m_evenPostProcessFramebuffer, 1, attachments);
        }   
        // Odd
        {
            glCreateTextures(GL_TEXTURE_2D, 1, &m_oddPostProcessTexture);
            glTextureParameteri(m_oddPostProcessTexture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTextureParameteri(m_oddPostProcessTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTextureStorage2D(m_oddPostProcessTexture, 1, GL_RGBA8, m_sceneTextureDimension.width,
                m_sceneTextureDimension.height);
            glNamedFramebufferTexture(m_oddPostProcessFramebuffer, GL_COLOR_ATTACHMENT0,
                            m_oddPostProcessTexture, 0);

            glNamedFramebufferTexture(m_oddPostProcessFramebuffer, GL_COLOR_ATTACHMENT0,
                                        m_oddPostProcessTexture, 0);
            unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
            glNamedFramebufferDrawBuffers(m_oddPostProcessFramebuffer, 1, attachments);
        }        
    }
    
    void Graphics::PostProcessDrawQuad()
    {
        glBindVertexArray(m_quadVAO);

        // clear screen
        glEnable(GL_BLEND);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glDisable(GL_DEPTH_TEST);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    void Graphics::PostProcessingPass()
    {
        // Shaders passes
        glBindTextureUnit(1, m_sceneTexture);
        glViewport(0, 0, m_sceneTextureDimension.width, m_sceneTextureDimension.height);
        int nextTexture = 0;
        for (int i = 0; i < m_postProcessShaders.size(); i++)
        {
            if (!m_postProcessShaders[i].active)
                continue;

            if (m_postProcessShaders[i].useKernels && m_postProcessShaders[i].kernels.size() > 0)
            {
                for (int k = 0; k < m_postProcessShaders[i].kernels.size(); k++)
                {
                    if (!m_postProcessShaders[i].kernels[i].active)
                        continue;
                    
                    if (k % 2 == 1)
                    {
                        glBindFramebuffer(GL_FRAMEBUFFER, m_oddPostProcessFramebuffer);
                        glBindProgramPipeline(m_postProcessShaders[i].pipeline);
                        glProgramUniformMatrix4fv(m_postProcess.fragment, 0, 1, 0, m_postProcessShaders[i].kernelsMatrices[k].AsPtr());
                        PostProcessDrawQuad();
                        glBindTextureUnit(1, m_oddPostProcessTexture);
                    }
                    else
                    {
                        glBindFramebuffer(GL_FRAMEBUFFER, m_evenPostProcessFramebuffer);
                        glBindProgramPipeline(m_postProcessShaders[i].pipeline);
                        glProgramUniformMatrix4fv(m_postProcess.fragment, 0, 1, 0, m_postProcessShaders[i].kernelsMatrices[k].AsPtr());
                        PostProcessDrawQuad();
                        glBindTextureUnit(1, m_evenPostProcessTexture);
                    }
                    nextTexture = (k+1) % 2; 
                }
            }
            else
            {
                if (i % 2 == 1)
                {
                    glBindFramebuffer(GL_FRAMEBUFFER, m_oddPostProcessFramebuffer);
                    glBindProgramPipeline(m_postProcessShaders[i].pipeline);
                    PostProcessDrawQuad();
                    glBindTextureUnit(1, m_oddPostProcessTexture);
                }
                else
                {
                    glBindFramebuffer(GL_FRAMEBUFFER, m_evenPostProcessFramebuffer);
                    glBindProgramPipeline(m_postProcessShaders[i].pipeline);
                    PostProcessDrawQuad();
                    glBindTextureUnit(1, m_evenPostProcessTexture);
                }
                nextTexture = (i+1) % 2; 
            }
        }

        // Kernels passes
        glBindProgramPipeline(m_postProcess.pipeline);
        glBindVertexArray(m_quadVAO);

        for (int i = 0; i < m_kernelCount; i++)
        {
            if (!m_kernels[i].active)
                continue;
            
            if ((nextTexture + i) % 2 == 1)
            {
                glBindFramebuffer(GL_FRAMEBUFFER, m_oddPostProcessFramebuffer);
                glProgramUniformMatrix4fv(m_postProcess.fragment, 0, 1, 0, m_kernelsMatrices[i].AsPtr());
                PostProcessDrawQuad();
                glBindTextureUnit(1, m_oddPostProcessTexture);
            }
            else
            {
                glBindFramebuffer(GL_FRAMEBUFFER, m_evenPostProcessFramebuffer);
                glProgramUniformMatrix4fv(m_postProcess.fragment, 0, 1, 0, m_kernelsMatrices[i].AsPtr());
                PostProcessDrawQuad();
                glBindTextureUnit(1, m_evenPostProcessTexture);
            }
        }

        // Identity kernel to draw the final render
        glBindFramebuffer(GL_FRAMEBUFFER, m_imguiFramebuffer);
        glViewport(0, 0, dimension.width, dimension.height);

        static float identity[16] = {
            0.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 0.f, 0.f,
            0.f, 0.f, 0.f, 0.f
        };

        glProgramUniformMatrix4fv(m_postProcess.fragment, 0, 1, 0, identity);
        
        PostProcessDrawQuad();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glEnable(GL_DEPTH_TEST);
    }

    Kernel* Graphics::AddKernel(RedFoxMaths::Mat4 kernel)
    {
        if (m_kernelCreated > m_kernelCount)
        {
            for (int i = 0; i < m_kernelCount; i++)
            {
                if (m_kernels[i].deleted)
                {
                    m_kernels[i].kernel = kernel;
                    m_kernels[i].deleted = false;
                    m_kernels[i].active = true;

                    m_kernelCount++;
                    return &m_kernels[i];
                }
            }
        }

        Kernel result;
        result.uniqueId = m_kernelCreated;
        result.kernel = kernel;

        m_kernels[result.uniqueId] = result;
        m_kernelCount++;
        m_kernelCreated++;

        return &m_kernels[result.uniqueId];
    }

    void Graphics::DeleteKernel(int id)
    {
        if (id > m_kernelCreated || m_kernels[id].deleted)
            return;

        m_kernels[id].deleted = true;

        if (m_kernels[id].active)
            m_kernelCount--;
    }

    void Graphics::EditKernel(int id, RedFoxMaths::Mat4 kernel)
    {
        if (id > m_kernelCreated || m_kernels[id].deleted)
            return;

        m_kernels[id].kernel = kernel;
    }

    void Graphics::ResetKernel(int id)
    {
        if (id > m_kernelCreated || m_kernels[id].deleted)
            return;

        float kernel[4][4] = {
            {0.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 0.0f}
        };
        
        m_kernels[id].kernel = kernel;
    }

    void Graphics::BindKernelBuffer(Memory* tempAlocator)
    {
        int activeCount = 0;
        for (int i = 0; i < m_kernelCreated; i++)
        {
            if (m_kernels[i].active)
                activeCount++;
        }
        
        m_kernelsMatrices = (RedFoxMaths::Mat4*)MyMalloc(tempAlocator, sizeof(RedFoxMaths::Mat4) * activeCount);
        
        int count = 0;
        for (int i = 0; i < m_kernelCreated; i++)
        {
            if (!m_kernels[i].deleted && m_kernels[i].active)
            {
                m_kernelsMatrices[count] = m_kernels[i].kernel;
                count++;
            }
        }

        for (int i = 0; i < m_postProcessShaders.size(); i++)
        {
            if (!m_postProcessShaders[i].active)
                continue;

            if (m_postProcessShaders[i].useKernels && m_postProcessShaders[i].kernels.size() > 0)
            {
                m_postProcessShaders[i].BindKernelBuffer(tempAlocator);
            }
        }
    }
} // namespace RedFoxEngine
