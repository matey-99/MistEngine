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

public:
	SceneHierarchyPanel(Ref<Editor> editor, Ref<Scene> scene);
	void Render();

	void UnselectEntity();

	inline Ref<Entity> GetSelectedEntity() const { return m_SelectedEntity; }

private:
	void TreeChildren(Ref<Entity> entity);
};