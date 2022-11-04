#include "File.hpp"

namespace uvke {
    File::File(std::string_view filename) {
        std::ifstream file(filename.data(), std::ios::ate | std::ios::binary);

        if(file.is_open()) {
            size_t size = (size_t)file.tellg();
            m_data = std::vector<char>(size);
            file.seekg(0);
            file.read(m_data.data(), size);
            file.close();
        } else {
            UVKE_ASSERT(-1);
        }
    }

    std::span<char> File::Load(std::string_view filename) {
        std::ifstream file(filename.data(), std::ios::ate | std::ios::binary);

        if(file.is_open()) {
            size_t size = (size_t)file.tellg();
            std::vector<char> data(size);
            file.seekg(0);
            file.read(data.data(), size);
            file.close();
            return data;
        } else {
            UVKE_ASSERT(-1);
        }
    }

    std::span<char> File::GetData() {
        return m_data;
    }
};