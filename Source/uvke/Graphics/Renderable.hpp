#pragma once
#ifndef UVKE_RENDERABLE_HEADER
#define UVKE_RENDERABLE_HEADER

#include "../uvke.hpp"
#include "../Core/Window.hpp"
#include "Camera.hpp"

namespace uvke {
    class UVKE_API Renderer;

    class UVKE_API Renderable {
    public:
        Renderable() = default;
        virtual ~Renderable() = default;

        virtual void Create(std::shared_ptr<Renderer> renderer) = 0;
        virtual void Update(std::shared_ptr<Camera> camera) = 0;
        virtual void Render(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, unsigned int frame) = 0;

    };
};

#endif