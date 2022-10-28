#include "Clock.hpp"

namespace uvke {
    Clock::Clock() : m_start(std::chrono::steady_clock::now()) {  }

    std::chrono::time_point<std::chrono::steady_clock> Clock::GetStart() {
        return m_start;
    }
    
    std::chrono::time_point<std::chrono::steady_clock> Clock::GetTime() {
        return std::chrono::steady_clock::now();
    }
};