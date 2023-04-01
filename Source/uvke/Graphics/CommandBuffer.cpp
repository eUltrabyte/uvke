#include "CommandBuffer.hpp"

namespace uvke {
    CommandBuffer::CommandBuffer(VkDevice device, unsigned int queueFamilyIndex)
        : m_device(device) {
        {
            m_commandBuffers.resize(2);

            VkCommandPoolCreateInfo commandPoolCreateInfo { };
            commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            commandPoolCreateInfo.pNext = nullptr;
            commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
            commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndex;

            UVKE_ASSERT(vkCreateCommandPool(m_device, &commandPoolCreateInfo, nullptr, &m_commandPool));

            VkCommandBufferAllocateInfo commandBufferAllocateInfo { };
            commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            commandBufferAllocateInfo.pNext = nullptr;
            commandBufferAllocateInfo.commandPool = m_commandPool;
            commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            commandBufferAllocateInfo.commandBufferCount = m_commandBuffers.size();

            UVKE_ASSERT(vkAllocateCommandBuffers(m_device, &commandBufferAllocateInfo, m_commandBuffers.data()));
        }

        UVKE_LOG("Command Buffer Created");
    }
    
    CommandBuffer::~CommandBuffer() {
        if(m_device != VK_NULL_HANDLE) {
            if(m_commandPool != VK_NULL_HANDLE) {
                if(m_commandBuffers.data() != nullptr) {
                    vkFreeCommandBuffers(m_device, m_commandPool, m_commandBuffers.size(), m_commandBuffers.data());
                }

                vkDestroyCommandPool(m_device, m_commandPool, nullptr);
            }
        }

        UVKE_LOG("Command Buffer Destroyed");
    }

    VkCommandBuffer CommandBuffer::Begin() {
        VkCommandBufferAllocateInfo commandBufferAllocateInfo { };
        commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        commandBufferAllocateInfo.pNext = nullptr;
        commandBufferAllocateInfo.commandPool = m_commandPool;
        commandBufferAllocateInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        UVKE_ASSERT(vkAllocateCommandBuffers(m_device, &commandBufferAllocateInfo, &commandBuffer));

        VkCommandBufferBeginInfo commandBufferBeginInfo { };
        commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        commandBufferBeginInfo.pNext = nullptr;
        commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        commandBufferBeginInfo.pInheritanceInfo = nullptr;

        UVKE_ASSERT(vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo));

        return commandBuffer;
    }
    
    void CommandBuffer::End(VkCommandBuffer commandBuffer, VkQueue queue) {
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo { };
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        UVKE_ASSERT(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));
        vkQueueWaitIdle(queue);

        vkFreeCommandBuffers(m_device, m_commandPool, 1, &commandBuffer);
    }
    
    void CommandBuffer::SetDevice(VkDevice device) {
        m_device = device;
    }
    
    void CommandBuffer::SetCommandPool(VkCommandPool commandPool) {
        m_commandPool = commandPool;
    }
    
    void CommandBuffer::SetCommandBuffers(std::vector<VkCommandBuffer> commandBuffers) {
        m_commandBuffers = commandBuffers;
    }
    
    VkDevice& CommandBuffer::GetDevice() {
        return m_device;
    }
    
    VkCommandPool& CommandBuffer::GetCommandPool() {
        return m_commandPool;
    }
    
    std::vector<VkCommandBuffer>& CommandBuffer::GetCommandBuffers() {
        return m_commandBuffers;
    }

    VkCommandBuffer& CommandBuffer::GetCommandBuffer(unsigned int index) {
        if(index < 0 || index >= m_commandBuffers.size()) {
            return m_commandBuffers[0];
        } else {
            return m_commandBuffers[index];
        }
    }
};