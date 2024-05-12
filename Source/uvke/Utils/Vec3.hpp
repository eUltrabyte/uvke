#pragma once
#ifndef UVKE_VEC3_HEADER
#define UVKE_VEC3_HEADER

#include "../uvkepch.hpp"
#include "../Core/Core.hpp"

namespace uvke {
    template<typename T>
    struct UVKE_API vec3 {
    public:
        static_assert(std::is_arithmetic_v<T>, "uvke vec3 type is not arithmetic as required");
        T x, y, z;

        vec3() : x(0), y(0), z(0) {  }
        vec3(T a) : x(a), y(a), z(a) {  }
        vec3(T a, T b, T c) : x(a), y(b), z(c) {  }
        vec3(const vec3<T>& vec) : x(vec.x), y(vec.y), z(vec.z) {  }
        template<typename U>
        vec3(U a) : x(static_cast<T>(a)), y(static_cast<T>(a)), z(static_cast<T>(a)) {  }
        template<typename U>
        vec3(U a, U b, U c) : x(static_cast<T>(a)), y(static_cast<T>(b)), z(static_cast<T>(c)) {  }
        template<typename U>
        vec3(const vec3<U>& vec) : x(static_cast<T>(vec.x)), y(static_cast<T>(vec.y)), z(static_cast<T>(vec.z)) {  }
        ~vec3() = default;

        vec3<T> operator+(const T& value) {
            return vec3<T>(x + value, y + value, z + value);
        }

        vec3<T> operator-(const T& value) {
            return vec3<T>(x - value, y - value, z - value);
        }

        vec3<T> operator*(const T& value) {
            return vec3<T>(x * value, y * value, z * value);
        }

        vec3<T> operator/(const T& value) {
            return vec3<T>(x / value, y / value, z / value);
        }

        vec3<T> operator+(const vec3<T>& value) {
            return vec3<T>(x + value.x, y + value.y, z + value.z);
        }

        vec3<T> operator-(const vec3<T>& value) {
            return vec3<T>(x - value.x, y - value.y, z - value.z);
        }

        vec3<T> operator*(const vec3<T>& value) {
            return vec3<T>(x * value.x, y * value.y, z * value.z);
        }

        vec3<T> operator/(const vec3<T>& value) {
            return vec3<T>(x / value.x, y / value.y, z / value.z);
        }

        vec3<T> operator+=(const T& value) {
            return vec3<T>(x += value, y += value, z += value);
        }

        vec3<T> operator-=(const T& value) {
            return vec3<T>(x -= value, y -= value, z -= value);
        }

        vec3<T> operator*=(const T& value) {
            return vec3<T>(x *= value, y *= value, z *= value);
        }

        vec3<T> operator/=(const T& value) {
            return vec3<T>(x /= value, y /= value, z /= value);
        }

        vec3<T> operator+=(const vec3<T>& value) {
            return vec3<T>(x += value.x, y += value.y, z += value.z);
        }

        vec3<T> operator-=(const vec3<T>& value) {
            return vec3<T>(x -= value.x, y -= value.y, z -= value.z);
        }

        vec3<T> operator*=(const vec3<T>& value) {
            return vec3<T>(x *= value.x, y *= value.y, z *= value.z);
        }

        vec3<T> operator/=(const vec3<T>& value) {
            return vec3<T>(x /= value.x, y /= value.y, z /= value.z);
        }

        template<typename U>
        vec3<T> operator+(const U& value) {
            return vec3<T>(x + static_cast<T>(value), y + static_cast<T>(value), z + static_cast<T>(value));
        }

        template<typename U>
        vec3<T> operator-(const U& value) {
            return vec3<T>(x - static_cast<T>(value), y - static_cast<T>(value), z - static_cast<T>(value));
        }

        template<typename U>
        vec3<T> operator*(const U& value) {
            return vec3<T>(x * static_cast<T>(value), y * static_cast<T>(value), z * static_cast<T>(value));
        }

        template<typename U>
        vec3<T> operator/(const U& value) {
            return vec3<T>(x / static_cast<T>(value), y / static_cast<T>(value), z / static_cast<T>(value));
        }

        template<typename U>
        vec3<T> operator+(const vec3<U>& value) {
            return vec3<T>(x + static_cast<T>(value.x), y + static_cast<T>(value.y), z + static_cast<T>(value.z));
        }

        template<typename U>
        vec3<T> operator-(const vec3<U>& value) {
            return vec3<T>(x - static_cast<T>(value.x), y - static_cast<T>(value.y), z - static_cast<T>(value.z));
        }

        template<typename U>
        vec3<T> operator*(const vec3<U>& value) {
            return vec3<T>(x * static_cast<T>(value.x), y * static_cast<T>(value.y), z * static_cast<T>(value.z));
        }

        template<typename U>
        vec3<T> operator/(const vec3<U>& value) {
            return vec3<T>(x / static_cast<T>(value.x), y / static_cast<T>(value.y), z / static_cast<T>(value.z));
        }

        template<typename U>
        vec3<T> operator+=(const U& value) {
            return vec3<T>(x += static_cast<T>(value), y += static_cast<T>(value), z += static_cast<T>(value));
        }

        template<typename U>
        vec3<T> operator-=(const U& value) {
            return vec3<T>(x -= static_cast<T>(value), y -= static_cast<T>(value), z -= static_cast<T>(value));
        }

        template<typename U>
        vec3<T> operator*=(const U& value) {
            return vec3<T>(x *= static_cast<T>(value), y *= static_cast<T>(value), z *= static_cast<T>(value));
        }

        template<typename U>
        vec3<T> operator/=(const U& value) {
            return vec3<T>(x /= static_cast<T>(value), y /= static_cast<T>(value), z /= static_cast<T>(value));
        }

        template<typename U>
        vec3<T> operator+=(const vec3<U>& value) {
            return vec3<T>(x += static_cast<T>(value.x), y += static_cast<T>(value.y), z += static_cast<T>(value.z));
        }

        template<typename U>
        vec3<T> operator-=(const vec3<U>& value) {
            return vec3<T>(x -= static_cast<T>(value.x), y -= static_cast<T>(value.y), z -= static_cast<T>(value.z));
        }

        template<typename U>
        vec3<T> operator*=(const vec3<U>& value) {
            return vec3<T>(x *= static_cast<T>(value.x), y *= static_cast<T>(value.y), z *= static_cast<T>(value.z));
        }

        template<typename U>
        vec3<T> operator/=(const vec3<U>& value) {
            return vec3<T>(x /= static_cast<T>(value.x), y /= static_cast<T>(value.y), z /= static_cast<T>(value.z));
        }

        bool operator==(const vec3<T>&) const = default;
    };

    using vec3f = vec3<float>;
    using vec3i = vec3<int>;
    using vec3u = vec3<unsigned>;
    using vec3d = vec3<double>;
    using vec3l = vec3<long>;

    template<typename T>
    inline constexpr T Length(const vec3<T>& vec) {
        return Sqrt<T>((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
    }

    template<typename T>
    inline constexpr vec3<T> Normalize(const vec3<T>& vec) {
        T length = Length(vec);
        return vec3<T>(vec.x / length, vec.y / length, vec.z / length);
    }

    template<typename T>
    inline constexpr T DotProduct(const vec3<T>& x, const vec3<T>& y) {
        return x.x * y.x + x.y * y.y + x.z * y.z;
    }

    template<typename T>
    inline constexpr vec3<T> CrossProduct(const vec3<T>& x, const vec3<T>& y) {
        return vec3<T>(x.y * y.z - x.z * y.y, x.z * y.x - x.x * y.z, x.x * y.y - x.y * y.x);
    }

    template<typename T, typename U>
    inline constexpr T DotProduct(const vec3<T>& x, const vec3<U>& y) {
        return x.x * static_cast<T>(y.x) + x.y * static_cast<T>(y.y) + x.z * static_cast<T>(y.z);
    }

    template<typename T, typename U>
    inline constexpr vec3<T> CrossProduct(const vec3<T>& x, const vec3<U>& y) {
        return vec3<T>(x.y * static_cast<T>(y.z) - x.z * static_cast<T>(y.y), x.z * static_cast<T>(y.x) - x.x * static_cast<T>(y.z), x.x * static_cast<T>(y.y) - x.y * static_cast<T>(y.x));
    }
};

#endif