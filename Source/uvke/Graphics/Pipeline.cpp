#include "Pipeline.hpp"

namespace uvke {
    Pipeline::Pipeline(Base* base, Surface* surface, VertexBuffer* vertexBuffer, Descriptor* descriptor)
        : m_base(base), m_surface(surface), m_vertexBuffer(vertexBuffer), m_descriptor(descriptor) {
        m_shader = std::make_shared<Shader>(m_base, File::Load("Resource/Shaders/Rendering.vert.spv"), File::Load("Resource/Shaders/Rendering.frag.spv"));
        // m_shader = std::make_shared<Shader>(m_base, File::Load("Resource/Shaders/Lightless.vert.spv"), File::Load("Resource/Shaders/Lightless.frag.spv"));

        {
            VkPipelineCacheCreateInfo pipelineCacheCreateInfo { };
            pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
            pipelineCacheCreateInfo.pNext = nullptr;
            pipelineCacheCreateInfo.flags = 0;
            pipelineCacheCreateInfo.initialDataSize = 0;
            pipelineCacheCreateInfo.pInitialData = nullptr;

            UVKE_ASSERT(vkCreatePipelineCache(m_base->GetDevice(), &pipelineCacheCreateInfo, nullptr, &m_pipelineCache));
        }
        
        Create();
        
        UVKE_LOG_ADDRESS("Graphics Pipeline Created");
    }
    
    Pipeline::~Pipeline() {
        if(m_base->GetDevice() != VK_NULL_HANDLE) {
            if(m_pipeline != VK_NULL_HANDLE) {
                vkDestroyPipeline(m_base->GetDevice(), m_pipeline, nullptr);
            }

            if(m_pipelineLayout != VK_NULL_HANDLE) {
                vkDestroyPipelineLayout(m_base->GetDevice(), m_pipelineLayout, nullptr);
            }

            if(m_pipelineCache != VK_NULL_HANDLE) {
                vkDestroyPipelineCache(m_base->GetDevice(), m_pipelineCache, nullptr);
            }

            if(m_renderPass != VK_NULL_HANDLE) {
                vkDestroyRenderPass(m_base->GetDevice(), m_renderPass, nullptr);
            }
        }

        m_shader.reset();

        UVKE_LOG_ADDRESS("Graphics Pipeline Destroyed");
    }

    void Pipeline::Create() {
        {
            VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfos[] = { *m_shader->GetVertexShaderStageCreateInfo(), *m_shader->GetFragmentShaderStageCreateInfo() };

            std::vector<VkDynamicState> dynamicStates = {
                VK_DYNAMIC_STATE_VIEWPORT,
                VK_DYNAMIC_STATE_SCISSOR,
            };

            VkPipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo { };
            pipelineDynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
            pipelineDynamicStateCreateInfo.pNext = nullptr;
            pipelineDynamicStateCreateInfo.flags = 0;
            pipelineDynamicStateCreateInfo.dynamicStateCount = dynamicStates.size();
            pipelineDynamicStateCreateInfo.pDynamicStates = dynamicStates.data();

            VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo { };
            pipelineVertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
            pipelineVertexInputStateCreateInfo.pNext = nullptr;
            pipelineVertexInputStateCreateInfo.flags = 0;
            pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = 1;
            pipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = &m_vertexBuffer->GetVertexInputBindingDescription();
            pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = m_vertexBuffer->GetVertexInputAttributeDescription().size();
            pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = m_vertexBuffer->GetVertexInputAttributeDescription().data();

            VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo { };
            pipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
            pipelineInputAssemblyStateCreateInfo.pNext = nullptr;
            pipelineInputAssemblyStateCreateInfo.flags = 0;
            pipelineInputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
            pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

            VkViewport viewport { };
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = m_surface->GetExtent().width;
            viewport.height = m_surface->GetExtent().height;
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;

            VkRect2D scissor { };
            scissor.offset = { 0, 0 };
            scissor.extent = m_surface->GetExtent();

            VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo { };
            pipelineViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
            pipelineViewportStateCreateInfo.pNext = nullptr;
            pipelineViewportStateCreateInfo.flags = 0;
            pipelineViewportStateCreateInfo.viewportCount = 1;
            pipelineViewportStateCreateInfo.pViewports = &viewport;
            pipelineViewportStateCreateInfo.scissorCount = 1;
            pipelineViewportStateCreateInfo.pScissors = &scissor;

            VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo { };
            pipelineRasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
            pipelineRasterizationStateCreateInfo.pNext = nullptr;
            pipelineRasterizationStateCreateInfo.flags = 0;
            pipelineRasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
            pipelineRasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
            pipelineRasterizationStateCreateInfo.lineWidth = 1.0f;
            pipelineRasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
            pipelineRasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
            pipelineRasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
            pipelineRasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
            pipelineRasterizationStateCreateInfo.depthBiasConstantFactor = 0.0f;
            pipelineRasterizationStateCreateInfo.depthBiasClamp = 0.0f;
            pipelineRasterizationStateCreateInfo.depthBiasSlopeFactor = 0.0f;

            VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo { };
            pipelineMultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
            pipelineMultisampleStateCreateInfo.pNext = nullptr;
            pipelineMultisampleStateCreateInfo.flags = 0;
            pipelineMultisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
            pipelineMultisampleStateCreateInfo.rasterizationSamples = m_base->GetSampleCount();
            pipelineMultisampleStateCreateInfo.minSampleShading = 1.0f;
            pipelineMultisampleStateCreateInfo.pSampleMask = nullptr;
            pipelineMultisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
            pipelineMultisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;

            VkPipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo { };
            pipelineDepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
            pipelineDepthStencilStateCreateInfo.pNext = nullptr;
            pipelineDepthStencilStateCreateInfo.flags = 0;
            pipelineDepthStencilStateCreateInfo.depthTestEnable = VK_TRUE;
            pipelineDepthStencilStateCreateInfo.depthWriteEnable = VK_TRUE;
            pipelineDepthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS;
            pipelineDepthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
            pipelineDepthStencilStateCreateInfo.minDepthBounds = 0.0f;
            pipelineDepthStencilStateCreateInfo.maxDepthBounds = 1.0f;
            pipelineDepthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;
            pipelineDepthStencilStateCreateInfo.front = { };
            pipelineDepthStencilStateCreateInfo.back = { };

            VkPipelineColorBlendAttachmentState pipelineColorBlendAttachmentState { };
            pipelineColorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
            pipelineColorBlendAttachmentState.blendEnable = VK_TRUE;
            pipelineColorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
            pipelineColorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
            pipelineColorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
            pipelineColorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
            pipelineColorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
            pipelineColorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;

            VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo { };
            pipelineColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
            pipelineColorBlendStateCreateInfo.pNext = nullptr;
            pipelineColorBlendStateCreateInfo.flags = 0;
            pipelineColorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
            pipelineColorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_COPY;
            pipelineColorBlendStateCreateInfo.attachmentCount = 1;
            pipelineColorBlendStateCreateInfo.pAttachments = &pipelineColorBlendAttachmentState;
            pipelineColorBlendStateCreateInfo.blendConstants[0] = 0.0f;
            pipelineColorBlendStateCreateInfo.blendConstants[1] = 0.0f;
            pipelineColorBlendStateCreateInfo.blendConstants[2] = 0.0f;
            pipelineColorBlendStateCreateInfo.blendConstants[3] = 0.0f;

            VkAttachmentDescription colorAttachmentDescription { };
            colorAttachmentDescription.format = m_surface->GetFormat().format;
            colorAttachmentDescription.samples = m_base->GetSampleCount();
            colorAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            colorAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            colorAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            colorAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            colorAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            colorAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            VkAttachmentReference colorAttachmentReference { };
            colorAttachmentReference.attachment = 0;
            colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            VkAttachmentDescription depthAttachmentDescription { };
            depthAttachmentDescription.format = m_base->GetDepthFormat();
            depthAttachmentDescription.samples = m_base->GetSampleCount();
            depthAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            depthAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            depthAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            depthAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            depthAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            depthAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

            VkAttachmentReference depthAttachmentReference { };
            depthAttachmentReference.attachment = 1;
            depthAttachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

            VkAttachmentDescription colorAttachmentResolveDescription { };
            colorAttachmentResolveDescription.format = m_surface->GetFormat().format;
            colorAttachmentResolveDescription.samples = VK_SAMPLE_COUNT_1_BIT;
            colorAttachmentResolveDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            colorAttachmentResolveDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            colorAttachmentResolveDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            colorAttachmentResolveDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            colorAttachmentResolveDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            colorAttachmentResolveDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

            VkAttachmentReference colorAttachmentResolveReference { };
            colorAttachmentResolveReference.attachment = 2;
            colorAttachmentResolveReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            
            VkSubpassDescription subpassDescription { };
            subpassDescription.flags = 0;
            subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
            subpassDescription.colorAttachmentCount = 1;
            subpassDescription.pColorAttachments = &colorAttachmentReference;
            subpassDescription.pDepthStencilAttachment = &depthAttachmentReference;
            subpassDescription.pResolveAttachments = &colorAttachmentResolveReference;

            VkSubpassDependency subpassDependency { };
            subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
            subpassDependency.dstSubpass = 0;
            subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            subpassDependency.srcAccessMask = 0;
            subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

            std::array<VkAttachmentDescription, 3> attachmentDescriptions { };
            attachmentDescriptions[0] = colorAttachmentDescription;
            attachmentDescriptions[1] = depthAttachmentDescription;
            attachmentDescriptions[2] = colorAttachmentResolveDescription;

            VkRenderPassCreateInfo renderPassCreateInfo { };
            renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
            renderPassCreateInfo.pNext = nullptr;
            renderPassCreateInfo.flags = 0;
            renderPassCreateInfo.attachmentCount = attachmentDescriptions.size();
            renderPassCreateInfo.pAttachments = attachmentDescriptions.data();
            renderPassCreateInfo.subpassCount = 1;
            renderPassCreateInfo.pSubpasses = &subpassDescription;
            renderPassCreateInfo.dependencyCount = 1;
            renderPassCreateInfo.pDependencies = &subpassDependency;

            UVKE_ASSERT(vkCreateRenderPass(m_base->GetDevice(), &renderPassCreateInfo, nullptr, &m_renderPass));

            VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo { };
            pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            pipelineLayoutCreateInfo.pNext = nullptr;
            pipelineLayoutCreateInfo.flags = 0;
            pipelineLayoutCreateInfo.setLayoutCount = 1;
            pipelineLayoutCreateInfo.pSetLayouts = &m_descriptor->GetDescriptorSetLayout();
            pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
            pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

            UVKE_ASSERT(vkCreatePipelineLayout(m_base->GetDevice(), &pipelineLayoutCreateInfo, nullptr, &m_pipelineLayout));
            
            VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo { };
            graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
            graphicsPipelineCreateInfo.pNext = nullptr;
            graphicsPipelineCreateInfo.flags = 0;
            graphicsPipelineCreateInfo.stageCount = 2;
            graphicsPipelineCreateInfo.pStages = pipelineShaderStageCreateInfos;
            graphicsPipelineCreateInfo.pVertexInputState = &pipelineVertexInputStateCreateInfo;
            graphicsPipelineCreateInfo.pInputAssemblyState = &pipelineInputAssemblyStateCreateInfo;
            graphicsPipelineCreateInfo.pViewportState = &pipelineViewportStateCreateInfo;
            graphicsPipelineCreateInfo.pRasterizationState = &pipelineRasterizationStateCreateInfo;
            graphicsPipelineCreateInfo.pMultisampleState = &pipelineMultisampleStateCreateInfo;
            graphicsPipelineCreateInfo.pDepthStencilState = &pipelineDepthStencilStateCreateInfo;
            graphicsPipelineCreateInfo.pColorBlendState = &pipelineColorBlendStateCreateInfo;
            graphicsPipelineCreateInfo.pDynamicState = &pipelineDynamicStateCreateInfo;
            graphicsPipelineCreateInfo.layout = m_pipelineLayout;
            graphicsPipelineCreateInfo.renderPass = m_renderPass;
            graphicsPipelineCreateInfo.subpass = 0;
            graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
            graphicsPipelineCreateInfo.basePipelineIndex = -1;

            UVKE_ASSERT(vkCreateGraphicsPipelines(m_base->GetDevice(), m_pipelineCache, 1, &graphicsPipelineCreateInfo, nullptr, &m_pipeline));
        }
        
        UVKE_LOG("Graphics Pipeline Created");
    }

    void Pipeline::Recreate() {
        vkDestroyPipeline(m_base->GetDevice(), m_pipeline, nullptr);

        vkDestroyPipelineLayout(m_base->GetDevice(), m_pipelineLayout, nullptr);
        vkDestroyRenderPass(m_base->GetDevice(), m_renderPass, nullptr);

        Create();
        
        UVKE_LOG("Graphics Pipeline Recreated");
    }

    void Pipeline::Render(Framebuffer* framebuffer, CommandBuffer* commandBuffer, unsigned int frame, unsigned int index, std::vector<Component*> components, Interface* interfaces) {
        vkResetCommandBuffer(commandBuffer->GetCommandBuffer(frame), 0);

        VkCommandBufferBeginInfo commandBufferBeginInfo { };
        commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        commandBufferBeginInfo.pNext = nullptr;
        commandBufferBeginInfo.flags = 0;
        commandBufferBeginInfo.pInheritanceInfo = nullptr;

        UVKE_ASSERT(vkBeginCommandBuffer(commandBuffer->GetCommandBuffer(frame), &commandBufferBeginInfo));

        VkRenderPassBeginInfo renderPassBeginInfo { };
        renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBeginInfo.pNext = nullptr;
        renderPassBeginInfo.renderPass = m_renderPass;
        renderPassBeginInfo.framebuffer = framebuffer->GetFramebuffer(index);
        renderPassBeginInfo.renderArea.offset = { 0, 0 };
        renderPassBeginInfo.renderArea.extent = m_surface->GetExtent();

        std::array<VkClearValue, 3> clearValues { };
        clearValues[0].color = { { 0.2f, 0.3f, 0.5f, 1.0f } };
        clearValues[1].depthStencil = { 1.0f, 0 };
        clearValues[2].color = { 0.0f, 0.0f, 0.0f, 0.0f };

        renderPassBeginInfo.clearValueCount = clearValues.size();
        renderPassBeginInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer->GetCommandBuffer(frame), &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        for(auto i = 0; i < components.size(); ++i) {
            if(const auto renderableComponent = dynamic_cast<RenderableComponent*>(components[i])) {
                vkCmdBindPipeline(commandBuffer->GetCommandBuffer(frame), VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);

                VkViewport viewport { };
                viewport.x = 0.0f;
                viewport.y = 0.0f;
                viewport.width = m_surface->GetExtent().width;
                viewport.height = m_surface->GetExtent().height;
                viewport.minDepth = 0.0f;
                viewport.maxDepth = 1.0f;

                vkCmdSetViewport(commandBuffer->GetCommandBuffer(frame), 0, 1, &viewport);

                VkRect2D scissor { };
                scissor.offset = { 0, 0 };
                scissor.extent = m_surface->GetExtent();

                vkCmdSetScissor(commandBuffer->GetCommandBuffer(frame), 0, 1, &scissor);

                renderableComponent->Render(commandBuffer->GetCommandBuffer(frame), m_pipelineLayout, frame);
            }
        }

        interfaces->Render(commandBuffer, frame);

        vkCmdEndRenderPass(commandBuffer->GetCommandBuffer(frame));

        UVKE_ASSERT(vkEndCommandBuffer(commandBuffer->GetCommandBuffer(frame)));
    }

    void Pipeline::SetBase(Base* base) {
        m_base = base;
    }

    std::shared_ptr<Shader> Pipeline::GetShader() {
        return m_shader;
    }
    
    VkRenderPass& Pipeline::GetRenderPass() {
        return m_renderPass;
    }
    
    VkPipelineCache& Pipeline::GetPipelineCache() {
        return m_pipelineCache;
    }
    
    VkPipelineLayout& Pipeline::GetPipelineLayout() {
        return m_pipelineLayout;
    }
    
    VkPipeline& Pipeline::GetPipeline() {
        return m_pipeline;
    }
};