#include "ContentBrowserPanel.h"

#include "imgui.h"
#include "Scene.h"

ContentBrowserPanel::ContentBrowserPanel(Ref<Scene> scene) : m_Scene(scene)
{
	m_SupportedFileFormats.push_back("obj");
	m_SupportedFileFormats.push_back("3ds");
	m_SupportedFileFormats.push_back("fbx");
}

void ContentBrowserPanel::Render()
{
	ImGui::Begin("Content Browser");
	
	std::string path = "res/models";
	for (auto& file : std::filesystem::recursive_directory_iterator(path))
	{
		std::stringstream ss;
		ss << file.path();
		std::string filepath = ss.str();

		bool supported = false;
		for (std::string str : m_SupportedFileFormats)
		{
			if (str == filepath.substr(filepath.find_last_of(".") + 1, filepath.find_last_of('"') - (filepath.find_last_of(".") + 1)))
			{
				supported = true;
				break;
			}
		}
		if (!supported)
			continue;

		std::string filename = filepath.substr(filepath.find_last_of("\\") + 1, filepath.find_last_of('"') - (filepath.find_last_of("\\") + 1));
		std::string entityName = filepath.substr(filepath.find_last_of("\\") + 1, filepath.find_last_of('.') - (filepath.find_last_of("\\") + 1));
		std::string correctedFilepath = filepath.substr(filepath.find_first_of('"') + 1, filepath.length() - 2);
		
		std::size_t index = 0;
		while (true)
		{
			index = correctedFilepath.find("\\\\", index);
			if (index == std::string::npos)
				break;

			correctedFilepath.replace(index, 2, "/");
		}

		bool selected = false;
		ImGui::Selectable(filename.c_str(), &selected);

		if (selected)
		{
			unsigned int countSameName = 0;
			for (auto entity : m_Scene->GetEntities())
			{
				if (entity->GetName().substr(0, entity->GetName().find_last_of(" ")) == entityName)
					countSameName++;
			}
			if (countSameName > 0)
			{
				entityName += " (" + std::to_string(countSameName) + ")";
			}

			m_Scene->AddEntity(correctedFilepath, entityName);
		}
	}

	ImGui::End();
}
