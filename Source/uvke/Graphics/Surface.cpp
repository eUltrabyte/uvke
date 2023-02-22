#include "Surface.hpp"

namespace uvke {
    Surface::Surface(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device, Window& window)
        : m_instance(instance), m_physicalDevice(physicalDevice), m_device(device), m_extent({ 0, 0 }) {
        window.CreatePlatformSurface(m_instance, &m_surface);

        UVKE_LOG("Surface Created Successfully");
    }

    Surface::~Surface() {
        if(m_instance != VK_NULL_HANDLE && m_surface != VK_NULL_HANDLE) {
            vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
        }

        UVKE_LOG("Surface Destroyed");
    }

    void Surface::CheckQueues() {
        if(m_multiQueue) {
            m_queues.clear();
            m_queues.resize(2);
            vkGetDeviceQueue(m_device, m_queueFamilyIndex, 0, &m_queues[0]);
            vkGetDeviceQueue(m_device, m_queueFamilyIndex, 0, &m_queues[1]);
        } else {
            m_queues.clear();
            m_queues.resize(1);
            vkGetDeviceQueue(m_device, m_queueFamilyIndex, 0, &m_queues[0]);
        }

        m_surfaceFormat = GetSuitableSurfaceFormat();
        m_presentMode = GetSuitablePresentMode();
    }

    void Surface::SetInstance(VkInstance instance) {
        m_instance = instance;
    }

    void Surface::SetPhysicalDevice(VkPhysicalDevice physicalDevice) {
        m_physicalDevice = physicalDevice;
    }

    void Surface::SetDevice(VkDevice device) {
        m_device = device;
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

    void Surface::SetSwapExtent(Window& window) {
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice, m_surface, &m_surfaceCapabilities);

        if(m_surfaceCapabilities.currentExtent.width != std::numeric_limits<unsigned int>::infinity() || m_surfaceCapabilities.currentExtent.height != std::numeric_limits<unsigned int>::infinity()) {
            m_extent = m_surfaceCapabilities.currentExtent;
        } else {
            window.Update();
            m_extent.width = std::clamp(static_cast<unsigned int>(window.GetWindowProps()->size.x), m_surfaceCapabilities.minImageExtent.width, m_surfaceCapabilities.maxImageExtent.width);
            m_extent.height = std::clamp(static_cast<unsigned int>(window.GetWindowProps()->size.y), m_surfaceCapabilities.minImageExtent.height, m_surfaceCapabilities.maxImageExtent.height);
        }
    }

    VkInstance& Surface::GetInstance() {
        return m_instance;
    }

    VkPhysicalDevice& Surface::GetPhysicalDevice() {
        return m_physicalDevice;
    }

    VkDevice& Surface::GetDevice() {
        return m_device;
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