#pragma once
#ifndef UVKE_CAMERA_HEADER
#define UVKE_CAMERA_HEADER

#include "../uvke.hpp"
#include "../Core/Base.hpp"
#include "../Window/Window.hpp"
#include "UniformBuffer.hpp"

namespace uvke {
    enum UVKE_API Projection {
        Orthographic = 0,
        Perspectivic = 1,
        Frustumic = 2,
    };

    class UVKE_API Camera {
    public:
        Camera(const Projection& projection = Projection::Orthographic, const vec2f& size = { 1.0f, 1.0f });
        virtual ~Camera();

        virtual void Move(Window* window, float speed, float sensitivity);

        virtual void SetModel(const mat4x4f& model);
        virtual void SetView(const mat4x4f& view);
        virtual void SetProjection(const mat4x4f& projection);

        virtual UniformBufferObject& GetUBO();
        virtual vec3f& GetPosition();
        virtual float& GetYaw();
        virtual float& GetPitch();
        virtual vec3f& GetDirection();

    private:
        UniformBufferObject m_ubo;
        vec3f m_position;
        vec3f m_front;
        vec3f m_up;
        float m_yaw;
        float m_pitch;
        vec3f m_direction;
        bool m_firstMouseMove;
        vec2d m_lastMousePosition;

    };
};

#endif