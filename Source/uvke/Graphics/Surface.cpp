#include "Surface.hpp"

namespace uvke {
    Surface::Surface(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device)
        : m_instance(instance), m_physicalDevice(physicalDevice), m_device(device) {
    }

    Surface::~Surface() {
        vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
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

    VkSurfaceKHR& Surface::GetSurface() {
        return m_surface;
    }

    VkSurfaceFormatKHR& Surface::GetSurfaceFormat() {
        return m_surfaceFormat;
    }

    VkPresentModeKHR& Surface::GetPresentMode() {
        return m_presentMode;
    }

    VkExtent2D& Surface::GetExtent() {
        return m_extent;
    }
};