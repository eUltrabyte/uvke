#pragma once
#ifndef UVKE_DESCRIPTOR_HEADER
#define UVKE_DESCRIPTOR_HEADER

#include "../uvke.hpp"
#include "../Core/Base.hpp"

namespace uvke {
    class UVKE_API Descriptor {
    public:
        Descriptor(std::shared_ptr<Base> base = nullptr);
        virtual ~Descriptor();

        virtual void SetBase(std::shared_ptr<Base> base);

        virtual std::shared_ptr<Base> GetBase();
        virtual std::array<VkDescriptorSetLayoutBinding, 2>& GetDescriptorSetLayoutBindings();
        virtual VkDescriptorSetLayoutBinding& GetDescriptorSetLayoutBinding(unsigned int index);
        virtual VkDescriptorSetLayout& GetDescriptorSetLayout();

    protected:
        std::shared_ptr<Base> m_base;

    private:
        std::array<VkDescriptorSetLayoutBinding, 2> m_descriptorSetLayoutBindings;
        VkDescriptorSetLayout m_descriptorSetLayout;

    };
};

#endif