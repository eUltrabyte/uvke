#pragma once
#ifndef UVKE_VEC2_HEADER
#define UVKE_VEC2_HEADER

#include "../uvkepch.hpp"

namespace uvke {
    template<typename T>
    struct UVKE_API vec2 {
    public:
        static_assert(std::is_arithmetic_v<T>, "uvke vec2 type is not arithmetic as required");
        T x, y;

        vec2() : x(0), y(0) {  }
        vec2(T a) : x(a), y(a) {  }
        vec2(T a, T b) : x(a), y(b) {  }
        vec2(const vec2<T>& vec) : x(vec.x), y(vec.y) {  }
        template<typename U>
        vec2(U a) : x(static_cast<T>(a)), y(static_cast<T>(a)) {  }
        template<typename U>
        vec2(U a, U b) : x(static_cast<T>(a)), y(static_cast<T>(b)) {  }
        template<typename U>
        vec2(const vec2<U>& vec) : x(static_cast<T>(vec.x)), y(static_cast<T>(vec.y)) {  }

        vec2<T> operator+=(const T& value) {
            return vec2<T>(x += value, y += value);
        }

        vec2<T> operator-=(const T& value) {
            return vec2<T>(x -= value, y -= value);
        }

        vec2<T> operator*=(const T& value) {
            return vec2<T>(x *= value, y *= value);
        }

        vec2<T> operator/=(const T& value) {
            return vec2<T>(x /= value, y /= value);
        }

        vec2<T> operator+=(const vec2<T>& value) {
            return vec2<T>(x += value.x, y += value.y);
        }

        vec2<T> operator-=(const vec2<T>& value) {
            return vec2<T>(x -= value.x, y -= value.y);
        }

        vec2<T> operator*=(const vec2<T>& value) {
            return vec2<T>(x *= value.x, y *= value.y);
        }

        vec2<T> operator/=(const vec2<T>& value) {
            return vec2<T>(x /= value.x, y /= value.y);
        }

        template<typename U>
        vec2<T> operator+=(const U& value) {
            return vec2<T>(x += static_cast<T>(value), y += static_cast<T>(value));
        }

        template<typename U>
        vec2<T> operator-=(const U& value) {
            return vec2<T>(x -= static_cast<T>(value), y -= static_cast<T>(value));
        }

        template<typename U>
        vec2<T> operator*=(const U& value) {
            return vec2<T>(x *= static_cast<T>(value), y *= static_cast<T>(value));
        }

        template<typename U>
        vec2<T> operator/=(const U& value) {
            return vec2<T>(x /= static_cast<T>(value), y /= static_cast<T>(value));
        }

        template<typename U>
        vec2<T> operator+=(const vec2<U>& value) {
            return vec2<T>(x += static_cast<T>(value.x), y += static_cast<T>(value.y));
        }

        template<typename U>
        vec2<T> operator-=(const vec2<U>& value) {
            return vec2<T>(x -= static_cast<T>(value.x), y -= static_cast<T>(value.y));
        }

        template<typename U>
        vec2<T> operator*=(const vec2<U>& value) {
            return vec2<T>(x *= static_cast<T>(value.x), y *= static_cast<T>(value.y));
        }

        template<typename U>
        vec2<T> operator/=(const vec2<U>& value) {
            return vec2<T>(x /= static_cast<T>(value.x), y /= static_cast<T>(value.y));
        }
    };

    using vec2f = vec2<float>;
    using vec2i = vec2<int>;
    using vec2u = vec2<unsigned>;
    using vec2d = vec2<double>;
    using vec2l = vec2<long>;

    template<typename T>
    constexpr T Length(const vec2<T>& vec) {
        return Sqrt<T>((vec.x * vec.x) + (vec.y * vec.y));
    }

    template<typename T>
    constexpr vec2<T> Normalize(const vec2<T>& vec) {
        T length = Length(vec);
        return vec2<T>(vec.x / length, vec.y / length);
    }

    template<typename T>
    constexpr T DotProduct(const vec2<T>& x, const vec2<T>& y) {
        return x.x * y.x + x.y * y.y;
    }

    template<typename T>
    constexpr vec2<T> CrossProduct(const vec2<T>& x, const vec2<T>& y) {
        return vec2<T>(x.x * y.y - x.y * y.x, x.y * y.x - x.x * y.y);
    }

    template<typename T, typename U>
    constexpr T DotProduct(const vec2<T>& x, const vec2<U>& y) {
        return x.x * y.x + x.y * y.y;
    }

    template<typename T, typename U>
    constexpr vec2<T> CrossProduct(const vec2<T>& x, const vec2<U>& y) {
        return vec2<T>(x.x * y.y - x.y * y.x, x.y * y.x - x.x * y.y);
    }
};

#endif