#include "IndexBuffer.hpp"

namespace uvke {
    IndexBuffer::IndexBuffer(Base* base, std::vector<unsigned int> indices)
        : m_base(base), m_indices(indices) {
        if(m_indices.size()) {
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

            VkMemoryAllocateInfo memoryAllocateInfo { };
            memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            memoryAllocateInfo.pNext = nullptr;
            memoryAllocateInfo.allocationSize = Helper::GetRequirementsSize(m_base, m_buffer);
            memoryAllocateInfo.memoryTypeIndex = Helper::FindMemoryIndex(m_base, m_buffer);

            UVKE_ASSERT(vkAllocateMemory(m_base->GetDevice(), &memoryAllocateInfo, nullptr, &m_bufferMemory));

            vkBindBufferMemory(m_base->GetDevice(), m_buffer, m_bufferMemory, 0);
        } else {
            m_buffer = VK_NULL_HANDLE;
            m_bufferMemory = VK_NULL_HANDLE;
        }

        UVKE_LOG_ADDRESS("Index Buffer Created");
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

        UVKE_LOG_ADDRESS("Index Buffer Desroyed");
    }

    void IndexBuffer::Bind(VkCommandBuffer commandBuffer) {
        vkCmdBindIndexBuffer(commandBuffer, m_buffer, 0, VK_INDEX_TYPE_UINT32);
    }

    void IndexBuffer::SetBase(Base* base) {
        m_base = base;
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