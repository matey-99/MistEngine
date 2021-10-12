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

    float* arr[3];
    arr[0] = &m_Material->m_Ambient.x;
    arr[1] = &m_Material->m_Ambient.y;
    arr[2] = &m_Material->m_Ambient.z;
    ImGui::DragFloat3("Ambient", *arr, 0.01f, 0.0f, 1.0f);

    arr[0] = &m_Material->m_Diffuse.x;
    arr[1] = &m_Material->m_Diffuse.y;
    arr[2] = &m_Material->m_Diffuse.z;
    ImGui::DragFloat3("Diffuse", *arr, 0.01f, 0.0f, 1.0f);

    arr[0] = &m_Material->m_Specular.x;
    arr[1] = &m_Material->m_Specular.y;
    arr[2] = &m_Material->m_Specular.z;
    ImGui::DragFloat3("Specular", *arr, 0.01f, 0.0f, 1.0f);

    ImGui::DragFloat("Shininess", &m_Material->m_Shininess, 1.0f, 0.0f, 256.0f);
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    if (ImGui::Button("Save"))
        MaterialSerializer::Serialize(m_Material);

    if (ImGui::Button("Close"))
        m_Editor->HideMaterialEditor();

	ImGui::End();
}
