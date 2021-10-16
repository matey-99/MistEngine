#include "ImGuiRenderer.h"

#include "Serialization/SceneSerializer.h"

ImGuiRenderer::ImGuiRenderer()
{
}

void ImGuiRenderer::Setup(GLFWwindow* window, const char* glsl_version, Ref<Scene> scene)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_IO = &ImGui::GetIO(); (void)m_IO;

    m_IO->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    m_IO->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    m_IO->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (m_IO->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    m_Scene = scene;

    m_Editor = CreateRef<Editor>();
    m_Editor->Initialize(m_Scene);
}

void ImGuiRenderer::Render(Ref<Framebuffer> framebuffer)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    ImGui::Begin("Debug");
    ImGui::Text("%.1f FPS (%.3f ms/frame)", ImGui::GetIO().Framerate, 1000 / ImGui::GetIO().Framerate);
    ImGui::End();

    ImGui::Begin("Scene");

    auto bg = m_Scene->GetBackgroundColor();
    ImGui::ColorEdit3("Background color", (float*)&ImVec4((*bg).x, (*bg).y, (*bg).z, (*bg).w));

    if (ImGui::Button("Save scene"))
        SceneSerializer::Serialize(m_Scene);

    ImGui::End();

    ImGui::Begin("Camera");
    ImGui::Text("Position: x = %f, y = %f, z = %f", m_Scene->GetCamera()->Position.x, m_Scene->GetCamera()->Position.y, m_Scene->GetCamera()->Position.z);
    ImGui::Text("Rotation: yaw = %f, pitch = %f", m_Scene->GetCamera()->Yaw, m_Scene->GetCamera()->Pitch);
    ImGui::DragFloat("Movement speed", &m_Scene->GetCamera()->MovementSpeed);
    ImGui::End();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Viewport");
    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
    {
        m_ViewportSize = glm::vec2(viewportPanelSize.x, viewportPanelSize.y);
        framebuffer->Resize((unsigned int)m_ViewportSize.x, (unsigned int)m_ViewportSize.y);
    }
    uint32_t viewportTexture = framebuffer->GetColorAttachment();
    ImGui::Image((void*)viewportTexture, ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
    ImGui::PopStyleVar();

    m_Editor->Update();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiRenderer::EndFrame()
{
    if (m_IO->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void ImGuiRenderer::CleanUp()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiRenderer::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
}
