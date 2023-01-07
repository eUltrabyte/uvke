#include "VertexBuffer.hpp"

namespace uvke {
    VertexBuffer::VertexBuffer(VkPhysicalDevice physicalDevice, VkDevice device)
        : m_physicalDevice(physicalDevice), m_device(device) {
        m_vertices = std::vector<Vertex> {
            {{ 0.0f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }},
            {{ 0.5f, 0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }},
            {{ -0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }},
        };

        m_vertexInputBindingDescription = { };
        m_vertexInputBindingDescription.binding = 0;
        m_vertexInputBindingDescription.stride = sizeof(Vertex);
        m_vertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        m_vertexInputAttributeDescription = { };
        m_vertexInputAttributeDescription[0].binding = 0;
        m_vertexInputAttributeDescription[0].location = 0;
        m_vertexInputAttributeDescription[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        m_vertexInputAttributeDescription[0].offset = offsetof(Vertex, Vertex::position);

        m_vertexInputAttributeDescription[1].binding = 0;
        m_vertexInputAttributeDescription[1].location = 1;
        m_vertexInputAttributeDescription[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
        m_vertexInputAttributeDescription[1].offset = offsetof(Vertex, Vertex::color);

        VkBufferCreateInfo bufferCreateInfo = { };
        bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferCreateInfo.pNext = nullptr;
        bufferCreateInfo.flags = 0;
        bufferCreateInfo.size = sizeof(m_vertices[0]) * m_vertices.size();
        bufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        bufferCreateInfo.queueFamilyIndexCount = 0;
        bufferCreateInfo.pQueueFamilyIndices = nullptr;

        UVKE_ASSERT(vkCreateBuffer(m_device, &bufferCreateInfo, nullptr, &m_buffer));

        VkMemoryRequirements memoryRequirements { };
        vkGetBufferMemoryRequirements(m_device, m_buffer, &memoryRequirements);

        VkPhysicalDeviceMemoryProperties memoryProperties { };
        vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memoryProperties);

        unsigned int filter = memoryRequirements.memoryTypeBits;
        VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        unsigned int index = 0;
        for(auto i = 0; i < memoryProperties.memoryTypeCount; ++i) {
            if(filter & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                index = i;
                break;
            }
        }

        VkMemoryAllocateInfo memoryAllocateInfo { };
        memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memoryAllocateInfo.pNext = nullptr;
        memoryAllocateInfo.allocationSize = memoryRequirements.size;
        memoryAllocateInfo.memoryTypeIndex = index;

        UVKE_ASSERT(vkAllocateMemory(m_device, &memoryAllocateInfo, nullptr, &m_bufferMemory));

        vkBindBufferMemory(m_device, m_buffer, m_bufferMemory, 0);

        void* rawData;
        vkMapMemory(m_device, m_bufferMemory, 0, bufferCreateInfo.size, 0, &rawData);
        std::memcpy(rawData, m_vertices.data(), bufferCreateInfo.size);
        vkUnmapMemory(m_device, m_bufferMemory);
    }

    VertexBuffer::~VertexBuffer() {
        vkDestroyBuffer(m_device, m_buffer, nullptr);
        vkFreeMemory(m_device, m_bufferMemory, nullptr);
    }

    void VertexBuffer::Bind(VkCommandBuffer commandBuffer) {
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, &m_buffer, offsets);
    }

    std::vector<Vertex>& VertexBuffer::GetVertices() {
        return m_vertices;
    }

    VkVertexInputBindingDescription& VertexBuffer::GetVertexInputBindingDescription() {
        return m_vertexInputBindingDescription;
    }
    
    std::array<VkVertexInputAttributeDescription, 2>& VertexBuffer::GetVertexInputAttributeDescription() {
        return m_vertexInputAttributeDescription;
    }

    VkBuffer& VertexBuffer::GetBuffer() {
        return m_buffer;
    }
};