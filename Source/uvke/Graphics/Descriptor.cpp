#include "Descriptor.hpp"

namespace uvke {
    Descriptor::Descriptor(Base* base)
        : m_base(base) {
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

            UVKE_ASSERT(vkCreateDescriptorSetLayout(m_base->GetDevice(), &descriptorSetLayoutCreateInfo, nullptr, &m_descriptorSetLayout));
        }

        UVKE_LOG_ADDRESS("Descriptor Created");
    }
    
    Descriptor::~Descriptor() {
        if(m_base->GetDevice() != VK_NULL_HANDLE) {
            if(m_descriptorSetLayout != VK_NULL_HANDLE) {
                vkDestroyDescriptorSetLayout(m_base->GetDevice(), m_descriptorSetLayout, nullptr);
            }
        }

        UVKE_LOG_ADDRESS("Descriptor Destroyed");
    }

    void Descriptor::SetBase(Base* base) {
        m_base = base;
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