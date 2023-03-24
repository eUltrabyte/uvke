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

    void CommandBuffer::Record(unsigned int frame, unsigned int index, std::shared_ptr<Surface> surface, std::shared_ptr<Pipeline> pipeline, std::shared_ptr<Framebuffer> framebuffer, std::vector<std::shared_ptr<VertexBuffer>> vertexBuffers, std::vector<std::shared_ptr<IndexBuffer>> indexBuffers, std::vector<std::shared_ptr<UniformBuffer>> uniformBuffers) {
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

        auto objects = (vertexBuffers.size() + indexBuffers.size() + uniformBuffers.size()) / 3;
        for(auto i = 0; i < vertexBuffers.size(); ++i) {
            vertexBuffers[i]->Bind(m_commandBuffers[frame]);
            indexBuffers[i]->Bind(m_commandBuffers[frame]);
            uniformBuffers[i]->Bind(m_commandBuffers[frame], pipeline->GetPipelineLayout(), frame);

            vkCmdDrawIndexed(m_commandBuffers[frame], indexBuffers[i]->GetIndices().size(), 1, 0, 0, 0);
        }

        /* ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        ImGui::Render();

        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), m_commandBuffers[frame]); */

        vkCmdEndRenderPass(m_commandBuffers[frame]);

        UVKE_ASSERT(vkEndCommandBuffer(m_commandBuffers[frame]));
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