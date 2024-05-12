#pragma once
#ifndef UVKE_VERTEX_BUFFER_HEADER
#define UVKE_VERTEX_BUFFER_HEADER

#include "../uvke.hpp"
#include "../Core/Base.hpp"
#include "../Core/Helper.hpp"

namespace uvke {
    struct UVKE_API Vertex {
        vec3f position = { 0.0f, 0.0f, 0.0f };
        vec4f color = { 0.0f, 0.0f, 0.0f, 0.0f };
        vec2f texCoord = { 0.0f, 0.0f };
        vec3f normal = { 0.0f, 0.0f, 0.0f };

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
    template<typename T>
	struct hash<uvke::vec2<T>> {
		size_t operator()(uvke::vec2<T> const& vec) const;
	};

    template<typename T>
	struct hash<uvke::vec3<T>> {
		size_t operator()(uvke::vec3<T> const& vec) const;
	};

    template<typename T>
	struct hash<uvke::vec4<T>> {
		size_t operator()(uvke::vec4<T> const& vec) const;
	};

    template<>
    struct hash<uvke::Vertex> {
        size_t operator()(uvke::Vertex const& vertex) const {
            return ((hash<uvke::vec3f>()(vertex.position) ^ (hash<uvke::vec4f>()(vertex.color) << 1)) >> 1) ^ (hash<uvke::vec2f>()(vertex.texCoord) << 1);
        }
    };
}

#endif