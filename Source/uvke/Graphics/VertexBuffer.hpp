#pragma once
#ifndef UVKE_VERTEX_BUFFER_HEADER
#define UVKE_VERTEX_BUFFER_HEADER

#include "../uvke.hpp"
#include "../Core/Base.hpp"
#include "../Core/Helper.hpp"

namespace uvke {
    struct UVKE_API Vertex {
        vec3f position;
        vec3f color;
        vec2f texCoord;
    };

    class UVKE_API VertexBuffer {
    public:
        VertexBuffer(Base* base = nullptr, std::vector<Vertex> vertices = std::vector<Vertex>(0));
        virtual ~VertexBuffer();

        virtual void Bind(VkCommandBuffer commandBuffer);

        virtual void SetBase(Base* base);

        virtual std::vector<Vertex>& GetVertices();
        virtual VkVertexInputBindingDescription& GetVertexInputBindingDescription();
        virtual std::array<VkVertexInputAttributeDescription, 3>& GetVertexInputAttributeDescription();
        virtual unsigned int GetSize();
        virtual VkBuffer& GetBuffer();
        virtual VkDeviceMemory& GetBufferMemory();

    protected:
        Base* m_base;

    private:
        std::vector<Vertex> m_vertices;
        VkVertexInputBindingDescription m_vertexInputBindingDescription;
        std::array<VkVertexInputAttributeDescription, 3> m_vertexInputAttributeDescription;
        VkBuffer m_buffer;
        VkDeviceMemory m_bufferMemory;

    };
};

#endif