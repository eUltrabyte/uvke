#pragma once
#ifndef UVKE_INTERFACE_HEADER
#define UVKE_INTERFACE_HEADER

#include "../uvke.hpp"
#include "../Core/Base.hpp"
#include "../Window/Window.hpp"
#include "Surface.hpp"
#include "CommandBuffer.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

namespace uvke {
    struct UVKE_API Stats {
    public:
        float renderTime = 0.0f;
        int fps = 0;
        vec3f position = vec3f(0.0f, 0.0f, 0.0f);
        float yaw = 0.0f;
        float pitch = 0.0f;
        vec3f direction = vec3f(0.0f, 0.0f, 0.0f);
    };

    class UVKE_API Interface {
    public:
        Interface(Base* base = nullptr, Window* window = nullptr, Surface* surface = nullptr, CommandBuffer* commandBuffer = nullptr, VkRenderPass renderPass = nullptr);
        virtual ~Interface();

        virtual void Render(CommandBuffer* commandBuffer, unsigned int frame);

        virtual void SetStats(Stats stats);

    protected:
        Base* m_base;

    private:
        VkDescriptorPool m_descriptorPool;
        Stats m_stats;

    };
};

#endif