#pragma once

#include "Scene.h"

class ContentBrowserPanel
{
private:
	Ref<Scene> m_Scene;
	std::vector<std::string> m_SupportedFileFormats;

public:
	ContentBrowserPanel(Ref<Scene> scene);
	void Render();

};