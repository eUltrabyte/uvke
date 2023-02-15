#pragma once
#ifndef UVKE_SURFACE_HEADER
#define UVKE_SURFACE_HEADER

#include "../uvke.hpp"
#include "../Core/Window.hpp"

namespace uvke {
    class UVKE_API Surface {
    public:
        Surface(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device);
        virtual ~Surface();

        virtual void CheckQueues();

        virtual void SetInstance(VkInstance instance);
        virtual void SetPhysicalDevice(VkPhysicalDevice physicalDevice);
        virtual void SetDevice(VkDevice device);
        virtual void SetQueueFamily(unsigned int queueFamilyIndex);
        virtual void SetMultiQueueMode(bool multiQueue);
        virtual void SetSurface(VkSurfaceKHR surface);
        virtual void SetSurfaceFormat(VkSurfaceFormatKHR surfaceFormat);
        virtual void SetPresentMode(VkPresentModeKHR presentMode);
        virtual void SetExtent(VkExtent2D extent);

        virtual VkInstance& GetInstance();
        virtual VkPhysicalDevice& GetPhysicalDevice();
        virtual VkDevice& GetDevice();
        virtual unsigned int GetQueueFamily();
        virtual bool IsMultiQueueMode();
        virtual std::vector<VkQueue>& GetQueues();
        virtual VkSurfaceKHR& GetSurface();
        virtual VkSurfaceFormatKHR& GetFormat();
        virtual VkPresentModeKHR& GetPresentMode();
        virtual VkExtent2D& GetExtent();
        virtual VkSurfaceCapabilitiesKHR& GetCapabilities();
        
        virtual VkExtent2D GetSwapExtent(Window& window) {
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice, m_surface, &m_surfaceCapabilities);

            if(m_surfaceCapabilities.currentExtent.width != std::numeric_limits<unsigned int>::infinity() || m_surfaceCapabilities.currentExtent.height != std::numeric_limits<unsigned int>::infinity()) {
                return m_surfaceCapabilities.currentExtent;
            } else {
                window.Update();
                VkExtent2D fixedExtent = { static_cast<unsigned int>(window.GetWindowProps()->size.x), static_cast<unsigned int>(window.GetWindowProps()->size.y) };
                fixedExtent.width = std::clamp(fixedExtent.width, m_surfaceCapabilities.minImageExtent.width, m_surfaceCapabilities.maxImageExtent.width);
                fixedExtent.height = std::clamp(fixedExtent.height, m_surfaceCapabilities.minImageExtent.height, m_surfaceCapabilities.maxImageExtent.height);
                return fixedExtent;
            }
        }

    protected:
        VkInstance m_instance;
        VkPhysicalDevice m_physicalDevice;
        VkDevice m_device;

    private:
        VkSurfaceFormatKHR GetSuitableSurfaceFormat() {
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

        VkPresentModeKHR GetSuitablePresentMode() {
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

        unsigned int m_queueFamilyIndex;
        bool m_multiQueue;
        std::vector<VkQueue> m_queues;
        VkSurfaceKHR m_surface;
        VkSurfaceFormatKHR m_surfaceFormat;
        VkPresentModeKHR m_presentMode;
        VkExtent2D m_extent;
        VkSurfaceCapabilitiesKHR m_surfaceCapabilities;

    };
};

#endif