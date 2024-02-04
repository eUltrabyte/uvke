#pragma once
#ifndef UVKE_PIPELINE_HEADER
#define UVKE_PIPELINE_HEADER

#include "../uvke.hpp"
#include "Surface.hpp"
#include "Framebuffer.hpp"
#include "CommandBuffer.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Descriptor.hpp"
#include "UniformBuffer.hpp"
#include "Interface.hpp"
#include "Component.hpp"
#include "RenderableComponent.hpp"

namespace uvke {
    struct UVKE_API ObjectPushConstant {
        bool hasTexture;
        bool dump1;
        bool dump2;
        bool dump3;
    };

    class UVKE_API Pipeline {
    public:
        Pipeline(Base* base = nullptr, Surface* surface = nullptr, VertexBuffer* vertexBuffer = nullptr, Descriptor* descriptor = nullptr);
        virtual ~Pipeline();

        virtual void Recreate();
        virtual void Render(Framebuffer* framebuffer, CommandBuffer* commandBuffer, unsigned int frame, unsigned int index, std::vector<Component*> components, Interface* interfaces);

        virtual void SetBase(Base* base);

        virtual std::shared_ptr<Shader> GetShader();
        virtual VkRenderPass& GetRenderPass();
        virtual VkPipelineCache& GetPipelineCache();
        virtual VkPipelineLayout& GetPipelineLayout();
        virtual VkPipeline& GetPipeline();

    protected:
        Base* m_base;
        Surface* m_surface;
        VertexBuffer* m_vertexBuffer;
        Descriptor* m_descriptor;

    private:
        std::shared_ptr<Shader> m_shader;
        VkRenderPass m_renderPass;
        VkPipelineCache m_pipelineCache;
        VkPipelineLayout m_pipelineLayout;
        VkPipeline m_pipeline;

    };
};

#endif