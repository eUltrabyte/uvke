#pragma once
#ifndef UVKE_SWAPCHAIN_HEADER
#define UVKE_SWAPCHAIN_HEADER

#include "../uvke.hpp"
#include "../Core/Window.hpp"
#include "../Core/Base.hpp"
#include "Surface.hpp"

namespace uvke {
    class UVKE_API Swapchain {
    public:
        Swapchain(std::shared_ptr<Base> base = nullptr, std::shared_ptr<Surface> surface = nullptr);
        virtual ~Swapchain();

        virtual void Recreate(std::shared_ptr<Window> window, VkRenderPass renderPass);

        virtual void SetBase(std::shared_ptr<Base> base);
        virtual void SetSurface(std::shared_ptr<Surface> surface);
        virtual void SetImageCount(unsigned int count);
        virtual void SetImages(const std::vector<VkImage>& images);
        virtual void SetImageViews(const std::vector<VkImageView>& imageViews);

        virtual std::shared_ptr<Base> GetBase();
        virtual std::shared_ptr<Surface> GetSurface();
        virtual unsigned int& GetImageCount();
        virtual VkSwapchainKHR& GetSwapchain();
        virtual std::vector<VkImage>& GetImages();
        virtual VkImage& GetImage(unsigned int index);
        virtual std::vector<VkImageView>& GetImageViews();
        virtual VkImageView& GetImageView(unsigned int index);
        virtual bool& IsRecreated();

    protected:
        std::shared_ptr<Base> m_base;
        std::shared_ptr<Surface> m_surface;

    private:
        unsigned int m_imageCount;
        VkSwapchainKHR m_swapchain;
        std::vector<VkImage> m_images;
        std::vector<VkImageView> m_imageViews;
        bool m_isRecreated;

    };
};

#endif