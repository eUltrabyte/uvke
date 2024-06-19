#pragma once
#ifndef UVKE_COMPONENT_HEADER
#define UVKE_COMPONENT_HEADER

#include "../uvke.hpp"
#include "../Window/Window.hpp"
#include "Camera.hpp"

namespace uvke {
    class UVKE_API Renderer;

    class UVKE_API Component {
    public:
        Component() = default;
        virtual ~Component() = default;

        virtual void Create(Renderer* renderer) = 0;

    };
};

#endif