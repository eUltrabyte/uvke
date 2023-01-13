#pragma once
#ifndef UVKE_UNIFORM_BUFFER_HEADER
#define UVKE_UNIFORM_BUFFER_HEADER

#include "../uvke.hpp"
#include "Shader.hpp"

namespace uvke {
    struct UniformBufferObject {
        mat4x4f model;
        mat4x4f view;
        mat4x4f projection;
    };

    class UVKE_API UniformBuffer {
    public:

    protected:
        

    private:

    };
};

#endif