#pragma once
#ifndef UVKE_FILE_HEADER
#define UVKE_FILE_HEADER

#include "../uvke.hpp"

namespace uvke {
    class UVKE_API File {
    public:
        File() = default;
        File(std::string_view filename);
        File(const std::vector<char>& data);
        File(std::string_view filename, const std::vector<char>& data);
        virtual ~File() = default;

        static void Save(std::string_view filename, const std::vector<char>& data) {
            std::ofstream file(filename.data());
            file.write(data.data(), data.size());
            file.close();
        }

        static std::vector<char> Load(std::string_view filename) {
            std::ifstream file(filename.data(), std::ios::ate | std::ios::binary);
            if(file.is_open()) {
                size_t size = (size_t)file.tellg();
                std::vector<char> data(size);
                file.seekg(0);
                file.read(data.data(), size);
                file.close();
                return data;
            } else {
                return std::vector<char>(0);
            }
        }

        std::vector<char> GetData() {
            return m_data;
        }

    private:
        std::vector<char> m_data;

    };
};

#endif