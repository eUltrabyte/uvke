#pragma once
#ifndef UVKE_DESCRIPTOR_HEADER
#define UVKE_DESCRIPTOR_HEADER

#include "../uvke.hpp"
#include "../Core/Base.hpp"

namespace uvke {
    class UVKE_API Descriptor {
    public:
        Descriptor(Base* base = nullptr);
        virtual ~Descriptor();

        virtual void SetBase(Base* base);

        virtual std::array<VkDescriptorSetLayoutBinding, 2>& GetDescriptorSetLayoutBindings();
        virtual VkDescriptorSetLayoutBinding& GetDescriptorSetLayoutBinding(unsigned int index);
        virtual VkDescriptorSetLayout& GetDescriptorSetLayout();

    protected:
        Base* m_base;

    private:
        std::array<VkDescriptorSetLayoutBinding, 2> m_descriptorSetLayoutBindings;
        VkDescriptorSetLayout m_descriptorSetLayout;

    };
};

#endif