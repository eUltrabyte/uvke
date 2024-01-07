#pragma once
#ifndef UVKE_RENDERABLE_COMPONENT_HEADER
#define UVKE_RENDERABLE_COMPONENT_HEADER

#include "../uvke.hpp"
#include "StagingBuffer.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "UniformBuffer.hpp"
#include "Component.hpp"

namespace uvke {
    enum UVKE_API RenderType {
        Triangles = 0,
        Lines = UVKE_BYTE(0),
        Points = UVKE_BYTE(1),
    };

    class UVKE_API Renderer;

    class UVKE_API RenderableComponent : public Component {
    public:
        RenderableComponent() = default;
        virtual ~RenderableComponent() = default;

        virtual void Update(Camera* camera) = 0;
        virtual void Render(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, unsigned int frame) = 0;

        virtual RenderType& GetRenderType() {
            return m_renderType;
        }

        virtual VertexBuffer* GetVertexBuffer() {
            return m_vertexBuffer.get();
        }

        virtual IndexBuffer* GetIndexBuffer() {
            return m_indexBuffer.get();
        }

        virtual UniformBuffer* GetUniformBuffer() {
            return m_uniformBuffer.get();
        }

    protected:
        RenderType m_renderType = RenderType::Triangles;
        std::unique_ptr<VertexBuffer> m_vertexBuffer;
        std::unique_ptr<IndexBuffer> m_indexBuffer;
        std::unique_ptr<UniformBuffer> m_uniformBuffer;

    };
};

#endif