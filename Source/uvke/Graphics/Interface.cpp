#include "Interface.hpp"

namespace uvke {
    Interface::Interface(Base* base, Window* window, Surface* surface, CommandBuffer* commandBuffer, VkRenderPass renderPass)
        : m_base(base), m_stats({ }) {
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
        imguiVulkanInitInfo.MSAASamples = m_base->GetSampleCount();

        ImGui_ImplVulkan_Init(&imguiVulkanInitInfo, renderPass);

        ImGui_ImplVulkan_CreateFontsTexture();

        UVKE_LOG_ADDRESS("Interface Created");
    }

    Interface::~Interface() {
        ImGui_ImplVulkan_DestroyFontsTexture();
        
        vkDestroyDescriptorPool(m_base->GetDevice(), m_descriptorPool, nullptr);
        ImGui_ImplVulkan_Shutdown();

        UVKE_LOG_ADDRESS("Interface Destroyed");
    }

    void Interface::Render(CommandBuffer* commandBuffer, unsigned int frame) {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Stats");

        ImGui::Text("Render Time - %.4fms", m_stats.renderTime);
        ImGui::Text("Frame Per Second - %i", m_stats.fps);
        ImGui::Text("Position - %f/%f/%f", m_stats.position.x, m_stats.position.y, m_stats.position.z);
        ImGui::Text("Yaw - %f", m_stats.yaw);
        ImGui::Text("Pitch - %f", m_stats.pitch);
        ImGui::Text("Direction - %f/%f/%f", m_stats.direction.x, m_stats.direction.y, m_stats.direction.z);


        ImGui::End();

        ImGui::Render();
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer->GetCommandBuffer(frame));
    }

    void Interface::SetStats(Stats stats) {
        m_stats = stats;
    }
};