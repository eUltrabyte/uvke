#pragma once
#ifndef UVKE_SAMPLER_HEADER
#define UVKE_SAMPLER_HEADER

#include "../uvke.hpp"
#include "Surface.hpp"
#include "Texture.hpp"

namespace uvke {
    class UVKE_API Sampler {
    public:
        Sampler(VkPhysicalDevice physicalDevice = nullptr, VkDevice device = nullptr, std::shared_ptr<Texture> texture = nullptr);
        virtual ~Sampler();

        virtual VkPhysicalDevice& GetPhysicalDevice();
        virtual VkDevice& GetDevice();
        virtual VkImageView& GetImageView();
        virtual VkSampler& GetSampler();

    protected:
        VkPhysicalDevice m_physicalDevice;
        VkDevice m_device;

    private:
        VkImageView m_imageView;
        VkSampler m_sampler;

    };
};

#endif