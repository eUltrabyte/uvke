#include "MeshLoader.hpp"

namespace uvke {
    MeshLoader::MeshLoader(std::string_view filename) {
        m_vertices = std::vector<Vertex>(0);
        m_indices = std::vector<unsigned int>(0);

        std::vector<vec3f> vertices = std::vector<vec3f>(0);
        std::vector<vec2f> texCoords = std::vector<vec2f>(0);
        std::string line;
        std::ifstream file(filename.data());
        std::vector<std::string> strings = std::vector<std::string>(0);

        while(std::getline(file, line)) {
            if(line.empty()) {
                strings = std::vector<std::string>(0);
            }

            strings = std::vector<std::string>(1);

            for(const char symbol : line) {
                if(symbol == ' ') {
                    if(!strings.back().empty()) {
                        strings.emplace_back();
                    }
                } else {
                    strings.back() += symbol;
                }
            }

            if(!strings.empty() && strings.back().empty()) {
                strings.pop_back();
            }

            if(!strings.empty()) {
                if(strings[0] == "v") {
                    vertices.emplace_back(vec3f { std::stof(strings[1]), std::stof(strings[2]), std::stof(strings[3]) });
                } else if(strings[0] == "vt") {
                    texCoords.emplace_back(vec2f { std::stof(strings[1]), std::stof(strings[2]) });
                } else if(strings[0] == "f") {
                    for(const char symbol : line) {
                        if(symbol == '/') {
                            if(!strings.back().empty()) {
                                strings.emplace_back();
                            }
                        } else {
                            strings.back() += symbol;
                        }
                    }

                    if(!strings.empty() && strings.back().empty()) {
                        strings.pop_back();
                    }

                    unsigned int faces = strings.size() - 1;
                    if(faces == 3) {
                        m_indices.emplace_back(static_cast<unsigned int>(std::stof(strings[1]) - 1));
                        m_indices.emplace_back(static_cast<unsigned int>(std::stof(strings[2]) - 1));
                        m_indices.emplace_back(static_cast<unsigned int>(std::stof(strings[3]) - 1));
                    } else if(faces > 3) {
                        m_indices.emplace_back(static_cast<unsigned int>(std::stof(strings[1]) - 1));
                        m_indices.emplace_back(static_cast<unsigned int>(std::stof(strings[2]) - 1));
                        m_indices.emplace_back(static_cast<unsigned int>(std::stof(strings[3]) - 1));

                        m_indices.emplace_back(static_cast<unsigned int>(std::stof(strings[faces]) - 1));
                        m_indices.emplace_back(static_cast<unsigned int>(std::stof(strings[faces - 1]) - 1));
                        m_indices.emplace_back(static_cast<unsigned int>(std::stof(strings[1]) - 1));

                        for(auto i = 0; i < faces - 4; ++i) {
                            m_indices.emplace_back(static_cast<unsigned int>(std::stof(strings[1]) - 1));
                            m_indices.emplace_back(static_cast<unsigned int>(std::stof(strings[i + 3]) - 1));
                            m_indices.emplace_back(static_cast<unsigned int>(std::stof(strings[i + 4]) - 1));
                        }
                    }
                }
            }
        }

        for(auto i = 0; i < vertices.size(); ++i) {
            if(texCoords.empty()) {
                m_vertices.emplace_back( Vertex { vertices[i], { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } });
            } else {
                m_vertices.emplace_back( Vertex { vertices[i], { 1.0f, 1.0f, 1.0f, 1.0f }, texCoords[i] });
            }
        }
    }

    std::vector<Vertex> MeshLoader::GetVertices() {
        return m_vertices;
    }

    std::vector<unsigned int> MeshLoader::GetIndices() {
        return m_indices;
    }
};