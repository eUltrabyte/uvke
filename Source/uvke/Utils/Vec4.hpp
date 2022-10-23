#pragma once
#ifndef UVKE_VEC4_HEADER
#define UVKE_VEC4_HEADER

#include "../uvkepch.hpp"

namespace uvke {
    template<typename T>
    struct UVKE_API vec4 {
    public:
        static_assert(std::is_arithmetic_v<T>, "uvke vec4 type is not arithmetic as required");
        T x, y, z, w;

        vec4() : x(0), y(0), z(0), w(0) {  }
        vec4(T a) : x(a), y(a), z(a), w(a) {  }
        vec4(T a, T b, T c, T d) : x(a), y(b), z(c), w(d) {  }
        vec4(const vec4<T>& vec) : x(vec.x), y(vec.y), z(vec.z), w(vec.w) {  }
        template<typename U>
        vec4(U a) : x(static_cast<T>(a)), y(static_cast<T>(a)), z(static_cast<T>(a)), w(static_cast<T>(a)) {  }
        template<typename U>
        vec4(U a, U b, U c, U d) : x(static_cast<T>(a)), y(static_cast<T>(b)), z(static_cast<T>(c)), w(static_cast<T>(d)) {  }
        template<typename U>
        vec4(const vec4<U>& vec) : x(static_cast<T>(vec.x)), y(static_cast<T>(vec.y)), z(static_cast<T>(vec.z)), w(static_cast<T>(vec.w)) {  }

        vec4<T> operator+=(const T& value) {
            return vec4<T>(x += value, y += value, z += value, w += value);
        }

        vec4<T> operator-=(const T& value) {
            return vec4<T>(x -= value, y -= value, z -= value, w -= value);
        }

        vec4<T> operator*=(const T& value) {
            return vec4<T>(x *= value, y *= value, z *= value, w *= value);
        }

        vec4<T> operator/=(const T& value) {
            return vec4<T>(x /= value, y /= value, z /= value, w /= value);
        }

        vec4<T> operator+=(const vec4<T>& value) {
            return vec4<T>(x += value.x, y += value.y, z += value.z, w += value.w);
        }

        vec4<T> operator-=(const vec4<T>& value) {
            return vec4<T>(x -= value.x, y -= value.y, z -= value.z, w -= value.w);
        }

        vec4<T> operator*=(const vec4<T>& value) {
            return vec4<T>(x *= value.x, y *= value.y, z *= value.z, w *= value.w);
        }

        vec4<T> operator/=(const vec4<T>& value) {
            return vec4<T>(x /= value.x, y /= value.y, z /= value.z, w /= value.w);
        }

        template<typename U>
        vec4<T> operator+=(const U& value) {
            return vec4<T>(x += static_cast<T>(value), y += static_cast<T>(value), z += static_cast<T>(value), w += static_cast<T>(value));
        }

        template<typename U>
        vec4<T> operator-=(const U& value) {
            return vec4<T>(x -= static_cast<T>(value), y -= static_cast<T>(value), z -= static_cast<T>(value), w -= static_cast<T>(value));
        }

        template<typename U>
        vec4<T> operator*=(const U& value) {
            return vec4<T>(x *= static_cast<T>(value), y *= static_cast<T>(value), z *= static_cast<T>(value), w *= static_cast<T>(value));
        }

        template<typename U>
        vec4<T> operator/=(const U& value) {
            return vec4<T>(x /= static_cast<T>(value), y /= static_cast<T>(value), z /= static_cast<T>(value), w /= static_cast<T>(value));
        }

        template<typename U>
        vec4<T> operator+=(const vec4<U>& value) {
            return vec4<T>(x += static_cast<T>(value.x), y += static_cast<T>(value.y), z += static_cast<T>(value.z), w += static_cast<T>(value.w));
        }

        template<typename U>
        vec4<T> operator-=(const vec4<U>& value) {
            return vec4<T>(x -= static_cast<T>(value.x), y -= static_cast<T>(value.y), z -= static_cast<T>(value.z), w -= static_cast<T>(value.w));
        }

        template<typename U>
        vec4<T> operator*=(const vec4<U>& value) {
            return vec4<T>(x *= static_cast<T>(value.x), y *= static_cast<T>(value.y), z *= static_cast<T>(value.z), w *= static_cast<T>(value.w));
        }

        template<typename U>
        vec4<T> operator/=(const vec4<U>& value) {
            return vec4<T>(x /= static_cast<T>(value.x), y /= static_cast<T>(value.y), z /= static_cast<T>(value.z), w /= static_cast<T>(value.w));
        }
    };

    using vec4f = vec4<float>;
    using vec4i = vec4<int>;
    using vec4u = vec4<unsigned>;
    using vec4d = vec4<double>;
    using vec4l = vec4<long>;
};

#endif