#pragma once

#include "typedefs.h"
#include "SceneHierarchyPanel.h"
#include "EntityDetailsPanel.h"
#include "ContentBrowserPanel.h"
#include "MaterialEditorPanel.h"

class Editor : public std::enable_shared_from_this<Editor>
{
private:
	Ref<SceneHierarchyPanel> m_SceneHierarchyPanel;
	Ref<EntityDetailsPanel> m_EntityDetailsPanel;
	Ref<ContentBrowserPanel> m_ContentBrowserPanel;
	Ref<MaterialEditorPanel> m_MaterialEditorPanel;

	bool m_DetailsPanel;
	bool m_MaterialEditor;

public:
	Editor();
	void Initialize(Ref<Scene> scene);
	void Update();

	void ShowDetails(Ref<Entity> entity);
	void HideDetails();

	void ShowMaterialEditor(Ref<Material> material);
	void HideMaterialEditor();

private:
	Ref<Editor> GetReference();
};