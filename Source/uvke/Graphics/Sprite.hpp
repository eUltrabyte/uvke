#pragma once
#ifndef UVKE_SPRITE_HEADER
#define UVKE_SPRITE_HEADER

#include "../uvke.hpp"
#include "StagingBuffer.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "UniformBuffer.hpp"
#include "Renderable.hpp"
#include "Renderer.hpp"

namespace uvke {
    class UVKE_API Sprite : public Renderable {
    public:
        Sprite(const vec2f& size = { 1.0f, 1.0f });
        virtual ~Sprite();

        virtual void Create(std::shared_ptr<Renderer> renderer) override;
        virtual void Update(std::shared_ptr<Window> window) override;
        virtual void Render(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, unsigned int frame) override;

        virtual std::vector<Vertex>& GetVertices();
        virtual std::vector<unsigned int>& GetIndices();

    private:
        std::vector<Vertex> m_vertices;
        std::vector<unsigned int> m_indices;
        std::shared_ptr<VertexBuffer> m_vertexBuffer;
        std::shared_ptr<IndexBuffer> m_indexBuffer;
        std::shared_ptr<UniformBuffer> m_uniformBuffer;
      
    };
};

#endif