#include "SceneSerializer.h"

void SceneSerializer::Serialize(Ref<Scene> scene)
{
	std::ofstream file("../../res/scenes/basic.scene", std::ios::trunc);
	if (file.is_open())
	{
		std::string text = "";
		
		for (auto entity : scene->GetEntities())
		{
			text += "#entity\n";
			text += "\tname='" + entity->GetName() + "'\n";
			text += "\tpath='" + entity->GetPath() + "'\n";

			text += "\t#entity_transform\n";

			if (entity->GetTransform()->Parent)
				text += "\t\tparent='" + entity->GetTransform()->Parent->Owner->GetName() + "'\n";

			text += "\t\t#vec3:position:x='" + std::to_string(entity->GetTransform()->Position.x) 
				+ "':y='" + std::to_string(entity->GetTransform()->Position.y) 
				+ "':z='" + std::to_string(entity->GetTransform()->Position.z) + "'\n";
			text += "\t\t#vec3:rotation:x='" + std::to_string(entity->GetTransform()->Rotation.x)
				+ "':y='" + std::to_string(entity->GetTransform()->Rotation.y)
				+ "':z='" + std::to_string(entity->GetTransform()->Rotation.z) + "'\n";
			text += "\t\t#vec3:scale:x='" + std::to_string(entity->GetTransform()->Scale.x)
				+ "':y='" + std::to_string(entity->GetTransform()->Scale.y)
				+ "':z='" + std::to_string(entity->GetTransform()->Scale.z) + "'\n";
		}

		file << text;
	}

	file.close();

}

Ref<Scene> SceneSerializer::Deserialize(std::string path)
{
	std::ifstream file(path);
	if (!file.is_open())
	{
		file.close();
		return false;
	}

	Ref<Scene> scene = CreateRef<Scene>();

	std::string serializedScene = "";
	std::string line;
	std::vector<std::string> parentsNames = std::vector<std::string>();

	while (std::getline(file, line))
	{
		if (line == "#entity")
		{
			std::getline(file, line);
			std::string name = GetData(line);

			std::getline(file, line);
			std::string path = GetData(line);

			std::getline(file, line);
			std::getline(file, line);

			std::string parentName = "NULL";
			if ((int)line.find("parent", 0) > -1)
			{
				parentName = GetData(line);
				std::getline(file, line);
			}
			parentsNames.push_back(parentName);

			float x[3], y[3], z[3];
			for (int i = 0; i < 3; i++)
			{
				int index = line.find("x=");
				x[i] = std::stof(line.substr(line.find("x='") + 3, line.find("':y") - (line.find("x='") + 3)));
				line = line.substr(line.find("':") + 2);

				y[i] = std::stof(line.substr(3, line.find("':z") - 3));
				line = line.substr(line.find("':") + 2);

				z[i] = std::stof(GetData(line));

				if (i < 2)
					getline(file, line);
			}

			scene->AddEntity(path, name);

			Ref<Transform> transform = scene->FindEntity(name)->GetTransform();

			transform->Position.x = x[0];
			transform->Position.y = y[0];
			transform->Position.z = z[0];

			transform->Rotation.x = x[1];
			transform->Rotation.y = y[1];
			transform->Rotation.z = z[1];

			transform->Scale.x = x[2];
			transform->Scale.y = y[2];
			transform->Scale.z = z[2];
		}
	}

	for (int i = 0; i < parentsNames.size(); i++)
	{
		if (parentsNames[i] == "NULL")
			continue;

		scene->GetEntities()[i]->GetTransform()->SetParent(scene->FindEntity(parentsNames[i])->GetTransform());
	}

	file.close();
	return scene;
}

std::string SceneSerializer::GetData(std::string line)
{
	return line.substr(line.find_first_of("'") + 1, line.find_last_of("'") - (line.find_first_of("'") + 1));
}
