#include "VertexBuffer.hpp"

namespace uvke {
    VertexBuffer::VertexBuffer(std::shared_ptr<Base> base, std::vector<Vertex> vertices)
        : m_base(base), m_vertices(vertices) {
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
            m_vertexInputAttributeDescription[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            m_vertexInputAttributeDescription[1].offset = offsetof(Vertex, Vertex::color);

            m_vertexInputAttributeDescription[2].binding = 0;
            m_vertexInputAttributeDescription[2].location = 2;
            m_vertexInputAttributeDescription[2].format = VK_FORMAT_R32G32_SFLOAT;
            m_vertexInputAttributeDescription[2].offset = offsetof(Vertex, Vertex::texCoord);

            VkBufferCreateInfo bufferCreateInfo = { };
            bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferCreateInfo.pNext = nullptr;
            bufferCreateInfo.flags = 0;
            bufferCreateInfo.size = sizeof(Vertex) * m_vertices.size();
            bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
            bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            bufferCreateInfo.queueFamilyIndexCount = 0;
            bufferCreateInfo.pQueueFamilyIndices = nullptr;

            UVKE_ASSERT(vkCreateBuffer(m_base->GetDevice(), &bufferCreateInfo, nullptr, &m_buffer));

            VkMemoryRequirements memoryRequirements { };
            vkGetBufferMemoryRequirements(m_base->GetDevice(), m_buffer, &memoryRequirements);

            VkPhysicalDeviceMemoryProperties memoryProperties { };
            vkGetPhysicalDeviceMemoryProperties(m_base->GetPhysicalDevice(), &memoryProperties);

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

            UVKE_ASSERT(vkAllocateMemory(m_base->GetDevice(), &memoryAllocateInfo, nullptr, &m_bufferMemory));
        }

        vkBindBufferMemory(m_base->GetDevice(), m_buffer, m_bufferMemory, 0);

        UVKE_LOG("Vertex Buffer Created");
    }

    VertexBuffer::~VertexBuffer() {
        if(m_base->GetDevice() != VK_NULL_HANDLE) {
            if(m_buffer != VK_NULL_HANDLE) {
                vkDestroyBuffer(m_base->GetDevice(), m_buffer, nullptr);
            }

            if(m_bufferMemory != VK_NULL_HANDLE) {
                vkFreeMemory(m_base->GetDevice(), m_bufferMemory, nullptr);
            }
        }

        UVKE_LOG("Vertex Buffer Destroyed");
    }

    void VertexBuffer::Bind(VkCommandBuffer commandBuffer) {
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, &m_buffer, offsets);
    }

    void VertexBuffer::SetBase(std::shared_ptr<Base> base) {
        m_base = base;
    }

    std::shared_ptr<Base> VertexBuffer::GetBase() {
        return m_base;
    }

    std::vector<Vertex>& VertexBuffer::GetVertices() {
        return m_vertices;
    }

    VkVertexInputBindingDescription& VertexBuffer::GetVertexInputBindingDescription() {
        return m_vertexInputBindingDescription;
    }
    
    std::array<VkVertexInputAttributeDescription, 3>& VertexBuffer::GetVertexInputAttributeDescription() {
        return m_vertexInputAttributeDescription;
    }

    unsigned int VertexBuffer::GetSize() {
        return sizeof(Vertex) * m_vertices.size();
    }

    VkBuffer& VertexBuffer::GetBuffer() {
        return m_buffer;
    }

    VkDeviceMemory& VertexBuffer::GetBufferMemory() {
        return m_bufferMemory;
    }
};