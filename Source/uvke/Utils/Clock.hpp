#pragma once
#ifndef UVKE_CLOCK_HEADER
#define UVKE_CLOCK_HEADER

#include "../uvke.hpp"

namespace uvke {
    class UVKE_API Clock {
    public:
        Clock();
        virtual ~Clock();

        virtual std::chrono::time_point<std::chrono::steady_clock> GetStart();
        virtual std::chrono::time_point<std::chrono::steady_clock> GetTime();

    private:
        std::chrono::time_point<std::chrono::steady_clock> m_start;

    };
};

#endif