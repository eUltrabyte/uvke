#include "Camera.hpp"
#include <GLFW/glfw3.h>

namespace uvke {
    Camera::Camera(const Projection& projection, const vec2f& size)
        : m_position(0.0f, 0.0f, 0.0f), m_front(0.0f, 0.0f, -1.0f), m_up(0.0f, 1.0f, 0.0f), m_yaw(-90.0f), m_pitch(0.0f), m_direction(0.0f, 0.0f, 0.0f), m_firstMouseMove(true), m_lastMousePosition(0, 0) {
        m_ubo.model = Identity<float>();
        m_ubo.view = LookAt<float>(m_position, m_position + m_front, m_up);

        switch(projection) {
            case Projection::Orthographic: {
                m_ubo.projection = Ortho<float>(-size.x, size.x, size.y, -size.y, -150.0f, 100.0f);
            } break;

            case Projection::Perspectivic: {
                // m_ubo.projection = Perspective<float>(Radians(45.0f), size.x / size.y, 0.1f, 1000.0f);
                m_ubo.projection = InfinitePerspective<float>(Radians(45.0f), size.x / size.y, 0.1f);
                m_ubo.projection.data[1][1] *= -1;
            } break;

            case Projection::Frustumic: {
                m_ubo.projection = Frustum<float>(-size.x, size.x, size.y, -size.y, -150.0f, 100.0f);
            } break;
        }

        UVKE_LOG_ADDRESS("Camera Created");
    }

    Camera::~Camera() {
        UVKE_LOG_ADDRESS("Camera Destroyed");
    }

    void Camera::Move(Window* window, float speed, float sensitivity) {
        if(window->GetKey(GLFW_KEY_W) == GLFW_PRESS) {
            m_position += m_front * speed;
        } else if(window->GetKey(GLFW_KEY_S) == GLFW_PRESS) {
            m_position -= m_front * speed;
        }

        if(window->GetKey(GLFW_KEY_SPACE) == GLFW_PRESS) {
            m_position -= m_up * speed;
        } else if(window->GetKey(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            m_position += m_up * speed;
        }

        if(window->GetKey(GLFW_KEY_A) == GLFW_PRESS) {
            m_position -= Normalize<float>(CrossProduct<float>(m_front, m_up)) * speed;
        } else if(window->GetKey(GLFW_KEY_D) == GLFW_PRESS) {
            m_position += Normalize<float>(CrossProduct<float>(m_front, m_up)) * speed;
        }

        if(m_firstMouseMove) {
            m_lastMousePosition = vec2d(window->GetMouse().x, window->GetMouse().y);
            m_firstMouseMove = false;
        }

        vec2f offset = vec2f(window->GetMouse().x - m_lastMousePosition.x, window->GetMouse().y - m_lastMousePosition.y);
        m_lastMousePosition = vec2d(window->GetMouse().x, window->GetMouse().y);

        offset *= sensitivity;

        m_yaw += offset.x;
        m_pitch += offset.y;

        if(m_pitch > 89.0f) m_pitch = 89.0f;
        if(m_pitch < -89.0f) m_pitch = -89.0f;

        if(m_yaw > 359.0f) m_yaw = 0.0f;
        if(m_yaw < -359.0f) m_yaw = 0.0f;

        m_direction = vec3f(Cos<float>(Radians<float>(m_yaw)) * Cos<float>(Radians<float>(m_pitch)), Sin<float>(Radians<float>(m_pitch)), Sin<float>(Radians<float>(m_yaw)) * Cos<float>(Radians<float>(m_pitch)));

        m_front = Normalize<float>(m_direction);
        
        m_ubo.view = LookAt<float>(m_position, m_position + m_front, m_up);
    }

    void Camera::SetModel(const mat4x4f& model) {
        m_ubo.model = model;
    }

    void Camera::SetView(const mat4x4f& view) {
        m_ubo.view = view;
    }
    
    void Camera::SetProjection(const mat4x4f& projection) {
        m_ubo.projection = projection;
    }

    void Camera::SetYaw(float yaw) {
        m_yaw = yaw;
    }
    
    void Camera::SetPitch(float pitch) {
        m_pitch = pitch;
    }
        
    void Camera::SetDirection(const vec3f& direction) {
        m_direction = direction;
    }

    UniformBufferObject& Camera::GetUBO() {
        return m_ubo;
    }

    vec3f& Camera::GetPosition() {
        return m_position;
    }
    
    float& Camera::GetYaw() {
        return m_yaw;
    }
    
    float& Camera::GetPitch() {
        return m_pitch;
    }
    
    vec3f& Camera::GetDirection() {
        return m_direction;
    }
};