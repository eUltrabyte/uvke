#include "Descriptor.hpp"

namespace uvke {
    Descriptor::Descriptor(VkDevice device)
        : m_device(device) {
        {
            VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = { };
            descriptorSetLayoutBinding.binding = 0;
            descriptorSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorSetLayoutBinding.descriptorCount = 1;
            descriptorSetLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
            descriptorSetLayoutBinding.pImmutableSamplers = nullptr;

            VkDescriptorSetLayoutBinding samplerSetLayoutBinding = { };
            samplerSetLayoutBinding.binding = 1;
            samplerSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            samplerSetLayoutBinding.descriptorCount = 1;
            samplerSetLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
            samplerSetLayoutBinding.pImmutableSamplers = nullptr;

            m_descriptorSetLayoutBindings = { descriptorSetLayoutBinding, samplerSetLayoutBinding };

            VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo { };
            descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            descriptorSetLayoutCreateInfo.pNext = nullptr;
            descriptorSetLayoutCreateInfo.flags = 0;
            descriptorSetLayoutCreateInfo.bindingCount = m_descriptorSetLayoutBindings.size();
            descriptorSetLayoutCreateInfo.pBindings = m_descriptorSetLayoutBindings.data();

            UVKE_ASSERT(vkCreateDescriptorSetLayout(m_device, &descriptorSetLayoutCreateInfo, nullptr, &m_descriptorSetLayout));
        }

        UVKE_LOG("Descriptor Created");
    }
    
    Descriptor::~Descriptor() {
        if(m_device != VK_NULL_HANDLE) {
            if(m_descriptorSetLayout != VK_NULL_HANDLE) {
                vkDestroyDescriptorSetLayout(m_device, m_descriptorSetLayout, nullptr);
            }
        }

        UVKE_LOG("Descriptor Destroyed");
    }

    std::array<VkDescriptorSetLayoutBinding, 2>& Descriptor::GetDescriptorSetLayoutBindings() {
        return m_descriptorSetLayoutBindings;
    }
    
    VkDescriptorSetLayoutBinding& Descriptor::GetDescriptorSetLayoutBinding(unsigned int index) {
        if(index < 0 || index >= m_descriptorSetLayoutBindings.size()) {
            return m_descriptorSetLayoutBindings[0];
        } else {
            return m_descriptorSetLayoutBindings[index];
        }
    }
    
    VkDescriptorSetLayout& Descriptor::GetDescriptorSetLayout() {
        return m_descriptorSetLayout;
    }
};