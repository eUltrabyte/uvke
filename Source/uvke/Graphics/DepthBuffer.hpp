#pragma once
#ifndef UVKE_DEPTH_BUFFER_HEADER
#define UVKE_DEPTH_BUFFER_HEADER

#include "../uvke.hpp"

namespace uvke {
    class UVKE_API DepthBuffer {
    public:
        DepthBuffer();
        virtual ~DepthBuffer();

    private:
        VkImage m_image;
        VkDeviceMemory m_imageMemory;
        VkImageView m_imageView;

    };
};

#endif