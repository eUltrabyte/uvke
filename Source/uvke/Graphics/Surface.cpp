#include "Surface.hpp"

namespace uvke {
    Surface::Surface(Base* base, Window* window)
        : m_base(base), m_extent({ 0, 0 }) {
        window->CreatePlatformSurface(m_base->GetInstance(), &m_surface);

        m_queueFamilyIndex = m_base->GetQueueFamily();
        m_multiQueue = m_base->IsMultiQueueSupported();

        UVKE_LOG("Surface Created");
    }

    Surface::~Surface() {
        if(m_base->GetInstance() != VK_NULL_HANDLE && m_surface != VK_NULL_HANDLE) {
            vkDestroySurfaceKHR(m_base->GetInstance(), m_surface, nullptr);
        }

        m_queues.clear();

        UVKE_LOG("Surface Destroyed");
    }

    void Surface::CheckQueues() {
        if(m_multiQueue) {
            m_queues.clear();
            m_queues.resize(2);
            vkGetDeviceQueue(m_base->GetDevice(), m_queueFamilyIndex, 0, &m_queues[0]);
            vkGetDeviceQueue(m_base->GetDevice(), m_queueFamilyIndex, 0, &m_queues[1]);
        } else {
            m_queues.clear();
            m_queues.resize(1);
            vkGetDeviceQueue(m_base->GetDevice(), m_queueFamilyIndex, 0, &m_queues[0]);
        }

        m_surfaceFormat = GetSuitableSurfaceFormat();
        m_presentMode = GetSuitablePresentMode();

        UVKE_LOG("Queue Family Index - " + std::to_string(m_queueFamilyIndex));

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(m_base->GetPhysicalDevice(), m_queueFamilyIndex, m_surface, &presentSupport);
        UVKE_LOG("Queue Present Support - " + std::string(presentSupport ? "True" : "False"));
    }

    void Surface::SetBase(Base* base) {
        m_base = base;
    }

    void Surface::SetQueueFamily(unsigned int queueFamilyIndex) {
        m_queueFamilyIndex = queueFamilyIndex;
    }

    void Surface::SetMultiQueueMode(bool multiQueue) {
        m_multiQueue = multiQueue;
    }
    
    void Surface::SetSurface(VkSurfaceKHR surface) {
        m_surface = surface;
    }

    void Surface::SetSurfaceFormat(VkSurfaceFormatKHR surfaceFormat) {
        m_surfaceFormat = surfaceFormat;
    }

    void Surface::SetPresentMode(VkPresentModeKHR presentMode) {
        m_presentMode = presentMode;
    }

    void Surface::SetExtent(VkExtent2D extent) {
        m_extent = extent;
    }

    void Surface::SetSwapExtent(Window* window) {
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_base->GetPhysicalDevice(), m_surface, &m_surfaceCapabilities);

        if(m_surfaceCapabilities.currentExtent.width != std::numeric_limits<unsigned int>::infinity() || m_surfaceCapabilities.currentExtent.height != std::numeric_limits<unsigned int>::infinity()) {
            m_extent = m_surfaceCapabilities.currentExtent;
        } else {
            window->Update();
            m_extent.width = std::clamp(static_cast<unsigned int>(window->GetWindowProps()->size.x), m_surfaceCapabilities.minImageExtent.width, m_surfaceCapabilities.maxImageExtent.width);
            m_extent.height = std::clamp(static_cast<unsigned int>(window->GetWindowProps()->size.y), m_surfaceCapabilities.minImageExtent.height, m_surfaceCapabilities.maxImageExtent.height);
        }
    }
    
    unsigned int Surface::GetQueueFamily() {
        return m_queueFamilyIndex;
    }
        
    bool Surface::IsMultiQueueMode() {
        return m_multiQueue;
    }

    std::vector<VkQueue>& Surface::GetQueues() {
        return m_queues;
    }

    VkQueue& Surface::GetQueue(unsigned int index) {
        if(index < 0 || index >= m_queues.size()) {
            return m_queues[0];
        } else {
            return m_queues[index];
        }
    }

    VkSurfaceKHR& Surface::GetSurface() {
        return m_surface;
    }

    VkSurfaceFormatKHR& Surface::GetFormat() {
        return m_surfaceFormat;
    }

    VkPresentModeKHR& Surface::GetPresentMode() {
        return m_presentMode;
    }

    VkExtent2D& Surface::GetExtent() {
        return m_extent;
    }

    VkSurfaceCapabilitiesKHR& Surface::GetCapabilities() {
        return m_surfaceCapabilities;
    }
};