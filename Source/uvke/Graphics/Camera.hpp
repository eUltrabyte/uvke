#pragma once
#ifndef UVKE_CAMERA_HEADER
#define UVKE_CAMERA_HEADER

#include "../uvke.hpp"
#include "../Core/Base.hpp"
#include "UniformBuffer.hpp"

namespace uvke {
    enum UVKE_API Projection {
        Orthographic = 0,
        Perspectivic = 1,
    };

    class UVKE_API Camera {
    public:
        Camera(const Projection& projection = Projection::Orthographic, const vec2f& size = { 1.0f, 1.0f });
        virtual ~Camera() = default;

        virtual void Update(std::shared_ptr<UniformBuffer> uniformBuffer);

        virtual void SetModel(const mat4x4f& model);
        virtual void SetView(const mat4x4f& view);
        virtual void SetProjection(const mat4x4f& projection);

        virtual mat4x4f& GetModel();
        virtual mat4x4f& GetView();
        virtual mat4x4f& GetProjection();

    private:
        UniformBufferObject m_ubo;

    };
};

#endif