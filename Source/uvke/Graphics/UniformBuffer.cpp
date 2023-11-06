#include "UniformBuffer.hpp"

namespace uvke {
    UniformBuffer::UniformBuffer(Base* base, Sampler* sampler, Descriptor* descriptor, Texture* texture)
        : m_base(base) {
        {
            VkBufferCreateInfo bufferCreateInfo = { };
            bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferCreateInfo.pNext = nullptr;
            bufferCreateInfo.flags = 0;
            bufferCreateInfo.size = sizeof(UniformBufferObject);
            bufferCreateInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
            bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            bufferCreateInfo.queueFamilyIndexCount = 0;
            bufferCreateInfo.pQueueFamilyIndices = nullptr;

            UVKE_ASSERT(vkCreateBuffer(m_base->GetDevice(), &bufferCreateInfo, nullptr, &m_buffer));

            VkMemoryAllocateInfo memoryAllocateInfo { };
            memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            memoryAllocateInfo.pNext = nullptr;
            memoryAllocateInfo.allocationSize = Helper::GetRequirementsSize(m_base, m_buffer);
            memoryAllocateInfo.memoryTypeIndex = Helper::FindMemoryIndex(m_base, m_buffer);

            UVKE_ASSERT(vkAllocateMemory(m_base->GetDevice(), &memoryAllocateInfo, nullptr, &m_bufferMemory));

            vkBindBufferMemory(m_base->GetDevice(), m_buffer, m_bufferMemory, 0);

            unsigned int frames = 2;
            m_descriptorSets.resize(frames);
            
            std::array<VkDescriptorPoolSize, 2> descriptorPoolSizes { };
            descriptorPoolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorPoolSizes[0].descriptorCount = frames;

            descriptorPoolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorPoolSizes[1].descriptorCount = frames;

            VkDescriptorPoolCreateInfo descriptorPoolCreateInfo { };
            descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            descriptorPoolCreateInfo.pNext = nullptr;
            descriptorPoolCreateInfo.flags = 0;
            descriptorPoolCreateInfo.poolSizeCount = descriptorPoolSizes.size();
            descriptorPoolCreateInfo.pPoolSizes = descriptorPoolSizes.data();
            descriptorPoolCreateInfo.maxSets = frames;

            UVKE_ASSERT(vkCreateDescriptorPool(m_base->GetDevice(), &descriptorPoolCreateInfo, nullptr, &m_descriptorPool));

            std::vector<VkDescriptorSetLayout> descriptorSetLayouts(2, descriptor->GetDescriptorSetLayout());
            VkDescriptorSetAllocateInfo descriptorSetAllocateInfo { };
            descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            descriptorSetAllocateInfo.pNext = nullptr;
            descriptorSetAllocateInfo.descriptorPool = m_descriptorPool;
            descriptorSetAllocateInfo.descriptorSetCount = m_descriptorSets.size();
            descriptorSetAllocateInfo.pSetLayouts = descriptorSetLayouts.data();

            UVKE_ASSERT(vkAllocateDescriptorSets(m_base->GetDevice(), &descriptorSetAllocateInfo, m_descriptorSets.data()));
        }

        std::array<VkWriteDescriptorSet, 2> writeDescriptorSets { };

        for(auto i = 0; i < 2; ++i) {
            VkDescriptorBufferInfo descriptorBufferInfo { };
            descriptorBufferInfo.buffer = m_buffer;
            descriptorBufferInfo.offset = 0;
            descriptorBufferInfo.range = sizeof(UniformBufferObject);

            writeDescriptorSets[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeDescriptorSets[0].dstSet = m_descriptorSets[i];
            writeDescriptorSets[0].dstBinding = 0;
            writeDescriptorSets[0].dstArrayElement = 0;
            writeDescriptorSets[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            writeDescriptorSets[0].descriptorCount = 1;
            writeDescriptorSets[0].pBufferInfo = &descriptorBufferInfo;
            writeDescriptorSets[0].pImageInfo = nullptr;
            writeDescriptorSets[0].pTexelBufferView = nullptr;

            if(sampler != nullptr) {
                VkDescriptorImageInfo descriptorImageInfo { };
                descriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                descriptorImageInfo.imageView = texture->GetImage()->GetImageView();
                descriptorImageInfo.sampler = sampler->GetSampler();
                
                writeDescriptorSets[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                writeDescriptorSets[1].dstSet = m_descriptorSets[i];
                writeDescriptorSets[1].dstBinding = 1;
                writeDescriptorSets[1].dstArrayElement = 0;
                writeDescriptorSets[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                writeDescriptorSets[1].descriptorCount = 1;
                writeDescriptorSets[1].pBufferInfo = nullptr;
                writeDescriptorSets[1].pImageInfo = &descriptorImageInfo;
                writeDescriptorSets[1].pTexelBufferView = nullptr;

                vkUpdateDescriptorSets(m_base->GetDevice(), writeDescriptorSets.size(), writeDescriptorSets.data(), 0, nullptr);
            } else {
                vkUpdateDescriptorSets(m_base->GetDevice(), writeDescriptorSets.size() - 1, writeDescriptorSets.data(), 0, nullptr);
            }
        }

        UVKE_LOG("Uniform Buffer Created");
    }

    UniformBuffer::~UniformBuffer() {
        if(m_base->GetDevice() != VK_NULL_HANDLE) {
            vkDeviceWaitIdle(m_base->GetDevice());

            if(m_descriptorPool != VK_NULL_HANDLE) {
                vkDestroyDescriptorPool(m_base->GetDevice(), m_descriptorPool, nullptr);
            }

            if(m_buffer != VK_NULL_HANDLE) {
                vkDestroyBuffer(m_base->GetDevice(), m_buffer, nullptr);
            }

            if(m_bufferMemory != VK_NULL_HANDLE) {
                vkFreeMemory(m_base->GetDevice(), m_bufferMemory, nullptr);
            }
        }

        UVKE_LOG("Uniform Buffer Destroyed");
    }

    void UniformBuffer::Update(const UniformBufferObject& ubo) {
        void* rawMemory;
        vkMapMemory(m_base->GetDevice(), m_bufferMemory, 0, sizeof(UniformBufferObject), 0, &rawMemory);
        std::memcpy(rawMemory, &ubo, sizeof(UniformBufferObject));
        vkUnmapMemory(m_base->GetDevice(), m_bufferMemory);
    }

    void UniformBuffer::Bind(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, unsigned int frame) {
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &m_descriptorSets[frame], 0, nullptr);
    }

    void UniformBuffer::SetBase(Base* base) {
        m_base = base;
    }

    VkDescriptorPool& UniformBuffer::GetDescriptorPool() {
        return m_descriptorPool;
    }

    std::vector<VkDescriptorSet>& UniformBuffer::GetDescriptorSets() {
        return m_descriptorSets;
    }

    unsigned int UniformBuffer::GetSize() {
        return sizeof(UniformBufferObject);
    }

    VkBuffer& UniformBuffer::GetBuffer() {
        return m_buffer;
    }
};