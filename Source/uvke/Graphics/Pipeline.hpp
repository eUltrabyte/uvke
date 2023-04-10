#pragma once
#ifndef UVKE_PIPELINE_HEADER
#define UVKE_PIPELINE_HEADER

#include "../uvke.hpp"
#include "Surface.hpp"
#include "Framebuffer.hpp"
#include "CommandBuffer.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "UniformBuffer.hpp"
#include "Interface.hpp"

namespace uvke {
    class UVKE_API Pipeline {
    public:
        Pipeline(VkDevice device = nullptr, std::shared_ptr<Surface> surface = nullptr, std::shared_ptr<Shader> shader = nullptr, std::shared_ptr<VertexBuffer> vertexBuffer = nullptr, std::shared_ptr<UniformBuffer> uniformBuffer = nullptr);
        virtual ~Pipeline();

        virtual void Recreate();
        virtual void Render(std::shared_ptr<Framebuffer> framebuffer, std::shared_ptr<CommandBuffer> commandBuffer, unsigned int frame, unsigned int index, std::vector<std::shared_ptr<VertexBuffer>> vertexBuffers, std::vector<std::shared_ptr<IndexBuffer>> indexBuffers, std::vector<std::shared_ptr<UniformBuffer>> uniformBuffers, std::shared_ptr<Interface> interfaces);

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