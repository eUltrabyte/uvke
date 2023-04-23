#pragma once
#ifndef UVKE_DESCRIPTOR_HEADER
#define UVKE_DESCRIPTOR_HEADER

#include "../uvke.hpp"

namespace uvke {
    class UVKE_API Descriptor {
    public:
        Descriptor(VkDevice device = nullptr);
        virtual ~Descriptor();

        virtual std::array<VkDescriptorSetLayoutBinding, 2>& GetDescriptorSetLayoutBindings();
        virtual VkDescriptorSetLayoutBinding& GetDescriptorSetLayoutBinding(unsigned int index);
        virtual VkDescriptorSetLayout& GetDescriptorSetLayout();

    protected:
        VkDevice m_device;

    private:
        std::array<VkDescriptorSetLayoutBinding, 2> m_descriptorSetLayoutBindings;
        VkDescriptorSetLayout m_descriptorSetLayout;

    };
};

#endif