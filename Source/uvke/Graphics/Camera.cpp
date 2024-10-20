#include "Camera.hpp"

namespace uvke {
    Camera::Camera(const Projection& projection, const glm::vec2& size)
        : m_position(0.0f, 0.0f, 0.0f), m_front(0.0f, 0.0f, -1.0f), m_up(0.0f, 1.0f, 0.0f), m_yaw(-90.0f), m_pitch(0.0f), m_direction(0.0f, 0.0f, 0.0f), m_firstMouseMove(true), m_lastMousePosition(0, 0) {
        m_ubo.model = glm::mat4(1.0f);
        m_ubo.view = glm::lookAt(m_position, m_position + m_front, m_up);

        switch(projection) {
            case Projection::Orthographic: {
                m_ubo.projection = glm::ortho(-size.x, size.x, size.y, -size.y, -150.0f, 100.0f);
            } break;

            case Projection::Perspectivic: {
                m_ubo.projection = glm::perspective(Radians(90.0f), size.x / size.y, 0.1f, 1000.0f);
            } break;

            case Projection::Frustumic: {
                m_ubo.projection = glm::frustum(-size.x, size.x, size.y, -size.y, -150.0f, 100.0f);
            } break;
        }

        UVKE_LOG_ADDRESS("Camera Created");
    }

    Camera::~Camera() {
        UVKE_LOG_ADDRESS("Camera Destroyed");
    }

    void Camera::Move(Window* window, float speed, float sensitivity) {
        int factor;
        if(window->GetKey(Keys::LeftControl) == KeyAction::Press) {
            factor = 2;
        } else {
            factor = 1;
        }

        if(window->GetKey(Keys::W) == KeyAction::Press) {
            m_position += m_front * (speed * factor);
        } else if(window->GetKey(Keys::S) == KeyAction::Press) {
            m_position -= m_front * (speed * factor);
        }

        if(window->GetKey(Keys::Space) == KeyAction::Press) {
            m_position -= m_up * (speed * factor);
        } else if(window->GetKey(Keys::LeftShift) == KeyAction::Press) {
            m_position += m_up * (speed * factor);
        }

        if(window->GetKey(Keys::A) == KeyAction::Press) {
            m_position -= glm::normalize(glm::cross(m_front, m_up)) * speed;
        } else if(window->GetKey(Keys::D) == KeyAction::Press) {
            m_position += glm::normalize(glm::cross(m_front, m_up)) * speed;
        }

        if(m_firstMouseMove) {
            m_lastMousePosition = glm::dvec2(window->GetMouse().x, window->GetMouse().y);
            m_firstMouseMove = false;
        }

        glm::vec2 offset = glm::vec2(window->GetMouse().x - m_lastMousePosition.x, window->GetMouse().y - m_lastMousePosition.y);
        m_lastMousePosition = glm::dvec2(window->GetMouse().x, window->GetMouse().y);

        offset *= sensitivity;

        m_yaw += offset.x;
        m_pitch += offset.y;

        if(m_pitch > 89.0f) m_pitch = 89.0f;
        if(m_pitch < -89.0f) m_pitch = -89.0f;

        if(m_yaw > 359.0f) m_yaw = 0.0f;
        if(m_yaw < -359.0f) m_yaw = 0.0f;

        m_direction = glm::vec3(Cos<float>(Radians<float>(m_yaw)) * Cos<float>(Radians<float>(m_pitch)), Sin<float>(Radians<float>(m_pitch)), Sin<float>(Radians<float>(m_yaw)) * Cos<float>(Radians<float>(m_pitch)));

        m_front = glm::normalize(m_direction);
        
        m_ubo.view = glm::lookAt(m_position, m_position + m_front, m_up);
    }

    void Camera::SetModel(const glm::mat4& model) {
        m_ubo.model = model;
    }

    void Camera::SetView(const glm::mat4& view) {
        m_ubo.view = view;
    }
    
    void Camera::SetProjection(const glm::mat4& projection) {
        m_ubo.projection = projection;
    }

    void Camera::SetYaw(float yaw) {
        m_yaw = yaw;
    }
    
    void Camera::SetPitch(float pitch) {
        m_pitch = pitch;
    }
        
    void Camera::SetDirection(const glm::vec3& direction) {
        m_direction = direction;
    }

    UniformBufferObject& Camera::GetUBO() {
        return m_ubo;
    }

    glm::vec3& Camera::GetPosition() {
        return m_position;
    }
    
    float& Camera::GetYaw() {
        return m_yaw;
    }
    
    float& Camera::GetPitch() {
        return m_pitch;
    }
    
    glm::vec3& Camera::GetDirection() {
        return m_direction;
    }
};