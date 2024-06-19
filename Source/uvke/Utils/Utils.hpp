#pragma once
#ifndef UVKE_UTILS_HEADER
#define UVKE_UTILS_HEADER

#include "../uvkepch.hpp"

#ifdef UVKE_MATH_USE_SIMD
    #include <immintrin.h>

    namespace uvke {
        namespace simd {
            #ifdef UVKE_MATH_USE_SIMD
                using vec4 = __m128;
            #endif
        };
    };
#endif

namespace uvke {
    namespace priv {
        inline static const float PI = 3.141592653f;
        inline static const float ONEQTRPI = 0.7853981632f;
        inline static const float THRQTRPI = 2.3561944897f;
        inline static const float ACCURENCY = 0.00000001f;
    };

    template<typename T>
    inline constexpr float Abs(const T& value) {
        static_assert(std::is_arithmetic_v<T>, "uvke abs type is not arithmetic as expected");
        return value < 0 ? -value : value;
    }

    template<typename T>
    inline constexpr float Pow(const T& value, int power) {
        static_assert(std::is_arithmetic_v<T>, "uvke pow type is not arithmetic as expected");
        if(power == 0) { return 1.0f; }

        float current = Pow(value, power / 2);
        if(power % 2 == 0) {
            return current * current;
        } else {
            if(power > 0) {
                return value * power * power;
            } else {
                return (power * power) / value;
            }
        }
    }

    template<typename T>
    inline constexpr float Rsqrt(const T& value) {
        static_assert(std::is_arithmetic_v<T>, "uvke rsqrt type is not arithmetic as expected");
        float current = value;
        long x = *(long*)&current;
        x = 0x5f3759df - (x >> 1);
        current = *(float*)&x;
        current = current * (1.5f - ((value * 0.5f) * current * current));
        return current;
    }

    template<typename T>
    inline constexpr float Sqrt(const T& value) {
        static_assert(std::is_arithmetic_v<T>, "uvke sqrt type is not arithmetic as expected");
        return 1.0f / Rsqrt<T>(value);
    }

    template<typename T>
    inline constexpr float Sin(const T& value) {
        static_assert(std::is_arithmetic_v<T>, "uvke sin type is not arithmetic as expected");
        float current = value;
        float accurency = 1.0f;
        float factor = 1.0f;
        float power = value;

        for(int i = 1; Abs<float>(accurency) > priv::ACCURENCY && i < 100; ++i) {
            factor *= ((2 * i) * (2 * i + 1));
            power *= -1 * value * value;
            accurency = power / factor;
            current += accurency;
        }

        return current;
    }

    template<typename T>
    inline constexpr float Arcsin(const T& value) {
        static_assert(std::is_arithmetic_v<T>, "uvke arcsin type is not arithmetic as expected");
        return Pow(Sin(value), -1);
    }

    template<typename T>
    inline constexpr float Cos(const T& value) {
        static_assert(std::is_arithmetic_v<T>, "uvke cos type is not arithmetic as expected");
        float current = 1.0f;
        float accurency = 1.0f;

        for(int i = 1; Abs<float>(accurency / current) > priv::ACCURENCY && i < 100; ++i) {
            accurency = (-accurency * value * value) / ((2 * i - 1) * (2 * i));
            current += accurency;
        }

        return current;
    }

    template<typename T>
    inline constexpr float Arccos(const T& value) {
        static_assert(std::is_arithmetic_v<T>, "uvke arccos type is not arithmetic as expected");
        return Pow(Cos(value), -1);
    }

    template<typename T>
    inline constexpr float Tan(const T& value) {
        static_assert(std::is_arithmetic_v<T>, "uvke tan type is not arithmetic as expected");
        return Sin(value) / Cos(value);
    }

    template<typename T>
    inline constexpr float Arctan(const T& value) {
        static_assert(std::is_arithmetic_v<T>, "uvke arctan type is not arithmetic as expected");
        return Pow(Tan(value), -1);
    }

    template<typename T>
    inline constexpr float Cot(const T& value) {
        static_assert(std::is_arithmetic_v<T>, "uvke cot type is not arithmetic as expected");
        float current = priv::PI - value;
        return Sin(current) / Cos(current);
    }

    template<typename T>
    inline constexpr float Arccot(const T& value) {
        static_assert(std::is_arithmetic_v<T>, "uvke arccot type is not arithmetic as expected");
        return Pow(Cot(value), -1);
    }

    template<typename T>
    inline constexpr float Radians(const T& degrees) {
        static_assert(std::is_arithmetic_v<T>, "uvke radians type is not arithmetic as expected");
        return degrees / 180.0f * priv::PI;
    }

    template<typename T>
    inline constexpr float Degrees(const T& radians) {
        static_assert(std::is_arithmetic_v<T>, "uvke degrees type is not arithmetic as expected");
        return radians * 180.0f / priv::PI;
    }

    template<typename T, typename U, typename O>
    inline constexpr float Lerp(const T& x, const U& y, const O& fraction) {
        static_assert(std::is_arithmetic_v<T>, "uvke lerp <T> type is not arithmetic as expected");
        static_assert(std::is_arithmetic_v<U>, "uvke lerp <U> type is not arithmetic as expected");
        static_assert(std::is_arithmetic_v<O>, "uvke lerp <O> type is not arithmetic as expected");
        return x + (static_cast<T>(y) - x) * static_cast<T>(fraction);
    }

    template<typename T>
    inline constexpr float Atan(const T& x, const T& y) {
        static_assert(std::is_arithmetic_v<T>, "uvke atan type is not arithmetic as expected");
        float angle = 0.0f;
        float z = 0.0f;
        float current = Abs<float>(x) + 1e-10f;
        if(y < 0.0f) {
            z = (y + current) / (y - current);
            angle = priv::THRQTRPI;
        } else {
            z = (y - current) / (y + current);
            angle = priv::ONEQTRPI;
        }

        angle += (0.1963f * z * z - 0.9817f) * z;
        if(x < 0.0f) {
            return -angle;
        } else {
            return angle;
        }
    }

    inline constexpr void CombineHash(size_t& seed, size_t hash) {
		hash += 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= hash;
        return;
	}
};

#endif
