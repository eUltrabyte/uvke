#include "Renderer.hpp"

namespace uvke {
    Renderer::Renderer(Window& window) {
        {
            VkApplicationInfo appInfo { };
            appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo.pNext = nullptr;
            appInfo.applicationVersion = VK_MAKE_API_VERSION(1, 1, 0, 0);
            appInfo.pApplicationName = window.GetWindowProps()->title.c_str();
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

        {
            VkPhysicalDeviceFeatures physicalDeviceFeatures { };
            vkGetPhysicalDeviceFeatures(m_physicalDevice, &physicalDeviceFeatures);

            float deviceQueuePriorities[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

            std::vector<const char*> deviceExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
            };

            VkDeviceQueueCreateInfo deviceQueueCreateInfo { };
            deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            deviceQueueCreateInfo.pNext = nullptr;
            deviceQueueCreateInfo.flags = 0;
            deviceQueueCreateInfo.queueFamilyIndex = 0;
            deviceQueueCreateInfo.queueCount = 1;
            deviceQueueCreateInfo.pQueuePriorities = deviceQueuePriorities;

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

        window.CreateSurface(m_instance, &m_surface);

        {
            unsigned int queueFamilyIndex = GetQueueFamily();
            vkGetDeviceQueue(m_device, queueFamilyIndex, 0, &m_queue);

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, queueFamilyIndex, m_surface, &presentSupport);
            UVKE_LOG("Queue Present Support - " + std::string(presentSupport ? "true" : "false"));
        }

        m_surfaceFormat = GetSurfaceFormat();
        m_presentMode = GetPresentMode();
        m_extent = GetSwapExtent(window);

        UVKE_LOG("Format - " + std::to_string(m_surfaceFormat.format));
        UVKE_LOG("Present Mode - " + std::to_string(m_presentMode));
        UVKE_LOG("Extent - " + std::to_string(m_extent.width) + "/" + std::to_string(m_extent.height));

        {
            VkSwapchainCreateInfoKHR swapchainCreateInfo { };
            swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            swapchainCreateInfo.pNext = nullptr;
            swapchainCreateInfo.flags = 0;
            swapchainCreateInfo.surface = m_surface;
            swapchainCreateInfo.minImageCount = m_swapchainImageCount;
            swapchainCreateInfo.imageFormat = m_surfaceFormat.format;
            swapchainCreateInfo.imageColorSpace = m_surfaceFormat.colorSpace;
            swapchainCreateInfo.imageExtent = m_extent;
            swapchainCreateInfo.imageArrayLayers = 1;
            swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            // TODO SUPPORT FOR MORE QUEUES
            swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            swapchainCreateInfo.queueFamilyIndexCount = 0;
            swapchainCreateInfo.pQueueFamilyIndices = nullptr;
            swapchainCreateInfo.preTransform = m_swapchainPreTransform;
            swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            swapchainCreateInfo.presentMode = m_presentMode;
            swapchainCreateInfo.clipped = VK_TRUE;
            swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

            UVKE_ASSERT(vkCreateSwapchainKHR(m_device, &swapchainCreateInfo, nullptr, &m_swapchain));
        }

        {
            unsigned int swapchainImageCount = 0;
            vkGetSwapchainImagesKHR(m_device, m_swapchain, &swapchainImageCount, nullptr);
            m_swapchainImages = std::vector<VkImage>(swapchainImageCount);
            vkGetSwapchainImagesKHR(m_device, m_swapchain, &swapchainImageCount, m_swapchainImages.data());
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
                imageViewCreateInfo.format = m_surfaceFormat.format;
                imageViewCreateInfo.components = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
                imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
                imageViewCreateInfo.subresourceRange.levelCount = 1;
                imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
                imageViewCreateInfo.subresourceRange.layerCount = 1;

                UVKE_ASSERT(vkCreateImageView(m_device, &imageViewCreateInfo, nullptr, &m_swapchainImageViews[i]));
            }
        }

        Shader shader(m_device, File::Load("Resource/Shader.vert.spv"), File::Load("Resource/Shader.frag.spv"));
        UVKE_LOG("Shaders Loaded");
    }

    Renderer::~Renderer() {
        for(auto i = 0; i < m_swapchainImageViews.size(); ++i) {
            vkDestroyImageView(m_device, m_swapchainImageViews[i], nullptr);
        }

        vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
        vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
        vkDestroyDevice(m_device, nullptr);
        vkDestroyInstance(m_instance, nullptr);
    }
};