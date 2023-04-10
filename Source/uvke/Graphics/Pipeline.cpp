#include "Pipeline.hpp"

namespace uvke {
    Pipeline::Pipeline(VkDevice device, std::shared_ptr<Surface> surface, std::shared_ptr<Shader> shader, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<UniformBuffer> uniformBuffer)
        : m_device(device), m_surface(surface), m_shader(shader), m_vertexBuffer(vertexBuffer), m_uniformBuffer(uniformBuffer) {
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
            pipelineRasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE; // VK_FRONT_FACE_CLOCKWISE;
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
            pipelineMultisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
            pipelineMultisampleStateCreateInfo.minSampleShading = 1.0f;
            pipelineMultisampleStateCreateInfo.pSampleMask = nullptr;
            pipelineMultisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
            pipelineMultisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;

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

            VkAttachmentDescription attachmentDescription { };
            attachmentDescription.format = m_surface->GetFormat().format;
            attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
            attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

            VkAttachmentReference attachmentReference { };
            attachmentReference.attachment = 0;
            attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            
            VkSubpassDescription subpassDescription { };
            subpassDescription.flags = 0;
            subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
            subpassDescription.colorAttachmentCount = 1;
            subpassDescription.pColorAttachments = &attachmentReference;

            VkRenderPassCreateInfo renderPassCreateInfo { };
            renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
            renderPassCreateInfo.pNext = nullptr;
            renderPassCreateInfo.flags = 0;
            renderPassCreateInfo.attachmentCount = 1;
            renderPassCreateInfo.pAttachments = &attachmentDescription;
            renderPassCreateInfo.subpassCount = 1;
            renderPassCreateInfo.pSubpasses = &subpassDescription;

            UVKE_ASSERT(vkCreateRenderPass(m_device, &renderPassCreateInfo, nullptr, &m_renderPass));

            VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo { };
            pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            pipelineLayoutCreateInfo.pNext = nullptr;
            pipelineLayoutCreateInfo.flags = 0;
            pipelineLayoutCreateInfo.setLayoutCount = 1;
            pipelineLayoutCreateInfo.pSetLayouts = &m_uniformBuffer->GetDescriptorSetLayout();
            pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
            pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

            UVKE_ASSERT(vkCreatePipelineLayout(m_device, &pipelineLayoutCreateInfo, nullptr, &m_pipelineLayout));
            
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
            graphicsPipelineCreateInfo.pDepthStencilState = nullptr;
            graphicsPipelineCreateInfo.pColorBlendState = &pipelineColorBlendStateCreateInfo;
            graphicsPipelineCreateInfo.pDynamicState = &pipelineDynamicStateCreateInfo;
            graphicsPipelineCreateInfo.layout = m_pipelineLayout;
            graphicsPipelineCreateInfo.renderPass = m_renderPass;
            graphicsPipelineCreateInfo.subpass = 0;
            graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
            graphicsPipelineCreateInfo.basePipelineIndex = -1;

            UVKE_ASSERT(vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo, nullptr, &m_pipeline));
        }
        
        UVKE_LOG("Graphics Pipeline Created");
    }
    
    Pipeline::~Pipeline() {
        if(m_device != VK_NULL_HANDLE) {
            if(m_pipeline != VK_NULL_HANDLE) {
                vkDestroyPipeline(m_device, m_pipeline, nullptr);
            }

            if(m_pipelineLayout != VK_NULL_HANDLE) {
                vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr);
            }

            if(m_renderPass != VK_NULL_HANDLE) {
                vkDestroyRenderPass(m_device, m_renderPass, nullptr);
            }
        }

        UVKE_LOG("Graphics Pipeline Destroyed");
    }

    void Pipeline::Recreate() {
        {
            vkDestroyPipeline(m_device, m_pipeline, nullptr);
            vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr);
            vkDestroyRenderPass(m_device, m_renderPass, nullptr);

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
            pipelineRasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE; // VK_FRONT_FACE_CLOCKWISE;
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
            pipelineMultisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
            pipelineMultisampleStateCreateInfo.minSampleShading = 1.0f;
            pipelineMultisampleStateCreateInfo.pSampleMask = nullptr;
            pipelineMultisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
            pipelineMultisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;

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

            VkAttachmentDescription attachmentDescription { };
            attachmentDescription.format = m_surface->GetFormat().format;
            attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
            attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

            VkAttachmentReference attachmentReference { };
            attachmentReference.attachment = 0;
            attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            
            VkSubpassDescription subpassDescription { };
            subpassDescription.flags = 0;
            subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
            subpassDescription.colorAttachmentCount = 1;
            subpassDescription.pColorAttachments = &attachmentReference;

            VkRenderPassCreateInfo renderPassCreateInfo { };
            renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
            renderPassCreateInfo.pNext = nullptr;
            renderPassCreateInfo.flags = 0;
            renderPassCreateInfo.attachmentCount = 1;
            renderPassCreateInfo.pAttachments = &attachmentDescription;
            renderPassCreateInfo.subpassCount = 1;
            renderPassCreateInfo.pSubpasses = &subpassDescription;

            UVKE_ASSERT(vkCreateRenderPass(m_device, &renderPassCreateInfo, nullptr, &m_renderPass));

            VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo { };
            pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            pipelineLayoutCreateInfo.pNext = nullptr;
            pipelineLayoutCreateInfo.flags = 0;
            pipelineLayoutCreateInfo.setLayoutCount = 1;
            pipelineLayoutCreateInfo.pSetLayouts = &m_uniformBuffer->GetDescriptorSetLayout();
            pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
            pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

            UVKE_ASSERT(vkCreatePipelineLayout(m_device, &pipelineLayoutCreateInfo, nullptr, &m_pipelineLayout));
            
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
            graphicsPipelineCreateInfo.pDepthStencilState = nullptr;
            graphicsPipelineCreateInfo.pColorBlendState = &pipelineColorBlendStateCreateInfo;
            graphicsPipelineCreateInfo.pDynamicState = &pipelineDynamicStateCreateInfo;
            graphicsPipelineCreateInfo.layout = m_pipelineLayout;
            graphicsPipelineCreateInfo.renderPass = m_renderPass;
            graphicsPipelineCreateInfo.subpass = 0;
            graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
            graphicsPipelineCreateInfo.basePipelineIndex = -1;

            UVKE_ASSERT(vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo, nullptr, &m_pipeline));
        }
        
        UVKE_LOG("Graphics Pipeline Recreated");
    }

    void Pipeline::Render(std::shared_ptr<Framebuffer> framebuffer, std::shared_ptr<CommandBuffer> commandBuffer, unsigned int frame, unsigned int index, std::vector<std::shared_ptr<VertexBuffer>> vertexBuffers, std::vector<std::shared_ptr<IndexBuffer>> indexBuffers, std::vector<std::shared_ptr<UniformBuffer>> uniformBuffers, std::shared_ptr<Interface> interfaces) {
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
        VkClearValue clearValue = { { { 0.0f, 0.0f, 0.0f, 1.0f } } };
        renderPassBeginInfo.clearValueCount = 1;
        renderPassBeginInfo.pClearValues = &clearValue;

        vkCmdBeginRenderPass(commandBuffer->GetCommandBuffer(frame), &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

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

        auto objects = (vertexBuffers.size() + indexBuffers.size() + uniformBuffers.size()) / 3;
        for(auto i = 0; i < vertexBuffers.size(); ++i) {
            vertexBuffers[i]->Bind(commandBuffer->GetCommandBuffer(frame));
            indexBuffers[i]->Bind(commandBuffer->GetCommandBuffer(frame));
            uniformBuffers[i]->Bind(commandBuffer->GetCommandBuffer(frame), m_pipelineLayout, frame);

            vkCmdDrawIndexed(commandBuffer->GetCommandBuffer(frame), indexBuffers[i]->GetIndices().size(), 1, 0, 0, 0);
        }

        /* ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        ImGui::Render();

        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer->GetCommandBuffer(frame)); */

        interfaces->Render(commandBuffer, frame);

        vkCmdEndRenderPass(commandBuffer->GetCommandBuffer(frame));

        UVKE_ASSERT(vkEndCommandBuffer(commandBuffer->GetCommandBuffer(frame)));
    }

    void Pipeline::SetDevice(VkDevice device) {
        m_device = device;
    }
    
    void Pipeline::SetRenderPass(VkRenderPass renderPass) {
        m_renderPass = renderPass;
    }
    
    void Pipeline::SetPipelineLayout(VkPipelineLayout pipelineLayout) {
        m_pipelineLayout = pipelineLayout;
    }
    
    void Pipeline::SetPipeline(VkPipeline pipeline) {
        m_pipeline = pipeline;
    }
    
    VkDevice& Pipeline::GetDevice() {
        return m_device;
    }
    
    std::shared_ptr<Surface> Pipeline::GetSurface() {
        return m_surface;
    }
    
    std::shared_ptr<Shader> Pipeline::GetShader() {
        return m_shader;
    }
    
    std::shared_ptr<VertexBuffer> Pipeline::GetVertexBuffer() {
        return m_vertexBuffer;
    }
    
    std::shared_ptr<UniformBuffer> Pipeline::GetUniformBuffer() {
        return m_uniformBuffer;
    }
    
    VkRenderPass& Pipeline::GetRenderPass() {
        return m_renderPass;
    }
    
    VkPipelineLayout& Pipeline::GetPipelineLayout() {
        return m_pipelineLayout;
    }
    
    VkPipeline& Pipeline::GetPipeline() {
        return m_pipeline;
    }
};