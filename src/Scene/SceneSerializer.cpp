#include "SceneSerializer.h"

#include "yaml/yaml.h"
#include "Scene/Component/StaticMeshComponent.h"
#include "Scene/Component/Light/DirectionalLight.h"
#include "Scene/Component/Light/PointLight.h"
#include "Scene/Component/Light/SpotLight.h"
#include "Material/MaterialManager.h"

void SceneSerializer::Serialize(Ref<Scene> scene)
{
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "Scene" << YAML::Value << "Untitled";
	out << YAML::Key << "Camera";
	out << YAML::BeginMap;
	out << YAML::Key << "Position" << YAML::Value << scene->m_Camera->Position;
	out << YAML::Key << "Yaw" << YAML::Value << scene->m_Camera->Yaw;
	out << YAML::Key << "Pitch" << YAML::Value << scene->m_Camera->Pitch;
	out << YAML::EndMap;
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
	
	if (YAML::Node camera = data["Camera"])
	{
		glm::vec3 cameraPosition = camera["Position"].as<glm::vec3>();
		float cameraYaw = camera["Yaw"].as<float>();
		float cameraPitch = camera["Pitch"].as<float>();

		scene->m_Camera->Position = cameraPosition;
		scene->m_Camera->Yaw = cameraYaw;
		scene->m_Camera->Pitch = cameraPitch;
	}



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

			e->GetTransform()->LocalPosition = transform["Position"].as<glm::vec3>();
			e->GetTransform()->LocalRotation = transform["Rotation"].as<glm::vec3>();
			e->GetTransform()->LocalScale = transform["Scale"].as<glm::vec3>();

			if (auto mesh = entity["Model"])
			{
				std::string path = mesh["Mesh"].as<std::string>();
				std::vector<std::string> materialsPaths;
				YAML::Node materials = mesh["Materials"];
				for (auto material : materials)
				{
					materialsPaths.push_back(material["Path"].as<std::string>());

				}
				e->AddComponent<StaticMeshComponent>(path.c_str(), materialsPaths);
			}

			if (auto dirLight = entity["Directional Light"])
			{
				e->AddComponent<DirectionalLight>(e, scene->m_LightsUniformBuffer);

				glm::vec3 direction = dirLight["Direction"].as<glm::vec3>();
				glm::vec3 color = dirLight["Color"].as<glm::vec3>();

				auto l = e->GetComponent<DirectionalLight>();
				l->SetDirection(direction);
				l->SetColor(color);
			}

			if (auto pointLight = entity["Point Light"])
			{
				e->AddComponent<PointLight>(e, scene->m_LightsUniformBuffer);

				float linear = pointLight["Linear"].as<float>();
				float quadratic = pointLight["Quadratic"].as<float>();
				glm::vec3 color = pointLight["Color"].as<glm::vec3>();

				auto l = e->GetComponent<PointLight>();
				l->SetLinear(linear);
				l->SetQuadratic(quadratic);
				l->SetColor(color);
			}

			if (auto spotLight = entity["Spot Light"])
			{
				e->AddComponent<SpotLight>(e, scene->m_LightsUniformBuffer);

				glm::vec3 direction = spotLight["Direction"].as<glm::vec3>();
				float innerCutOff = spotLight["Inner Cut Off"].as<float>();
				float outerCutOff = spotLight["Outer Cut Off"].as<float>();
				float linear = spotLight["Linear"].as<float>();
				float quadratic = spotLight["Quadratic"].as<float>();
				glm::vec3 color = spotLight["Color"].as<glm::vec3>();

				auto l = e->GetComponent<SpotLight>();
				l->SetDirection(direction);
				l->SetInnerCutOff(innerCutOff);
				l->SetOuterCutOff(outerCutOff);
				l->SetLinear(linear);
				l->SetQuadratic(quadratic);
				l->SetColor(color);
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
	out << YAML::Key << "Position" << YAML::Value << transform->LocalPosition;
	out << YAML::Key << "Rotation" << YAML::Value << transform->LocalRotation;
	out << YAML::Key << "Scale" << YAML::Value << transform->LocalScale;
	out << YAML::EndMap;

	if (auto mesh = entity->GetComponent<StaticMeshComponent>())
	{
		out << YAML::Key << "Model";
		out << YAML::BeginMap;
		out << YAML::Key << "Mesh" << YAML::Value << mesh->GetPath();
		out << YAML::Key << "Materials" << YAML::Value << YAML::BeginSeq;
		out << YAML::BeginMap;
		for (int i = 0; i < mesh->GetMaterialsPaths().size(); i++)
		{
			out << YAML::Key << "Material" << YAML::Value << i;
			out << YAML::Key << "Path" << YAML::Value << mesh->GetMaterialsPaths().at(i);
		}
		out << YAML::EndMap;
		out << YAML::EndSeq;
		out << YAML::EndMap;
	}
	if (auto dirLight = entity->GetComponent<DirectionalLight>())
	{
		out << YAML::Key << "Directional Light";
		out << YAML::BeginMap;
		out << YAML::Key << "Direction" << YAML::Value << dirLight->GetDirection();
		out << YAML::Key << "Color" << YAML::Value << dirLight->GetColor();
		out << YAML::EndMap;
	}
	if (auto pointLight = entity->GetComponent<PointLight>())
	{
		out << YAML::Key << "Point Light";
		out << YAML::BeginMap;
		out << YAML::Key << "Linear" << YAML::Value << pointLight->GetLinear();
		out << YAML::Key << "Quadratic" << YAML::Value << pointLight->GetQuadratic();
		out << YAML::Key << "Color" << YAML::Value << pointLight->GetColor();
		out << YAML::EndMap;
	}
	if (auto spotLight = entity->GetComponent<SpotLight>())
	{
		out << YAML::Key << "Spot Light";
		out << YAML::BeginMap;
		out << YAML::Key << "Direction" << YAML::Value << spotLight->GetDirection();
		out << YAML::Key << "Inner Cut Off" << YAML::Value << spotLight->GetInnerCutOff();
		out << YAML::Key << "Outer Cut Off" << YAML::Value << spotLight->GetOuterCutOff();
		out << YAML::Key << "Linear" << YAML::Value << spotLight->GetLinear();
		out << YAML::Key << "Quadratic" << YAML::Value << spotLight->GetQuadratic();
		out << YAML::Key << "Color" << YAML::Value << spotLight->GetColor();
		out << YAML::EndMap;
	}

	out << YAML::EndMap;
}
