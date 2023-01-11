#pragma once
#ifndef UVKE_VERTEX_BUFFER_HEADER
#define UVKE_VERTEX_BUFFER_HEADER

#include "../uvke.hpp"
#include "Shader.hpp"

namespace uvke {
    struct Vertex {
        vec3f position;
        vec4f color;
    };

    class UVKE_API VertexBuffer {
    public:
        VertexBuffer(VkPhysicalDevice physicalDevice = nullptr, VkDevice device = nullptr, std::vector<Vertex> vertices = std::vector<Vertex>(0));
        virtual ~VertexBuffer();

        virtual void Bind(VkCommandBuffer commandBuffer);

        virtual std::vector<Vertex>& GetVertices();
        virtual VkVertexInputBindingDescription& GetVertexInputBindingDescription();
        virtual std::array<VkVertexInputAttributeDescription, 2>& GetVertexInputAttributeDescription();
        virtual unsigned int GetSize();
        virtual VkBuffer& GetBuffer();

    private:
        VkPhysicalDevice m_physicalDevice;
        VkDevice m_device;
        std::vector<Vertex> m_vertices;
        VkVertexInputBindingDescription m_vertexInputBindingDescription;
        std::array<VkVertexInputAttributeDescription, 2> m_vertexInputAttributeDescription;
        VkBuffer m_buffer;
        VkDeviceMemory m_bufferMemory;

    };
};

#endif