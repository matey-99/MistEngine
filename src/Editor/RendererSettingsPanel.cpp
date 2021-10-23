#include "RendererSettingsPanel.h"

RendererSettingsPanel::RendererSettingsPanel(Ref<Editor> editor, Ref<Renderer> renderer)
    : m_Editor(editor), m_Renderer(renderer)
{

}

void RendererSettingsPanel::Render()
{
    ImGui::Begin("Renderer Settings");

    ImGui::Checkbox("Post Processing", &m_Renderer->m_PostProcessing);
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::DragFloat("Gamma", &m_Renderer->m_Gamma, 0.1f, 0.0f, 10.0f);
    ImGui::DragFloat("Exposure", &m_Renderer->m_Exposure, 0.1f, 0.0f, 10.0f);

    ImGui::End();
}
