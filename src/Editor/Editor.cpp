#include "Editor.h"

Editor::Editor()
{
	m_SceneHierarchyPanel = Ref<SceneHierarchyPanel>();
	m_EntityDetailsPanel = Ref<EntityDetailsPanel>();
	m_ContentBrowserPanel = Ref<ContentBrowserPanel>();

	m_DetailsPanel = false;
}

void Editor::Initialize(Ref<Scene> scene)
{
	m_SceneHierarchyPanel = CreateRef<SceneHierarchyPanel>(GetReference(), scene);
	m_EntityDetailsPanel = CreateRef<EntityDetailsPanel>(GetReference());
	m_ContentBrowserPanel = CreateRef<ContentBrowserPanel>(scene);
}

void Editor::Update()
{
	m_SceneHierarchyPanel->Render();

	if (m_DetailsPanel)
	{
		m_EntityDetailsPanel->Render();
	}

	m_ContentBrowserPanel->Render();
}

void Editor::ShowDetails(Ref<Entity> entity)
{
	m_EntityDetailsPanel->SetEntity(entity);
	m_DetailsPanel = true;
}

void Editor::HideDetails()
{
	m_DetailsPanel = false;
}

Ref<Editor> Editor::GetReference()
{
	return shared_from_this();
}
