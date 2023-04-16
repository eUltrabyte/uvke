#pragma once
#ifndef UVKE_INTERFACE_HEADER
#define UVKE_INTERFACE_HEADER

#include "../uvke.hpp"
#include "../Core/Base.hpp"
#include "../Core/Window.hpp"
#include "Surface.hpp"
#include "CommandBuffer.hpp"

namespace uvke {
    class UVKE_API Interface {
    public:
        Interface(std::shared_ptr<Base> base, std::shared_ptr<Window> window, std::shared_ptr<Surface> surface, std::shared_ptr<CommandBuffer> commandBuffer, VkRenderPass renderPass);
        virtual ~Interface();

        virtual void Render(std::shared_ptr<CommandBuffer> commandBuffer, unsigned int frame);

        virtual void SetRenderTime(float renderTime);
        virtual void SetFPS(int fps);

    protected:
        std::shared_ptr<Base> m_base;

    private:
        VkDescriptorPool m_descriptorPool;
        float m_renderTime;
        int m_fps;

    };
};

#endif