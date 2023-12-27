#pragma once
#ifndef UVKE_HELPER
#define UVKE_HELPER

#include "../uvke.hpp"
#include "../Core/Base.hpp"

namespace uvke {
    class UVKE_API Helper {
    public:
        Helper() = default;
        virtual ~Helper() = default;

        static VkDeviceSize GetRequirementsSize(Base* base, VkBuffer buffer) {
            VkMemoryRequirements memoryRequirements { };
            vkGetBufferMemoryRequirements(base->GetDevice(), buffer, &memoryRequirements);
            return memoryRequirements.size;
        }

        static VkDeviceSize GetRequirementsSize(Base* base, VkImage image) {
            VkMemoryRequirements memoryRequirements { };
            vkGetImageMemoryRequirements(base->GetDevice(), image, &memoryRequirements);
            return memoryRequirements.size;
        }

        static int FindMemoryIndex(Base* base, VkBuffer buffer) {
            VkMemoryRequirements memoryRequirements { };
            vkGetBufferMemoryRequirements(base->GetDevice(), buffer, &memoryRequirements);

            VkPhysicalDeviceMemoryProperties memoryProperties { };
            vkGetPhysicalDeviceMemoryProperties(base->GetPhysicalDevice(), &memoryProperties);

            unsigned int filter = memoryRequirements.memoryTypeBits;
            VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

            for(auto i = 0; i < memoryProperties.memoryTypeCount; ++i) {
                if(filter & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                    return i;
                }
            }

            return 0;
        }

        static int FindMemoryIndex(Base* base, VkImage image) {
            VkMemoryRequirements memoryRequirements { };
            vkGetImageMemoryRequirements(base->GetDevice(), image, &memoryRequirements);

            VkPhysicalDeviceMemoryProperties memoryProperties { };
            vkGetPhysicalDeviceMemoryProperties(base->GetPhysicalDevice(), &memoryProperties);

            unsigned int filter = memoryRequirements.memoryTypeBits;
            VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

            for(auto i = 0; i < memoryProperties.memoryTypeCount; ++i) {
                if(filter & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                    return i;
                }
            }

            return 0;
        }
    };
};

#endif