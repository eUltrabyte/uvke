#pragma once
#ifndef UVKE_ENTRY_HEADER
#define UVKE_ENTRY_HEADER

#include "../uvke.hpp"
#include "WindowManager.hpp"

namespace uvke {
    using App = char;
};

extern uvke::App* Create(int argc, char** argv);

namespace uvke {
    namespace priv {
        bool IsRunning = true;

        auto Main(int argc, char** argv) -> int {
            uvke::Core core;
            uvke::WindowManager windowManager;

            for(; IsRunning ;) {
                App* app = Create(argc, argv);
                delete app;
            }

            return 0;
        }
    };
};

auto main(int argc, char** argv) -> int { return uvke::priv::Main(argc, argv); }

#endif