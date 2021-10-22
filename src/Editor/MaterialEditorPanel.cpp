#include "MaterialEditorPanel.h"

#include "imgui.h"
#include "Editor.h"
#include "Serialization/MaterialSerializer.h"

MaterialEditorPanel::MaterialEditorPanel(Ref<Editor> editor) : m_Editor(editor)
{
	m_Material = Ref<Material>();
}

void MaterialEditorPanel::Render()
{
	ImGui::Begin("Material Editor");

    ImGui::Text(m_Material->GetName().c_str());
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    auto color = &m_Material->m_Color;
    ImGui::ColorEdit3("Color", (float*)color);

    ImGui::DragFloat("Shininess", &m_Material->m_Shininess, 1.0f, 0.0f, 256.0f);
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    if (ImGui::Button("Save"))
        MaterialSerializer::Serialize(m_Material);

    if (ImGui::Button("Close"))
        m_Editor->HideMaterialEditor();

	ImGui::End();
}
