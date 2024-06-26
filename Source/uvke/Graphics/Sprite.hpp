#pragma once
#ifndef UVKE_SPRITE_HEADER
#define UVKE_SPRITE_HEADER

#include "../uvke.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "RenderableComponent.hpp"
#include "Renderer.hpp"

namespace uvke {
    class UVKE_API Sprite : public RenderableComponent {
    public:
        Sprite(const vec4f& color);
        Sprite(const vec2f& size = { 1.0f, 1.0f }, std::string_view filename = "");
        virtual ~Sprite();

        virtual void Create(Renderer* renderer) override;
        virtual void Update(Camera* camera) override;
        virtual void Render(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, unsigned int frame) override;

        virtual void SetPosition(const vec3f& position);
        virtual void SetScale(const vec3f& scale);
        virtual void SetRotation(float angle, const vec3f& direction = { 0.0f, 1.0f, 0.0f });
        virtual void SetVertices(const std::vector<Vertex>& vertices);
        virtual void SetIndices(const std::vector<unsigned int>& indices);

        virtual std::vector<Vertex>& GetVertices();
        virtual std::vector<unsigned int>& GetIndices();
        virtual Texture* GetTexture();

    private:
        mat4x4f m_model;
        std::vector<Vertex> m_vertices;
        std::vector<unsigned int> m_indices;
        std::string_view m_filename;
        std::unique_ptr<Texture> m_texture;
      
    };
};

#endif