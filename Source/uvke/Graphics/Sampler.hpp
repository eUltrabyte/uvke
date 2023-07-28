#pragma once
#ifndef UVKE_SAMPLER_HEADER
#define UVKE_SAMPLER_HEADER

#include "../uvke.hpp"
#include "Surface.hpp"
#include "Texture.hpp"

namespace uvke {
    class UVKE_API Sampler {
    public:
        Sampler(Base* base = nullptr, Texture* texture = nullptr);
        virtual ~Sampler();

        virtual void SetBase(Base* base);

        virtual VkImageView& GetImageView();
        virtual VkSampler& GetSampler();

    protected:
        Base* m_base;

    private:
        VkImageView m_imageView;
        VkSampler m_sampler;

    };
};

#endif