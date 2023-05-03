#include "IndexBuffer.hpp"

namespace uvke {
    IndexBuffer::IndexBuffer(std::shared_ptr<Base> base, std::vector<unsigned int> indices)
        : m_base(base), m_indices(indices) {
        VkBufferCreateInfo bufferCreateInfo = { };
        bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferCreateInfo.pNext = nullptr;
        bufferCreateInfo.flags = 0;
        bufferCreateInfo.size = sizeof(unsigned int) * m_indices.size();
        bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
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

        vkBindBufferMemory(m_base->GetDevice(), m_buffer, m_bufferMemory, 0);

        UVKE_LOG("Index Buffer Created");
    }

    IndexBuffer::~IndexBuffer() {
        if(m_base->GetDevice() != VK_NULL_HANDLE) {
            if(m_buffer != VK_NULL_HANDLE) {
                vkDestroyBuffer(m_base->GetDevice(), m_buffer, nullptr);
            }

            if(m_bufferMemory != VK_NULL_HANDLE) {
                vkFreeMemory(m_base->GetDevice(), m_bufferMemory, nullptr);
            }
        }

        UVKE_LOG("Index Buffer Desroyed");
    }

    void IndexBuffer::Bind(VkCommandBuffer commandBuffer) {
        vkCmdBindIndexBuffer(commandBuffer, m_buffer, 0, VK_INDEX_TYPE_UINT32);
    }

    void IndexBuffer::SetBase(std::shared_ptr<Base> base) {
        m_base = base;
    }

    std::shared_ptr<Base> IndexBuffer::GetBase() {
        return m_base;
    }

    std::vector<unsigned int>& IndexBuffer::GetIndices() {
        return m_indices;
    }

    unsigned int IndexBuffer::GetSize() {
        return sizeof(unsigned int) * m_indices.size();
    }

    VkBuffer& IndexBuffer::GetBuffer() {
        return m_buffer;
    }

    VkDeviceMemory& IndexBuffer::GetBufferMemory() {
        return m_bufferMemory;
    }
};