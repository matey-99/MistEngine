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
		bool selected = false;
		ImGui::Selectable(entity->GetName().c_str(), &selected);

		if (selected)
			m_Editor->ShowDetails(entity);
	}
	ImGui::End();
}
