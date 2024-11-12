#include "Light.hpp"

namespace uvke {
    Light::Light() {
        m_type = LightType::Ambient;

        UVKE_LOG_ADDRESS("Light Setup");
    }

    Light::Light(const LightType& type) {
        m_type = type;

        UVKE_LOG_ADDRESS("Light Setup");
    }

    Light::~Light() {
        UVKE_LOG_ADDRESS("Light Destroyed");
    }

    void Light::Create(Renderer* renderer) {


        UVKE_LOG("Light Created");
    }
};