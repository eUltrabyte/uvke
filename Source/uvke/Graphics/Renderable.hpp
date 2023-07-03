#pragma once
#ifndef UVKE_RENDERABLE_HEADER
#define UVKE_RENDERABLE_HEADER

#include "../uvke.hpp"
#include "../Core/Window.hpp"
#include "Camera.hpp"

namespace uvke {
    enum UVKE_API RenderType {
        Triangles = 0,
        Lines = UVKE_BYTE(0),
        Points = UVKE_BYTE(1),
    };

    class UVKE_API Renderer;

    class UVKE_API Renderable {
    public:
        Renderable() = default;
        virtual ~Renderable() = default;

        virtual void Create(Renderer* renderer) = 0;
        virtual void Update(Camera* camera) = 0;
        virtual void Render(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, unsigned int frame) = 0;

        virtual RenderType& GetRenderType() {
            return m_renderType;
        }

    protected:
        RenderType m_renderType = RenderType::Triangles;

    };
};

#endif