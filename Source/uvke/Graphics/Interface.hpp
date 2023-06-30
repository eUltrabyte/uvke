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
        Interface(Base* base = nullptr, Window* window = nullptr, Surface* surface = nullptr, CommandBuffer* commandBuffer = nullptr, VkRenderPass renderPass = nullptr);
        virtual ~Interface();

        virtual void Render(CommandBuffer* commandBuffer, unsigned int frame);

        virtual void SetRenderTime(float renderTime);
        virtual void SetFPS(int fps);

    protected:
        Base* m_base;

    private:
        VkDescriptorPool m_descriptorPool;
        float m_renderTime;
        int m_fps;

    };
};

#endif