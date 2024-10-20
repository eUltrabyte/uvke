#pragma once
#ifndef UVKE_VERTEX_BUFFER_HEADER
#define UVKE_VERTEX_BUFFER_HEADER

#include "../uvke.hpp"
#include "../Core/Base.hpp"
#include "../Core/Helper.hpp"

namespace uvke {
    struct UVKE_API Vertex {
        glm::vec3 position = { 0.0f, 0.0f, 0.0f };
        glm::vec4 color = { 0.0f, 0.0f, 0.0f, 0.0f };
        glm::vec2 texCoord = { 0.0f, 0.0f };
        glm::vec3 normal = { 0.0f, 0.0f, 0.0f };

        bool operator==(const Vertex&) const = default;
    };

    class UVKE_API VertexBuffer {
    public:
        VertexBuffer(Base* base = nullptr, std::vector<Vertex> vertices = std::vector<Vertex>(0));
        virtual ~VertexBuffer();

        virtual void Bind(VkCommandBuffer commandBuffer);

        virtual void SetBase(Base* base);

        virtual std::vector<Vertex>& GetVertices();
        virtual VkVertexInputBindingDescription& GetVertexInputBindingDescription();
        virtual std::array<VkVertexInputAttributeDescription, 4>& GetVertexInputAttributeDescription();
        virtual unsigned int GetSize();
        virtual VkBuffer& GetBuffer();
        virtual VkDeviceMemory& GetBufferMemory();

    protected:
        Base* m_base;

    private:
        std::vector<Vertex> m_vertices;
        VkVertexInputBindingDescription m_vertexInputBindingDescription;
        std::array<VkVertexInputAttributeDescription, 4> m_vertexInputAttributeDescription;
        VkBuffer m_buffer;
        VkDeviceMemory m_bufferMemory;

    };
};

namespace std {
    template<>
    struct hash<uvke::Vertex> {
        size_t operator()(uvke::Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.position) ^ (hash<glm::vec4>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}

#endif
