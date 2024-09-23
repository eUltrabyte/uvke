#pragma once
#ifndef UVKE_QUATERNION_HEADER
#define UVKE_QUATERNION_HEADER

#include "../uvkepch.hpp"
#include "../Core/Core.hpp"
#include "Vec3.hpp"
#include "Mat4x4.hpp"

namespace uvke {
    template<typename T>
    struct UVKE_API quat {
    public:
        static_assert(std::is_arithmetic_v<T>, "uvke quat type is not arithmetic as required");
        T x, y, z, w;

        quat() : x(0), y(0), z(0), w(0) {  }
        quat(T a) : x(a), y(a), z(a), w(a) {  }

        quat(T a, T b, T c) {
            T angle = a * 0.5;
            vec2<T> p = { Sin<T>(angle), Cos<T>(angle) };

            angle = b * 0.5;
            vec2<T> r = { Sin<T>(angle), Cos<T>(angle) };

            angle = c * 0.5;
            vec2<T> q = { Sin<T>(angle), Cos<T>(angle) };

            vec4<T> value = {
                r.y * q.y,
                r.x * q.y,
                r.y * q.x,
                r.x * q.x,
            };

            vec4<T> result = {
                p.x * value.x - p.y * value.w,
                p.y * value.y + p.x * value.z,
                p.y * value.z - p.x * value.y,
                p.y * value.x + p.x * value.w
            };

            vec4<T> normalized = Normalize<T>(result);

            x = normalized.x;
            y = normalized.y;
            z = normalized.z;
            w = normalized.w;
        }

        quat(T a, T b, T c, T d) : x(a), y(b), z(c), w(d) {  }
        quat(const quat<T>& value) : x(value.x), y(value.y), z(value.z), w(value.w) {  }
        template<typename U>
        quat(U a) : x(static_cast<T>(a)), y(static_cast<T>(a)), z(static_cast<T>(a)), w(static_cast<T>(a)) {  }
        template<typename U>
        quat(U a, U b, U c, U d) : x(static_cast<T>(a)), y(static_cast<T>(b)), z(static_cast<T>(c)), w(static_cast<T>(d)) {  }
        template<typename U>
        quat(const quat<U>& value) : x(static_cast<T>(value.x)), y(static_cast<T>(value.y)), z(static_cast<T>(value.z)), w(static_cast<T>(value.w)) {  }
        ~quat() = default;

        quat<T> operator+(const T& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { value, value, value, value };
                simd::vec4 result = _mm_add_ps(p, r);

                return quat<T>(static_cast<T>(result[0]), static_cast<T>(result[1]), static_cast<T>(result[2]), static_cast<T>(result[3]));
            #else
                return quat<T>(x + value, y + value, z + value, w + value);
            #endif
        }

        quat<T> operator-(const T& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { value, value, value, value };
                simd::vec4 result = _mm_sub_ps(p, r);

                return quat<T>(static_cast<T>(result[0]), static_cast<T>(result[1]), static_cast<T>(result[2]), static_cast<T>(result[3]));
            #else
                return quat<T>(x - value, y - value, z - value, w - value);
            #endif
        }

        quat<T> operator*(const T& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { value, value, value, value };
                simd::vec4 result = _mm_mul_ps(p, r);

                return quat<T>(static_cast<T>(result[0]), static_cast<T>(result[1]), static_cast<T>(result[2]), static_cast<T>(result[3]));
            #else
                return quat<T>(x * value, y * value, z * value, w * value);
            #endif
        }

        quat<T> operator/(const T& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { value, value, value, value };
                simd::vec4 result = _mm_div_ps(p, r);

                return quat<T>(static_cast<T>(result[0]), static_cast<T>(result[1]), static_cast<T>(result[2]), static_cast<T>(result[3]));
            #else
                return quat<T>(x / value, y / value, z / value, w / value);
            #endif
        }

        quat<T> operator+(const quat<T>& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { value.x, value.y, value.z, value.w };
                simd::vec4 result = _mm_add_ps(p, r);

                return quat<T>(static_cast<T>(result[0]), static_cast<T>(result[1]), static_cast<T>(result[2]), static_cast<T>(result[3]));
            #else
                return quat<T>(x + value.x, y + value.y, z + value.z, w + value.w);
            #endif
        }

        quat<T> operator-(const quat<T>& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { value.x, value.y, value.z, value.w };
                simd::vec4 result = _mm_sub_ps(p, r);

                return quat<T>(static_cast<T>(result[0]), static_cast<T>(result[1]), static_cast<T>(result[2]), static_cast<T>(result[3]));
            #else
                return quat<T>(x - value.x, y - value.y, z - value.z, w - value.w);
            #endif
        }

        quat<T> operator*(const quat<T>& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { value.x, value.y, value.z, value.w };
                simd::vec4 result = _mm_mul_ps(p, r);

                return quat<T>(static_cast<T>(result[0]), static_cast<T>(result[1]), static_cast<T>(result[2]), static_cast<T>(result[3]));
            #else
                return quat<T>(x * value.x, y * value.y, z * value.z, w * value.w);
            #endif
        }

        quat<T> operator/(const quat<T>& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { value.x, value.y, value.z, value.w };
                simd::vec4 result = _mm_div_ps(p, r);

                return quat<T>(static_cast<T>(result[0]), static_cast<T>(result[1]), static_cast<T>(result[2]), static_cast<T>(result[3]));
            #else
                return quat<T>(x / value.x, y / value.y, z / value.z, w / value.w);
            #endif
        }

        quat<T> operator+=(const T& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { value, value, value, value };
                simd::vec4 result = _mm_add_ps(p, r);

                return quat<T>(x = static_cast<T>(result[0]), y = static_cast<T>(result[1]), z = static_cast<T>(result[2]), w = static_cast<T>(result[3]));
            #else
                return quat<T>(x += value, y += value, z += value, w += value);
            #endif
        }

        quat<T> operator-=(const T& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { value, value, value, value };
                simd::vec4 result = _mm_sub_ps(p, r);

                return quat<T>(x = static_cast<T>(result[0]), y = static_cast<T>(result[1]), z = static_cast<T>(result[2]), w = static_cast<T>(result[3]));
            #else
                return quat<T>(x -= value, y -= value, z -= value, w -= value);
            #endif
        }

        quat<T> operator*=(const T& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { value, value, value, value };
                simd::vec4 result = _mm_mul_ps(p, r);

                return quat<T>(x = static_cast<T>(result[0]), y = static_cast<T>(result[1]), z = static_cast<T>(result[2]), w = static_cast<T>(result[3]));
            #else
                return quat<T>(x *= value, y *= value, z *= value, w *= value);
            #endif
        }

        quat<T> operator/=(const T& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { value, value, value, value };
                simd::vec4 result = _mm_div_ps(p, r);

                return quat<T>(x = static_cast<T>(result[0]), y = static_cast<T>(result[1]), z = static_cast<T>(result[2]), w = static_cast<T>(result[3]));
            #else
                return quat<T>(x /= value, y /= value, z /= value, w /= value);
            #endif
        }

        quat<T> operator+=(const quat<T>& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { value.x, value.y, value.z, value.w };
                simd::vec4 result = _mm_add_ps(p, r);

                return quat<T>(x = static_cast<T>(result[0]), y = static_cast<T>(result[1]), z = static_cast<T>(result[2]), w = static_cast<T>(result[3]));
            #else
                return quat<T>(x += value.x, y += value.y, z += value.z, w += value.w);
            #endif
        }

        quat<T> operator-=(const quat<T>& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { value.x, value.y, value.z, value.w };
                simd::vec4 result = _mm_sub_ps(p, r);

                return quat<T>(x = static_cast<T>(result[0]), y = static_cast<T>(result[1]), z = static_cast<T>(result[2]), w = static_cast<T>(result[3]));
            #else
                return quat<T>(x -= value.x, y -= value.y, z -= value.z, w -= value.w);
            #endif
        }

        quat<T> operator*=(const quat<T>& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { value.w, value.w, value.w, value.w };
                simd::vec4 r = { w, x, y, z };
                simd::vec4 q = _mm_mul_ps(p, r);

                p = { value.x, value.x, value.y, value.z };
                r = { x, w, w, w };
                simd::vec4 s = _mm_mul_ps(p, r);

                p = { value.y, value.y, value.z, value.x };
                r = { y, z, x, y };
                simd::vec4 t = _mm_mul_ps(p, r);

                p = { value.z, value.z, value.x, value.y };
                r = { z, y, z, x };
                simd::vec4 u = _mm_mul_ps(p, r);

                quat<T> result;
                result.w = q[0] - s[0] - t[0] - u[0];
                result.x = q[1] + s[1] + t[1] - u[1];
                result.y = q[2] + s[2] + t[2] - u[2];
                result.z = q[3] + s[3] + t[3] - u[3];

                return result;
            #else
                quat<T> result;
                result.w = (value.w * w) - (value.x * x) - (value.y * y) - (value.z * z);
                result.x = (value.w * x) + (value.x * w) + (value.y * z) - (value.z * y);
                result.y = (value.w * y) + (value.y * w) + (value.z * x) - (value.x * z);
                result.z = (value.w * z) + (value.z * w) + (value.x * y) - (value.y * x);

                return result;
            #endif
        }

        quat<T> operator/=(const quat<T>& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { value.x, value.y, value.z, value.w };
                simd::vec4 result = _mm_div_ps(p, r);

                return quat<T>(x = static_cast<T>(result[0]), y = static_cast<T>(result[1]), z = static_cast<T>(result[2]), w = static_cast<T>(result[3]));
            #else
                return quat<T>(x /= value.x, y /= value.y, z /= value.z, w /= value.w);
            #endif
        }

        template<typename U>
        quat<T> operator+(const U& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { static_cast<T>(value), static_cast<T>(value), static_cast<T>(value), static_cast<T>(value) };
                simd::vec4 result = _mm_add_ps(p, r);

                return quat<T>(static_cast<T>(result[0]), static_cast<T>(result[1]), static_cast<T>(result[2]), static_cast<T>(result[3]));
            #else
                return quat<T>(x + static_cast<T>(value), y + static_cast<T>(value), z + static_cast<T>(value), w + static_cast<T>(value));
            #endif
        }

        template<typename U>
        quat<T> operator-(const U& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { static_cast<T>(value), static_cast<T>(value), static_cast<T>(value), static_cast<T>(value) };
                simd::vec4 result = _mm_sub_ps(p, r);

                return quat<T>(static_cast<T>(result[0]), static_cast<T>(result[1]), static_cast<T>(result[2]), static_cast<T>(result[3]));
            #else
                return quat<T>(x - static_cast<T>(value), y - static_cast<T>(value), z - static_cast<T>(value), w - static_cast<T>(value));
            #endif
        }

        template<typename U>
        quat<T> operator*(const U& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { static_cast<T>(value), static_cast<T>(value), static_cast<T>(value), static_cast<T>(value) };
                simd::vec4 result = _mm_mul_ps(p, r);

                return quat<T>(static_cast<T>(result[0]), static_cast<T>(result[1]), static_cast<T>(result[2]), static_cast<T>(result[3]));
            #else
                return quat<T>(x * value, y * value, z * value, w * value);
            #endif
        }

        template<typename U>
        quat<T> operator/(const U& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { static_cast<T>(value), static_cast<T>(value), static_cast<T>(value), static_cast<T>(value) };
                simd::vec4 result = _mm_div_ps(p, r);

                return quat<T>(static_cast<T>(result[0]), static_cast<T>(result[1]), static_cast<T>(result[2]), static_cast<T>(result[3]));
            #else
                return quat<T>(x / static_cast<T>(value), y / static_cast<T>(value), z / static_cast<T>(value), w / static_cast<T>(value));
            #endif
        }

        template<typename U>
        quat<T> operator+(const quat<U>& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { static_cast<T>(value.x), static_cast<T>(value.y), static_cast<T>(value.z), static_cast<T>(value.w) };
                simd::vec4 result = _mm_add_ps(p, r);

                return quat<T>(static_cast<T>(result[0]), static_cast<T>(result[1]), static_cast<T>(result[2]), static_cast<T>(result[3]));
            #else
                return quat<T>(x + static_cast<T>(value.x), y + static_cast<T>(value.y), z + static_cast<T>(value.z), w + static_cast<T>(value.w));
            #endif
        }

        template<typename U>
        quat<T> operator-(const quat<U>& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { static_cast<T>(value.x), static_cast<T>(value.y), static_cast<T>(value.z), static_cast<T>(value.w) };
                simd::vec4 result = _mm_sub_ps(p, r);

                return quat<T>(static_cast<T>(result[0]), static_cast<T>(result[1]), static_cast<T>(result[2]), static_cast<T>(result[3]));
            #else
                return quat<T>(x - static_cast<T>(value.x), y - static_cast<T>(value.y), z - static_cast<T>(value.z), w - static_cast<T>(value.w));
            #endif
        }

        template<typename U>
        quat<T> operator*(const quat<U>& value) {
            vec4<T> vec = { static_cast<T>(value.x), static_cast<T>(value.y), static_cast<T>(value.z), static_cast<T>(value.w) };

            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { vec.w, vec.w, vec.w, vec.w };
                simd::vec4 r = { w, x, y, z };
                simd::vec4 q = _mm_mul_ps(p, r);

                p = { vec.x, vec.x, vec.y, vec.z };
                r = { x, w, w, w };
                simd::vec4 s = _mm_mul_ps(p, r);

                p = { vec.y, vec.y, vec.z, vec.x };
                r = { y, z, x, y };
                simd::vec4 t = _mm_mul_ps(p, r);

                p = { vec.z, vec.z, vec.x, vec.y };
                r = { z, y, z, x };
                simd::vec4 u = _mm_mul_ps(p, r);

                quat<T> result;
                result.w = static_cast<T>(q[0]) - static_cast<T>(s[0]) - static_cast<T>(t[0]) - static_cast<T>(u[0]);
                result.x = static_cast<T>(q[1]) + static_cast<T>(s[1]) + static_cast<T>(t[1]) - static_cast<T>(u[1]);
                result.y = static_cast<T>(q[2]) + static_cast<T>(s[2]) + static_cast<T>(t[2]) - static_cast<T>(u[2]);
                result.z = static_cast<T>(q[3]) + static_cast<T>(s[3]) + static_cast<T>(t[3]) - static_cast<T>(u[3]);

                return result;
            #else
                quat<T> result;
                result.w = (vec.w * w) - (vec.x * x) - (vec.y * y) - (vec.z * z);
                result.x = (vec.w * x) + (vec.x * w) + (vec.y * z) - (vec.z * y);
                result.y = (vec.w * y) + (vec.y * w) + (vec.z * x) - (vec.x * z);
                result.z = (vec.w * z) + (vec.z * w) + (vec.x * y) - (vec.y * x);

                return result;
            #endif
        }

        template<typename U>
        quat<T> operator/(const quat<U>& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { static_cast<T>(value.x), static_cast<T>(value.y), static_cast<T>(value.z), static_cast<T>(value.w) };
                simd::vec4 result = _mm_div_ps(p, r);

                return quat<T>(static_cast<T>(result[0]), static_cast<T>(result[1]), static_cast<T>(result[2]), static_cast<T>(result[3]));
            #else
                return quat<T>(x / static_cast<T>(value.x), y / static_cast<T>(value.y), z / static_cast<T>(value.z), w / static_cast<T>(value.w));
            #endif
        }

        template<typename U>
        quat<T> operator+=(const U& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { static_cast<T>(value), static_cast<T>(value), static_cast<T>(value), static_cast<T>(value) };
                simd::vec4 result = _mm_add_ps(p, r);

                return quat<T>(x = static_cast<T>(result[0]), y = static_cast<T>(result[1]), z = static_cast<T>(result[2]), w = static_cast<T>(result[3]));
            #else
                return quat<T>(x += static_cast<T>(value), y += static_cast<T>(value), z += static_cast<T>(value), w += static_cast<T>(value));
            #endif
        }

        template<typename U>
        quat<T> operator-=(const U& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { static_cast<T>(value), static_cast<T>(value), static_cast<T>(value), static_cast<T>(value) };
                simd::vec4 result = _mm_sub_ps(p, r);

                return quat<T>(x = static_cast<T>(result[0]), y = static_cast<T>(result[1]), z = static_cast<T>(result[2]), w = static_cast<T>(result[3]));
            #else
                return quat<T>(x -= static_cast<T>(value), y -= static_cast<T>(value), z -= static_cast<T>(value), w -= static_cast<T>(value));
            #endif
        }

        template<typename U>
        quat<T> operator*=(const U& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { static_cast<T>(value), static_cast<T>(value), static_cast<T>(value), static_cast<T>(value) };
                simd::vec4 result = _mm_mul_ps(p, r);

                return quat<T>(x = static_cast<T>(result[0]), y = static_cast<T>(result[1]), z = static_cast<T>(result[2]), w = static_cast<T>(result[3]));
            #else
                return quat<T>(x *= static_cast<T>(value), y *= static_cast<T>(value), z *= static_cast<T>(value), w *= static_cast<T>(value));
            #endif
        }

        template<typename U>
        quat<T> operator/=(const U& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { static_cast<T>(value), static_cast<T>(value), static_cast<T>(value), static_cast<T>(value) };
                simd::vec4 result = _mm_div_ps(p, r);

                return quat<T>(x = static_cast<T>(result[0]), y = static_cast<T>(result[1]), z = static_cast<T>(result[2]), w = static_cast<T>(result[3]));
            #else
                return quat<T>(x /= static_cast<T>(value), y /= static_cast<T>(value), z /= static_cast<T>(value), w /= static_cast<T>(value));
            #endif
        }

        template<typename U>
        quat<T> operator+=(const quat<U>& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { static_cast<T>(value.x), static_cast<T>(value.y), static_cast<T>(value.z), static_cast<T>(value.w) };
                simd::vec4 result = _mm_add_ps(p, r);

                return quat<T>(x = static_cast<T>(result[0]), y = static_cast<T>(result[1]), z = static_cast<T>(result[2]), w = static_cast<T>(result[3]));
            #else
                return quat<T>(x += static_cast<T>(value.x), y += static_cast<T>(value.y), z += static_cast<T>(value.z), w += static_cast<T>(value.w));
            #endif
        }

        template<typename U>
        quat<T> operator-=(const quat<U>& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { static_cast<T>(value.x), static_cast<T>(value.y), static_cast<T>(value.z), static_cast<T>(value.w) };
                simd::vec4 result = _mm_sub_ps(p, r);

                return quat<T>(x = static_cast<T>(result[0]), y = static_cast<T>(result[1]), z = static_cast<T>(result[2]), w = static_cast<T>(result[3]));
            #else
                return quat<T>(x -= static_cast<T>(value.x), y -= static_cast<T>(value.y), z -= static_cast<T>(value.z), w -= static_cast<T>(value.w));
            #endif
        }

        template<typename U>
        quat<T> operator*=(const quat<U>& value) {
            vec4<T> vec = { static_cast<T>(value.x), static_cast<T>(value.y), static_cast<T>(value.z), static_cast<T>(value.w) };

            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { vec.w, vec.w, vec.w, vec.w };
                simd::vec4 r = { w, x, y, z };
                simd::vec4 q = _mm_mul_ps(p, r);

                p = { vec.x, vec.x, vec.y, vec.z };
                r = { x, w, w, w };
                simd::vec4 s = _mm_mul_ps(p, r);

                p = { vec.y, vec.y, vec.z, vec.x };
                r = { y, z, x, y };
                simd::vec4 t = _mm_mul_ps(p, r);

                p = { vec.z, vec.z, vec.x, vec.y };
                r = { z, y, z, x };
                simd::vec4 u = _mm_mul_ps(p, r);

                quat<T> result;
                result.w = static_cast<T>(q[0]) - static_cast<T>(s[0]) - static_cast<T>(t[0]) - static_cast<T>(u[0]);
                result.x = static_cast<T>(q[1]) + static_cast<T>(s[1]) + static_cast<T>(t[1]) - static_cast<T>(u[1]);
                result.y = static_cast<T>(q[2]) + static_cast<T>(s[2]) + static_cast<T>(t[2]) - static_cast<T>(u[2]);
                result.z = static_cast<T>(q[3]) + static_cast<T>(s[3]) + static_cast<T>(t[3]) - static_cast<T>(u[3]);

                return result;
            #else
                quat<T> result;
                result.w = (vec.w * w) - (vec.x * x) - (vec.y * y) - (vec.z * z);
                result.x = (vec.w * x) + (vec.x * w) + (vec.y * z) - (vec.z * y);
                result.y = (vec.w * y) + (vec.y * w) + (vec.z * x) - (vec.x * z);
                result.z = (vec.w * z) + (vec.z * w) + (vec.x * y) - (vec.y * x);

                return result;
            #endif
        }

        template<typename U>
        quat<T> operator/=(const quat<U>& value) {
            #ifdef UVKE_MATH_USE_SIMD
                simd::vec4 p = { x, y, z, w };
                simd::vec4 r = { static_cast<T>(value.x), static_cast<T>(value.y), static_cast<T>(value.z), static_cast<T>(value.w) };
                simd::vec4 result = _mm_div_ps(p, r);

                return quat<T>(x = static_cast<T>(result[0]), y = static_cast<T>(result[1]), z = static_cast<T>(result[2]), w = static_cast<T>(result[3]));
            #else
                return quat<T>(x /= static_cast<T>(value.x), y /= static_cast<T>(value.y), z /= static_cast<T>(value.z), w /= static_cast<T>(value.w));
            #endif
        }

        bool operator==(const quat<T>&) const = default;
    };

    using quatf = quat<float>;
    using quati = quat<int>;
    using quatu = quat<unsigned>;
    using quatd = quat<double>;
    using quatl = quat<long>;

    template<typename T>
    inline constexpr quat<T> Identity() {
        quat<T> result;
        result.x = 1;
        result.y = 1;
        result.z = 1;
        result.w = 1;

        return result;
    }

    template<typename T>
    inline constexpr T Length(const quat<T>& value) {
        #ifdef UVKE_MATH_USE_SIMD
            simd::vec4 p = { value.x, value.y, value.z, value.w };
            simd::vec4 r = { value.x, value.y, value.z, value.w };
            simd::vec4 multiplied = _mm_mul_ps(p, r);

            return Sqrt<T>(multiplied[0] + multiplied[1] + multiplied[2] + multiplied[3]);
        #else
            return Sqrt<T>((value.x * value.x) + (value.y * value.y) + (value.z * value.z) + (value.w * value.w));
        #endif
    }

    template<typename T>
    inline constexpr quat<T> Normalize(const quat<T>& value) {
        T length = Length(value);

        #ifdef UVKE_MATH_USE_SIMD
            simd::vec4 p = { value.x, value.y, value.z, value.w };
            simd::vec4 r = { length, length, length, length };
            simd::vec4 divided = _mm_div_ps(p, r);

            return quat<T>(divided[0], divided[1], divided[2], divided[3]);
        #else
            return quat<T>(value.x / length, value.y / length, value.z / length, value.w / length);
        #endif
    }

    template<typename T>
    inline constexpr quat<T> Inverse(const quat<T>& value) {
        quat<T> result = value;

        #ifdef UVKE_MATH_USE_SIMD
            simd::vec4 p = { result.x, result.y, result.z, result.w, };
            simd::vec4 r = { -1, -1, -1, 1 };
            simd::vec4 data = _mm_mul_ps(p, r);
            result = quat<T> { data[0], data[1], data[2], data[3] };

            return result;
        #else
            result = quat<T> { -value.x, -value.y, -value.z, value.w };

            return result;
        #endif
    }
};

#endif