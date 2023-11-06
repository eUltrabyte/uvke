#pragma once
#ifndef UVKE_FRAMEBUFFER_HEADER
#define UVKE_FRAMEBUFFER_HEADER

#include "../uvke.hpp"
#include "../Core/Base.hpp"
#include "Surface.hpp"
#include "Swapchain.hpp"
#include "DepthBuffer.hpp"
#include "Image.hpp"

namespace uvke {
    class UVKE_API Framebuffer {
    public:
        Framebuffer(Base* base = nullptr, Swapchain* swapchain = nullptr, Surface* surface = nullptr, DepthBuffer* depthBuffer = nullptr, VkRenderPass renderPass = nullptr);
        virtual ~Framebuffer();

        virtual void Recreate();

        virtual void SetBase(Base* base);
        virtual void SetSurface(Surface* surface);
        virtual void SetSwapchain(Swapchain* swapchain);
        virtual void SetRenderPass(VkRenderPass renderPass);

        virtual VkRenderPass& GetRenderPass();
        virtual std::vector<VkFramebuffer>& GetFramebuffers();
        virtual VkFramebuffer& GetFramebuffer(int index);

    protected:
        Base* m_base;
        Surface* m_surface;
        Swapchain* m_swapchain;
        DepthBuffer* m_depthBuffer;
        VkRenderPass m_renderPass;

    private:
        std::vector<VkFramebuffer> m_framebuffers;
        std::shared_ptr<Image> m_image;

    };
};

#endif