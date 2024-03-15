#include "Base.hpp"

namespace uvke {
    Base::Base(std::string_view name) {
        {
            VkApplicationInfo appInfo { };
            appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo.pNext = nullptr;
            appInfo.applicationVersion = VK_MAKE_API_VERSION(1, 1, 0, 0);
            appInfo.pApplicationName = name.data();
            appInfo.engineVersion = VK_MAKE_API_VERSION(1, 1, 2, 0);
            appInfo.pEngineName = "uvke";
            appInfo.apiVersion = Core::GetVulkanVersion();

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

            #ifdef UVKE_DEBUG
                extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            #endif

            VkInstanceCreateInfo instanceCreateInfo { };
            instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

            #ifdef UVKE_DEBUG
                VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo { };
                debugUtilsMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
                debugUtilsMessengerCreateInfo.pNext = nullptr;
                debugUtilsMessengerCreateInfo.flags = 0;
                debugUtilsMessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
                debugUtilsMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
                debugUtilsMessengerCreateInfo.pfnUserCallback = Core::DebugUtilsMessengerCallback;
                debugUtilsMessengerCreateInfo.pUserData = nullptr;

                instanceCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugUtilsMessengerCreateInfo;
            #elif
                instanceCreateInfo.pNext = nullptr;
            #endif

            instanceCreateInfo.flags = 0;
            instanceCreateInfo.pApplicationInfo = &appInfo;
            instanceCreateInfo.enabledLayerCount = layers.size();
            instanceCreateInfo.ppEnabledLayerNames = layers.data();
            instanceCreateInfo.enabledExtensionCount = extensions.size();
            instanceCreateInfo.ppEnabledExtensionNames = extensions.data();


            UVKE_ASSERT(vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance));

            #ifdef UVKE_DEBUG
                 UVKE_ASSERT(Core::CreateDebugUtilsMessengerEXT(m_instance, &debugUtilsMessengerCreateInfo, nullptr, &m_debugUtilsMessenger));
            #endif
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

        FindDepthFormat(VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

        UVKE_LOG_ADDRESS("Base Created");
    }

    Base::~Base() {
        if(m_device != VK_NULL_HANDLE) {
            vkDeviceWaitIdle(m_device);

            vkDestroyDevice(m_device, nullptr);
        }

        #ifdef UVKE_DEBUG
            if(m_debugUtilsMessenger != VK_NULL_HANDLE) {
                Core::DestroyDebugUtilsMessengerEXT(m_instance, m_debugUtilsMessenger, nullptr);
            }
        #endif

        if(m_instance != VK_NULL_HANDLE) {
            vkDestroyInstance(m_instance, nullptr);
        }

        UVKE_LOG_ADDRESS("Base Destroyed");
    }

    void Base::FindDepthFormat(VkImageTiling tiling, VkFormatFeatureFlags features) {
        std::vector<VkFormat> depthFormats = {
            VK_FORMAT_D32_SFLOAT,
            VK_FORMAT_D32_SFLOAT_S8_UINT,
            VK_FORMAT_D24_UNORM_S8_UINT
        };

        for(auto i = 0; i < depthFormats.size(); ++i) {
            VkFormatProperties formatProperties;
            vkGetPhysicalDeviceFormatProperties(m_physicalDevice, depthFormats[i], &formatProperties);

            if(tiling == VK_IMAGE_TILING_LINEAR && (formatProperties.linearTilingFeatures & features) == features) {
                m_depthFormat = depthFormats[i];
                break;
            } else if(tiling == VK_IMAGE_TILING_OPTIMAL && (formatProperties.optimalTilingFeatures & features) == features) {
                m_depthFormat = depthFormats[i];
                break;
            }

            UVKE_FATAL("Supported Format Not Found!");
        }
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

    VkFormat Base::GetDepthFormat() {
        return m_depthFormat;
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

    bool Base::HasStencilComponent() {
        return m_depthFormat == VK_FORMAT_D32_SFLOAT_S8_UINT || m_depthFormat == VK_FORMAT_D24_UNORM_S8_UINT;
    }

    VkSampleCountFlagBits Base::GetSampleCount() {
        return m_sampleCount;
    }
};