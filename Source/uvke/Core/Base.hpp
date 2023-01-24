#pragma once
#ifndef UVKE_BASE_HEADER
#define UVKE_BASE_HEADER

#include "../uvke.hpp"

namespace uvke {
    class Base {
    public:
        Base(std::string_view name = "uvke Base");
        virtual ~Base();

        virtual VkInstance& GetInstance();
        virtual VkPhysicalDevice& GetPhysicalDevice();
        virtual VkDevice& GetDevice();
        virtual unsigned int GetQueueFamily();

    protected:
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

        unsigned int GetSuitableQueueFamily() {
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

    private:
        VkInstance m_instance;
        VkPhysicalDevice m_physicalDevice;
        VkDevice m_device;
        unsigned int m_queueFamilyIndex;

    };
};

#endif
