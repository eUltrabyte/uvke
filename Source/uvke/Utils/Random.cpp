#include "Random.hpp"

namespace uvke {
    Random::Random() {
        m_engine.seed(std::random_device()());
    }

    float Random::Get() {
        return m_uniformDistribution(m_engine) / 4294967295.0f;
    }
    
    uint32_t Random::Get(uint32_t min, uint32_t max) {
        return min + (m_uniformDistribution(m_engine) % (max - min + 1));
    }

    std::mt19937 Random::GetEngine() {
        return m_engine;
    }
    
    std::uniform_int_distribution<std::mt19937::result_type> Random::GetUniformDistribution() {
        return m_uniformDistribution;
    }
};