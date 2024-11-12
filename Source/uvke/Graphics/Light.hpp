#pragma once
#ifndef UVKE_LIGHT_HEADER
#define UVKE_LIGHT_HEADER

#include "../uvke.hpp"
#include "LightComponent.hpp"
#include "Renderer.hpp"

namespace uvke {
    class UVKE_API Light : public LightComponent {
    public:
        Light();
        Light(const LightType& type);
        virtual ~Light();

        virtual void Create(Renderer* renderer) override;

    };
};

#endif