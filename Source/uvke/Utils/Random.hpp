#pragma once
#ifndef UVKE_RANDOM_HEADER
#define UVKE_RANDOM_HEADER

#include "../uvke.hpp"

namespace uvke {
    class UVKE_API Random {
    public:
        Random();
        virtual ~Random() = default;

        virtual float Get();
        virtual uint32_t Get(uint32_t min, uint32_t max);

        virtual std::mt19937 GetEngine();
        virtual std::uniform_int_distribution<std::mt19937::result_type> GetUniformDistribution();

    private:
        std::mt19937 m_engine;
		std::uniform_int_distribution<std::mt19937::result_type> m_uniformDistribution;

    };
};

#endif