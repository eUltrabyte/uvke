#pragma once
#ifndef UVKE_DEPTH_BUFFER_HEADER
#define UVKE_DEPTH_BUFFER_HEADER

#include "../uvke.hpp"
#include "../Core/Base.hpp"
#include "../Core/Helper.hpp"
#include "Surface.hpp"
#include "CommandBuffer.hpp"
#include "Image.hpp"

namespace uvke {
    class UVKE_API DepthBuffer {
    public:
        DepthBuffer(Base* base = nullptr, Surface* surface = nullptr);
        virtual ~DepthBuffer();

        virtual void Recreate(Base* base, Surface* surface);

        virtual void LayoutTransition(CommandBuffer* commandBuffer, VkQueue queue);

        virtual VkImage& GetImage();
        virtual VkDeviceMemory& GetImageMemory();
        virtual VkImageView& GetImageView();

    protected:
        Base* m_base;

    private:
        std::unique_ptr<Image> m_image;

    };
};

#endif