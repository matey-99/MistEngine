#pragma once

#include "typedefs.h"
#include "SceneHierarchyPanel.h"
#include "EntityDetailsPanel.h"

class Editor : public std::enable_shared_from_this<Editor>
{
private:
	Ref<SceneHierarchyPanel> m_SceneHierarchyPanel;
	Ref<EntityDetailsPanel> m_EntityDetailsPanel;

	bool m_DetailsPanel;

public:
	Editor();
	void Initialize(Ref<Scene> scene);
	void Update();

	void ShowDetails(Ref<Entity> entity);
	void HideDetails();

private:
	Ref<Editor> GetReference();
};