#include "ImGuiRenderer.h"

#include <glm/gtc/type_ptr.hpp>
#include <ImGuizmo.h>

#include "Math/Math.h"

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

    m_Editor = Editor::GetInstance();
    m_Editor->Initialize(m_Scene);
}

void ImGuiRenderer::Render(Ref<Framebuffer> framebuffer)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();

    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    ImGui::Begin("Debug");
    ImGui::Text("%.1f FPS (%.3f ms/frame)", ImGui::GetIO().Framerate, 1000 / ImGui::GetIO().Framerate);
    ImGui::End();

    ImGui::Begin("Camera");
    ImGui::Text("Position: x = %f, y = %f, z = %f", m_Scene->GetCamera()->Position.x, m_Scene->GetCamera()->Position.y, m_Scene->GetCamera()->Position.z);
    ImGui::Text("Rotation: yaw = %f, pitch = %f", m_Scene->GetCamera()->Yaw, m_Scene->GetCamera()->Pitch);
    ImGui::DragFloat("Movement speed", &m_Scene->GetCamera()->MovementSpeed);
    ImGui::End();

    ImGui::Begin("Toolbar");
    if (ImGui::Button("Translate"))
        m_Editor->SetGizmoOperation(ImGuizmo::OPERATION::TRANSLATE);
    ImGui::SameLine();
    if (ImGui::Button("Rotate"))
        m_Editor->SetGizmoOperation(ImGuizmo::OPERATION::ROTATE);
    ImGui::SameLine();
    if (ImGui::Button("Scale"))
        m_Editor->SetGizmoOperation(ImGuizmo::OPERATION::SCALE);
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

    Ref<Entity> selectedEntity = m_Editor->GetSceneHierarchyPanel()->GetSelectedEntity();
    if (selectedEntity)
    {
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

        Ref<Camera> camera = m_Scene->GetCamera();

        Ref<Transform> transform = selectedEntity->GetTransform();
        
        glm::mat4 view = camera->GetViewMatrix();
        glm::mat4 projection = camera->GetProjectionMatrix();
        glm::mat4 model = transform->ModelMatrix;

        ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection),
            m_Editor->GetGizmoOperation(), ImGuizmo::LOCAL, glm::value_ptr(model));

        if (ImGuizmo::IsUsing())
        {
            glm::vec3 position, rotation, scale;
            Math::DecomposeMatrix(model, position, rotation, scale);

            switch (m_Editor->GetGizmoOperation())
            {
            case ImGuizmo::OPERATION::TRANSLATE:
                transform->SetWorldPosition(position / scale);
                break;
            case ImGuizmo::OPERATION::ROTATE:
                transform->LocalRotation = glm::degrees(rotation);
                break;
            case ImGuizmo::OPERATION::SCALE:
                transform->LocalScale = scale;
                break;
            }
        }
    }

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
