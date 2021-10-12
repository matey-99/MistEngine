#pragma once

#include "Scene.h"

class Editor;

class ContentBrowserPanel
{
private:
	Ref<Editor> m_Editor;
	Ref<Scene> m_Scene;
	std::vector<std::string> m_SupportedFileFormats;

public:
	ContentBrowserPanel(Ref<Editor> editor, Ref<Scene> scene);
	void Render();

private:
	void ShowObjects();
	void ShowContextMenu();
};