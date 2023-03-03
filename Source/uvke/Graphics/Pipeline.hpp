#pragma once
#ifndef UVKE_PIPELINE_HEADER
#define UVKE_PIPELINE_HEADER

#include "../uvke.hpp"
#include "Surface.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "UniformBuffer.hpp"

namespace uvke {
    class UVKE_API Pipeline {
    public:
        Pipeline(VkDevice device, std::shared_ptr<Surface> surface, std::shared_ptr<Shader> shader, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<UniformBuffer> uniformBuffer);
        virtual ~Pipeline();

        virtual void Recreate();

        virtual void SetDevice(VkDevice device);
        virtual void SetRenderPass(VkRenderPass renderPass);
        virtual void SetPipelineLayout(VkPipelineLayout pipelineLayout);
        virtual void SetPipeline(VkPipeline pipeline);

        virtual VkDevice& GetDevice();
        virtual std::shared_ptr<Surface> GetSurface();
        virtual std::shared_ptr<Shader> GetShader();
        virtual std::shared_ptr<VertexBuffer> GetVertexBuffer();
        virtual std::shared_ptr<UniformBuffer> GetUniformBuffer();
        virtual VkRenderPass& GetRenderPass();
        virtual VkPipelineLayout& GetPipelineLayout();
        virtual VkPipeline& GetPipeline();

    protected:
        VkDevice m_device;
        std::shared_ptr<Surface> m_surface;
        std::shared_ptr<Shader> m_shader;
        std::shared_ptr<VertexBuffer> m_vertexBuffer;
        std::shared_ptr<UniformBuffer> m_uniformBuffer;

    private:
        VkRenderPass m_renderPass;
        VkPipelineLayout m_pipelineLayout;
        VkPipeline m_pipeline;

    };
};

#endif