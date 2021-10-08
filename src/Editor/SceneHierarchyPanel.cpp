#include "SceneHierarchyPanel.h"

#include "imgui.h"
#include "Editor.h"

SceneHierarchyPanel::SceneHierarchyPanel(Ref<Editor> editor, Ref<Scene> scene) : m_Editor(editor), m_Scene(scene)
{
	m_SelectedEntity = Ref<Entity>();
}

void SceneHierarchyPanel::Render()
{
	ImGui::Begin("Scene Hierarchy");
	for (auto entity : m_Scene->GetEntities())
	{
		for (unsigned int i = 0; i < entity->GetTransform()->FindDepth(entity->GetTransform()); i++)
		{
			ImGui::Text("->");
			ImGui::SameLine();
		}

		bool selected = false;
		ImGui::Selectable(entity->GetName().c_str(), &selected);

		if (selected)
			m_Editor->ShowDetails(entity);
	}
	ImGui::End();
}
