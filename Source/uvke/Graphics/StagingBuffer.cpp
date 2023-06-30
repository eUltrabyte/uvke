#include "StagingBuffer.hpp"

namespace uvke {
    StagingBuffer::StagingBuffer(Base* base, unsigned int size)
        : m_base(base), m_size(size) {
        {
            VkBufferCreateInfo bufferCreateInfo = { };
            bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferCreateInfo.pNext = nullptr;
            bufferCreateInfo.flags = 0;
            bufferCreateInfo.size = m_size;
            bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
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

        UVKE_LOG("Staging Buffer Created");
    }

    StagingBuffer::~StagingBuffer() {
        if(m_base->GetDevice() != VK_NULL_HANDLE) {
            if(m_buffer != VK_NULL_HANDLE) {
                vkDestroyBuffer(m_base->GetDevice(), m_buffer, nullptr);
            }

            if(m_bufferMemory != VK_NULL_HANDLE) {
                vkFreeMemory(m_base->GetDevice(), m_bufferMemory, nullptr);
            }
        }

        UVKE_LOG("Staging Buffer Destroyed");
    }

    void StagingBuffer::Map(void* data) {
        void* rawData;
        vkMapMemory(m_base->GetDevice(), m_bufferMemory, 0, m_size, 0, &rawData);
        std::memcpy(rawData, data, m_size);
        vkUnmapMemory(m_base->GetDevice(), m_bufferMemory);
    }

    void StagingBuffer::Copy(VkCommandPool commandPool, VkQueue queue, VkBuffer destination, VkDeviceSize size) {
        VkCommandBufferAllocateInfo commandBufferAllocateInfo { };
        commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        commandBufferAllocateInfo.pNext = nullptr;
        commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        commandBufferAllocateInfo.commandPool = commandPool;
        commandBufferAllocateInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        UVKE_ASSERT(vkAllocateCommandBuffers(m_base->GetDevice(), &commandBufferAllocateInfo, &commandBuffer));

        VkCommandBufferBeginInfo commandBufferBeginInfo { };
        commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        commandBufferBeginInfo.pNext = nullptr;
        commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        commandBufferBeginInfo.pInheritanceInfo = nullptr;

        vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);

        VkBufferCopy region { };
        region.srcOffset = 0;
        region.dstOffset = 0;
        region.size = m_size;

        vkCmdCopyBuffer(commandBuffer, m_buffer, destination, 1, &region);

        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo { };
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.pNext = nullptr;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(queue);

        vkFreeCommandBuffers(m_base->GetDevice(), commandPool, 1, &commandBuffer);
    }

    void StagingBuffer::SetBase(Base* base) {
        m_base = base;
    }
    
    VkBuffer& StagingBuffer::GetBuffer() {
        return m_buffer;
    }
    
    VkDeviceMemory& StagingBuffer::GetBufferMemory() {
        return m_bufferMemory;
    }
};