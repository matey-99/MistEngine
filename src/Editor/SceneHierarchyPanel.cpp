#include "SceneHierarchyPanel.h"

#include "Editor.h"

SceneHierarchyPanel::SceneHierarchyPanel(Ref<Editor> editor, Ref<Scene> scene) : m_Editor(editor), m_Scene(scene)
{
	m_SelectedEntity = Ref<Entity>();
	m_OrderedEntities = std::vector<Ref<Entity>>();
}

void SceneHierarchyPanel::TreeChildren(Ref<Entity> entity)
{
	auto children = entity->GetTransform()->Children;
	for (int i = 0; i < children.size(); i++)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
		if (children[i]->Children.empty())
			flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;
		else
			flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

		bool open = false;
		auto e = m_Scene->FindEntity(children[i]->ID);
		open = ImGui::TreeNodeEx(e->GetName().c_str(), flags);

		if (ImGui::IsItemClicked())
			m_Editor->ShowDetails(e);

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("entity"))
			{
				Ref<Entity>* childEntity = static_cast<Ref<Entity>*>(payload->Data);
				childEntity->get()->GetTransform()->SetParent(children[i]);
			}
			ImGui::EndDragDropTarget();
		}

		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("entity", &e, sizeof(Entity));
			ImGui::Text(e->GetName().c_str());
			ImGui::EndDragDropSource();
		}

		if (!(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen) && open)
		{
			TreeChildren(e);
			ImGui::TreePop();
		}
	}
}

void SceneHierarchyPanel::Render()
{
	ImGui::Begin("Scene Hierarchy");
	if (ImGui::TreeNodeEx("Root", ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::IsItemClicked())
			m_Editor->ShowDetails(m_Scene->GetRoot());

		TreeChildren(m_Scene->GetRoot());
		ImGui::TreePop();
	}

	bool addEntity = false;
	if (ImGui::BeginPopupContextWindow())
	{
		ImGui::MenuItem("Add Entity", "", &addEntity);

		ImGui::EndPopup();
	}

	if (addEntity)
	{
		std::string entityName = "New Entity";
		bool unique = false;
		int nr = 1;
		while (!unique)
		{
			unique = true;
			for (auto entity : m_Scene->GetEntities())
			{
				if (entity->GetName() == entityName)
				{
					entityName = "New Entity (" + std::to_string(nr) + ")";
					nr++;
					unique = false;
				}
			}
		}

		m_Scene->AddEntity(entityName);
	}

	ImGui::End();
}

void SceneHierarchyPanel::AddEntityChildren(Ref<Entity> parent)
{
	for (auto transform : parent->GetTransform()->Children)
	{
		auto entity = m_Scene->FindEntity(transform->ID);
		m_OrderedEntities.push_back(entity);

		if (transform->Children.size() > 0)
			AddEntityChildren(entity);
	}
}
