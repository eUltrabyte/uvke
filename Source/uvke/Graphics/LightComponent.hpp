#pragma once
#ifndef UVKE_LIGHT_COMPONENT_HEADER
#define UVKE_LIGHT_COMPONENT_HEADER

#include "../uvke.hpp"
#include "Component.hpp"

namespace uvke {
    class UVKE_API Renderer;

    enum UVKE_API LightType {
        Ambient = 0,
        Diffuse,
        Specular,
        Phong
    };

    class UVKE_API LightComponent : public Component {
    public:
        LightComponent() = default;
        virtual ~LightComponent() = default;

        virtual LightType& GetType() {
            return m_type;
        }

        virtual float& GetStrength() {
            return m_strength;
        }
        
        virtual glm::vec3& GetPosition() {
            return m_position;
        }
        
        virtual glm::vec3& GetDirection() {
            return m_direction;
        }

    protected:
        LightType m_type;
        float m_strength;
        glm::vec3 m_position;
        glm::vec3 m_direction;

    };
};

#endif