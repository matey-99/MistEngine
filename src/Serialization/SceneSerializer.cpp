#include "SceneSerializer.h"

#include "yaml.h"
#include "Model.h"
#include "Light.h"
#include "MaterialManager.h"

void SceneSerializer::Serialize(Ref<Scene> scene)
{
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "Scene" << YAML::Value << "Untitled";
	out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
	for (auto entity : scene->GetEntities())
	{
		SerializeEntity(out, entity);
	}
	out << YAML::EndSeq;
	out << YAML::EndMap;

	std::ofstream file("../../res/scenes/untitled.scene");
	file << out.c_str();
	file.close();
}

Ref<Scene> SceneSerializer::Deserialize(std::string path)
{
	Ref<Scene> scene = CreateRef<Scene>();

	std::ifstream file(path);
	std::stringstream ss;
	ss << file.rdbuf();

	YAML::Node data = YAML::Load(ss.str());
	if (!data["Scene"])
	{
		std::cout << "Cannot load scene!" << std::endl;
		return Ref<Scene>();
	}
	
	std::string sceneName = data["Scene"].as<std::string>();
	
	YAML::Node entities = data["Entities"];
	if (entities)
	{
		auto parentsIDs = std::vector<uint64_t>();

		for (auto entity : entities)
		{
			Ref<Entity> e = Ref<Entity>();
			if (entity["Transform"]["ID"].as<uint64_t>() == 0)
			{
				e = scene->AddRoot();
			}
			else
			{
				if (!scene->GetRoot())
				{
					std::cout << "Loaded scene doesn't contain root entity!" << std::endl;
				}

				e = scene->AddEntity(entity["Entity"].as<std::string>());
			}
			
			auto transform = entity["Transform"];
			e->GetTransform()->ID = transform["ID"].as<uint64_t>();
			
			if (auto parent = transform["Parent"])
			{
				parentsIDs.push_back(parent.as<uint64_t>());
			}
			else
			{
				parentsIDs.push_back(-1);
			}

			e->GetTransform()->Position = transform["Position"].as<glm::vec3>();
			e->GetTransform()->Rotation = transform["Rotation"].as<glm::vec3>();
			e->GetTransform()->Scale = transform["Scale"].as<glm::vec3>();

			if (auto model = entity["Model"])
			{
				std::string path = model["Mesh"].as<std::string>();
				std::string materialPath = model["Material"].as<std::string>();
				e->AddComponent<Model>(path.c_str(), materialPath);
			}

			if (auto light = entity["Light"])
			{
				e->AddComponent<Light>(e, scene->GetCamera(), MaterialManager::GetInstance()->GetShaderLibrary());

				LightType lt = (LightType)light["LightType"].as<int>();
				glm::vec3 ambient = light["Ambient"].as<glm::vec3>();
				glm::vec3 diffuse = light["Diffuse"].as<glm::vec3>();
				glm::vec3 specular = light["Specular"].as<glm::vec3>();

				auto l = e->GetComponent<Light>();
				l->SetAmbient(ambient);
				l->SetDiffuse(diffuse);
				l->SetSpecular(specular);
			}
		}

		for (int i = 0; i < parentsIDs.size(); i++)
		{
			if (parentsIDs[i] == -1)
				continue;

			scene->GetEntities()[i]->GetTransform()->SetParent(scene->FindTransform(parentsIDs[i]));
		}
	}

	file.close();
	return scene;
}

void SceneSerializer::SerializeEntity(YAML::Emitter& out, Ref<Entity> entity)
{
	out << YAML::BeginMap;
	out << YAML::Key << "Entity" << YAML::Value << entity->GetName();

	Ref<Transform> transform = entity->GetTransform();
	out << YAML::Key << "Transform";
	out << YAML::BeginMap;
	out << YAML::Key << "ID" << YAML::Value << transform->ID;
	if (transform->Parent)
	{
		out << YAML::Key << "Parent" << YAML::Value << transform->Parent->ID;
	}
	out << YAML::Key << "Position" << YAML::Value << transform->Position;
	out << YAML::Key << "Rotation" << YAML::Value << transform->Rotation;
	out << YAML::Key << "Scale" << YAML::Value << transform->Scale;
	out << YAML::EndMap;

	if (auto model = entity->GetComponent<Model>())
	{
		out << YAML::Key << "Model";
		out << YAML::BeginMap;
		out << YAML::Key << "Mesh" << YAML::Value << model->GetPath();
		out << YAML::Key << "Material" << YAML::Value << model->GetMaterialPath();
		out << YAML::EndMap;
	}
	if (auto light = entity->GetComponent<Light>())
	{
		out << YAML::Key << "Light";
		out << YAML::BeginMap;
		out << YAML::Key << "LightType" << YAML::Value << (int)light->GetLightType();
		out << YAML::Key << "Ambient" << YAML::Value << light->GetAmbient();
		out << YAML::Key << "Diffuse" << YAML::Value << light->GetDiffuse();
		out << YAML::Key << "Specular" << YAML::Value << light->GetSpecular();
		out << YAML::EndMap;
	}

	out << YAML::EndMap;
}
