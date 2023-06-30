#include "Interface.hpp"

namespace uvke {
    Interface::Interface(Base* base, Window* window, Surface* surface, CommandBuffer* commandBuffer, VkRenderPass renderPass)
        : m_base(base), m_renderTime(0.0f), m_fps(0) {
        std::vector<VkDescriptorPoolSize> poolSizes = {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
        };

        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1000;
        pool_info.poolSizeCount = poolSizes.size();
        pool_info.pPoolSizes = poolSizes.data();

        UVKE_ASSERT(vkCreateDescriptorPool(m_base->GetDevice(), &pool_info, nullptr, &m_descriptorPool));

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = nullptr;

        ImGui::StyleColorsDark();
        ImGui::GetStyle().Colors[ImGuiCol_TitleBg] = ImVec4(1.0f, 0.33f, 0.01f, 1.0f);
        ImGui::GetStyle().Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.0f, 0.33f, 0.01f, 1.0f);
        ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive] = ImVec4(1.0f, 0.33f, 0.01f, 1.0f);

        ImGui_ImplGlfw_InitForVulkan(window->GetWindow(), true);

        ImGui_ImplVulkan_InitInfo imguiVulkanInitInfo = { };
        imguiVulkanInitInfo.Instance = m_base->GetInstance();
        imguiVulkanInitInfo.PhysicalDevice = m_base->GetPhysicalDevice();
        imguiVulkanInitInfo.Device = m_base->GetDevice();
        imguiVulkanInitInfo.Queue = surface->GetQueue(0);
        imguiVulkanInitInfo.DescriptorPool = m_descriptorPool;
        imguiVulkanInitInfo.MinImageCount = 3;
        imguiVulkanInitInfo.ImageCount = 3;
        imguiVulkanInitInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

        ImGui_ImplVulkan_Init(&imguiVulkanInitInfo, renderPass);

        VkCommandBuffer fontsCommandBuffer = commandBuffer->Begin();
        ImGui_ImplVulkan_CreateFontsTexture(fontsCommandBuffer);
        commandBuffer->End(fontsCommandBuffer, surface->GetQueue(0));

        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }

    Interface::~Interface() {
        vkDestroyDescriptorPool(m_base->GetDevice(), m_descriptorPool, nullptr);
        ImGui_ImplVulkan_Shutdown();
    }

    void Interface::Render(CommandBuffer* commandBuffer, unsigned int frame) {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Stats");

        ImGui::Text("Render Time: %.4fms", m_renderTime);
        ImGui::Text("FPS: %i", m_fps);

        ImGui::End();

        ImGui::Render();
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer->GetCommandBuffer(frame));
    }

    void Interface::SetRenderTime(float renderTime) {
        m_renderTime = renderTime;
    }

    void Interface::SetFPS(int fps) {
        m_fps = fps;
    }
};