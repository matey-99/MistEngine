#include "SceneHierarchyPanel.h"

#include "imgui.h"
#include "Editor.h"

SceneHierarchyPanel::SceneHierarchyPanel(Ref<Editor> editor, Ref<Scene> scene) : m_Editor(editor), m_Scene(scene)
{
	m_SelectedEntity = Ref<Entity>();
	m_OrderedEntities = std::vector<Ref<Entity>>();
}

void SceneHierarchyPanel::Render()
{
	m_OrderedEntities = std::vector<Ref<Entity>>();
	for (auto entity : m_Scene->GetEntities())
	{
		if (!entity->GetTransform()->Parent)
		{
			m_OrderedEntities.push_back(entity);

			AddEntityChildren(entity);
		}
	}

	ImGui::Begin("Scene Hierarchy");
	for (auto entity : m_OrderedEntities)
	{
		for (unsigned int i = 0; i < entity->GetTransform()->FindDepth(entity->GetTransform()); i++)
		{
			ImGui::Text("|_");
			ImGui::SameLine();
		}

		if (ImGui::Button(entity->GetName().c_str()))
		{
			m_Editor->ShowDetails(entity);
		}

		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("ENTITY", &entity, sizeof(Entity));
			ImGui::Text(entity->GetName().c_str());
			ImGui::EndDragDropSource();
		}
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY"))
			{
				Ref<Entity>* childEntity = static_cast<Ref<Entity>*>(payload->Data);
				childEntity->get()->GetTransform()->SetParent(entity->GetTransform());
			}
		}
	}
	ImGui::End();
}

void SceneHierarchyPanel::AddEntityChildren(Ref<Entity> parent)
{
	for (auto entity : parent->GetTransform()->Children)
	{
		m_OrderedEntities.push_back(entity->GetEntity());

		if (entity->Children.size() > 0)
			AddEntityChildren(entity->GetEntity());
	}
}
