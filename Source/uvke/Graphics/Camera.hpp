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
        Camera(const Projection& projection = Projection::Orthographic, const glm::vec2& size = glm::vec2(1.0f, 1.0f));
        virtual ~Camera();

        virtual void Move(Window* window, float speed, float sensitivity);

        virtual void SetModel(const glm::mat4& model);
        virtual void SetView(const glm::mat4& view);
        virtual void SetProjection(const glm::mat4& projection);
        virtual void SetYaw(float yaw);
        virtual void SetPitch(float pitch);
        virtual void SetDirection(const glm::vec3& direction);

        virtual UniformBufferObject& GetUBO();
        virtual glm::vec3& GetPosition();
        virtual float& GetYaw();
        virtual float& GetPitch();
        virtual glm::vec3& GetDirection();

    private:
        UniformBufferObject m_ubo;
        glm::vec3 m_position;
        glm::vec3 m_front;
        glm::vec3 m_up;
        float m_yaw;
        float m_pitch;
        glm::vec3 m_direction;
        bool m_firstMouseMove;
        glm::dvec2 m_lastMousePosition;

    };
};

#endif