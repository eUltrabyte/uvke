#include "Texture.hpp"

namespace uvke {
    Texture::Texture(Base* base, const glm::ivec2& size, std::string_view filename)
        : m_base(base) {
        glm::ivec2 imageSize = size;
        m_pixels = stbi_load(filename.data(), &imageSize.x, &imageSize.y, &m_channel, STBI_rgb_alpha);
            
        m_image = std::make_unique<Image>(m_base, imageSize);

        UVKE_LOG_ADDRESS("Texture Created");
    }
    
    Texture::~Texture() {
        m_image.reset();

        UVKE_LOG_ADDRESS("Texture Destroyed");
    }

    void Texture::Allocate() {
        stbi_image_free(m_pixels);

        m_image->Allocate();

        UVKE_LOG("Texture Allocated");
    }

    void Texture::LayoutTransition(CommandBuffer* commandBuffer, VkQueue queue, VkImageLayout oldLayout, VkImageLayout newLayout) {
        m_image->LayoutTransition(commandBuffer, queue, oldLayout, newLayout);
    }

    void Texture::CopyFromBuffer(CommandBuffer* commandBuffer, VkQueue queue, VkBuffer source) {
        m_image->CopyFromBuffer(commandBuffer, queue, source);
    }

    void Texture::CopyToBuffer(CommandBuffer* commandBuffer, VkQueue queue, VkBuffer destination) {
        m_image->CopyToBuffer(commandBuffer, queue, destination);
    }

    void Texture::SetData(CommandBuffer* commandBuffer, VkQueue queue, const glm::uvec2& size, void* data) {
        m_image->SetData(commandBuffer, queue, size, data);
    }
    
    void Texture::SetBase(Base* base) {
        m_base = base;
    }

    glm::uvec2& Texture::GetSize() {
        return m_image->GetSize();
    }
    
    int& Texture::GetChannel() {
        return m_channel;
    }
    
    unsigned char* Texture::GetPixels() {
        return m_pixels;
    }
    
    Image* Texture::GetImage() {
        return m_image.get();
    }
};