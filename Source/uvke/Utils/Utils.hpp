#pragma once
#ifndef UVKE_UTILS_HEADER
#define UVKE_UTILS_HEADER

namespace uvke {
    static const double PI = 3.14159265359;
    static const float ACCURENCY = 0.00000001f;

    template<typename T>
    constexpr float Abs(const T& value) {
        static_assert(std::is_arithmetic_v<T>, "uvke Abs Type Is Not Arithmetic As Expected");
        return value < 0 ? -value : value;
    }

    template<typename T>
    constexpr float Rsqrt(const T& value) {
        static_assert(std::is_arithmetic_v<T>, "uvke Rsqrt Type Is Not Arithmetic As Expected");
        float y = value;
        long x = *(long*)&y;
        x = 0x5f3759df - (x >> 1);
        y = *(float*)&x;
        y = y * (1.5f - ((value * 0.5f) * y * y));
        return y;
    }

    template<typename T>
    constexpr float Sqrt(const T& value) {
        static_assert(std::is_arithmetic_v<T>, "uvke Sqrt Type Is Not Arithmetic As Expected");
        return 1.0f / Rsqrt<T>(value);
    }

    template<typename T>
    constexpr float Sin(const T& value) {
        static_assert(std::is_arithmetic_v<T>, "uvke Sin Type Is Not Arithmetic As Expected");
        float current = value;
        float accurency = 1.0f;
        float factor = 1.0f;
        float power = value;

        for(int i = 1; Abs<float>(accurency) > ACCURENCY && i < 100; ++i) {
            factor *= ((2 * i) * (2 * i + 1));
            power *= -1 * value * value;
            accurency = power / factor;
            current += accurency;
        }

        return current;
    }

    template<typename T>
    constexpr float Cos(const T& value) {
        static_assert(std::is_arithmetic_v<T>, "uvke Cos Type Is Not Arithmetic As Expected");
        float current = 1.0f;
        float accurency = 1.0f;

        for(int i = 1; Abs<float>(accurency / current) > ACCURENCY && i < 100; ++i) {
            accurency = (-accurency * value * value) / ((2 * i - 1) * (2 * i));
            current += accurency;
        }

        return current;
    }

    template<typename T>
    constexpr float Tan(const T& value) {
        static_assert(std::is_arithmetic_v<T>, "uvke Tan Type Is Not Arithmetic As Expected");
        return Sin(value) / Cos(value);
    }

    template<typename T>
    constexpr float Cot(const T& value) {
        static_assert(std::is_arithmetic_v<T>, "uvke Cot Type Is Not Arithmetic As Expected");
        float current = PI - value;
        return Sin(current) / Cos(current);
    }

    template<typename T>
    constexpr float Radians(const T& degrees) {
        return degrees / 180.0f * PI;
    }

    template<typename T>
    constexpr float Degrees(const T& radians) {
        return radians * 180.0f / PI;
    }

    template<typename T>
    constexpr float Lerp(const T& x, const T& y, const T& fraction) {
        return x + (y - x) * fraction;
    }
};

#endif
