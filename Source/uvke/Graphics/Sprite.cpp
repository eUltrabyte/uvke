#include "Sprite.hpp"

namespace uvke {
    Sprite::Sprite(const vec2f& size) {
        m_renderType = RenderType::Triangles;

        m_vertices = std::vector<Vertex> {
            { { -size.x, -size.y, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
            { { size.x, -size.y, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
            { { size.x, size.y, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },
            { { -size.x, size.y, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }
        };

        m_indices = std::vector<unsigned int> {
            0, 1, 2, 2, 3, 0
        };

        m_model = Identity<float>();

        UVKE_LOG("Sprite Setup");
    }

    Sprite::~Sprite() {
        m_uniformBuffer.reset();
        m_indexBuffer.reset();
        m_vertexBuffer.reset();

        m_texture.reset();

        UVKE_LOG("Sprite Destroyed");
    }

    void Sprite::Create(Renderer* renderer) {
        m_texture = std::make_unique<Texture>(renderer->GetBase(), "Resource/uvke.png");

        std::unique_ptr<StagingBuffer> m_stagingBuffer = std::make_unique<StagingBuffer>(renderer->GetBase(), static_cast<unsigned int>(m_texture->GetSize().x * m_texture->GetSize().y * 4));
        m_stagingBuffer->Map(m_texture->GetPixels());
        m_texture->Allocate();

        m_texture->LayoutTransition(renderer->GetCommandBuffer(), renderer->GetSurface()->GetQueue(0), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        m_texture->CopyFromBuffer(renderer->GetCommandBuffer(), renderer->GetSurface()->GetQueue(0), m_stagingBuffer->GetBuffer());
        m_texture->LayoutTransition(renderer->GetCommandBuffer(), renderer->GetSurface()->GetQueue(0), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        m_stagingBuffer.reset();

        m_vertexBuffer = std::make_unique<VertexBuffer>(renderer->GetBase(), m_vertices);
        m_indexBuffer = std::make_unique<IndexBuffer>(renderer->GetBase(), m_indices);
        m_uniformBuffer = std::make_unique<UniformBuffer>(renderer->GetBase(), renderer->GetSampler(), renderer->GetDescriptor(), m_texture.get());

        m_stagingBuffer = std::make_unique<StagingBuffer>(renderer->GetBase(), m_vertexBuffer->GetSize());
        m_stagingBuffer->Map(m_vertexBuffer->GetVertices().data());
        m_stagingBuffer->Copy(renderer->GetCommandBuffer()->GetCommandPool(), renderer->GetSurface()->GetQueue(0), m_vertexBuffer->GetBuffer(), m_vertexBuffer->GetSize());
        m_stagingBuffer.reset();

        m_stagingBuffer = std::make_unique<StagingBuffer>(renderer->GetBase(), m_indexBuffer->GetSize());
        m_stagingBuffer->Map(m_indexBuffer->GetIndices().data());
        m_stagingBuffer->Copy(renderer->GetCommandBuffer()->GetCommandPool(), renderer->GetSurface()->GetQueue(0), m_indexBuffer->GetBuffer(), m_indexBuffer->GetSize());
        m_stagingBuffer.reset();

        UVKE_LOG("Sprite Created");
    }

    void Sprite::Update(Camera* camera) {
        camera->SetModel(m_model);
        camera->Update(m_uniformBuffer.get());
    }

    void Sprite::Render(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, unsigned int frame) {
        m_vertexBuffer->Bind(commandBuffer);
        m_indexBuffer->Bind(commandBuffer);
        m_uniformBuffer->Bind(commandBuffer, pipelineLayout, frame);

        vkCmdDrawIndexed(commandBuffer, m_indexBuffer->GetIndices().size(), 1, 0, 0, 0);
    }

    void Sprite::SetPosition(const vec3f& position) {
        m_model = Translate<float>(m_model, vec3f(position.x, position.y, position.z));
    }

    void Sprite::SetScale(const vec3f& scale) {
        m_model = Scale<float>(m_model, vec3f(scale.x, scale.y, scale.z));
    }

    void Sprite::SetRotation(float angle) {
        m_model = Rotate<float>(m_model, vec3f(0.0f, 1.0f, 0.0f), Radians<float>(angle));
    }

    void Sprite::SetVertices(const std::vector<Vertex>& vertices) {
        m_vertices = vertices;
    }
    
    void Sprite::SetIndices(const std::vector<unsigned int>& indices) {
        m_indices = indices;
    }

    std::vector<Vertex>& Sprite::GetVertices() {
        return m_vertices;
    }

    std::vector<unsigned int>& Sprite::GetIndices() {
        return m_indices;
    }

    VertexBuffer* Sprite::GetVertexBuffer() {
        return m_vertexBuffer.get();
    }
    
    IndexBuffer* Sprite::GetIndexBuffer() {
        return m_indexBuffer.get();
    }
    
    UniformBuffer* Sprite::GetUniformBuffer() {
        return m_uniformBuffer.get();
    }
};