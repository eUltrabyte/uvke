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
            m_data = std::vector<char>(0);
        }
    }

    File::File(const std::vector<char>& data) {
        m_data = data;
    }

    File::File(std::string_view filename, const std::vector<char>& data) {
        std::ofstream file(filename.data());
        file.write(data.data(), data.size());
        file.close();
    }
};