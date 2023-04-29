#pragma once
#ifndef UVKE_SAMPLER_HEADER
#define UVKE_SAMPLER_HEADER

#include "../uvke.hpp"
#include "Surface.hpp"
#include "Texture.hpp"

namespace uvke {
    class UVKE_API Sampler {
    public:
        Sampler(std::shared_ptr<Base> base = nullptr, std::shared_ptr<Texture> texture = nullptr);
        virtual ~Sampler();

        virtual void SetBase(std::shared_ptr<Base> base);

        virtual std::shared_ptr<Base> GetBase();
        virtual VkImageView& GetImageView();
        virtual VkSampler& GetSampler();

    protected:
        std::shared_ptr<Base> m_base;

    private:
        VkImageView m_imageView;
        VkSampler m_sampler;

    };
};

#endif