#include "VertexBuffer.hpp"

namespace uvke {
    VertexBuffer::VertexBuffer(VkPhysicalDevice physicalDevice, VkDevice device, std::vector<Vertex> vertices)
        : m_physicalDevice(physicalDevice), m_device(device), m_vertices(vertices) {
        {
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
            bufferCreateInfo.size = sizeof(Vertex) * m_vertices.size();
            bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
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
        }

        vkBindBufferMemory(m_device, m_buffer, m_bufferMemory, 0);

        UVKE_LOG("Vertex Buffer Created");
    }

    VertexBuffer::~VertexBuffer() {
        if(m_device != VK_NULL_HANDLE) {
            if(m_buffer != VK_NULL_HANDLE) {
                vkDestroyBuffer(m_device, m_buffer, nullptr);
            }

            if(m_bufferMemory != VK_NULL_HANDLE) {
                vkFreeMemory(m_device, m_bufferMemory, nullptr);
            }
        }

        UVKE_LOG("Vertex Buffer Destroyed");
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

    unsigned int VertexBuffer::GetSize() {
        return sizeof(Vertex) * m_vertices.size();
    }

    VkBuffer& VertexBuffer::GetBuffer() {
        return m_buffer;
    }
};