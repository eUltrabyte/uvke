#pragma once
#ifndef UVKE_SURFACE_HEADER
#define UVKE_SURFACE_HEADER

#include "../uvke.hpp"
#include "../Core/Base.hpp"
#include "../Core/Window.hpp"

namespace uvke {
    class UVKE_API Surface {
    public:
        Surface(std::shared_ptr<Base> base = nullptr, std::shared_ptr<Window> window = nullptr);
        virtual ~Surface();

        virtual void CheckQueues();

        virtual void SetBase(std::shared_ptr<Base> base);
        virtual void SetQueueFamily(unsigned int queueFamilyIndex);
        virtual void SetMultiQueueMode(bool multiQueue);
        virtual void SetSurface(VkSurfaceKHR surface);
        virtual void SetSurfaceFormat(VkSurfaceFormatKHR surfaceFormat);
        virtual void SetPresentMode(VkPresentModeKHR presentMode);
        virtual void SetExtent(VkExtent2D extent);
        virtual void SetSwapExtent(std::shared_ptr<Window> window);

        virtual std::shared_ptr<Base> GetBase();
        virtual unsigned int GetQueueFamily();
        virtual bool IsMultiQueueMode();
        virtual std::vector<VkQueue>& GetQueues();
        virtual VkQueue& GetQueue(unsigned int index);
        virtual VkSurfaceKHR& GetSurface();
        virtual VkSurfaceFormatKHR& GetFormat();
        virtual VkPresentModeKHR& GetPresentMode();
        virtual VkExtent2D& GetExtent();
        virtual VkSurfaceCapabilitiesKHR& GetCapabilities();
        
    protected:
        std::shared_ptr<Base> m_base;

    private:
        VkSurfaceFormatKHR GetSuitableSurfaceFormat() {
            std::vector<VkSurfaceFormatKHR> surfaceFormats;
            {
                unsigned int surfaceFormatsCount = 0;
                vkGetPhysicalDeviceSurfaceFormatsKHR(m_base->GetPhysicalDevice(), m_surface, &surfaceFormatsCount, nullptr);
                surfaceFormats = std::vector<VkSurfaceFormatKHR>(surfaceFormatsCount);
                vkGetPhysicalDeviceSurfaceFormatsKHR(m_base->GetPhysicalDevice(), m_surface, &surfaceFormatsCount, surfaceFormats.data());
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
                vkGetPhysicalDeviceSurfacePresentModesKHR(m_base->GetPhysicalDevice(), m_surface, &presentModesCount, nullptr);
                presentModes = std::vector<VkPresentModeKHR>(presentModesCount);
                vkGetPhysicalDeviceSurfacePresentModesKHR(m_base->GetPhysicalDevice(), m_surface, &presentModesCount, presentModes.data());
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