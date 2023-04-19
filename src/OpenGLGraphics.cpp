#include "OpenGLGraphics.hpp"
#include "glcorearb.h"
#include "imgui_impl_opengl3.h"

#define MEMORY_IMPLEMENTATION
#include "MyMemory.hpp"

namespace RedFoxEngine
{

    void Graphics::InitGraphics(Memory* tempArena, WindowDimension dimension)
    {
        InitShaders(tempArena);
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
                100 * sizeof(ObjMaterial),
                nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);

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

        //V-SYNC   
        wglSwapIntervalEXT(0);
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

    /*If the dimensions of the window change, we need to resize*/
    void Graphics::UpdateImGUIFrameBuffer(WindowDimension& dimension,
        WindowDimension content)
    {
        dimension = content;
        glDeleteTextures(1, &m_imguiTexture);
        glDeleteFramebuffers(1, &m_imguiFramebuffer);
        InitImGUIFramebuffer(dimension);
    }

    void Graphics::InitModel(Model* model)
    {
        // vertex buffer containing triangle vertices
        unsigned int vbo;
        {
            glCreateBuffers(1, &vbo);
            glNamedBufferStorage(vbo, model->obj.vertexCount * (sizeof(ObjVertex)),
                model->obj.vertices, 0);
        }
        unsigned int ebo;
        {
            glCreateBuffers(1, &ebo);
            glNamedBufferStorage(ebo, model->obj.indexCount * sizeof(u32),
                model->obj.indices, 0);
        }
        // vertex input
        {
            glCreateVertexArrays(1, &model->vao);

            int vbuf_index = 0;
            glVertexArrayVertexBuffer(model->vao, vbuf_index, vbo, 0,
                sizeof(ObjVertex));

            glVertexArrayElementBuffer(model->vao, ebo);

            int a_pos = 0;
            glEnableVertexArrayAttrib(model->vao, a_pos);
            glVertexArrayAttribFormat(model->vao, a_pos, 3, GL_FLOAT, GL_FALSE,
                offsetof(struct ObjVertex, position));
            glVertexArrayAttribBinding(model->vao, a_pos, vbuf_index);

            int a_normal = 1;
            glEnableVertexArrayAttrib(model->vao, a_normal);
            glVertexArrayAttribFormat(model->vao, a_normal, 3, GL_FLOAT, GL_FALSE,
                offsetof(struct ObjVertex, normal));
            glVertexArrayAttribBinding(model->vao, a_normal, vbuf_index);

            int a_uv = 2;
            glEnableVertexArrayAttrib(model->vao, a_uv);
            glVertexArrayAttribFormat(model->vao, a_uv, 2, GL_FLOAT, GL_FALSE,
                offsetof(struct ObjVertex, textureUV));
            glVertexArrayAttribBinding(model->vao, a_uv, vbuf_index);
            int a_materialID = 3;
            glEnableVertexArrayAttrib(model->vao, a_materialID);
            glVertexArrayAttribIFormat(model->vao, a_materialID, 1, GL_UNSIGNED_INT,
                offsetof(struct ObjVertex, materialID));
            glVertexArrayAttribBinding(model->vao, a_materialID, vbuf_index);
        }
        InitModelTextures(&model->obj);
        DeInitGraphicsObj(&model->obj);
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

    void Graphics::InitModelTextures(ObjModel* model)
    {
        // TODO(V. Caraulan): Wrap in waiting for thread function
        WaitForSingleObject(model->images.thread, INFINITE);

        for (int i = 0; i < (int)model->materials.count; i++)
        {//TODO: if we ever have 1 model draw function, we nedd to get rid of this
            model->materials.material[i].diffuseMap.index0 += m_textures.textureCount;
            model->materials.material[i].normalMap.index0 += m_textures.textureCount;
        }
        for (int i = 0; i < (int)model->images.count; i++)
        {
            if (model->images.data[i].height && model->images.data[i].width)
            {
                m_textures.textures[m_textures.textureCount++] = InitTexture(model->images.data[i].data,
                    model->images.data[i].width, model->images.data[i].height, true, false);
            }
        }
    }

    void Graphics::SetViewProjectionMatrix(RedFoxMaths::Mat4 vp)
    {
        m_viewProjection = vp;
    }

    void Graphics::DrawSkyDome(SkyDome skyDome, float time)
    {
        glBindProgramPipeline(m_sky.pipeline);
        glBindVertexArray(m_models[1].vao); // TODO:Maybe pass the model as a parameter

        RedFoxMaths::Mat4 mvp = m_viewProjection * skyDome.model;

        glBindTextureUnit(0, skyDome.topTint);
        glBindTextureUnit(1, skyDome.botTint);
        glBindTextureUnit(2, skyDome.sun);
        glBindTextureUnit(3, skyDome.moon);
        glBindTextureUnit(4, skyDome.clouds);
        glProgramUniform3fv(m_sky.vertex, 0, 1, &skyDome.sunPosition.x);
        glProgramUniformMatrix4fv(m_sky.vertex, 1, 1, GL_TRUE, mvp.AsPtr());
        glProgramUniform1f(m_sky.fragment, 0, time);

        glDrawElements(GL_TRIANGLES, m_models[1].obj.indexCount, GL_UNSIGNED_INT, 0);
    }

    void Graphics::Draw(RedFoxMaths::Mat4* p_modelMatrices, u64* p_modelCountIndex, WindowDimension p_windowDimension, SkyDome p_skyDome, float p_time)
    {
        static int test = 0;
        if (test % 1 == 0)
            DrawShadowMaps(p_modelMatrices, p_modelCountIndex);

        glBindFramebuffer(GL_FRAMEBUFFER, m_imguiFramebuffer);
        glCullFace(GL_BACK);
        glViewport(0, 0, p_windowDimension.width,
            p_windowDimension.height);
        glClearColor(0, 0, 0, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glDisable(GL_CULL_FACE);
        DrawSkyDome(p_skyDome, p_time / 500);

        glEnable(GL_CULL_FACE);
        DrawGameObjects(p_modelMatrices, p_modelCountIndex);
        // swap the buffers to show output

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        test++;
    }

    void Graphics::DrawGameObjects(RedFoxMaths::Mat4* modelMatrices, u64* modelCountIndex)
    {
        GLint u_matrix = 0; //location inside shader
        glProgramUniformMatrix4fv(m_blinnPhong.vertex, u_matrix, 1, GL_TRUE,
            m_viewProjection.AsPtr());

        int totalCount = 0;
        for (int i = 0; i < (int)m_modelCount; i++)
        {
            if (modelCountIndex[i])
                DrawModelInstances(&m_models[i], &modelMatrices[totalCount],
                    modelCountIndex[i]);
            totalCount += modelCountIndex[i];
        }
    }

    void Graphics::DrawModelInstances(Model* model,
        RedFoxMaths::Mat4* modelMatrices, int instanceCount)
    {
        // provide vertex input
        glNamedBufferSubData(m_matrixSSBO, 0, sizeof(RedFoxMaths::Mat4) * instanceCount, modelMatrices);
        Material materials[100] = {};

        for (int i = 0; i < (int)model->obj.materials.count; i++)
        {
            materials[i].ambient = model->obj.materials.material[i].ambient;
            materials[i].Opaqueness = model->obj.materials.material[i].Opaqueness;
            materials[i].diffuse = model->obj.materials.material[i].diffuse;
            materials[i].Shininess = model->obj.materials.material[i].Shininess;
            materials[i].specular = model->obj.materials.material[i].specular;
            materials[i].emissive = model->obj.materials.material[i].emissive;
            if (model->obj.materials.material[i].hasTexture == 0)
                materials[i].diffuseMap = -1;
            else
                materials[i].diffuseMap = model->obj.materials.material[i].diffuseMap.index0;
            if (model->obj.materials.material[i].hasNormal == 0)
                materials[i].normalMap = -1;
            else
                materials[i].normalMap = model->obj.materials.material[i].normalMap.index0;
        }
        glNamedBufferSubData(m_materialSSBO, 0, sizeof(ObjMaterial) * model->obj.materials.count, materials);
        glBindProgramPipeline(m_blinnPhong.pipeline);
        glBindVertexArray(model->vao);

        BindLights();
        glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 3, m_matrixSSBO, 0, instanceCount);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_matrixSSBO);
        glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 4, m_textureSSBO, 0, m_textures.textureCount);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, m_textureSSBO);
        glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 5, m_materialSSBO, 0, model->obj.materials.count);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, m_materialSSBO);
        glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 6, m_shadowMapsSSBO, 0, lightStorage.lightCount);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, m_shadowMapsSSBO);

        glDrawElementsInstanced(GL_TRIANGLES, model->obj.indexCount,
            GL_UNSIGNED_INT, 0, instanceCount);
    }

    void Graphics::DrawShadowMaps(RedFoxMaths::Mat4* modelMatrices, u64* modelCountIndex)
    {
        GLuint64 textureHandles[128];
        for (int i = 0; i < (int)m_textures.textureCount; i++)
            textureHandles[i] = glGetTextureSamplerHandleARB(m_textures.textures[i], m_textureSampler);
        glNamedBufferSubData(m_textureSSBO, 0, sizeof(u64) * (m_textures.textureCount), textureHandles);
        glCullFace(GL_FRONT);

        for (int lightIndex = 0; lightIndex < lightStorage.lightCount; lightIndex++)
        {
            int totalIndex = 0;
            if (lightStorage.lights[lightIndex].GetType() == LightType::NONE)
                continue;

            glBindFramebuffer(GL_FRAMEBUFFER, lightStorage.lights[lightIndex].lightInfo.shadowParameters.depthMapFBO);
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
                    glNamedBufferSubData(m_matrixSSBO, 0,
                        sizeof(RedFoxMaths::Mat4) * countIndex, &modelMatrices[totalIndex]);
                    DrawModelShadowInstances(&m_models[i], countIndex);
                    totalIndex += countIndex;
                }
            }
        }
        GLuint64 shadowMapsHandles[128];
        for (int i = 0; i < (int)lightStorage.lightCount; i++)
            shadowMapsHandles[i] = glGetTextureHandleARB(lightStorage.lights[i].lightInfo.shadowParameters.depthMap);
        glNamedBufferSubData(m_shadowMapsSSBO, 0, sizeof(u64) * (lightStorage.lightCount), shadowMapsHandles);
    }

    void Graphics::DrawModelShadowInstances(Model* model, int instanceCount)
    {
        // provide vertex input
        glBindProgramPipeline(m_shadow.pipeline);
        glBindVertexArray(model->vao);
        glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 0, m_matrixSSBO, 0, instanceCount);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_matrixSSBO);
        glDrawElementsInstanced(GL_TRIANGLES, model->obj.indexCount, GL_UNSIGNED_INT,
            0, instanceCount);
    }

} // namespace RedFoxEngine
