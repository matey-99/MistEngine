#pragma once

#include "Scene.h"
#include "imgui.h"

class Editor;

class SceneHierarchyPanel
{
private:
	Ref<Editor> m_Editor;
	Ref<Scene> m_Scene;
	Ref<Entity> m_SelectedEntity;

	std::vector<Ref<Entity>> m_OrderedEntities;

public:
	SceneHierarchyPanel(Ref<Editor> editor, Ref<Scene> scene);
	void Render();

private:
	void TreeChildren(Ref<Entity> entity);
	void AddEntityChildren(Ref<Entity> parent);
};