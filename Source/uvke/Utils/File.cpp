#include "File.hpp"

namespace uvke {
    File::File(std::string_view filename)
    : m_data(std::vector<std::string>()) {
        std::ifstream file(filename.data());

        if(file.is_open()) {
            std::string line = "";

            while(std::getline(file, line)) {
                m_data.emplace_back(line);
            }
            file.close();
        } else {
            m_data = std::vector<std::string>(0);
        }
    }

    File::File(const std::vector<std::string>& data)
    : m_data(std::vector<std::string>()) {
        m_data = data;
    }

    File::File(std::string_view filename, const std::vector<std::string>& data) {
        std::ofstream file(filename.data());

        for(auto i = 0; i < data.size(); ++i) {
            file << data[i];
        }
        file.close();
    }

    void File::Save(std::string_view filename) {
        std::ofstream file(filename.data());

        for(auto i = 0; i < m_data.size(); ++i) {
            file << m_data[i];
        }
        file.close();
    }
    
    void File::Load(std::string_view filename) {
        std::ifstream file(filename.data());

        if(file.is_open()) {
            std::string line = "";
            
            while(std::getline(file, line)) {
                m_data.emplace_back(line);
            }
            file.close();
        } else {
            m_data = std::vector<std::string>(0);
        }
    }
};