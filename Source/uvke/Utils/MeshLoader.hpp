#pragma once
#ifndef UVKE_MESH_LOADER_HEADER
#define UVKE_MESH_LOADER_HEADER

#include "../uvke.hpp"
#include "../Graphics/RenderableComponent.hpp"

namespace uvke {
    class UVKE_API MeshLoader {
    public:
        MeshLoader(std::string_view filename);
        virtual ~MeshLoader() = default;

        virtual std::vector<Vertex> GetVertices();
        virtual std::vector<unsigned int> GetIndices();

    private:
        std::vector<Vertex> m_vertices;
        std::vector<unsigned int> m_indices;

    };
};

#endif