#include "Editor.h"

Ref<Editor> Editor::s_Instance{};
std::mutex Editor::s_Mutex;

Editor::Editor()
{
	m_SceneHierarchyPanel = Ref<SceneHierarchyPanel>();
	m_EntityDetailsPanel = Ref<EntityDetailsPanel>();
	m_ContentBrowserPanel = Ref<ContentBrowserPanel>();
	m_MaterialEditorPanel = Ref<MaterialEditorPanel>();
	m_WorldSettingsPanel = Ref<WorldSettingsPanel>();
	m_Viewport = Ref<Viewport>();

	m_DetailsPanel = false;
	m_MaterialEditor = false;
}


Editor::~Editor()
{
}

Ref<Editor> Editor::GetInstance()
{
	std::lock_guard<std::mutex> lock(s_Mutex);
	if (s_Instance == nullptr)
		s_Instance = CreateRef<Editor>();

	return s_Instance;
}

void Editor::Initialize(Ref<Scene> scene)
{
	m_SceneHierarchyPanel = CreateRef<SceneHierarchyPanel>(GetReference(), scene);
	m_EntityDetailsPanel = CreateRef<EntityDetailsPanel>(GetReference());
	m_ContentBrowserPanel = CreateRef<ContentBrowserPanel>(GetReference(), scene);
	m_MaterialEditorPanel = CreateRef<MaterialEditorPanel>(GetReference());
	m_WorldSettingsPanel = CreateRef<WorldSettingsPanel>(GetReference(), scene);
	m_Viewport = CreateRef<Viewport>(GetReference(), scene);
}

void Editor::Render()
{
	m_SceneHierarchyPanel->Render();
	m_WorldSettingsPanel->Render();

	if (m_DetailsPanel)
	{
		m_EntityDetailsPanel->Render();
	}
	if (m_MaterialEditor)
	{
		m_MaterialEditorPanel->Render();
	}

	m_ContentBrowserPanel->Render();
	m_Viewport->Render(Renderer::GetInstance()->GetMainSceneFramebuffer());
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
