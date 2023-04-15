#pragma once
#ifndef UVKE_RENDERABLE_HEADER
#define UVKE_RENDERABLE_HEADER

#include "../uvke.hpp"

namespace uvke {
    class UVKE_API Renderer;

    class UVKE_API Renderable {
    public:
        Renderable();
        virtual ~Renderable();

        virtual void Create(std::shared_ptr<Renderer> renderer) = 0;

    };
};

#endif