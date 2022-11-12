#pragma once
#ifndef UVKE_RENDERER_HEADER
#define UVKE_RENDERER_HEADER

#include "../uvke.hpp"
#include "Shader.hpp"

namespace uvke {
    class UVKE_API Renderer {
    public:
        Renderer(Window& window);
        virtual ~Renderer();

        virtual void Render();
    
    private:
        VkPhysicalDevice GetSuitablePhysicalDevice() {
            std::vector<VkPhysicalDevice> physicalDevices;
            {
                unsigned int physicalDevicesCount = 0;
                vkEnumeratePhysicalDevices(m_instance, &physicalDevicesCount, nullptr);
                physicalDevices = std::vector<VkPhysicalDevice>(physicalDevicesCount);
                vkEnumeratePhysicalDevices(m_instance, &physicalDevicesCount, physicalDevices.data());
            }

            std::vector<VkPhysicalDeviceProperties> physicalDevicesProperties(physicalDevices.size());
            std::vector<VkPhysicalDeviceFeatures> physicalDevicesFeatures(physicalDevices.size());

            for(auto i = 0; i < physicalDevices.size(); ++i) {
                vkGetPhysicalDeviceProperties(physicalDevices[i], &physicalDevicesProperties[i]);
                vkGetPhysicalDeviceFeatures(physicalDevices[i], &physicalDevicesFeatures[i]);

                if(physicalDevicesProperties.at(i).deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && physicalDevicesFeatures.at(i).geometryShader == VK_TRUE) {
                    UVKE_LOG("GPU - " + std::string(physicalDevicesProperties.at(i).deviceName));
                    return physicalDevices[i];
                }
            }

            return physicalDevices[0];
        }

        unsigned int GetQueueFamily() {
            std::vector<VkQueueFamilyProperties> queueFamilyProperties;
            {
                unsigned int queueFamilyPropertiesCount = 0;
                vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyPropertiesCount, nullptr);
                queueFamilyProperties = std::vector<VkQueueFamilyProperties>(queueFamilyPropertiesCount);
                vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyPropertiesCount, queueFamilyProperties.data());
            }

            for(auto i = 0; i < queueFamilyProperties.size(); ++i) {
                if(queueFamilyProperties.at(i).queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                    return i;
                }
            }

            return 0;
        }

        VkSurfaceFormatKHR GetSurfaceFormat() {
            std::vector<VkSurfaceFormatKHR> surfaceFormats;
            {
                unsigned int surfaceFormatsCount = 0;
                vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &surfaceFormatsCount, nullptr);
                surfaceFormats = std::vector<VkSurfaceFormatKHR>(surfaceFormatsCount);
                vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &surfaceFormatsCount, surfaceFormats.data());
            }

            for(auto i = 0; i < surfaceFormats.size(); ++i) {
                if(surfaceFormats.at(i).format == VK_FORMAT_B8G8R8A8_SRGB && surfaceFormats.at(i).colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                    return surfaceFormats.at(i);
                }
            }

            return surfaceFormats.at(0);
        }

        VkPresentModeKHR GetPresentMode() {
            std::vector<VkPresentModeKHR> presentModes;
            {
                unsigned int presentModesCount = 0;
                vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_surface, &presentModesCount, nullptr);
                presentModes = std::vector<VkPresentModeKHR>(presentModesCount);
                vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_surface, &presentModesCount, presentModes.data());
            }

            for(auto i = 0; i < presentModes.size(); ++i) {
                if(presentModes.at(i) == VK_PRESENT_MODE_MAILBOX_KHR) {
                    return presentModes.at(i);
                }
            }

            return VK_PRESENT_MODE_FIFO_KHR;
        }

        VkExtent2D GetSwapExtent(Window& window) {
            VkSurfaceCapabilitiesKHR surfaceCapabilities;
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice, m_surface, &surfaceCapabilities);

            m_swapchainPreTransform = surfaceCapabilities.currentTransform;

            m_swapchainImageCount = surfaceCapabilities.minImageCount + 1;
            if(surfaceCapabilities.maxImageCount > 0 && m_swapchainImageCount > surfaceCapabilities.maxImageCount) {
                m_swapchainImageCount = surfaceCapabilities.maxImageCount;
            }
            
            if(surfaceCapabilities.currentExtent.width != std::numeric_limits<unsigned int>::infinity()) {
                return surfaceCapabilities.currentExtent;
            } else {
                vec2i size(0, 0);
                glfwGetFramebufferSize(window.GetWindow(), &size.x, &size.y);

                VkExtent2D fixedExtent = { static_cast<unsigned int>(size.x), static_cast<unsigned int>(size.y) };
                fixedExtent.width = std::clamp(fixedExtent.width, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width);
                fixedExtent.height = std::clamp(fixedExtent.height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height);

                return fixedExtent;
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
            renderPassBeginInfo.renderArea.extent = m_extent;
            VkClearValue clearValue = {{{ 0.0f, 0.0f, 0.0f, 1.0f }}};
            renderPassBeginInfo.clearValueCount = 1;
            renderPassBeginInfo.pClearValues = &clearValue;

            vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);

            VkViewport viewport { };
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = m_extent.width;
            viewport.height = m_extent.height;
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;

            vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

            VkRect2D scissor { };
            scissor.offset = { 0, 0 };
            scissor.extent = m_extent;

            vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

            vkCmdDraw(commandBuffer, 3, 1, 0, 0);

            vkCmdEndRenderPass(commandBuffer);

            UVKE_ASSERT(vkEndCommandBuffer(commandBuffer));
        }

        VkInstance m_instance;
        VkPhysicalDevice m_physicalDevice;
        VkDevice m_device;
        VkQueue m_queue;
        VkSurfaceKHR m_surface;
        VkSurfaceFormatKHR m_surfaceFormat;
        VkPresentModeKHR m_presentMode;
        VkExtent2D m_extent;
        VkSurfaceTransformFlagBitsKHR m_swapchainPreTransform;
        unsigned int m_swapchainImageCount;
        VkSwapchainKHR m_swapchain;
        std::vector<VkImage> m_swapchainImages;
        std::vector<VkImageView> m_swapchainImageViews;
        VkRenderPass m_renderPass;
        VkPipelineLayout m_pipelineLayout;
        VkPipeline m_pipeline;
        std::vector<VkFramebuffer> m_framebuffers;
        VkCommandPool m_commandPool;
        VkCommandBuffer m_commandBuffer;
        VkSemaphore m_availableSemaphore;
        VkSemaphore m_finishedSemaphore;
        VkFence m_fence;

    };
};

#endif