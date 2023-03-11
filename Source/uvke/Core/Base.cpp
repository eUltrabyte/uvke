#include "Base.hpp"

namespace uvke {
    Base::Base(std::string_view name) {
        {
            VkApplicationInfo appInfo { };
            appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo.pNext = nullptr;
            appInfo.applicationVersion = VK_MAKE_API_VERSION(1, 1, 0, 0);
            appInfo.pApplicationName = name.data();
            appInfo.engineVersion = VK_MAKE_API_VERSION(1, 1, 0, 0);
            appInfo.pEngineName = "uvke";
            appInfo.apiVersion = uvke::GetSupportedVulkan();

            std::vector<const char*> layers = {
                #ifdef UVKE_DEBUG
                    "VK_LAYER_KHRONOS_validation"
                #endif
            };

            unsigned int glfwExtensionsCount = 0;
            const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);
            std::vector<const char*> extensions = std::vector<const char*>(glfwExtensions, glfwExtensions + glfwExtensionsCount);

            UVKE_LOG("Vulkan - " + std::to_string(VK_API_VERSION_MAJOR(appInfo.apiVersion)) + "." + std::to_string(VK_API_VERSION_MINOR(appInfo.apiVersion)) + "." + std::to_string(VK_API_VERSION_PATCH(appInfo.apiVersion)));
            UVKE_LOG("App - " + std::string(appInfo.pApplicationName));

            for(auto i = 0; i < layers.size(); ++i) {
                UVKE_LOG("Layer #" + std::to_string(i) + " - " + std::string(layers.at(i)));
            }

            for(auto i = 0; i < extensions.size(); ++i) {
                UVKE_LOG("Extension #" + std::to_string(i) + " - " + std::string(extensions.at(i)));
            }

            VkInstanceCreateInfo instanceCreateInfo { };
            instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            instanceCreateInfo.pNext = nullptr;
            instanceCreateInfo.flags = 0;
            instanceCreateInfo.pApplicationInfo = &appInfo;
            instanceCreateInfo.enabledLayerCount = layers.size();
            instanceCreateInfo.ppEnabledLayerNames = layers.data();
            instanceCreateInfo.enabledExtensionCount = extensions.size();
            instanceCreateInfo.ppEnabledExtensionNames = extensions.data();

            UVKE_ASSERT(vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance));
        }

        m_physicalDevice = GetSuitablePhysicalDevice();
        m_queueFamilyIndex = GetSuitableQueueFamily();

        {
            VkPhysicalDeviceFeatures physicalDeviceFeatures { };
            vkGetPhysicalDeviceFeatures(m_physicalDevice, &physicalDeviceFeatures);

            std::vector<float> deviceQueuePriorities(m_queueCount);
            for(auto i = 0; i < deviceQueuePriorities.size(); ++i) {
                deviceQueuePriorities[i] = 1.0f;
            }

            std::vector<const char*> deviceExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
            };

            VkDeviceQueueCreateInfo deviceQueueCreateInfo { };
            deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            deviceQueueCreateInfo.pNext = nullptr;
            deviceQueueCreateInfo.flags = 0;
            deviceQueueCreateInfo.queueFamilyIndex = m_queueFamilyIndex;
            deviceQueueCreateInfo.queueCount = deviceQueuePriorities.size();
            deviceQueueCreateInfo.pQueuePriorities = deviceQueuePriorities.data();

            VkDeviceCreateInfo deviceCreateInfo { };
            deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            deviceCreateInfo.pNext = nullptr;
            deviceCreateInfo.flags = 0;
            deviceCreateInfo.queueCreateInfoCount = 1;
            deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
            deviceCreateInfo.enabledLayerCount = 0;
            deviceCreateInfo.ppEnabledLayerNames = nullptr;
            deviceCreateInfo.enabledExtensionCount = deviceExtensions.size();
            deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
            deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;

            UVKE_ASSERT(vkCreateDevice(m_physicalDevice, &deviceCreateInfo, nullptr, &m_device));
        }

        UVKE_LOG("Base Created");
    }

    Base::~Base() {
        if(m_device != VK_NULL_HANDLE) {
            vkDestroyDevice(m_device, nullptr);
        }

        if(m_instance != VK_NULL_HANDLE) {
            vkDestroyInstance(m_instance, nullptr);
        }

        UVKE_LOG("Base Destroyed");
    }
    
    void Base::SetInstance(VkInstance instance) {
        m_instance = instance;
    }
    
    void Base::SetPhysicalDevice(VkPhysicalDevice physicalDevice) {
        m_physicalDevice = physicalDevice;
    }
    
    void Base::SetDevice(VkDevice device) {
        m_device = device;
    }

    VkInstance& Base::GetInstance() {
        return m_instance;
    }

    VkPhysicalDevice& Base::GetPhysicalDevice() {
        return m_physicalDevice;
    }

    VkDevice& Base::GetDevice() {
        return m_device;
    }

    unsigned int Base::GetQueueFamily() {
        return m_queueFamilyIndex;
    }

    unsigned int Base::GetQueueCount() {
        return m_queueCount;
    }
    
    bool Base::IsMultiQueueSupported() {
        return m_multiQueue;
    }
};