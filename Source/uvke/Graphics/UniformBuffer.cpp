#include "UniformBuffer.hpp"

namespace uvke {
    UniformBuffer::UniformBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkImageView imageView, VkSampler sampler)
        : m_physicalDevice(physicalDevice), m_device(device) {
        {
            VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = { };
            descriptorSetLayoutBinding.binding = 0;
            descriptorSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorSetLayoutBinding.descriptorCount = 1;
            descriptorSetLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
            descriptorSetLayoutBinding.pImmutableSamplers = nullptr;

            VkDescriptorSetLayoutBinding samplerSetLayoutBinding = { };
            samplerSetLayoutBinding.binding = 1;
            samplerSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            samplerSetLayoutBinding.descriptorCount = 1;
            samplerSetLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
            samplerSetLayoutBinding.pImmutableSamplers = nullptr;

            m_descriptorSetLayoutBindings = { descriptorSetLayoutBinding, samplerSetLayoutBinding };

            VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo { };
            descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            descriptorSetLayoutCreateInfo.pNext = nullptr;
            descriptorSetLayoutCreateInfo.flags = 0;
            descriptorSetLayoutCreateInfo.bindingCount = m_descriptorSetLayoutBindings.size();
            descriptorSetLayoutCreateInfo.pBindings = m_descriptorSetLayoutBindings.data();

            UVKE_ASSERT(vkCreateDescriptorSetLayout(m_device, &descriptorSetLayoutCreateInfo, nullptr, &m_descriptorSetLayout));

            VkBufferCreateInfo bufferCreateInfo = { };
            bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferCreateInfo.pNext = nullptr;
            bufferCreateInfo.flags = 0;
            bufferCreateInfo.size = sizeof(UniformBufferObject);
            bufferCreateInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
            bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            bufferCreateInfo.queueFamilyIndexCount = 0;
            bufferCreateInfo.pQueueFamilyIndices = nullptr;

            UVKE_ASSERT(vkCreateBuffer(m_device, &bufferCreateInfo, nullptr, &m_buffer));

            VkMemoryRequirements memoryRequirements { };
            vkGetBufferMemoryRequirements(m_device, m_buffer, &memoryRequirements);

            VkPhysicalDeviceMemoryProperties memoryProperties { };
            vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memoryProperties);

            unsigned int filter = memoryRequirements.memoryTypeBits;
            VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
            unsigned int index = 0;
            for(auto i = 0; i < memoryProperties.memoryTypeCount; ++i) {
                if(filter & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                    index = i;
                    break;
                }
            }

            VkMemoryAllocateInfo memoryAllocateInfo { };
            memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            memoryAllocateInfo.pNext = nullptr;
            memoryAllocateInfo.allocationSize = memoryRequirements.size;
            memoryAllocateInfo.memoryTypeIndex = index;

            UVKE_ASSERT(vkAllocateMemory(m_device, &memoryAllocateInfo, nullptr, &m_bufferMemory));

            vkBindBufferMemory(m_device, m_buffer, m_bufferMemory, 0);

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

            UVKE_ASSERT(vkCreateDescriptorPool(m_device, &descriptorPoolCreateInfo, nullptr, &m_descriptorPool));

            std::vector<VkDescriptorSetLayout> descriptorSetLayouts(2, m_descriptorSetLayout);
            VkDescriptorSetAllocateInfo descriptorSetAllocateInfo { };
            descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            descriptorSetAllocateInfo.pNext = nullptr;
            descriptorSetAllocateInfo.descriptorPool = m_descriptorPool;
            descriptorSetAllocateInfo.descriptorSetCount = m_descriptorSets.size();
            descriptorSetAllocateInfo.pSetLayouts = descriptorSetLayouts.data();

            UVKE_ASSERT(vkAllocateDescriptorSets(m_device, &descriptorSetAllocateInfo, m_descriptorSets.data()));
        }

        std::array<VkWriteDescriptorSet, 2> writeDescriptorSets { };
        for(auto i = 0; i < 2; ++i) {
            VkDescriptorBufferInfo descriptorBufferInfo { };
            descriptorBufferInfo.buffer = m_buffer;
            descriptorBufferInfo.offset = 0;
            descriptorBufferInfo.range = sizeof(UniformBufferObject);

            VkDescriptorImageInfo descriptorImageInfo { };
            descriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            descriptorImageInfo.imageView = imageView;
            descriptorImageInfo.sampler = sampler;

            writeDescriptorSets[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeDescriptorSets[0].dstSet = m_descriptorSets[i];
            writeDescriptorSets[0].dstBinding = 0;
            writeDescriptorSets[0].dstArrayElement = 0;
            writeDescriptorSets[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            writeDescriptorSets[0].descriptorCount = 1;
            writeDescriptorSets[0].pBufferInfo = &descriptorBufferInfo;
            writeDescriptorSets[0].pImageInfo = nullptr;
            writeDescriptorSets[0].pTexelBufferView = nullptr;

            writeDescriptorSets[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeDescriptorSets[1].dstSet = m_descriptorSets[i];
            writeDescriptorSets[1].dstBinding = 1;
            writeDescriptorSets[1].dstArrayElement = 0;
            writeDescriptorSets[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            writeDescriptorSets[1].descriptorCount = 1;
            writeDescriptorSets[1].pBufferInfo = nullptr;
            writeDescriptorSets[1].pImageInfo = &descriptorImageInfo;
            writeDescriptorSets[1].pTexelBufferView = nullptr;

            vkUpdateDescriptorSets(m_device, writeDescriptorSets.size(), writeDescriptorSets.data(), 0, nullptr);
        }

        UVKE_LOG("Uniform Buffer Created");
    }

    UniformBuffer::~UniformBuffer() {
        if(m_device != VK_NULL_HANDLE) {
            if(m_descriptorPool != VK_NULL_HANDLE) {
                vkDestroyDescriptorPool(m_device, m_descriptorPool, nullptr);
            }

            if(m_buffer != VK_NULL_HANDLE) {
                vkDestroyBuffer(m_device, m_buffer, nullptr);
            }

            if(m_bufferMemory != VK_NULL_HANDLE) {
                vkFreeMemory(m_device, m_bufferMemory, nullptr);
            }

            if(m_descriptorSetLayout != VK_NULL_HANDLE) {
                vkDestroyDescriptorSetLayout(m_device, m_descriptorSetLayout, nullptr);
            }
        }

        UVKE_LOG("Uniform Buffer Destroyed");
    }

    void UniformBuffer::Update(const UniformBufferObject& ubo) {
        void* rawMemory;
        vkMapMemory(m_device, m_bufferMemory, 0, sizeof(UniformBufferObject), 0, &rawMemory);
        std::memcpy(rawMemory, &ubo, sizeof(UniformBufferObject));
        vkUnmapMemory(m_device, m_bufferMemory);
    }

    void UniformBuffer::Bind(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, unsigned int frame) {
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &m_descriptorSets[frame], 0, nullptr);
    }

    VkDescriptorSetLayout& UniformBuffer::GetDescriptorSetLayout() {
        return m_descriptorSetLayout;
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