#pragma once
#ifndef UVKE_SAMPLER_HEADER
#define UVKE_SAMPLER_HEADER

#include "../uvke.hpp"
#include "Surface.hpp"
#include "Texture.hpp"

namespace uvke {
    class UVKE_API Sampler {
    public:
        Sampler(Base* base = nullptr, VkFilter filter = VK_FILTER_LINEAR);
        virtual ~Sampler();

        virtual void SetBase(Base* base);

        virtual VkSampler& GetSampler();

    protected:
        Base* m_base;

    private:
        VkSampler m_sampler;

    };
};

#endif