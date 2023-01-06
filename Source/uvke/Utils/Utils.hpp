#pragma once
#ifndef UVKE_UTILS_HEADER
#define UVKE_UTILS_HEADER

namespace uvke {
    static const double PI = 3.14159265359;

    template<typename T>
    float Abs(T value) {
        static_assert(std::is_arithmetic_v<T>, "uvke Abs Type Is Not Arithmetic As Expected");
        return value < 0 ? -value : value;
    }

    template<typename T>
    float Rsqrt(T value) {
        static_assert(std::is_arithmetic_v<T>, "uvke Rsqrt Type Is Not Arithmetic As Expected");
        float y = value;
        long x = *(long*)&y;
        x = 0x5f3759df - (x >> 1);
        y = *(float*)&x;
        y = y * (1.5f - ((value * 0.5f) * y * y));
        return y;
    }

    template<typename T>
    float Sqrt(T value) {
        static_assert(std::is_arithmetic_v<T>, "uvke Sqrt Type Is Not Arithmetic As Expected");
        return 1.0f / Rsqrt<T>(value);
    }

    template<typename T>
    float Sin(T value) {
        static_assert(std::is_arithmetic_v<T>, "uvke Sin Type Is Not Arithmetic As Expected");
        float current = value;
        float accurency = 1.0f;
        float factor = 1.0f;
        float power = value;

        for(int i = 1; Abs<float>(accurency) > 0.00000001f && i < 100; ++i) {
            factor *= ((2 * i) * (2 * i + 1));
            power *= -1 * value * value;
            accurency = power / factor;
            current += accurency;
        }

        return current;
    }

    template<typename T>
    float Cos(T value) {
        static_assert(std::is_arithmetic_v<T>, "uvke Cos Type Is Not Arithmetic As Expected");
        float current = 1.0f;
        float accurency = 1.0f;

        for(int i = 1; Abs<float>(accurency / current) > 0.00000001f && i < 100; ++i) {
            accurency = (-accurency * value * value) / ((2 * i - 1) * (2 * i));
            current += accurency;
        }

        return current;
    }

    template<typename T>
    float Tan(T value) {
        static_assert(std::is_arithmetic_v<T>, "uvke Tan Type Is Not Arithmetic As Expected");
        return Sin(value) / Cos(value);
    }

    template<typename T>
    float Cot(T value) {
        static_assert(std::is_arithmetic_v<T>, "uvke Cot Type Is Not Arithmetic As Expected");
        float current = PI - value;
        return Sin(current) / Cos(current);
    }
};

#endif
