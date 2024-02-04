#include "MeshLoader.hpp"

namespace uvke {
    MeshLoader::MeshLoader(std::string_view filename) {
        m_vertices = std::vector<Vertex>(0);
        m_indices = std::vector<unsigned int>(0);

        std::vector<vec3f> vertices = std::vector<vec3f>(0);
        std::vector<vec2f> texCoords = std::vector<vec2f>(0);
        std::string line;
        std::ifstream file(filename.data(), std::ios::in);

        while(std::getline(file, line)) {
            if(line.substr(0, 2) == "v ") {
                std::istringstream stream(line.substr(2));
                vec3f vertex;
                stream >> vertex.x >> vertex.y >> vertex.z;
                vertices.emplace_back(vertex);
            } else if(line.substr(0, 3) == "vt ") {
                std::istringstream stream(line.substr(3));
                vec2f coords;
                stream >> coords.x >> coords.y;
                texCoords.emplace_back(coords);
            } else if(line.substr(0, 2) == "f ") {
                std::string temp = line;
                std::replace(temp.begin(), temp.end(), '/', ' ');
                std::istringstream stream(temp.substr(2));
                vec4u face;
                unsigned int dump;

                stream >> face.x >> dump >> dump >> face.y >> dump >> dump >> face.z >> dump >> dump >> face.w >> dump >> dump;
                --face.x;
                --face.y;
                --face.z;
                --face.w;

                m_indices.emplace_back(face.x);
                m_indices.emplace_back(face.y);
                m_indices.emplace_back(face.z);
                m_indices.emplace_back(face.z);
                m_indices.emplace_back(face.w);
                m_indices.emplace_back(face.x);
            }
        }

        for(auto i = 0; i < vertices.size(); ++i) {
            if(texCoords[i].x && texCoords[i].y) {
                m_vertices.emplace_back( Vertex { vertices[i], { 1.0f, 1.0f, 1.0f, 1.0f }, texCoords[i] });
            } else {
                m_vertices.emplace_back( Vertex { vertices[i], { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } });
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