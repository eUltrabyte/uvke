#pragma once
#ifndef UVKE_CLOCK_HEADER
#define UVKE_CLOCK_HEADER

#include "../uvke.hpp"

namespace uvke {
    class UVKE_API Clock {
    public:
        Clock();
        virtual ~Clock() = default;

        virtual void Restart();
        virtual void Sleep(unsigned int time);

        virtual std::chrono::time_point<std::chrono::steady_clock> GetStart();
        virtual std::chrono::milliseconds GetElapsedTime();
        virtual std::time_t GetTime();
        virtual float GetDeltaTime();

    private:
        std::chrono::time_point<std::chrono::steady_clock> m_start;

    };
};

#endif