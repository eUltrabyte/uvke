#include "Sampler.hpp"

namespace uvke {
    Sampler::Sampler(Base* base, VkFilter filter)
        : m_base(base) {
        {
            VkPhysicalDeviceFeatures physicalDeviceFeatures { };
            vkGetPhysicalDeviceFeatures(m_base->GetPhysicalDevice(), &physicalDeviceFeatures);

            VkPhysicalDeviceProperties physicalDeviceProperties { };
            vkGetPhysicalDeviceProperties(m_base->GetPhysicalDevice(), &physicalDeviceProperties);

            VkSamplerCreateInfo samplerCreateInfo { };
            samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
            samplerCreateInfo.pNext = nullptr;
            samplerCreateInfo.flags = 0;
            samplerCreateInfo.magFilter = filter;
            samplerCreateInfo.minFilter = filter;
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

        UVKE_LOG_ADDRESS("Sampler Created");
    }

    Sampler::~Sampler() {
        if(m_base->GetDevice() != VK_NULL_HANDLE) {
            if(m_sampler != VK_NULL_HANDLE) {
                vkDestroySampler(m_base->GetDevice(), m_sampler, nullptr);
            }
        }

        UVKE_LOG_ADDRESS("Sampler Destroyed");
    }

    void Sampler::SetBase(Base* base) {
        m_base = base;
    }

    VkSampler& Sampler::GetSampler() {
        return m_sampler;
    }
};