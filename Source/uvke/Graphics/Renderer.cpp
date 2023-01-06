#include "Renderer.hpp"

namespace uvke {
    Renderer::Renderer(Window& window)
        : m_window(window), m_framebufferRecreated(false) {
        {
            VkApplicationInfo appInfo { };
            appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo.pNext = nullptr;
            appInfo.applicationVersion = VK_MAKE_API_VERSION(1, 1, 0, 0);
            appInfo.pApplicationName = window.GetWindowProps()->title.c_str();
            appInfo.engineVersion = VK_MAKE_API_VERSION(1, 1, 0, 0);
            appInfo.pEngineName = "uvke";
            appInfo.apiVersion = uvke::GetSupportedVulkan();

            std::vector<const char*> layers = {
                #ifdef UVKE_DEBUG
                    "VK_LAYER_KHRONOS_validation"
                #endif
            };

            unsigned int glfwExtensionsCount = 0;
            const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);
            std::vector<const char*> extensions = std::vector<const char*>(glfwExtensions, glfwExtensions + glfwExtensionsCount);

            UVKE_LOG("Vulkan - " + std::to_string(VK_API_VERSION_MAJOR(appInfo.apiVersion)) + "." + std::to_string(VK_API_VERSION_MINOR(appInfo.apiVersion)) + "." + std::to_string(VK_API_VERSION_PATCH(appInfo.apiVersion)));
            UVKE_LOG("App - " + std::string(appInfo.pApplicationName));

            for(auto i = 0; i < layers.size(); ++i) {
                UVKE_LOG("Layer #" + std::to_string(i) + " - " + std::string(layers.at(i)));
            }

            for(auto i = 0; i < extensions.size(); ++i) {
                UVKE_LOG("Extension #" + std::to_string(i) + " - " + std::string(extensions.at(i)));
            }

            VkInstanceCreateInfo instanceCreateInfo { };
            instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            instanceCreateInfo.pNext = nullptr;
            instanceCreateInfo.flags = 0;
            instanceCreateInfo.pApplicationInfo = &appInfo;
            instanceCreateInfo.enabledLayerCount = layers.size();
            instanceCreateInfo.ppEnabledLayerNames = layers.data();
            instanceCreateInfo.enabledExtensionCount = extensions.size();
            instanceCreateInfo.ppEnabledExtensionNames = extensions.data();

            UVKE_ASSERT(vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance));
        }

        m_physicalDevice = GetSuitablePhysicalDevice();
        unsigned int queueFamilyIndex = GetQueueFamily();

        {
            VkPhysicalDeviceFeatures physicalDeviceFeatures { };
            vkGetPhysicalDeviceFeatures(m_physicalDevice, &physicalDeviceFeatures);

            std::vector<float> deviceQueuePriorities(4);
            for(auto i = 0; i < deviceQueuePriorities.size(); ++i) {
                deviceQueuePriorities[i] = 1.0f;
            }

            std::vector<const char*> deviceExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
            };

            VkDeviceQueueCreateInfo deviceQueueCreateInfo { };
            deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            deviceQueueCreateInfo.pNext = nullptr;
            deviceQueueCreateInfo.flags = 0;
            deviceQueueCreateInfo.queueFamilyIndex = queueFamilyIndex;
            deviceQueueCreateInfo.queueCount = deviceQueuePriorities.size();
            deviceQueueCreateInfo.pQueuePriorities = deviceQueuePriorities.data();

            VkDeviceCreateInfo deviceCreateInfo { };
            deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            deviceCreateInfo.pNext = nullptr;
            deviceCreateInfo.flags = 0;
            deviceCreateInfo.queueCreateInfoCount = 1;
            deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
            deviceCreateInfo.enabledLayerCount = 0;
            deviceCreateInfo.ppEnabledLayerNames = nullptr;
            deviceCreateInfo.enabledExtensionCount = deviceExtensions.size();
            deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
            deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;

            UVKE_ASSERT(vkCreateDevice(m_physicalDevice, &deviceCreateInfo, nullptr, &m_device));
        }

        m_window.CreateSurface(m_instance, &m_surface);

        {
            vkGetDeviceQueue(m_device, queueFamilyIndex, 0, &m_queue);

            UVKE_LOG("Queue Family Index - " + std::to_string(queueFamilyIndex));

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, queueFamilyIndex, m_surface, &presentSupport);
            UVKE_LOG("Queue Present Support - " + std::string(presentSupport ? "True" : "False"));
        }

        m_surfaceFormat = GetSurfaceFormat();
        m_presentMode = GetPresentMode();
        m_extent = GetSwapExtent();

        UVKE_LOG("Format - " + std::to_string(m_surfaceFormat.format));
        UVKE_LOG("Present Mode - " + std::to_string(m_presentMode));
        UVKE_LOG("Extent - " + std::to_string(m_extent.width) + "/" + std::to_string(m_extent.height));

        {
            VkSwapchainCreateInfoKHR swapchainCreateInfo { };
            swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            swapchainCreateInfo.pNext = nullptr;
            swapchainCreateInfo.flags = 0;
            swapchainCreateInfo.surface = m_surface;
            swapchainCreateInfo.minImageCount = m_swapchainImageCount;
            swapchainCreateInfo.imageFormat = m_surfaceFormat.format;
            swapchainCreateInfo.imageColorSpace = m_surfaceFormat.colorSpace;
            swapchainCreateInfo.imageExtent = m_extent;
            swapchainCreateInfo.imageArrayLayers = 1;
            swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            // TODO SUPPORT FOR MORE QUEUES
            swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            swapchainCreateInfo.queueFamilyIndexCount = 0;
            swapchainCreateInfo.pQueueFamilyIndices = nullptr;
            swapchainCreateInfo.preTransform = m_swapchainPreTransform;
            swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            swapchainCreateInfo.presentMode = m_presentMode;
            swapchainCreateInfo.clipped = VK_TRUE;
            swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

            UVKE_ASSERT(vkCreateSwapchainKHR(m_device, &swapchainCreateInfo, nullptr, &m_swapchain));
        }

        {
            unsigned int swapchainImageCount = 0;
            vkGetSwapchainImagesKHR(m_device, m_swapchain, &swapchainImageCount, nullptr);
            m_swapchainImages = std::vector<VkImage>(swapchainImageCount);
            vkGetSwapchainImagesKHR(m_device, m_swapchain, &swapchainImageCount, m_swapchainImages.data());
        }

        {
            m_swapchainImageViews = std::vector<VkImageView>(m_swapchainImages.size());

            for(auto i = 0; i < m_swapchainImageViews.size(); ++i) {
                VkImageViewCreateInfo imageViewCreateInfo { };
                imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
                imageViewCreateInfo.pNext = nullptr;
                imageViewCreateInfo.flags = 0;
                imageViewCreateInfo.image = m_swapchainImages[i];
                imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
                imageViewCreateInfo.format = m_surfaceFormat.format;
                imageViewCreateInfo.components = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
                imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
                imageViewCreateInfo.subresourceRange.levelCount = 1;
                imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
                imageViewCreateInfo.subresourceRange.layerCount = 1;

                UVKE_ASSERT(vkCreateImageView(m_device, &imageViewCreateInfo, nullptr, &m_swapchainImageViews[i]));
            }
        }

        Shader shader(m_device, File::Load("Resource/Shader.vert.spv"), File::Load("Resource/Shader.frag.spv"));
        UVKE_LOG("Shaders Loaded");

        {
            VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfos[] = { *shader.GetVertexShaderStageCreateInfo(), *shader.GetFragmentShaderStageCreateInfo() };

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

            m_vertexBuffer = new VertexBuffer(m_physicalDevice, m_device);

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
            viewport.width = m_extent.width;
            viewport.height = m_extent.height;
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;

            VkRect2D scissor { };
            scissor.offset = { 0, 0 };
            scissor.extent = m_extent;

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
            pipelineRasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
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
            attachmentDescription.format = m_surfaceFormat.format;
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
            pipelineLayoutCreateInfo.setLayoutCount = 0;
            pipelineLayoutCreateInfo.pSetLayouts = nullptr;
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
            UVKE_LOG("Graphics Pipeline Created");
        }

        {
            m_framebuffers = std::vector<VkFramebuffer>(m_swapchainImageViews.size());

            for(auto i = 0; i < m_framebuffers.size(); ++i) {
                VkImageView imageView[] = { m_swapchainImageViews[i] };

                VkFramebufferCreateInfo framebufferCreateInfo { };
                framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
                framebufferCreateInfo.pNext = nullptr;
                framebufferCreateInfo.flags = 0;
                framebufferCreateInfo.renderPass = m_renderPass;
                framebufferCreateInfo.attachmentCount = 1;
                framebufferCreateInfo.pAttachments = imageView;
                framebufferCreateInfo.width = m_extent.width;
                framebufferCreateInfo.height = m_extent.height;
                framebufferCreateInfo.layers = 1;

                UVKE_ASSERT(vkCreateFramebuffer(m_device, &framebufferCreateInfo, nullptr, &m_framebuffers[i]));
            }

            UVKE_LOG("Framebuffers Created");
        }

        {
            VkCommandPoolCreateInfo commandPoolCreateInfo { };
            commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            commandPoolCreateInfo.pNext = nullptr;
            commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
            commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndex;

            UVKE_ASSERT(vkCreateCommandPool(m_device, &commandPoolCreateInfo, nullptr, &m_commandPool));
        }

        {
            VkCommandBufferAllocateInfo commandBufferAllocateInfo { };
            commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            commandBufferAllocateInfo.pNext = nullptr;
            commandBufferAllocateInfo.commandPool = m_commandPool;
            commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            commandBufferAllocateInfo.commandBufferCount = 1;

            UVKE_ASSERT(vkAllocateCommandBuffers(m_device, &commandBufferAllocateInfo, &m_commandBuffer));
        }

        {
            VkSemaphoreCreateInfo semaphoreCreateInfo { };
            semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
            semaphoreCreateInfo.pNext = nullptr;
            semaphoreCreateInfo.flags = 0;

            UVKE_ASSERT(vkCreateSemaphore(m_device, &semaphoreCreateInfo, nullptr, &m_availableSemaphore));
            UVKE_ASSERT(vkCreateSemaphore(m_device, &semaphoreCreateInfo, nullptr, &m_finishedSemaphore));

            VkFenceCreateInfo fenceCreateInfo { };
            fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            fenceCreateInfo.pNext = nullptr;
            fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

            UVKE_ASSERT(vkCreateFence(m_device, &fenceCreateInfo, nullptr, &m_fence));
        }
    }

    Renderer::~Renderer() {
        vkDeviceWaitIdle(m_device);

        vkDestroyFence(m_device, m_fence, nullptr);
        vkDestroySemaphore(m_device, m_finishedSemaphore, nullptr);
        vkDestroySemaphore(m_device, m_availableSemaphore, nullptr);

        vkDestroyCommandPool(m_device, m_commandPool, nullptr);

        for(auto i = 0; i < m_framebuffers.size(); ++i) {
            vkDestroyFramebuffer(m_device, m_framebuffers[i], nullptr);
        }

        vkDestroyPipeline(m_device, m_pipeline, nullptr);

        vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr);
        vkDestroyRenderPass(m_device, m_renderPass, nullptr);

        m_vertexBuffer->~VertexBuffer();
        delete m_vertexBuffer;

        for(auto i = 0; i < m_swapchainImageViews.size(); ++i) {
            vkDestroyImageView(m_device, m_swapchainImageViews[i], nullptr);
        }

        vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
        vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
        vkDestroyDevice(m_device, nullptr);
        vkDestroyInstance(m_instance, nullptr);
    }

    void Renderer::Render() {
        vkQueueWaitIdle(m_queue);

        unsigned int index = 0;
        VkResult result = vkAcquireNextImageKHR(m_device, m_swapchain, std::numeric_limits<unsigned long long>::infinity(), m_availableSemaphore, VK_NULL_HANDLE, &index);
        if(result == VK_ERROR_OUT_OF_DATE_KHR) {
            RecreateSwapchain();
        } else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            UVKE_FATAL("Swapchain Acquire Image Error");
        }
        
        // TODO SUPPORT FOR MORE FENCES AND IMAGES IN FLIGHT
        vkWaitForFences(m_device, 1, &m_fence, VK_TRUE, std::numeric_limits<unsigned long long>::infinity());
        vkResetFences(m_device, 1, &m_fence);

        vkResetCommandBuffer(m_commandBuffer, 0);
        RecordCommandBuffer(m_commandBuffer, index);
        
        VkSubmitInfo submitInfo { };
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.pNext = nullptr;
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &m_availableSemaphore;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &m_commandBuffer;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &m_finishedSemaphore;

        UVKE_ASSERT(vkQueueSubmit(m_queue, 1, &submitInfo, m_fence));

        VkPresentInfoKHR presentInfo { };
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.pNext = nullptr;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &m_finishedSemaphore;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &m_swapchain;
        presentInfo.pImageIndices = &index;
        presentInfo.pResults = nullptr;

        result = vkQueuePresentKHR(m_queue, &presentInfo);
        if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_framebufferRecreated) {
            m_framebufferRecreated = false;
            RecreateSwapchain();
        } else if(result != VK_SUCCESS) {
            UVKE_FATAL("Framebuffer Recreation Error");
        }
    }
};