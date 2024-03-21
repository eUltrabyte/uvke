#include "MeshLoader.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>

namespace uvke {
    MeshLoader::MeshLoader(std::string_view filename) {
        UVKE_LOG_ADDRESS("Mesh Loader Created");
        Load(filename);
    }

    MeshLoader::~MeshLoader() {
        Unload();
        UVKE_LOG_ADDRESS("Mesh Loader Destroyed");
    }

    void MeshLoader::Load(std::string_view filename) {
        m_vertices = std::vector<Vertex>(0);
        m_indices = std::vector<unsigned int>(0);

        tinyobj::attrib_t attributes;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::pair<std::string, std::string> error;
        if(!tinyobj::LoadObj(&attributes, &shapes, &materials, &error.first, &error.second, filename.data())) {
            UVKE_ASSERT(-1);
        } else {
            UVKE_LOG("Successfully Loaded Mesh - " + std::string(filename.data()));
        }

        for(const auto& shape : shapes) {
            for(const auto& index : shape.mesh.indices) {
                Vertex vertex { };

                vertex.position = {
                    attributes.vertices[3 * index.vertex_index + 0],
                    attributes.vertices[3 * index.vertex_index + 1],
                    attributes.vertices[3 * index.vertex_index + 2]
                };

                vertex.texCoord = {
                    attributes.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attributes.texcoords[2 * index.texcoord_index + 1]
                };

                vertex.color = { 1.0f, 1.0f, 1.0f, 1.0f };

                m_vertices.emplace_back(vertex);
                m_indices.emplace_back(m_indices.size());
            }
        }

        std::reverse(m_indices.begin(), m_indices.end());
    }

    void MeshLoader::Unload() {
        m_vertices.clear();
        m_indices.clear();
    }

    std::vector<Vertex> MeshLoader::GetVertices() {
        return m_vertices;
    }

    std::vector<unsigned int> MeshLoader::GetIndices() {
        return m_indices;
    }
};