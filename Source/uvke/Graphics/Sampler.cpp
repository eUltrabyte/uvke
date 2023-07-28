#include "Sampler.hpp"

namespace uvke {
    Sampler::Sampler(Base* base, Texture* texture)
        : m_base(base) {
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

            UVKE_ASSERT(vkCreateImageView(m_base->GetDevice(), &imageViewCreateInfo, nullptr, &m_imageView));
        }

        {
            VkPhysicalDeviceFeatures physicalDeviceFeatures { };
            vkGetPhysicalDeviceFeatures(m_base->GetPhysicalDevice(), &physicalDeviceFeatures);

            VkPhysicalDeviceProperties physicalDeviceProperties { };
            vkGetPhysicalDeviceProperties(m_base->GetPhysicalDevice(), &physicalDeviceProperties);

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

            UVKE_ASSERT(vkCreateSampler(m_base->GetDevice(), &samplerCreateInfo, nullptr, &m_sampler));
        }

        UVKE_LOG("Sampler Created");
    }

    Sampler::~Sampler() {
        if(m_base->GetDevice() != VK_NULL_HANDLE) {
            if(m_sampler != VK_NULL_HANDLE) {
                vkDestroySampler(m_base->GetDevice(), m_sampler, nullptr);
            }

            if(m_imageView != VK_NULL_HANDLE) {
                vkDestroyImageView(m_base->GetDevice(), m_imageView, nullptr);
            }
        }

        UVKE_LOG("Sampler Destroyed");
    }

    void Sampler::SetBase(Base* base) {
        m_base = base;
    }

    VkImageView& Sampler::GetImageView() {
        return m_imageView;
    }

    VkSampler& Sampler::GetSampler() {
        return m_sampler;
    }
};