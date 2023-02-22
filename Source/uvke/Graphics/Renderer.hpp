#pragma once
#ifndef UVKE_RENDERER_HEADER
#define UVKE_RENDERER_HEADER

#include "../uvke.hpp"
#include "../Core/Base.hpp"
#include "Surface.hpp"
#include "Swapchain.hpp"
#include "StagingBuffer.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "UniformBuffer.hpp"

namespace uvke {
    class UVKE_API Renderer {
    public:
        Renderer(std::shared_ptr<Base> base, std::shared_ptr<Window> window);
        virtual ~Renderer();

        virtual void Render();

    private:
        void RecordCommandBuffer(VkCommandBuffer commandBuffer, unsigned int index) {
            VkCommandBufferBeginInfo commandBufferBeginInfo { };
            commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            commandBufferBeginInfo.pNext = nullptr;
            commandBufferBeginInfo.flags = 0;
            commandBufferBeginInfo.pInheritanceInfo = nullptr;

            UVKE_ASSERT(vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo));

            VkRenderPassBeginInfo renderPassBeginInfo { };
            renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassBeginInfo.pNext = nullptr;
            renderPassBeginInfo.renderPass = m_renderPass;
            renderPassBeginInfo.framebuffer = m_framebuffers[index];
            renderPassBeginInfo.renderArea.offset = { 0, 0 };
            renderPassBeginInfo.renderArea.extent = m_surface->GetExtent();
            VkClearValue clearValue = { { { 0.0f, 0.0f, 0.0f, 1.0f } } };
            renderPassBeginInfo.clearValueCount = 1;
            renderPassBeginInfo.pClearValues = &clearValue;

            vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);

            VkViewport viewport { };
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = m_surface->GetExtent().width;
            viewport.height = m_surface->GetExtent().height;
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;

            vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

            VkRect2D scissor { };
            scissor.offset = { 0, 0 };
            scissor.extent = m_surface->GetExtent();

            vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

            m_vertexBuffer->Bind(commandBuffer);
            m_indexBuffer->Bind(commandBuffer);
            m_uniformBuffer->Bind(commandBuffer, m_pipelineLayout);

            vkCmdDrawIndexed(commandBuffer, m_indexBuffer->GetIndices().size(), 1, 0, 0, 0);

            vkCmdEndRenderPass(commandBuffer);

            UVKE_ASSERT(vkEndCommandBuffer(commandBuffer));
        }

        std::shared_ptr<Base> m_base;
        std::shared_ptr<Window> m_window;
        std::shared_ptr<Surface> m_surface;
        std::shared_ptr<Swapchain> m_swapchain;
        std::shared_ptr<StagingBuffer> m_stagingBuffer;
        std::shared_ptr<VertexBuffer> m_vertexBuffer;
        std::shared_ptr<IndexBuffer> m_indexBuffer;
        std::shared_ptr<UniformBuffer> m_uniformBuffer;
        VkRenderPass m_renderPass;
        VkPipelineLayout m_pipelineLayout;
        VkPipeline m_pipeline;
        std::vector<VkFramebuffer> m_framebuffers;
        VkCommandPool m_commandPool;
        VkCommandBuffer m_commandBuffer;
        VkSemaphore m_availableSemaphore;
        VkSemaphore m_finishedSemaphore;
        VkFence m_fence;
        bool m_framebufferRecreated;
        Clock m_clock;

    };
};

#endif