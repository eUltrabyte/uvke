#pragma once
#ifndef UVKE_RENDERABLE_COMPONENT_HEADER
#define UVKE_RENDERABLE_COMPONENT_HEADER

#include "../uvke.hpp"
#include "Component.hpp"

namespace uvke {
    enum UVKE_API RenderType {
        Triangles = 0,
        Lines = UVKE_BYTE(0),
        Points = UVKE_BYTE(1),
    };

    class UVKE_API Renderer;

    class UVKE_API RenderableComponent : public Component {
    public:
        RenderableComponent() = default;
        virtual ~RenderableComponent() = default;

        virtual void Render(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, unsigned int frame) = 0;

        virtual RenderType& GetRenderType() {
            return m_renderType;
        }

    protected:
        RenderType m_renderType = RenderType::Triangles;

    };
};

#endif