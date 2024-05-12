#pragma once
#ifndef UVKE_MAT4X4_HEADER
#define UVKE_MAT4X4_HEADER

#include "../uvkepch.hpp"
#include "../Core/Core.hpp"
#include "Vec3.hpp"

namespace uvke {
    template<typename T>
    struct UVKE_API mat4x4 {
    public:
        static_assert(std::is_arithmetic_v<T>, "uvke mat4x4 type is not arithmetic as required");
        std::array<std::array<T, 4>, 4> data;

        mat4x4() : data(std::array<std::array<T, 4>, 4> { std::array<T, 4> { 0, 0, 0, 0 }, std::array<T, 4> { 0, 0, 0, 0 }, std::array<T, 4> { 0, 0, 0, 0 }, std::array<T, 4> { 0, 0, 0, 0 } }) {  }
        mat4x4(const T& value) : data(std::array<std::array<T, 4>, 4> { std::array<T, 4> { value, value, value, value }, std::array<T, 4> { value, value, value, value }, std::array<T, 4> { value, value, value, value }, std::array<T, 4> { value, value, value, value } }) {  }
        mat4x4(const std::array<std::array<T, 4>, 4>& mat) : data(mat) {  }
        mat4x4(const mat4x4<T>& mat) : data(mat.data) {  }
        template<typename U>
        mat4x4(const std::array<std::array<U, 4>, 4>& mat) : data(static_cast<std::array<std::array<T, 4>, 4>>(mat)) {  }
        template<typename U>
        mat4x4(const mat4x4<U>& mat) : data(static_cast<std::array<std::array<T, 4>, 4>>(mat.data)) {  }
        ~mat4x4() = default;

        mat4x4<T> operator+(const T& value) {
            #ifdef UVKE_MATH_USE_SIMD
                for(int i = 0; i < data.size(); ++i) {
                    simd::vec4 p = { data[i][0], data[i][1], data[i][2], data[i][3], };
                    simd::vec4 r = { value, value, value, value };
                    simd::vec4 result = _mm_add_ps(p, r);
                    data[i] = std::array<T, 4> { result[0], result[1], result[2], result[3] };
                }

                return mat4x4<T>(data);
            #else
                for(int x = 0; x < data.size(); ++x) {
                    for(int y = 0; y < data[x].size(); ++y) {
                        data[x][y] += value;
                    }
                }

                return mat4x4<T>(data);
            #endif
        }

        mat4x4<T> operator-(const T& value) {
            #ifdef UVKE_MATH_USE_SIMD
                for(int i = 0; i < data.size(); ++i) {
                    simd::vec4 p = { data[i][0], data[i][1], data[i][2], data[i][3], };
                    simd::vec4 r = { value, value, value, value };
                    simd::vec4 result = _mm_sub_ps(p, r);
                    data[i] = std::array<T, 4> { result[0], result[1], result[2], result[3] };
                }

                return mat4x4<T>(data);
            #else
                for(int x = 0; x < data.size(); ++x) {
                    for(int y = 0; y < data[x].size(); ++y) {
                        data[x][y] -= value;
                    }
                }

                return mat4x4<T>(data);
            #endif
        }

        mat4x4<T> operator*(const T& value) {
            #ifdef UVKE_MATH_USE_SIMD
                for(int i = 0; i < data.size(); ++i) {
                    simd::vec4 p = { data[i][0], data[i][1], data[i][2], data[i][3], };
                    simd::vec4 r = { value, value, value, value };
                    simd::vec4 result = _mm_mul_ps(p, r);
                    data[i] = std::array<T, 4> { result[0], result[1], result[2], result[3] };
                }

                return mat4x4<T>(data);
            #else
                for(int x = 0; x < data.size(); ++x) {
                    for(int y = 0; y < data[x].size(); ++y) {
                        data[x][y] *= value;
                    }
                }

                return mat4x4<T>(data);
            #endif
        }

        mat4x4<T> operator/(const T& value) {
            #ifdef UVKE_MATH_USE_SIMD
                for(int i = 0; i < data.size(); ++i) {
                    simd::vec4 p = { data[i][0], data[i][1], data[i][2], data[i][3], };
                    simd::vec4 r = { value, value, value, value };
                    simd::vec4 result = _mm_div_ps(p, r);
                    data[i] = std::array<T, 4> { result[0], result[1], result[2], result[3] };
                }

                return mat4x4<T>(data);
            #else
                for(int x = 0; x < data.size(); ++x) {
                    for(int y = 0; y < data[x].size(); ++y) {
                        data[x][y] /= value;
                    }
                }

                return mat4x4<T>(data);
            #endif
        }

        mat4x4<T> operator+(const mat4x4<T>& value) {
            #ifdef UVKE_MATH_USE_SIMD
                for(int i = 0; i < data.size(); ++i) {
                    simd::vec4 p = { data[i][0], data[i][1], data[i][2], data[i][3], };
                    simd::vec4 r = { value.data[i][0], value.data[i][1], value.data[i][2], value.data[i][3] };
                    simd::vec4 result = _mm_add_ps(p, r);
                    data[i] = std::array<T, 4> { result[0], result[1], result[2], result[3] };
                }

                return mat4x4<T>(data);
            #else
                for(int x = 0; x < data.size(); ++x) {
                    for(int y = 0; y < data[x].size(); ++y) {
                        data[x][y] += value.data[x][y];
                    }
                }

                return mat4x4<T>(data);
            #endif
        }

        mat4x4<T> operator-(const mat4x4<T>& value) {
            #ifdef UVKE_MATH_USE_SIMD
                for(int i = 0; i < data.size(); ++i) {
                    simd::vec4 p = { data[i][0], data[i][1], data[i][2], data[i][3], };
                    simd::vec4 r = { value.data[i][0], value.data[i][1], value.data[i][2], value.data[i][3] };
                    simd::vec4 result = _mm_sub_ps(p, r);
                    data[i] = std::array<T, 4> { result[0], result[1], result[2], result[3] };
                }

                return mat4x4<T>(data);
            #else
                for(int x = 0; x < data.size(); ++x) {
                    for(int y = 0; y < data[x].size(); ++y) {
                        data[x][y] -= value.data[x][y];
                    }
                }

                return mat4x4<T>(data);
            #endif
        }

        mat4x4<T> operator*(const mat4x4<T>& value) {
            #ifdef UVKE_MATH_USE_SIMD
                for(int i = 0; i < data.size(); ++i) {
                    simd::vec4 p = { data[i][0], data[i][1], data[i][2], data[i][3], };
                    simd::vec4 r = { value.data[i][0], value.data[i][1], value.data[i][2], value.data[i][3] };
                    simd::vec4 result = _mm_mul_ps(p, r);
                    data[i] = std::array<T, 4> { result[0], result[1], result[2], result[3] };
                }

                return mat4x4<T>(data);
            #else
                for(int x = 0; x < data.size(); ++x) {
                    for(int y = 0; y < data[x].size(); ++y) {
                        data[x][y] *= value.data[x][y];
                    }
                }

                return mat4x4<T>(data);
            #endif
        }

        mat4x4<T> operator/(const mat4x4<T>& value) {
            #ifdef UVKE_MATH_USE_SIMD
                for(int i = 0; i < data.size(); ++i) {
                    simd::vec4 p = { data[i][0], data[i][1], data[i][2], data[i][3], };
                    simd::vec4 r = { value.data[i][0], value.data[i][1], value.data[i][2], value.data[i][3] };
                    simd::vec4 result = _mm_div_ps(p, r);
                    data[i] = std::array<T, 4> { result[0], result[1], result[2], result[3] };
                }

                return mat4x4<T>(data);
            #else
                for(int x = 0; x < data.size(); ++x) {
                    for(int y = 0; y < data[x].size(); ++y) {
                        data[x][y] /= value.data[x][y];
                    }
                }

                return mat4x4<T>(data);
            #endif
        }

        template<typename U>
        mat4x4<T> operator+=(const U& value) {
            #ifdef UVKE_MATH_USE_SIMD
                for(int i = 0; i < data.size(); ++i) {
                    simd::vec4 p = { data[i][0], data[i][1], data[i][2], data[i][3], };
                    simd::vec4 r = { static_cast<T>(value), static_cast<T>(value), static_cast<T>(value), static_cast<T>(value) };
                    simd::vec4 result = _mm_add_ps(p, r);
                    data[i] = std::array<T, 4> { result[0], result[1], result[2], result[3] };
                }

                return mat4x4<T>(data);
            #else
                for(int x = 0; x < data.size(); ++x) {
                    for(int y = 0; y < data[x].size(); ++y) {
                        data[x][y] += static_cast<T>(value);
                    }
                }

                return mat4x4<T>(data);
            #endif
        }

        template<typename U>
        mat4x4<T> operator-=(const U& value) {
            #ifdef UVKE_MATH_USE_SIMD
                for(int i = 0; i < data.size(); ++i) {
                    simd::vec4 p = { data[i][0], data[i][1], data[i][2], data[i][3], };
                    simd::vec4 r = { static_cast<T>(value), static_cast<T>(value), static_cast<T>(value), static_cast<T>(value) };
                    simd::vec4 result = _mm_sub_ps(p, r);
                    data[i] = std::array<T, 4> { result[0], result[1], result[2], result[3] };
                }

                return mat4x4<T>(data);
            #else
                for(int x = 0; x < data.size(); ++x) {
                    for(int y = 0; y < data[x].size(); ++y) {
                        data[x][y] -= static_cast<T>(value);
                    }
                }

                return mat4x4<T>(data);
            #endif
        }

        template<typename U>
        mat4x4<T> operator*=(const U& value) {
            #ifdef UVKE_MATH_USE_SIMD
                for(int i = 0; i < data.size(); ++i) {
                    simd::vec4 p = { data[i][0], data[i][1], data[i][2], data[i][3], };
                    simd::vec4 r = { static_cast<T>(value), static_cast<T>(value), static_cast<T>(value), static_cast<T>(value) };
                    simd::vec4 result = _mm_mul_ps(p, r);
                    data[i] = std::array<T, 4> { result[0], result[1], result[2], result[3] };
                }

                return mat4x4<T>(data);
            #else
                for(int x = 0; x < data.size(); ++x) {
                    for(int y = 0; y < data[x].size(); ++y) {
                        data[x][y] *= static_cast<T>(value);
                    }
                }

                return mat4x4<T>(data);
            #endif
        }

        template<typename U>
        mat4x4<T> operator/=(const U& value) {
            #ifdef UVKE_MATH_USE_SIMD
                for(int i = 0; i < data.size(); ++i) {
                    simd::vec4 p = { data[i][0], data[i][1], data[i][2], data[i][3], };
                    simd::vec4 r = { static_cast<T>(value), static_cast<T>(value), static_cast<T>(value), static_cast<T>(value) };
                    simd::vec4 result = _mm_div_ps(p, r);
                    data[i] = std::array<T, 4> { result[0], result[1], result[2], result[3] };
                }

                return mat4x4<T>(data);
            #else
                for(int x = 0; x < data.size(); ++x) {
                    for(int y = 0; y < data[x].size(); ++y) {
                        data[x][y] /= static_cast<T>(value);
                    }
                }

                return mat4x4<T>(data);
            #endif
        }

        template<typename U>
        mat4x4<T> operator+=(const mat4x4<U>& value) {
            #ifdef UVKE_MATH_USE_SIMD
                for(int i = 0; i < data.size(); ++i) {
                    simd::vec4 p = { data[i][0], data[i][1], data[i][2], data[i][3], };
                    simd::vec4 r = { static_cast<T>(value.data[i][0]), static_cast<T>(value.data[i][1]), static_cast<T>(value.data[i][2]), static_cast<T>(value.data[i][3]) };
                    simd::vec4 result = _mm_add_ps(p, r);
                    data[i] = std::array<T, 4> { result[0], result[1], result[2], result[3] };
                }

                return mat4x4<T>(data);
            #else
                for(int x = 0; x < data.size(); ++x) {
                    for(int y = 0; y < data[x].size(); ++y) {
                        data[x][y] += static_cast<T>(value.data[x][y]);
                    }
                }

                return mat4x4<T>(data);
            #endif
        }

        template<typename U>
        mat4x4<T> operator-=(const mat4x4<U>& value) {
            #ifdef UVKE_MATH_USE_SIMD
                for(int i = 0; i < data.size(); ++i) {
                    simd::vec4 p = { data[i][0], data[i][1], data[i][2], data[i][3], };
                    simd::vec4 r = { static_cast<T>(value.data[i][0]), static_cast<T>(value.data[i][1]), static_cast<T>(value.data[i][2]), static_cast<T>(value.data[i][3]) };
                    simd::vec4 result = _mm_sub_ps(p, r);
                    data[i] = std::array<T, 4> { result[0], result[1], result[2], result[3] };
                }

                return mat4x4<T>(data);
            #else
                for(int x = 0; x < data.size(); ++x) {
                    for(int y = 0; y < data[x].size(); ++y) {
                        data[x][y] -= static_cast<T>(value.data[x][y]);
                    }
                }

                return mat4x4<T>(data);
            #endif
        }

        template<typename U>
        mat4x4<T> operator*=(const mat4x4<U>& value) {
            #ifdef UVKE_MATH_USE_SIMD
                for(int i = 0; i < data.size(); ++i) {
                    simd::vec4 p = { data[i][0], data[i][1], data[i][2], data[i][3], };
                    simd::vec4 r = { static_cast<T>(value.data[i][0]), static_cast<T>(value.data[i][1]), static_cast<T>(value.data[i][2]), static_cast<T>(value.data[i][3]) };
                    simd::vec4 result = _mm_mul_ps(p, r);
                    data[i] = std::array<T, 4> { result[0], result[1], result[2], result[3] };
                }

                return mat4x4<T>(data);
            #else
                for(int x = 0; x < data.size(); ++x) {
                    for(int y = 0; y < data[x].size(); ++y) {
                        data[x][y] *= static_cast<T>(value.data[x][y]);
                    }
                }

                return mat4x4<T>(data);
            #endif
        }

        template<typename U>
        mat4x4<T> operator/=(const mat4x4<U>& value) {
            #ifdef UVKE_MATH_USE_SIMD
                for(int i = 0; i < data.size(); ++i) {
                    simd::vec4 p = { data[i][0], data[i][1], data[i][2], data[i][3], };
                    simd::vec4 r = { static_cast<T>(value.data[i][0]), static_cast<T>(value.data[i][1]), static_cast<T>(value.data[i][2]), static_cast<T>(value.data[i][3]) };
                    simd::vec4 result = _mm_div_ps(p, r);
                    data[i] = std::array<T, 4> { result[0], result[1], result[2], result[3] };
                }

                return mat4x4<T>(data);
            #else
                for(int x = 0; x < data.size(); ++x) {
                    for(int y = 0; y < data[x].size(); ++y) {
                        data[x][y] /= static_cast<T>(value.data[x][y]);
                    }
                }

                return mat4x4<T>(data);
            #endif
        }
    };

    using mat4x4f = mat4x4<float>;
    using mat4x4i = mat4x4<int>;
    using mat4x4u = mat4x4<unsigned>;
    using mat4x4d = mat4x4<double>;
    using mat4x4l = mat4x4<long>;

    template<typename T>
    inline constexpr mat4x4<T> Identity() {
        mat4x4<T> result;
        result.data[0][0] = 1;
        result.data[1][1] = 1;
        result.data[2][2] = 1;
        result.data[3][3] = 1;

        return result;
    }

    template<typename T>
    inline constexpr mat4x4<T> Perspective(const T& fov, const T& aspect, const T& zNear, const T& zFar) {
        T half = 1 / Tan<T>(fov);

        mat4x4<T> result;
        result.data[0][0] = 1 / aspect * half;
        result.data[1][1] = -(1 / half);
        result.data[2][2] = zFar / (zFar - zNear);
        result.data[2][3] = 1;
        result.data[3][2] = -(2 * zFar * zNear) / (zFar - zNear);

        return result;
    }

    template<typename T>
    inline constexpr mat4x4<T> InfinitePerspective(const T& fov, const T& aspect, const T& zNear) {
        T half = 1 / Tan<T>(fov);

        mat4x4<T> result;
        result.data[0][0] = 1 / aspect * half;
        result.data[1][1] = -(1 / half);
        result.data[2][2] = 1;
        result.data[2][3] = 1;
        result.data[3][2] = -(2 * zNear);

        return result;
    }

    template<typename T>
    inline constexpr mat4x4<T> Ortho(const T& left, const T& right, const T& bottom, const T& top, const T& zNear, const T& zFar) {
        mat4x4<T> result;
        result.data[0][0] = 2 / (right - left);
        result.data[1][1] = 2 / (bottom - top);
        result.data[2][2] = 1 / (zNear - zFar);
        result.data[3][0] = -(right + left) / (right - left);
        result.data[3][1] = -(bottom + top) / (bottom - top);
        result.data[3][2] = zNear / (zNear - zFar);
        result.data[3][3] = 1;

        return result;
    }

    template<typename T>
    inline constexpr mat4x4<T> Frustum(const T& left, const T& right, const T& bottom, const T& top, const T& zNear, const T& zFar) {
        mat4x4<T> result;
        result.data[0][0] = (2 * zNear) / (right - left);
        result.data[1][1] = (2 * zNear) / (bottom - top);
        result.data[2][0] = -(right + left) / (right - left);
        result.data[2][0] = -(bottom + top) / (bottom - top);
        result.data[2][2] = zNear / (zNear - zFar);
        result.data[2][3] = 1;
        result.data[3][2] = zNear * zFar / (zNear - zFar);

        return result;
    }

    template<typename T>
    inline constexpr mat4x4<T> LookAt(const vec3<T>& eye, const vec3<T>& center, const vec3<T>& up) {
        vec3<T> normalized = Normalize<T>(vec3<T>(center.x - eye.x, center.y - eye.y, center.z - eye.z));
        vec3<T> normalizedCrossProduct = Normalize<T>(CrossProduct<T, T>(normalized, up));
        vec3<T> crossProduct = CrossProduct<T, T>(normalizedCrossProduct, normalized);

        mat4x4<T> result = Identity<T>();
        result.data[0][0] = normalizedCrossProduct.x;
        result.data[1][0] = normalizedCrossProduct.y;
        result.data[2][0] = normalizedCrossProduct.z;
        
        result.data[0][1] = crossProduct.x;
        result.data[1][1] = crossProduct.y;
        result.data[2][1] = crossProduct.z;

        result.data[0][2] = normalized.x;
        result.data[1][2] = normalized.y;
        result.data[2][2] = normalized.z;

        result.data[3][0] = -DotProduct(normalizedCrossProduct, eye);
        result.data[3][1] = -DotProduct(crossProduct, eye);
        result.data[3][2] = -DotProduct(normalized, eye);

        return result;
    }

    template<typename T>
    inline constexpr mat4x4<T> Scale(const mat4x4<T>& matrix, const vec3<T>& scale) {
        mat4x4<T> result = Identity<T>();
        result.data[0][0] = scale.x;
        result.data[1][1] = scale.y;
        result.data[2][2] = scale.z;
        
        result *= matrix;
        return result;
    }

    template<typename T>
    inline constexpr mat4x4<T> Rotate(const mat4x4<T>& matrix, const vec3<T>& direction, const T& angle) {
        T cosinus = Cos<T>(angle);
        T sinus = Sin<T>(angle);
        vec3<T> axis = Normalize<T>(direction);
        vec3<T> calculation = vec3<T>(1 - cosinus * direction.x, 1 - cosinus * direction.y, 1 - cosinus * direction.z);

        mat4x4<T> result;
        result.data[0][0] = cosinus + calculation.x * axis.x;
        result.data[0][1] = calculation.x * axis.y + sinus * axis.z;
        result.data[0][2] = calculation.x * axis.z - sinus * axis.y;
        result.data[0][3] = 1;

        result.data[1][0] = calculation.y * axis.x - sinus * axis.z;
        result.data[1][1] = cosinus + calculation.y * axis.y;
        result.data[1][2] = calculation.y * axis.z + sinus * axis.x;
        result.data[1][3] = 1;

        result.data[2][0] = calculation.z * axis.x + sinus * axis.y;
        result.data[2][1] = calculation.z * axis.y - sinus * axis.x;
        result.data[2][2] = cosinus + calculation.z * axis.z;
        result.data[2][3] = 1;

        result.data[3][0] = 1;
        result.data[3][1] = 1;
        result.data[3][2] = 1;
        result.data[3][3] = 1;

        result *= matrix;
        return result;
    }

    template<typename T>
    inline constexpr mat4x4<T> Translate(const mat4x4<T>& matrix, const vec3<T>& translation) {
        mat4x4<T> result = matrix;
        result.data[3][0] = matrix.data[0][0] * translation.x + matrix.data[1][0] * translation.y + matrix.data[2][0] * translation.z + matrix.data[3][0];
        result.data[3][1] = matrix.data[0][1] * translation.x + matrix.data[1][1] * translation.y + matrix.data[2][1] * translation.z + matrix.data[3][1];
        result.data[3][2] = matrix.data[0][2] * translation.x + matrix.data[1][2] * translation.y + matrix.data[2][2] * translation.z + matrix.data[3][2];
        result.data[3][3] = matrix.data[0][3] * translation.x + matrix.data[1][3] * translation.y + matrix.data[2][3] * translation.z + matrix.data[3][3];

        return result;
    }

    template<typename T>
    inline constexpr mat4x4<T> Inverse(const mat4x4<T>& matrix) {
        mat4x4<T> result = matrix;

        #ifdef UVKE_MATH_USE_SIMD
            for(int i = 0; i < result.data.size(); ++i) {
                simd::vec4 p = { result.data[i][0], result.data[i][1], result.data[i][2], result.data[i][3], };
                simd::vec4 r = { -1, -1, -1, -1 };
                simd::vec4 data = _mm_mul_ps(p, r);
                result.data[i] = std::array<T, 4> { data[0], data[1], data[2], data[3] };
            }

            return result;
        #else
            for(int x = 0; x < result.data.size(); ++x) {
                for(int y = 0; y < result.data[x].size(); ++y) {
                    result.data[x][y] = -matrix.data[x][y];
                }
            }

            return result;
        #endif
    }
};

#endif