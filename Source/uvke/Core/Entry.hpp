#pragma once
#ifndef UVKE_ENTRY_HEADER
#define UVKE_ENTRY_HEADER

namespace uvke {
    using App = char;
};

extern uvke::App* Create(int argc, char** argv);

namespace uvke {
    namespace priv {
        bool IsRunning = true;

        auto Main(int argc, char** argv) -> int {
            uvke::priv::Init();

            for(; IsRunning ;) {
                App* app = Create(argc, argv);
                delete app;
            }
            
            return uvke::priv::Deinit();;
        }
    };
};

auto main(int argc, char** argv) -> int { return uvke::priv::Main(argc, argv); }

#endif