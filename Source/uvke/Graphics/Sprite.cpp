#include "Sprite.hpp"

namespace uvke {
    Sprite::Sprite(const vec2f& size)
        : m_position({ 0.0f, 0.0f }), m_scale({ 1.0f, 1.0f }) {
        m_vertices = std::vector<Vertex> {
            { { -size.x, -size.y, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
            { { size.x, -size.y, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
            { { size.x, size.y, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },
            { { -size.x, size.y, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }
        };

        m_indices = std::vector<unsigned int> {
            0, 1, 2, 2, 3, 0
        };

        UVKE_LOG("Sprite Setup");
    }

    Sprite::~Sprite() {
        m_uniformBuffer.reset();
        m_indexBuffer.reset();
        m_vertexBuffer.reset();

        UVKE_LOG("Sprite Destroyed");
    }

    void Sprite::Create(std::shared_ptr<Renderer> renderer) {
        m_vertexBuffer = std::make_shared<VertexBuffer>(renderer->GetBase(), m_vertices);
        m_indexBuffer = std::make_shared<IndexBuffer>(renderer->GetBase(), m_indices);
        m_uniformBuffer = std::make_shared<UniformBuffer>(renderer->GetBase(), renderer->GetSampler(), renderer->GetDescriptor());

        std::shared_ptr<StagingBuffer> m_stagingBuffer = std::make_shared<StagingBuffer>(renderer->GetBase(), m_vertexBuffer->GetSize());
        m_stagingBuffer->Map(m_vertexBuffer->GetVertices().data());
        m_stagingBuffer->Copy(renderer->GetCommandBuffer()->GetCommandPool(), renderer->GetSurface()->GetQueue(0), m_vertexBuffer->GetBuffer(), m_vertexBuffer->GetSize());
        m_stagingBuffer.reset();

        m_stagingBuffer = std::make_shared<StagingBuffer>(renderer->GetBase(), m_indexBuffer->GetSize());
        m_stagingBuffer->Map(m_indexBuffer->GetIndices().data());
        m_stagingBuffer->Copy(renderer->GetCommandBuffer()->GetCommandPool(), renderer->GetSurface()->GetQueue(0), m_indexBuffer->GetBuffer(), m_indexBuffer->GetSize());
        m_stagingBuffer.reset();

        UVKE_LOG("Sprite Created");
    }

    void Sprite::Update(std::shared_ptr<Camera> camera) {
        mat4x4f model = camera->GetModel();
        model = Scale<float>(model, vec3f(m_scale.x, m_scale.y, 1.0f));
        model = Translate<float>(model, vec3f(m_position.x, m_position.y, 0.0f));
        // model = Rotate<float>(model, vec3f(0.0f, 0.0f, 1.0f), Radians<float>(m_angle));

        camera->SetModel(model);
        camera->Update(m_uniformBuffer);
    }

    void Sprite::Render(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, unsigned int frame) {
        m_vertexBuffer->Bind(commandBuffer);
        m_indexBuffer->Bind(commandBuffer);
        m_uniformBuffer->Bind(commandBuffer, pipelineLayout, frame);

        vkCmdDrawIndexed(commandBuffer, m_indexBuffer->GetIndices().size(), 1, 0, 0, 0);
    }

    void Sprite::SetPosition(const vec2f& position) {
        m_position = position;
    }

    void Sprite::SetScale(const vec2f& scale) {
        m_scale = scale;
    }

    void Sprite::SetRotation(float angle) {
        m_angle = angle;
    }

    void Sprite::SetVertices(const std::vector<Vertex>& vertices) {
        m_vertices = vertices;
    }
    
    void Sprite::SetIndices(const std::vector<unsigned int>& indices) {
        m_indices = indices;
    }

    vec2f& Sprite::GetPosition() {
        return m_position;
    }

    vec2f& Sprite::GetScale() {
        return m_scale;
    }

    std::vector<Vertex>& Sprite::GetVertices() {
        return m_vertices;
    }

    std::vector<unsigned int>& Sprite::GetIndices() {
        return m_indices;
    }

    std::shared_ptr<VertexBuffer> Sprite::GetVertexBuffer() {
        return m_vertexBuffer;
    }
    
    std::shared_ptr<IndexBuffer> Sprite::GetIndexBuffer() {
        return m_indexBuffer;
    }
    
    std::shared_ptr<UniformBuffer> Sprite::GetUniformBuffer() {
        return m_uniformBuffer;
    }
};