#include "Editor.h"

Editor::Editor()
{
	m_SceneHierarchyPanel = Ref<SceneHierarchyPanel>();
	m_EntityDetailsPanel = Ref<EntityDetailsPanel>();
	m_ContentBrowserPanel = Ref<ContentBrowserPanel>();
	m_MaterialEditorPanel = Ref<MaterialEditorPanel>();

	m_DetailsPanel = false;
	m_MaterialEditor = false;
}

void Editor::Initialize(Ref<Scene> scene)
{
	m_SceneHierarchyPanel = CreateRef<SceneHierarchyPanel>(GetReference(), scene);
	m_EntityDetailsPanel = CreateRef<EntityDetailsPanel>(GetReference());
	m_ContentBrowserPanel = CreateRef<ContentBrowserPanel>(GetReference(), scene);
	m_MaterialEditorPanel = CreateRef<MaterialEditorPanel>(GetReference());
}

void Editor::Update()
{
	m_SceneHierarchyPanel->Render();

	if (m_DetailsPanel)
	{
		m_EntityDetailsPanel->Render();
	}
	if (m_MaterialEditor)
	{
		m_MaterialEditorPanel->Render();
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

void Editor::ShowMaterialEditor(Ref<Material> material)
{
	m_MaterialEditorPanel->SetMaterial(material);
	m_MaterialEditor = true;
}

void Editor::HideMaterialEditor()
{
	m_MaterialEditor = false;
}

Ref<Editor> Editor::GetReference()
{
	return shared_from_this();
}
