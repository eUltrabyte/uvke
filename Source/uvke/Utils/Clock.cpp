#include "Clock.hpp"

namespace uvke {
    Clock::Clock()
        : m_start(std::chrono::steady_clock::now()) {  }

    void Clock::Restart() {
        m_start = std::chrono::steady_clock::now();
    }

    void Clock::Sleep(unsigned int time) {
        std::this_thread::sleep_for(std::chrono::milliseconds(time));
    }

    std::chrono::time_point<std::chrono::steady_clock> Clock::GetStart() {
        return m_start;
    }

    std::chrono::milliseconds Clock::GetElapsedTime() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_start);
    }
    
    std::time_t Clock::GetTime() {
        return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    }

    float Clock::GetDeltaTime() {
        return std::chrono::duration<float, std::milli>(std::chrono::steady_clock::now() - m_start).count();
    }
};