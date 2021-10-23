#include "DebugPanel.h"

#include "Editor.h"
#include "Scene/Component/Model.h"

DebugPanel::DebugPanel(Ref<Editor> editor) 
	: m_Editor(editor)
{
}

void DebugPanel::Render()
{
	ImGui::Begin("Debug");

	ImGui::Text("Framerate: %.1f FPS (%.3f ms/frame)", ImGui::GetIO().Framerate, 1000 / ImGui::GetIO().Framerate);

	auto selectedEntity = m_Editor->GetSceneHierarchyPanel()->GetSelectedEntity();
	if (selectedEntity)
	{
		ImGui::Text("Selected entity: ");
		ImGui::SameLine();
		ImGui::Text(selectedEntity->GetName().c_str());

		if (selectedEntity->GetComponent<Model>())
			ImGui::Text("Vertices: %i", selectedEntity->GetComponent<Model>()->GetRenderedVerticesCount());
	}

	ImGui::End();
}
