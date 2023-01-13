#pragma once
#ifndef UVKE_MAT4X4_HEADER
#define UVKE_MAT4X4_HEADER

#include "../uvkepch.hpp"

namespace uvke {
    template<typename T>
    struct UVKE_API mat4x4 {
    public:
        static_assert(std::is_arithmetic_v<T>, "uvke mat4x4 type is not arithmetic as required");
        std::array<std::array<T, 4>, 4> data;

        mat4x4() : data(std::array<std::array<T, 4>, 4> { std::array<T, 4> { 0.0f, 0.0f, 0.0f, 0.0f }, std::array<T, 4> { 0.0f, 0.0f, 0.0f, 0.0f }, std::array<T, 4> { 0.0f, 0.0f, 0.0f, 0.0f }, std::array<T, 4> { 0.0f, 0.0f, 0.0f, 0.0f } }) {  }
        mat4x4(const std::array<std::array<T, 4>, 4>& mat) : data(mat) {  }
        mat4x4(const mat4x4<T>& mat) : data(mat.data) {  }
        template<typename U>
        mat4x4(const std::array<std::array<U, 4>, 4>& mat) : data(static_cast<std::array<std::array<T, 4>, 4>>(mat)) {  }
        template<typename U>
        mat4x4(const mat4x4<U>& mat) : data(static_cast<std::array<std::array<T, 4>, 4>>(mat.data)) {  }

        mat4x4<T> operator+=(const T& value) {
            for(int x = 0; x < data.size(); ++x) {
                for(int y = 0; y < data[x].size(); ++y) {
                    data[x][y] += value;
                }
            }

            return mat4x4<T>(data);
        }

        mat4x4<T> operator-=(const T& value) {
            for(int x = 0; x < data.size(); ++x) {
                for(int y = 0; y < data[x].size(); ++y) {
                    data[x][y] -= value;
                }
            }

            return mat4x4<T>(data);
        }

        mat4x4<T> operator*=(const T& value) {
            for(int x = 0; x < data.size(); ++x) {
                for(int y = 0; y < data[x].size(); ++y) {
                    data[x][y] *= value;
                }
            }

            return mat4x4<T>(data);
        }

        mat4x4<T> operator/=(const T& value) {
            for(int x = 0; x < data.size(); ++x) {
                for(int y = 0; y < data[x].size(); ++y) {
                    data[x][y] /= value;
                }
            }

            return mat4x4<T>(data);
        }

        mat4x4<T> operator+=(const mat4x4<T>& value) {
            for(int x = 0; x < data.size(); ++x) {
                for(int y = 0; y < data[x].size(); ++y) {
                    data[x][y] += value.data[x][y];
                }
            }

            return mat4x4<T>(data);
        }

        mat4x4<T> operator-=(const mat4x4<T>& value) {
            for(int x = 0; x < data.size(); ++x) {
                for(int y = 0; y < data[x].size(); ++y) {
                    data[x][y] -= value.data[x][y];
                }
            }

            return mat4x4<T>(data);
        }

        mat4x4<T> operator*=(const mat4x4<T>& value) {
            for(int x = 0; x < data.size(); ++x) {
                for(int y = 0; y < data[x].size(); ++y) {
                    data[x][y] *= value.data[x][y];
                }
            }

            return mat4x4<T>(data);
        }

        mat4x4<T> operator/=(const mat4x4<T>& value) {
            for(int x = 0; x < data.size(); ++x) {
                for(int y = 0; y < data[x].size(); ++y) {
                    data[x][y] /= value.data[x][y];
                }
            }

            return mat4x4<T>(data);
        }

        template<typename U>
        mat4x4<T> operator+=(const U& value) {
            for(int x = 0; x < data.size(); ++x) {
                for(int y = 0; y < data[x].size(); ++y) {
                    data[x][y] += static_cast<T>(value);
                }
            }

            return mat4x4<T>(data);
        }

        template<typename U>
        mat4x4<T> operator-=(const U& value) {
            for(int x = 0; x < data.size(); ++x) {
                for(int y = 0; y < data[x].size(); ++y) {
                    data[x][y] -= static_cast<T>(value);
                }
            }

            return mat4x4<T>(data);
        }

        template<typename U>
        mat4x4<T> operator*=(const U& value) {
            for(int x = 0; x < data.size(); ++x) {
                for(int y = 0; y < data[x].size(); ++y) {
                    data[x][y] *= static_cast<T>(value);
                }
            }

            return mat4x4<T>(data);
        }

        template<typename U>
        mat4x4<T> operator/=(const U& value) {
            for(int x = 0; x < data.size(); ++x) {
                for(int y = 0; y < data[x].size(); ++y) {
                    data[x][y] /= static_cast<T>(value);
                }
            }

            return mat4x4<T>(data);
        }

        template<typename U>
        mat4x4<T> operator+=(const mat4x4<U>& value) {
            for(int x = 0; x < data.size(); ++x) {
                for(int y = 0; y < data[x].size(); ++y) {
                    data[x][y] += static_cast<T>(value.data[x][y]);
                }
            }

            return mat4x4<T>(data);
        }

        template<typename U>
        mat4x4<T> operator-=(const mat4x4<U>& value) {
            for(int x = 0; x < data.size(); ++x) {
                for(int y = 0; y < data[x].size(); ++y) {
                    data[x][y] -= static_cast<T>(value.data[x][y]);
                }
            }

            return mat4x4<T>(data);
        }

        template<typename U>
        mat4x4<T> operator*=(const mat4x4<U>& value) {
            for(int x = 0; x < data.size(); ++x) {
                for(int y = 0; y < data[x].size(); ++y) {
                    data[x][y] *= static_cast<T>(value.data[x][y]);
                }
            }

            return mat4x4<T>(data);
        }

        template<typename U>
        mat4x4<T> operator/=(const mat4x4<U>& value) {
            for(int x = 0; x < data.size(); ++x) {
                for(int y = 0; y < data[x].size(); ++y) {
                    data[x][y] /= static_cast<T>(value.data[x][y]);
                }
            }

            return mat4x4<T>(data);
        }
    };

    using mat4x4f = mat4x4<float>;
    using mat4x4i = mat4x4<int>;
    using mat4x4u = mat4x4<unsigned>;
    using mat4x4d = mat4x4<double>;
    using mat4x4l = mat4x4<long>;
};

#endif