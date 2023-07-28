#include "Camera.hpp"

namespace uvke {
    Camera::Camera(const Projection& projection, const vec2f& size) {
        m_ubo.model = Identity<float>();
        m_ubo.view = LookAt<float>(vec3f(0.0f, 0.0f, -2.0f), vec3f(0.0f, 0.0f, 0.0f), vec3f(0.0f, 1.0f, -2.0f));
        
        switch(projection) {
            case Projection::Orthographic: {
                m_ubo.projection = Ortho<float>(-size.x, size.x, size.y, -size.y, -150.0f, 100.0f);
            } break;

            case Projection::Perspectivic: {
                m_ubo.projection = Perspective<float>(Radians(45.0f), size.x / size.y, 0.1f, 1000.0f);
                m_ubo.projection.data[1][1] *= -1;
            } break;

            case Projection::Frustumic: {
                m_ubo.projection = Frustum<float>(-size.x, size.x, size.y, -size.y, -150.0f, 100.0f);
            } break;
        }
    }

    void Camera::Update(UniformBuffer* uniformBuffer) {
        uniformBuffer->Update(m_ubo);
    }

    void Camera::Move(Window* window, const vec2f& speed) {
        if(window->GetKey(GLFW_KEY_UP) == GLFW_PRESS) {
            m_ubo.view = Translate<float>(m_ubo.view, vec3f(0.0f, speed.y, 0.0f));
        } else if(window->GetKey(GLFW_KEY_DOWN) == GLFW_PRESS) {
            m_ubo.view = Translate<float>(m_ubo.view, vec3f(0.0f, -speed.y, 0.0f));
        }

        if(window->GetKey(GLFW_KEY_LEFT) == GLFW_PRESS) {
            m_ubo.view = Translate<float>(m_ubo.view, vec3f(-speed.x, 0.0f, 0.0f));
        } else if(window->GetKey(GLFW_KEY_RIGHT) == GLFW_PRESS) {
            m_ubo.view = Translate<float>(m_ubo.view, vec3f(speed.x, 0.0f, 0.0f));
        }
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

    mat4x4f& Camera::GetModel() {
        return m_ubo.model;
    }
        
    mat4x4f& Camera::GetView() {
        return m_ubo.view;
    }
    
    mat4x4f& Camera::GetProjection() {
        return m_ubo.projection;
    }
};