#pragma once
#include <vulkan/vulkan_core.h>
#ifndef UVKE_BASE_HEADER
#define UVKE_BASE_HEADER

#include "../uvke.hpp"

namespace uvke {
    class Base {
    public:
        Base(std::string_view name = "uvke Base");
        virtual ~Base();

        virtual void SetInstance(VkInstance instance);
        virtual void SetPhysicalDevice(VkPhysicalDevice physicalDevice);
        virtual void SetDevice(VkDevice device);

        virtual VkInstance& GetInstance();
        virtual VkPhysicalDevice& GetPhysicalDevice();
        virtual VkDevice& GetDevice();
        virtual VkFormat GetDepthFormat();
        virtual VkSampleCountFlagBits GetMaxSampleCount();
        virtual unsigned int GetQueueFamily();
        virtual unsigned int GetQueueCount();
        virtual bool IsMultiQueueSupported();
        virtual bool HasStencilComponent();

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

                if(physicalDevicesProperties[i].deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && physicalDevicesFeatures[i].geometryShader == VK_TRUE) {
                    std::vector<VkFormat> depthFormats = {
                        VK_FORMAT_D32_SFLOAT,
                        VK_FORMAT_D32_SFLOAT_S8_UINT,
                        VK_FORMAT_D24_UNORM_S8_UINT
                    };

                    for(auto j = 0; j < depthFormats.size(); ++j) {
                        VkFormatProperties formatProperties;
                        vkGetPhysicalDeviceFormatProperties(physicalDevices[i], depthFormats[j], &formatProperties);

                        if(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
                            m_depthFormat = depthFormats[j];
                            break;
                        }

                        UVKE_FATAL("Supported Format Not Found!");
                    }
                    
                    VkSampleCountFlags sampleCount = physicalDevicesProperties[i].limits.framebufferColorSampleCounts & physicalDevicesProperties[i].limits.framebufferDepthSampleCounts;
                    if(sampleCount & VK_SAMPLE_COUNT_64_BIT) {
                        m_sampleCount = VK_SAMPLE_COUNT_64_BIT;
                    } else if(sampleCount & VK_SAMPLE_COUNT_32_BIT) {
                        m_sampleCount = VK_SAMPLE_COUNT_32_BIT;
                    } else if(sampleCount & VK_SAMPLE_COUNT_16_BIT) {
                        m_sampleCount = VK_SAMPLE_COUNT_16_BIT;
                    } else if(sampleCount & VK_SAMPLE_COUNT_8_BIT) {
                        m_sampleCount = VK_SAMPLE_COUNT_8_BIT;
                    } else if(sampleCount & VK_SAMPLE_COUNT_4_BIT) {
                        m_sampleCount = VK_SAMPLE_COUNT_4_BIT;
                    } else if(sampleCount & VK_SAMPLE_COUNT_2_BIT) {
                        m_sampleCount = VK_SAMPLE_COUNT_2_BIT;
                    } else {
                        m_sampleCount = VK_SAMPLE_COUNT_1_BIT;
                    }
                    
                    UVKE_LOG("GPU - " + std::string(physicalDevicesProperties[i].deviceName));
                    return physicalDevices[i];
                }
            }

            m_sampleCount = VK_SAMPLE_COUNT_1_BIT;
            return physicalDevices[0];
        }

        unsigned int GetSuitableQueueFamily() {
            std::vector<VkQueueFamilyProperties> queueFamilyProperties;
            {
                unsigned int queueFamilyPropertiesCount = 0;
                vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyPropertiesCount, nullptr);
                queueFamilyProperties = std::vector<VkQueueFamilyProperties>(queueFamilyPropertiesCount);
                vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyPropertiesCount, queueFamilyProperties.data());
            }

            for(auto i = 0; i < queueFamilyProperties.size(); ++i) {
                if(queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                    m_queueCount = queueFamilyProperties[i].queueCount;
                    if(m_queueCount > 1) {
                        m_multiQueue = true;
                    } else {
                        m_multiQueue = false;
                    }

                    return i;
                }
            }

            return 0;
        }

        VkInstance m_instance;
        VkPhysicalDevice m_physicalDevice;
        VkDevice m_device;
        VkFormat m_depthFormat;
        VkSampleCountFlagBits m_sampleCount;
        unsigned int m_queueFamilyIndex;
        unsigned int m_queueCount;
        bool m_multiQueue;

    };
};

#endif
