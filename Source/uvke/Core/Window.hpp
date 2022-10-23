#pragma once
#ifndef UVKE_WINDOW_HEADER
#define UVKE_WINDOW_HEADER

#include "../uvke.hpp"

namespace uvke {
    struct UVKE_API WindowProps {
    public:
        std::string title;
        vec2u size;
        unsigned style;

        WindowProps(std::string_view a = "uvke window", const vec2u& b = vec2u(1280, 720), unsigned c = 0) : title(a), size(b), style(c) {  }
    };

    class UVKE_API Window {
    public:

    private:
    };
};

#endif