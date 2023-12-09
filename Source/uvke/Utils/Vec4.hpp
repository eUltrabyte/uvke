#pragma once
#ifndef UVKE_VEC4_HEADER
#define UVKE_VEC4_HEADER

#include "../uvkepch.hpp"
#include "../Core/Core.hpp"

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
        ~vec4() = default;

        vec4<T> operator+(const T& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { value, value, value, value };
                simd::vec4 result = _mm_add_ps(p, r);
                return vec4<T>(static_cast<T>(result[0]), static_cast<T>(result[1]), static_cast<T>(result[2]), static_cast<T>(result[3]));
            #else
                return vec4<T>(x + value, y + value, z + value, w + value);
            #endif
        }

        vec4<T> operator-(const T& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { value, value, value, value };
                simd::vec4 result = _mm_sub_ps(p, r);
                return vec4<T>(static_cast<T>(result[0]), static_cast<T>(result[1]), static_cast<T>(result[2]), static_cast<T>(result[3]));
            #else
                return vec4<T>(x - value, y - value, z - value, w - value);
            #endif
        }

        vec4<T> operator*(const T& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { value, value, value, value };
                simd::vec4 result = _mm_mul_ps(p, r);
                return vec4<T>(static_cast<T>(result[0]), static_cast<T>(result[1]), static_cast<T>(result[2]), static_cast<T>(result[3]));
            #else
                return vec4<T>(x * value, y * value, z * value, w * value);
            #endif
        }

        vec4<T> operator/(const T& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { value, value, value, value };
                simd::vec4 result = _mm_div_ps(p, r);
                return vec4<T>(static_cast<T>(result[0]), static_cast<T>(result[1]), static_cast<T>(result[2]), static_cast<T>(result[3]));
            #else
                return vec4<T>(x / value, y / value, z / value, w / value);
            #endif
        }

        vec4<T> operator+(const vec4<T>& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { value.x, value.y, value.z, value.w };
                simd::vec4 result = _mm_add_ps(p, r);
                return vec4<T>(static_cast<T>(result[0]), static_cast<T>(result[1]), static_cast<T>(result[2]), static_cast<T>(result[3]));
            #else
                return vec4<T>(x + value.x, y + value.y, z + value.z, w + value.w);
            #endif
        }

        vec4<T> operator-(const vec4<T>& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { value.x, value.y, value.z, value.w };
                simd::vec4 result = _mm_sub_ps(p, r);
                return vec4<T>(static_cast<T>(result[0]), static_cast<T>(result[1]), static_cast<T>(result[2]), static_cast<T>(result[3]));
            #else
                return vec4<T>(x - value.x, y - value.y, z - value.z, w - value.w);
            #endif
        }

        vec4<T> operator*(const vec4<T>& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { value.x, value.y, value.z, value.w };
                simd::vec4 result = _mm_mul_ps(p, r);
                return vec4<T>(static_cast<T>(result[0]), static_cast<T>(result[1]), static_cast<T>(result[2]), static_cast<T>(result[3]));
            #else
                return vec4<T>(x * value.x, y * value.y, z * value.z, w * value.w);
            #endif
        }

        vec4<T> operator/(const vec4<T>& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { value.x, value.y, value.z, value.w };
                simd::vec4 result = _mm_div_ps(p, r);
                return vec4<T>(static_cast<T>(result[0]), static_cast<T>(result[1]), static_cast<T>(result[2]), static_cast<T>(result[3]));
            #else
                return vec4<T>(x / value.x, y / value.y, z / value.z, w / value.w);
            #endif
        }

        vec4<T> operator+=(const T& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { value, value, value, value };
                simd::vec4 result = _mm_add_ps(p, r);
                return vec4<T>(x = static_cast<T>(result[0]), y = static_cast<T>(result[1]), z = static_cast<T>(result[2]), w = static_cast<T>(result[3]));
            #else
                return vec4<T>(x += value, y += value, z += value, w += value);
            #endif
        }

        vec4<T> operator-=(const T& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { value, value, value, value };
                simd::vec4 result = _mm_sub_ps(p, r);
                return vec4<T>(x = static_cast<T>(result[0]), y = static_cast<T>(result[1]), z = static_cast<T>(result[2]), w = static_cast<T>(result[3]));
            #else
                return vec4<T>(x -= value, y -= value, z -= value, w -= value);
            #endif
        }

        vec4<T> operator*=(const T& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { value, value, value, value };
                simd::vec4 result = _mm_mul_ps(p, r);
                return vec4<T>(x = static_cast<T>(result[0]), y = static_cast<T>(result[1]), z = static_cast<T>(result[2]), w = static_cast<T>(result[3]));
            #else
                return vec4<T>(x *= value, y *= value, z *= value, w *= value);
            #endif
        }

        vec4<T> operator/=(const T& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { value, value, value, value };
                simd::vec4 result = _mm_div_ps(p, r);
                return vec4<T>(x = static_cast<T>(result[0]), y = static_cast<T>(result[1]), z = static_cast<T>(result[2]), w = static_cast<T>(result[3]));
            #else
                return vec4<T>(x /= value, y /= value, z /= value, w /= value);
            #endif
        }

        vec4<T> operator+=(const vec4<T>& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { value.x, value.y, value.z, value.w };
                simd::vec4 result = _mm_add_ps(p, r);
                return vec4<T>(x = static_cast<T>(result[0]), y = static_cast<T>(result[1]), z = static_cast<T>(result[2]), w = static_cast<T>(result[3]));
            #else
                return vec4<T>(x += value.x, y += value.y, z += value.z, w += value.w);
            #endif
        }

        vec4<T> operator-=(const vec4<T>& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { value.x, value.y, value.z, value.w };
                simd::vec4 result = _mm_sub_ps(p, r);
                return vec4<T>(x = static_cast<T>(result[0]), y = static_cast<T>(result[1]), z = static_cast<T>(result[2]), w = static_cast<T>(result[3]));
            #else
                return vec4<T>(x -= value.x, y -= value.y, z -= value.z, w -= value.w);
            #endif
        }

        vec4<T> operator*=(const vec4<T>& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { value.x, value.y, value.z, value.w };
                simd::vec4 result = _mm_mul_ps(p, r);
                return vec4<T>(x = static_cast<T>(result[0]), y = static_cast<T>(result[1]), z = static_cast<T>(result[2]), w = static_cast<T>(result[3]));
            #else
                return vec4<T>(x *= value.x, y *= value.y, z *= value.z, w *= value.w);
            #endif
        }

        vec4<T> operator/=(const vec4<T>& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { value.x, value.y, value.z, value.w };
                simd::vec4 result = _mm_div_ps(p, r);
                return vec4<T>(x = static_cast<T>(result[0]), y = static_cast<T>(result[1]), z = static_cast<T>(result[2]), w = static_cast<T>(result[3]));
            #else
                return vec4<T>(x /= value.x, y /= value.y, z /= value.z, w /= value.w);
            #endif
        }

        template<typename U>
        vec4<T> operator+(const U& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { static_cast<T>(value), static_cast<T>(value), static_cast<T>(value), static_cast<T>(value) };
                simd::vec4 result = _mm_add_ps(p, r);
                return vec4<T>(static_cast<T>(result[0]), static_cast<T>(result[1]), static_cast<T>(result[2]), static_cast<T>(result[3]));
            #else
                return vec4<T>(x + static_cast<T>(value), y + static_cast<T>(value), z + static_cast<T>(value), w + static_cast<T>(value));
            #endif
        }

        template<typename U>
        vec4<T> operator-(const U& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { static_cast<T>(value), static_cast<T>(value), static_cast<T>(value), static_cast<T>(value) };
                simd::vec4 result = _mm_sub_ps(p, r);
                return vec4<T>(static_cast<T>(result[0]), static_cast<T>(result[1]), static_cast<T>(result[2]), static_cast<T>(result[3]));
            #else
                return vec4<T>(x - static_cast<T>(value), y - static_cast<T>(value), z - static_cast<T>(value), w - static_cast<T>(value));
            #endif
        }

        template<typename U>
        vec4<T> operator*(const U& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { static_cast<T>(value), static_cast<T>(value), static_cast<T>(value), static_cast<T>(value) };
                simd::vec4 result = _mm_mul_ps(p, r);
                return vec4<T>(static_cast<T>(result[0]), static_cast<T>(result[1]), static_cast<T>(result[2]), static_cast<T>(result[3]));
            #else
                return vec4<T>(x * static_cast<T>(value), y * static_cast<T>(value), z * static_cast<T>(value), w * static_cast<T>(value));
            #endif
        }

        template<typename U>
        vec4<T> operator/(const U& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { static_cast<T>(value), static_cast<T>(value), static_cast<T>(value), static_cast<T>(value) };
                simd::vec4 result = _mm_div_ps(p, r);
                return vec4<T>(static_cast<T>(result[0]), static_cast<T>(result[1]), static_cast<T>(result[2]), static_cast<T>(result[3]));
            #else
                return vec4<T>(x / static_cast<T>(value), y / static_cast<T>(value), z / static_cast<T>(value), w / static_cast<T>(value));
            #endif
        }

        template<typename U>
        vec4<T> operator+(const vec4<U>& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { static_cast<T>(value.x), static_cast<T>(value.y), static_cast<T>(value.z), static_cast<T>(value.w) };
                simd::vec4 result = _mm_add_ps(p, r);
                return vec4<T>(static_cast<T>(result[0]), static_cast<T>(result[1]), static_cast<T>(result[2]), static_cast<T>(result[3]));
            #else
                return vec4<T>(x + static_cast<T>(value.x), y + static_cast<T>(value.y), z + static_cast<T>(value.z), w + static_cast<T>(value.w));
            #endif
        }

        template<typename U>
        vec4<T> operator-(const vec4<U>& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { static_cast<T>(value.x), static_cast<T>(value.y), static_cast<T>(value.z), static_cast<T>(value.w) };
                simd::vec4 result = _mm_sub_ps(p, r);
                return vec4<T>(static_cast<T>(result[0]), static_cast<T>(result[1]), static_cast<T>(result[2]), static_cast<T>(result[3]));
            #else
                return vec4<T>(x - static_cast<T>(value.x), y - static_cast<T>(value.y), z - static_cast<T>(value.z), w - static_cast<T>(value.w));
            #endif
        }

        template<typename U>
        vec4<T> operator*(const vec4<U>& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { static_cast<T>(value.x), static_cast<T>(value.y), static_cast<T>(value.z), static_cast<T>(value.w) };
                simd::vec4 result = _mm_mul_ps(p, r);
                return vec4<T>(static_cast<T>(result[0]), static_cast<T>(result[1]), static_cast<T>(result[2]), static_cast<T>(result[3]));
            #else
                return vec4<T>(x * static_cast<T>(value.x), y * static_cast<T>(value.y), z * static_cast<T>(value.z), w * static_cast<T>(value.w));
            #endif
        }

        template<typename U>
        vec4<T> operator/(const vec4<U>& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { static_cast<T>(value.x), static_cast<T>(value.y), static_cast<T>(value.z), static_cast<T>(value.w) };
                simd::vec4 result = _mm_div_ps(p, r);
                return vec4<T>(static_cast<T>(result[0]), static_cast<T>(result[1]), static_cast<T>(result[2]), static_cast<T>(result[3]));
            #else
                return vec4<T>(x / static_cast<T>(value.x), y / static_cast<T>(value.y), z / static_cast<T>(value.z), w / static_cast<T>(value.w));
            #endif
        }

        template<typename U>
        vec4<T> operator+=(const U& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { static_cast<T>(value), static_cast<T>(value), static_cast<T>(value), static_cast<T>(value) };
                simd::vec4 result = _mm_add_ps(p, r);
                return vec4<T>(x = static_cast<T>(result[0]), y = static_cast<T>(result[1]), z = static_cast<T>(result[2]), w = static_cast<T>(result[3]));
            #else
                return vec4<T>(x += static_cast<T>(value), y += static_cast<T>(value), z += static_cast<T>(value), w += static_cast<T>(value));
            #endif
        }

        template<typename U>
        vec4<T> operator-=(const U& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { static_cast<T>(value), static_cast<T>(value), static_cast<T>(value), static_cast<T>(value) };
                simd::vec4 result = _mm_sub_ps(p, r);
                return vec4<T>(x = static_cast<T>(result[0]), y = static_cast<T>(result[1]), z = static_cast<T>(result[2]), w = static_cast<T>(result[3]));
            #else
                return vec4<T>(x -= static_cast<T>(value), y -= static_cast<T>(value), z -= static_cast<T>(value), w -= static_cast<T>(value));
            #endif
        }

        template<typename U>
        vec4<T> operator*=(const U& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { static_cast<T>(value), static_cast<T>(value), static_cast<T>(value), static_cast<T>(value) };
                simd::vec4 result = _mm_mul_ps(p, r);
                return vec4<T>(x = static_cast<T>(result[0]), y = static_cast<T>(result[1]), z = static_cast<T>(result[2]), w = static_cast<T>(result[3]));
            #else
                return vec4<T>(x *= static_cast<T>(value), y *= static_cast<T>(value), z *= static_cast<T>(value), w *= static_cast<T>(value));
            #endif
        }

        template<typename U>
        vec4<T> operator/=(const U& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { static_cast<T>(value), static_cast<T>(value), static_cast<T>(value), static_cast<T>(value) };
                simd::vec4 result = _mm_div_ps(p, r);
                return vec4<T>(x = static_cast<T>(result[0]), y = static_cast<T>(result[1]), z = static_cast<T>(result[2]), w = static_cast<T>(result[3]));
            #else
                return vec4<T>(x /= static_cast<T>(value), y /= static_cast<T>(value), z /= static_cast<T>(value), w /= static_cast<T>(value));
            #endif
        }

        template<typename U>
        vec4<T> operator+=(const vec4<U>& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { static_cast<T>(value.x), static_cast<T>(value.y), static_cast<T>(value.z), static_cast<T>(value.w) };
                simd::vec4 result = _mm_add_ps(p, r);
                return vec4<T>(x = static_cast<T>(result[0]), y = static_cast<T>(result[1]), z = static_cast<T>(result[2]), w = static_cast<T>(result[3]));
            #else
                return vec4<T>(x += static_cast<T>(value.x), y += static_cast<T>(value.y), z += static_cast<T>(value.z), w += static_cast<T>(value.w));
            #endif
        }

        template<typename U>
        vec4<T> operator-=(const vec4<U>& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { static_cast<T>(value.x), static_cast<T>(value.y), static_cast<T>(value.z), static_cast<T>(value.w) };
                simd::vec4 result = _mm_sub_ps(p, r);
                return vec4<T>(x = static_cast<T>(result[0]), y = static_cast<T>(result[1]), z = static_cast<T>(result[2]), w = static_cast<T>(result[3]));
            #else
                return vec4<T>(x -= static_cast<T>(value.x), y -= static_cast<T>(value.y), z -= static_cast<T>(value.z), w -= static_cast<T>(value.w));
            #endif
        }

        template<typename U>
        vec4<T> operator*=(const vec4<U>& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { static_cast<T>(value.x), static_cast<T>(value.y), static_cast<T>(value.z), static_cast<T>(value.w) };
                simd::vec4 result = _mm_mul_ps(p, r);
                return vec4<T>(x = static_cast<T>(result[0]), y = static_cast<T>(result[1]), z = static_cast<T>(result[2]), w = static_cast<T>(result[3]));
            #else
                return vec4<T>(x *= static_cast<T>(value.x), y *= static_cast<T>(value.y), z *= static_cast<T>(value.z), w *= static_cast<T>(value.w));
            #endif
        }

        template<typename U>
        vec4<T> operator/=(const vec4<U>& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { static_cast<T>(value.x), static_cast<T>(value.y), static_cast<T>(value.z), static_cast<T>(value.w) };
                simd::vec4 result = _mm_div_ps(p, r);
                return vec4<T>(x = static_cast<T>(result[0]), y = static_cast<T>(result[1]), z = static_cast<T>(result[2]), w = static_cast<T>(result[3]));
            #else
                return vec4<T>(x /= static_cast<T>(value.x), y /= static_cast<T>(value.y), z /= static_cast<T>(value.z), w /= static_cast<T>(value.w));
            #endif
        }
    };

    using vec4f = vec4<float>;
    using vec4i = vec4<int>;
    using vec4u = vec4<unsigned>;
    using vec4d = vec4<double>;
    using vec4l = vec4<long>;

    template<typename T>
    inline constexpr T Length(const vec4<T>& vec) {
        #ifdef UVKE_MATH_USE_SIMD
            simd::vec4 p = { vec.x, vec.y, vec.z, vec.w };
            simd::vec4 r = { vec.x, vec.y, vec.z, vec.w };
            simd::vec4 multiplied = _mm_mul_ps(p, r);
            return Sqrt<T>(multiplied[0] + multiplied[1] + multiplied[2] + multiplied[3]);
        #else
            return Sqrt<T>((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z) + (vec.w * vec.w));
        #endif
    }

    template<typename T>
    inline constexpr vec4<T> Normalize(const vec4<T>& vec) {
        T length = Length(vec);

        #ifdef UVKE_MATH_USE_SIMD
            simd::vec4 p = { vec.x, vec.y, vec.z, vec.w };
            simd::vec4 r = { length, length, length, length };
            simd::vec4 divided = _mm_div_ps(p, r);
            return vec4<T>(divided[0], divided[1], divided[2], divided[3]);
        #else
            return vec4<T>(vec.x / length, vec.y / length, vec.z / length, vec.w / length);
        #endif
    }
};

#endif