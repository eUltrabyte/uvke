#pragma once
#ifndef UVKE_SPRITE_HEADER
#define UVKE_SPRITE_HEADER

#include "../uvke.hpp"
#include "StagingBuffer.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "UniformBuffer.hpp"
#include "Camera.hpp"
#include "Renderable.hpp"
#include "Renderer.hpp"

namespace uvke {
    class UVKE_API Sprite : public Renderable {
    public:
        Sprite(const vec2f& size = { 1.0f, 1.0f });
        virtual ~Sprite();

        virtual void Create(std::shared_ptr<Renderer> renderer) override;
        virtual void Update(std::shared_ptr<Camera> camera) override;
        virtual void Render(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, unsigned int frame) override;

        virtual void SetPosition(const vec2f& position);
        virtual void SetScale(const vec2f& scale);
        virtual void SetRotation(float angle);
        virtual void SetVertices(const std::vector<Vertex>& vertices);
        virtual void SetIndices(const std::vector<unsigned int>& indices);

        virtual std::vector<Vertex>& GetVertices();
        virtual std::vector<unsigned int>& GetIndices();
        virtual std::shared_ptr<VertexBuffer> GetVertexBuffer();
        virtual std::shared_ptr<IndexBuffer> GetIndexBuffer();
        virtual std::shared_ptr<UniformBuffer> GetUniformBuffer();

    private:
        mat4x4f m_model;
        std::vector<Vertex> m_vertices;
        std::vector<unsigned int> m_indices;
        std::shared_ptr<VertexBuffer> m_vertexBuffer;
        std::shared_ptr<IndexBuffer> m_indexBuffer;
        std::shared_ptr<UniformBuffer> m_uniformBuffer;
      
    };
};

#endif