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

    void CommandBuffer::Record(unsigned int frame, unsigned int index, std::shared_ptr<Surface> surface, std::shared_ptr<Pipeline> pipeline, std::shared_ptr<Framebuffer> framebuffer, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer, std::shared_ptr<UniformBuffer> uniformBuffer) {
        vkResetCommandBuffer(m_commandBuffers[frame], 0);

        VkCommandBufferBeginInfo commandBufferBeginInfo { };
        commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        commandBufferBeginInfo.pNext = nullptr;
        commandBufferBeginInfo.flags = 0;
        commandBufferBeginInfo.pInheritanceInfo = nullptr;

        UVKE_ASSERT(vkBeginCommandBuffer(m_commandBuffers[frame], &commandBufferBeginInfo));

        VkRenderPassBeginInfo renderPassBeginInfo { };
        renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBeginInfo.pNext = nullptr;
        renderPassBeginInfo.renderPass = pipeline->GetRenderPass();
        renderPassBeginInfo.framebuffer = framebuffer->GetFramebuffer(index);
        renderPassBeginInfo.renderArea.offset = { 0, 0 };
        renderPassBeginInfo.renderArea.extent = surface->GetExtent();
        VkClearValue clearValue = { { { 0.0f, 0.0f, 0.0f, 1.0f } } };
        renderPassBeginInfo.clearValueCount = 1;
        renderPassBeginInfo.pClearValues = &clearValue;

        vkCmdBeginRenderPass(m_commandBuffers[frame], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(m_commandBuffers[frame], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->GetPipeline());

        VkViewport viewport { };
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = surface->GetExtent().width;
        viewport.height = surface->GetExtent().height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        vkCmdSetViewport(m_commandBuffers[frame], 0, 1, &viewport);

        VkRect2D scissor { };
        scissor.offset = { 0, 0 };
        scissor.extent = surface->GetExtent();

        vkCmdSetScissor(m_commandBuffers[frame], 0, 1, &scissor);

        vertexBuffer->Bind(m_commandBuffers[frame]);
        indexBuffer->Bind(m_commandBuffers[frame]);
        uniformBuffer->Bind(m_commandBuffers[frame], pipeline->GetPipelineLayout());

        vkCmdDrawIndexed(m_commandBuffers[frame], indexBuffer->GetIndices().size(), 1, 0, 0, 0);

        vkCmdEndRenderPass(m_commandBuffers[frame]);

        UVKE_ASSERT(vkEndCommandBuffer(m_commandBuffers[frame]));
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