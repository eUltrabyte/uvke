#pragma once
#ifndef UVKE_RENDERER_HEADER
#define UVKE_RENDERER_HEADER

#include "../uvke.hpp"
#include "../Core/Base.hpp"
#include "Surface.hpp"
#include "StagingBuffer.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "UniformBuffer.hpp"

// TODO
// move vulkan swapchain to swapchain class
// move vulkan pipeline to pipeline class

namespace uvke {
    class UVKE_API Renderer {
    public:
        Renderer(Base& base, Window& window);
        virtual ~Renderer();

        virtual void Render();

    private:
        void SetSwapchainCapabilities() {
            m_swapchainPreTransform = m_surface->GetCapabilities().currentTransform;

            m_swapchainImageCount = m_surface->GetCapabilities().minImageCount + 1;
            if(m_surface->GetCapabilities().maxImageCount > 0 && m_swapchainImageCount > m_surface->GetCapabilities().maxImageCount) {
                m_swapchainImageCount = m_surface->GetCapabilities().maxImageCount;
            }
        }

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
            VkClearValue clearValue = {{{ 0.0f, 0.0f, 0.0f, 1.0f }}};
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

        void RecreateSwapchain() {
            m_window.Update();
            SetSwapchainCapabilities();

            vec2i size(m_window.GetWindowProps()->size.x, m_window.GetWindowProps()->size.y);
            for(; size.x == 0 || size.y == 0 ;) {
                m_window.Update();
                size = vec2i(m_window.GetWindowProps()->size.x, m_window.GetWindowProps()->size.y);
                m_surface->SetExtent(m_surface->GetSwapExtent(m_window));
                glfwWaitEvents();
            }

            if(size.x != 0 || size.y != 0) {
                m_surface->SetExtent(m_surface->GetSwapExtent(m_window));
            }

            vkDeviceWaitIdle(m_base.GetDevice());

            for(auto i = 0; i < m_framebuffers.size(); ++i) {
                vkDestroyFramebuffer(m_base.GetDevice(), m_framebuffers[i], nullptr);
            }

            for(auto i = 0; i < m_swapchainImageViews.size(); ++i) {
                vkDestroyImageView(m_base.GetDevice(), m_swapchainImageViews[i], nullptr);
            }

            {
                VkSwapchainCreateInfoKHR swapchainCreateInfo { };
                swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
                swapchainCreateInfo.pNext = nullptr;
                swapchainCreateInfo.flags = 0;
                swapchainCreateInfo.surface = m_surface->GetSurface();
                swapchainCreateInfo.minImageCount = m_swapchainImageCount;
                swapchainCreateInfo.imageFormat = m_surface->GetFormat().format;
                swapchainCreateInfo.imageColorSpace = m_surface->GetFormat().colorSpace;
                swapchainCreateInfo.imageExtent = m_surface->GetExtent();
                swapchainCreateInfo.imageArrayLayers = 1;
                swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

                if(m_surface->IsMultiQueueMode()) {
                    unsigned int indices[2] = { m_surface->GetQueueFamily(), m_surface->GetQueueFamily() + 1 };

                    swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
                    swapchainCreateInfo.queueFamilyIndexCount = 2;
                    swapchainCreateInfo.pQueueFamilyIndices = indices;
                } else {
                    swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
                    swapchainCreateInfo.queueFamilyIndexCount = 0;
                    swapchainCreateInfo.pQueueFamilyIndices = nullptr;
                }

                swapchainCreateInfo.preTransform = m_swapchainPreTransform;
                swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
                swapchainCreateInfo.presentMode = m_surface->GetPresentMode();
                swapchainCreateInfo.clipped = VK_TRUE;
                swapchainCreateInfo.oldSwapchain = m_swapchain;

                UVKE_ASSERT(vkCreateSwapchainKHR(m_base.GetDevice(), &swapchainCreateInfo, nullptr, &m_swapchain));
            }

            
            {
                unsigned int swapchainImageCount = 0;
                vkGetSwapchainImagesKHR(m_base.GetDevice(), m_swapchain, &swapchainImageCount, nullptr);
                m_swapchainImages = std::vector<VkImage>(swapchainImageCount);
                vkGetSwapchainImagesKHR(m_base.GetDevice(), m_swapchain, &swapchainImageCount, m_swapchainImages.data());
            }


            {
                m_swapchainImageViews = std::vector<VkImageView>(m_swapchainImages.size());

                for(auto i = 0; i < m_swapchainImageViews.size(); ++i) {
                    VkImageViewCreateInfo imageViewCreateInfo { };
                    imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
                    imageViewCreateInfo.pNext = nullptr;
                    imageViewCreateInfo.flags = 0;
                    imageViewCreateInfo.image = m_swapchainImages[i];
                    imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
                    imageViewCreateInfo.format = m_surface->GetFormat().format;
                    imageViewCreateInfo.components = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
                    imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                    imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
                    imageViewCreateInfo.subresourceRange.levelCount = 1;
                    imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
                    imageViewCreateInfo.subresourceRange.layerCount = 1;

                    UVKE_ASSERT(vkCreateImageView(m_base.GetDevice(), &imageViewCreateInfo, nullptr, &m_swapchainImageViews[i]));
                }
            }

            {
                m_framebuffers = std::vector<VkFramebuffer>(m_swapchainImageViews.size());

                for(auto i = 0; i < m_framebuffers.size(); ++i) {
                    VkFramebufferCreateInfo framebufferCreateInfo { };
                    framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
                    framebufferCreateInfo.pNext = nullptr;
                    framebufferCreateInfo.flags = 0;
                    framebufferCreateInfo.renderPass = m_renderPass;
                    framebufferCreateInfo.attachmentCount = 1;
                    framebufferCreateInfo.pAttachments = &m_swapchainImageViews[i];
                    framebufferCreateInfo.width = m_surface->GetExtent().width;
                    framebufferCreateInfo.height = m_surface->GetExtent().height;
                    framebufferCreateInfo.layers = 1;

                    UVKE_ASSERT(vkCreateFramebuffer(m_base.GetDevice(), &framebufferCreateInfo, nullptr, &m_framebuffers[i]));
                }
            }
        }

        Base& m_base;
        Window& m_window;
        Surface* m_surface;
        VkSurfaceTransformFlagBitsKHR m_swapchainPreTransform;
        unsigned int m_swapchainImageCount;
        VkSwapchainKHR m_swapchain;
        std::vector<VkImage> m_swapchainImages;
        std::vector<VkImageView> m_swapchainImageViews;
        StagingBuffer* m_stagingBuffer;
        VertexBuffer* m_vertexBuffer;
        IndexBuffer* m_indexBuffer;
        UniformBuffer* m_uniformBuffer;
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