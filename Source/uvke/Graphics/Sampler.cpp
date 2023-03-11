#include "Sampler.hpp"

namespace uvke {
    Sampler::Sampler(VkPhysicalDevice physicalDevice, VkDevice device, std::shared_ptr<Texture> texture)
        : m_physicalDevice(physicalDevice), m_device(device) {
        {
            VkImageViewCreateInfo imageViewCreateInfo { };
            imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            imageViewCreateInfo.pNext = nullptr;
            imageViewCreateInfo.flags = 0;
            imageViewCreateInfo.image = texture->GetImage();
            imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            imageViewCreateInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
            imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
            imageViewCreateInfo.subresourceRange.levelCount = 1;
            imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
            imageViewCreateInfo.subresourceRange.layerCount = 1;

            UVKE_ASSERT(vkCreateImageView(m_device, &imageViewCreateInfo, nullptr, &m_imageView));
        }

        {
            VkPhysicalDeviceFeatures physicalDeviceFeatures { };
            vkGetPhysicalDeviceFeatures(m_physicalDevice, &physicalDeviceFeatures);

            VkPhysicalDeviceProperties physicalDeviceProperties { };
            vkGetPhysicalDeviceProperties(m_physicalDevice, &physicalDeviceProperties);

            VkSamplerCreateInfo samplerCreateInfo { };
            samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
            samplerCreateInfo.pNext = nullptr;
            samplerCreateInfo.flags = 0;
            samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
            samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
            samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

            if(physicalDeviceFeatures.samplerAnisotropy) {
              samplerCreateInfo.anisotropyEnable = VK_TRUE;
              samplerCreateInfo.maxAnisotropy = physicalDeviceProperties.limits.maxSamplerAnisotropy;
            } else {
              samplerCreateInfo.anisotropyEnable = VK_FALSE;
              samplerCreateInfo.maxAnisotropy = 1.0f;
            }

            samplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
            samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;
            samplerCreateInfo.compareEnable = VK_FALSE;
            samplerCreateInfo.compareOp = VK_COMPARE_OP_ALWAYS;
            samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

            UVKE_ASSERT(vkCreateSampler(m_device, &samplerCreateInfo, nullptr, &m_sampler));
        }

        UVKE_LOG("Sampler Created");
    }

    Sampler::~Sampler() {
        if(m_device != VK_NULL_HANDLE) {
            if(m_sampler != VK_NULL_HANDLE) {
                vkDestroySampler(m_device, m_sampler, nullptr);
            }

            if(m_imageView != VK_NULL_HANDLE) {
                vkDestroyImageView(m_device, m_imageView, nullptr);
            }
        }

        UVKE_LOG("Sampler Destroyed");
    }

    VkPhysicalDevice& Sampler::GetPhysicalDevice() {
        return m_physicalDevice;
    }

    VkDevice& Sampler::GetDevice() {
        return m_device;
    }

    VkImageView& Sampler::GetImageView() {
        return m_imageView;
    }

    VkSampler& Sampler::GetSampler() {
        return m_sampler;
    }
};