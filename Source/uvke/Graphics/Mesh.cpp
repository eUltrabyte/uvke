#include "Mesh.hpp"

namespace uvke {
    Mesh::Mesh(const vec3f& size, std::string_view filename) {
        m_vertices = std::vector<Vertex> {
            { { -size.x, -size.y, -size.z }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
            { { size.x, -size.y, -size.z }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
            { { size.x, size.y, -size.z }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } },
            { { -size.x, size.y, -size.z }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } },

            { { size.x, -size.y, -size.z }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
            { { -size.x, -size.y, -size.z }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
            { { -size.x, -size.y, size.z }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } },
            { { size.x, -size.y, size.z }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } },

            { { size.x, -size.y, -size.z }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
            { { size.x, -size.y, size.z }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
            { { size.x, size.y, size.z }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } },
            { { size.x, size.y, -size.z }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } },

            { { size.x, -size.y, size.z }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
            { { -size.x, -size.y, size.z }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
            { { -size.x, size.y, size.z }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } },
            { { size.x, size.y, size.z }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } },

            { { size.x, size.y, size.z }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
            { { -size.x, size.y, size.z }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
            { { -size.x, size.y, -size.z }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } },
            { { size.x, size.y, -size.z }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } },

            { { -size.x, -size.y, size.z }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
            { { -size.x, -size.y, -size.z }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
            { { -size.x, size.y, -size.z }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } },
            { { -size.x, size.y, size.z }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } }
        };

        m_indices = std::vector<unsigned int> {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4,
            8, 9, 10, 10, 11, 8,
            12, 13, 14, 14, 15, 12,
            16, 17, 18, 18, 19, 16,
            20, 21, 22, 22, 23, 20
        };

        m_filename = filename;

        m_model = Identity<float>();

        UVKE_LOG_ADDRESS("Mesh Setup");
    }

    Mesh::~Mesh() {
        m_uniformBuffer.reset();
        m_indexBuffer.reset();
        m_vertexBuffer.reset();

        m_texture.reset();

        UVKE_LOG_ADDRESS("Mesh Destroyed");
    }

    void Mesh::Create(Renderer* renderer) {
        std::unique_ptr<StagingBuffer> m_stagingBuffer;

        if(m_filename.empty()) {
            m_texture = std::make_unique<Texture>(renderer->GetBase(), vec2i { 1, 1 });

            unsigned int* imageData = new unsigned int[m_texture->GetSize().x * m_texture->GetSize().y];
            for(auto i = 0; i < m_texture->GetSize().x * m_texture->GetSize().y; ++i) {
                imageData[i] = 0xffffffff;
            }

            m_stagingBuffer = std::make_unique<StagingBuffer>(renderer->GetBase(), static_cast<unsigned int>(m_texture->GetImage()->GetSize().x * m_texture->GetImage()->GetSize().y * 4));
            m_stagingBuffer->Map(imageData);
            m_texture->Allocate();
        } else {
            m_texture = std::make_unique<Texture>(renderer->GetBase(), vec2i { 0, 0 }, m_filename);
            
            m_stagingBuffer = std::make_unique<StagingBuffer>(renderer->GetBase(), static_cast<unsigned int>(m_texture->GetImage()->GetSize().x * m_texture->GetImage()->GetSize().y * 4));
            m_stagingBuffer->Map(m_texture->GetPixels());
            m_texture->Allocate();
        }

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

        UVKE_LOG("Mesh Created");
    }

    void Mesh::Update(Camera* camera) {
        camera->SetModel(m_model);
        m_uniformBuffer->Update(camera->GetUBO());
    }

    void Mesh::Render(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, unsigned int frame) {
        m_vertexBuffer->Bind(commandBuffer);
        m_indexBuffer->Bind(commandBuffer);
        m_uniformBuffer->Bind(commandBuffer, pipelineLayout, frame);

        vkCmdDrawIndexed(commandBuffer, m_indexBuffer->GetIndices().size(), 1, 0, 0, 0);
    }

    void Mesh::SetPosition(const vec3f& position) {
        m_model = Translate<float>(m_model, vec3f(position.x, position.y, position.z));
    }

    void Mesh::SetScale(const vec3f& scale) {
        m_model = Scale<float>(m_model, vec3f(scale.x, scale.y, scale.z));
    }

    void Mesh::SetRotation(float angle, const vec3f& direction) {
        m_model = Rotate<float>(m_model, direction, Radians<float>(angle));
    }

    void Mesh::SetVertices(const std::vector<Vertex>& vertices) {
        m_vertices = vertices;
    }

    void Mesh::SetIndices(const std::vector<unsigned int>& indices) {
        m_indices = indices;
    }

    void Mesh::SetMesh(MeshLoader* meshLoader) {
        m_vertices = meshLoader->GetVertices();
        m_indices = meshLoader->GetIndices();
    }

    std::vector<Vertex>& Mesh::GetVertices() {
        return m_vertices;
    }

    std::vector<unsigned int>& Mesh::GetIndices() {
        return m_indices;
    }

    Texture* Mesh::GetTexture() {
        return m_texture.get();
    }
};