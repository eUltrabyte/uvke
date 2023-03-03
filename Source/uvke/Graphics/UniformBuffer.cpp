#include "UniformBuffer.hpp"

namespace uvke {
    UniformBuffer::UniformBuffer(VkPhysicalDevice physicalDevice, VkDevice device)
        : m_physicalDevice(physicalDevice), m_device(device) {
        {
            m_descriptorSetLayoutBinding = { };
            m_descriptorSetLayoutBinding.binding = 0;
            m_descriptorSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            m_descriptorSetLayoutBinding.descriptorCount = 1;
            m_descriptorSetLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
            m_descriptorSetLayoutBinding.pImmutableSamplers = nullptr;

            VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo { };
            descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            descriptorSetLayoutCreateInfo.pNext = nullptr;
            descriptorSetLayoutCreateInfo.flags = 0;
            descriptorSetLayoutCreateInfo.bindingCount = 1;
            descriptorSetLayoutCreateInfo.pBindings = &m_descriptorSetLayoutBinding;

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

            VkDescriptorPoolSize descriptorPoolSize { };
            descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorPoolSize.descriptorCount = 1;

            VkDescriptorPoolCreateInfo descriptorPoolCreateInfo { };
            descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            descriptorPoolCreateInfo.pNext = nullptr;
            descriptorPoolCreateInfo.flags = 0;
            descriptorPoolCreateInfo.poolSizeCount = 1;
            descriptorPoolCreateInfo.pPoolSizes = &descriptorPoolSize;
            descriptorPoolCreateInfo.maxSets = 1;

            UVKE_ASSERT(vkCreateDescriptorPool(m_device, &descriptorPoolCreateInfo, nullptr, &m_descriptorPool));

            VkDescriptorSetLayout descriptorSetLayout = m_descriptorSetLayout;
            VkDescriptorSetAllocateInfo descriptorSetAllocateInfo { };
            descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            descriptorSetAllocateInfo.pNext = nullptr;
            descriptorSetAllocateInfo.descriptorPool = m_descriptorPool;
            descriptorSetAllocateInfo.descriptorSetCount = 1;
            descriptorSetAllocateInfo.pSetLayouts = &descriptorSetLayout;

            UVKE_ASSERT(vkAllocateDescriptorSets(m_device, &descriptorSetAllocateInfo, &m_descriptorSet));
        }

        VkDescriptorBufferInfo descriptorBufferInfo { };
        descriptorBufferInfo.buffer = m_buffer;
        descriptorBufferInfo.offset = 0;
        descriptorBufferInfo.range = sizeof(UniformBufferObject);

        VkWriteDescriptorSet writeDescriptorSet { };
        writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescriptorSet.dstSet = m_descriptorSet;
        writeDescriptorSet.dstBinding = 0;
        writeDescriptorSet.dstArrayElement = 0;
        writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writeDescriptorSet.descriptorCount = 1;
        writeDescriptorSet.pBufferInfo = &descriptorBufferInfo;
        writeDescriptorSet.pImageInfo = nullptr;
        writeDescriptorSet.pTexelBufferView = nullptr;

        vkUpdateDescriptorSets(m_device, 1, &writeDescriptorSet, 0, nullptr);

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

    void UniformBuffer::Bind(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout) {
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &m_descriptorSet, 0, nullptr);
    }

    VkDescriptorSetLayout& UniformBuffer::GetDescriptorSetLayout() {
        return m_descriptorSetLayout;
    }

    VkDescriptorPool& UniformBuffer::GetDescriptorPool() {
        return m_descriptorPool;
    }

    VkDescriptorSet& UniformBuffer::GetDescriptorSet() {
        return m_descriptorSet;
    }

    unsigned int UniformBuffer::GetSize() {
        return sizeof(UniformBufferObject);
    }

    VkBuffer& UniformBuffer::GetBuffer() {
        return m_buffer;
    }
};