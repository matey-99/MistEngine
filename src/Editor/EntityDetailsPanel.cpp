#include "EntityDetailsPanel.h"

#include "imgui.h"
#include "Editor.h"

EntityDetailsPanel::EntityDetailsPanel(Ref<Editor> editor) : m_Editor(editor)
{
	m_Entity = Ref<Entity>();
}

void EntityDetailsPanel::Render()
{
	ImGui::Begin("Details");
    float* arr[3];
    arr[0] = &m_Entity->GetTransform()->Position.x;
    arr[1] = &m_Entity->GetTransform()->Position.y;
    arr[2] = &m_Entity->GetTransform()->Position.z;
    ImGui::SliderFloat3("Position", *arr, -3.0f, 3.0f);

    arr[0] = &m_Entity->GetTransform()->Rotation.x;
    arr[1] = &m_Entity->GetTransform()->Rotation.y;
    arr[2] = &m_Entity->GetTransform()->Rotation.z;
    ImGui::SliderFloat3("Rotation", *arr, -180.0f, 180.0f);

    arr[0] = &m_Entity->GetTransform()->Scale.x;
    arr[1] = &m_Entity->GetTransform()->Scale.y;
    arr[2] = &m_Entity->GetTransform()->Scale.z;
    ImGui::SliderFloat3("Scale", *arr, 0.1f, 1.0f);

    if (ImGui::Button("Close"))
        m_Editor->HideDetails();

	ImGui::End();
}
