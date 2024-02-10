#include "MeshLoader.hpp"

namespace uvke {
    MeshLoader::MeshLoader(std::string_view filename) {
        UVKE_LOG_ADDRESS("Mesh Loader Created");
        Load(filename);
    }

    MeshLoader::~MeshLoader() {
        UVKE_LOG_ADDRESS("Mesh Loader Destroyed");
    }

    void MeshLoader::Load(std::string_view filename) {
        m_vertices = std::vector<Vertex>(0);
        m_texCoords = std::vector<vec2f>(0);
        m_indices = std::vector<unsigned int>(0);

        std::vector<vec3f> vertices = std::vector<vec3f>(0);
        std::vector<vec2f> texCoords = std::vector<vec2f>(0);

        std::ifstream file(filename.data(), std::ios::in);
        std::string line = "";
        if(file.is_open() && file.good()) {
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
                    vec4u face = { 0, 0, 0, 0 };
                    unsigned int dump = 0;
                    vec4u coords = { 0, 0, 0, 0 };

                    unsigned int faces = std::count(line.begin(), line.end(), ' ');
                    if(faces == 3) {
                        stream >> face.x >> coords.x >> dump >> face.y >> coords.y >> dump >> face.z >> coords.z >> dump;
                        face = { face.x - 1, face.y - 1, face.z - 1, 0 };
                        coords = { coords.x - 1, coords.y - 1, coords.z - 1, 0 };

                        m_texCoords.emplace_back(texCoords[coords.x]);
                        m_texCoords.emplace_back(texCoords[coords.y]);
                        m_texCoords.emplace_back(texCoords[coords.z]);

                        m_indices.emplace_back(face.x);
                        m_indices.emplace_back(face.y);
                        m_indices.emplace_back(face.z);
                    } else if(faces > 3) {
                        stream >> face.x >> coords.x >> dump >> face.y >> coords.y >> dump >> face.z >> coords.z >> dump >> face.w >> coords.w >> dump;
                        face = { face.x - 1, face.y - 1, face.z - 1, face.w - 1 };
                        coords = { coords.x - 1, coords.y - 1, coords.z - 1, coords.w - 1 };

                        m_texCoords.emplace_back(texCoords[coords.x]);
                        m_texCoords.emplace_back(texCoords[coords.y]);
                        m_texCoords.emplace_back(texCoords[coords.z]);
                        m_texCoords.emplace_back(texCoords[coords.w]);

                        m_indices.emplace_back(face.x);
                        m_indices.emplace_back(face.y);
                        m_indices.emplace_back(face.z);
                        m_indices.emplace_back(face.z);
                        m_indices.emplace_back(face.w);
                        m_indices.emplace_back(face.x);
                    } else {
                        UVKE_LOG("Non-Triangular Faces In Mesh - " + std::string(filename.data()));
                    }
                }
            }

            std::reverse(m_texCoords.begin(), m_texCoords.end());

            for(auto i = 0; i < vertices.size(); ++i) {
                m_vertices.emplace_back( Vertex { vertices[i], { 1.0f, 1.0f, 1.0f, 1.0f }, m_texCoords[i] });
            }

            std::reverse(m_indices.begin(), m_indices.end());

            UVKE_LOG("Mesh Loader Successfully Loaded Mesh - " + std::string(filename.data()));
        } else {
            UVKE_LOG("Mesh Loader Cannot Load Mesh - " + std::string(filename.data()));
        }
    }

    std::vector<Vertex> MeshLoader::GetVertices() {
        return m_vertices;
    }

    std::vector<unsigned int> MeshLoader::GetIndices() {
        return m_indices;
    }
};