#pragma once
#ifndef UVKE_MESH_HEADER
#define UVKE_MESH_HEADER

#include "../uvke.hpp"
#include "../Utils/MeshLoader.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "RenderableComponent.hpp"
#include "Renderer.hpp"

namespace uvke {
    class UVKE_API Mesh : public RenderableComponent {
    public:
        Mesh(const glm::vec3& size = { 1.0f, 1.0f, 1.0f }, std::string_view filename = "");
        virtual ~Mesh();

        virtual void Create(Renderer* renderer) override;
        virtual void Update(Camera* camera) override;
        virtual void Render(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, unsigned int frame) override;

        virtual void SetPosition(const glm::vec3& position);
        virtual void SetScale(const glm::vec3& scale);
        virtual void SetRotation(float angle, const glm::vec3& direction = { 0.0f, 1.0f, 0.0f });
        virtual void SetVertices(const std::vector<Vertex>& vertices);
        virtual void SetIndices(const std::vector<unsigned int>& indices);
        virtual void SetMesh(MeshLoader* meshLoader);

        virtual std::vector<Vertex>& GetVertices();
        virtual std::vector<unsigned int>& GetIndices();
        virtual Texture* GetTexture();
        virtual glm::mat4& GetModel();

    private:
        glm::mat4 m_model;
        std::vector<Vertex> m_vertices;
        std::vector<unsigned int> m_indices;
        std::string_view m_filename;
        std::unique_ptr<Texture> m_texture;
      
    };
};

#endif