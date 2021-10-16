#include "SceneSerializer.h"

#include "yaml.h"
#include "Model.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
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

			e->GetTransform()->LocalPosition = transform["Position"].as<glm::vec3>();
			e->GetTransform()->LocalRotation = transform["Rotation"].as<glm::vec3>();
			e->GetTransform()->LocalScale = transform["Scale"].as<glm::vec3>();

			if (auto model = entity["Model"])
			{
				std::string path = model["Mesh"].as<std::string>();
				std::string materialPath = model["Material"].as<std::string>();
				e->AddComponent<Model>(path.c_str(), materialPath);
			}

			if (auto dirLight = entity["Directional Light"])
			{
				e->AddComponent<DirectionalLight>(e);

				glm::vec3 direction = dirLight["Direction"].as<glm::vec3>();
				glm::vec3 ambient = dirLight["Ambient"].as<glm::vec3>();
				glm::vec3 diffuse = dirLight["Diffuse"].as<glm::vec3>();
				glm::vec3 specular = dirLight["Specular"].as<glm::vec3>();

				auto l = e->GetComponent<DirectionalLight>();
				l->SetDirection(direction);
				l->SetAmbient(ambient);
				l->SetDiffuse(diffuse);
				l->SetSpecular(specular);
			}

			if (auto pointLight = entity["Point Light"])
			{
				e->AddComponent<PointLight>(e);

				float linear = pointLight["Linear"].as<float>();
				float quadratic = pointLight["Quadratic"].as<float>();
				glm::vec3 ambient = pointLight["Ambient"].as<glm::vec3>();
				glm::vec3 diffuse = pointLight["Diffuse"].as<glm::vec3>();
				glm::vec3 specular = pointLight["Specular"].as<glm::vec3>();

				auto l = e->GetComponent<PointLight>();
				l->SetLinear(linear);
				l->SetQuadratic(quadratic);
				l->SetAmbient(ambient);
				l->SetDiffuse(diffuse);
				l->SetSpecular(specular);
			}

			if (auto spotLight = entity["Spot Light"])
			{
				e->AddComponent<SpotLight>(e);

				glm::vec3 direction = spotLight["Direction"].as<glm::vec3>();
				float innerCutOff = spotLight["Inner Cut Off"].as<float>();
				float outerCutOff = spotLight["Outer Cut Off"].as<float>();
				float linear = spotLight["Linear"].as<float>();
				float quadratic = spotLight["Quadratic"].as<float>();
				glm::vec3 ambient = spotLight["Ambient"].as<glm::vec3>();
				glm::vec3 diffuse = spotLight["Diffuse"].as<glm::vec3>();
				glm::vec3 specular = spotLight["Specular"].as<glm::vec3>();

				auto l = e->GetComponent<SpotLight>();
				l->SetDirection(direction);
				l->SetInnerCutOff(innerCutOff);
				l->SetOuterCutOff(outerCutOff);
				l->SetLinear(linear);
				l->SetQuadratic(quadratic);
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
	out << YAML::Key << "Position" << YAML::Value << transform->LocalPosition;
	out << YAML::Key << "Rotation" << YAML::Value << transform->LocalRotation;
	out << YAML::Key << "Scale" << YAML::Value << transform->LocalScale;
	out << YAML::EndMap;

	if (auto model = entity->GetComponent<Model>())
	{
		out << YAML::Key << "Model";
		out << YAML::BeginMap;
		out << YAML::Key << "Mesh" << YAML::Value << model->GetPath();
		out << YAML::Key << "Material" << YAML::Value << model->GetMaterialPath();
		out << YAML::EndMap;
	}
	if (auto dirLight = entity->GetComponent<DirectionalLight>())
	{
		out << YAML::Key << "Directional Light";
		out << YAML::BeginMap;
		out << YAML::Key << "Direction" << YAML::Value << dirLight->GetDirection();
		out << YAML::Key << "Ambient" << YAML::Value << dirLight->GetAmbient();
		out << YAML::Key << "Diffuse" << YAML::Value << dirLight->GetDiffuse();
		out << YAML::Key << "Specular" << YAML::Value << dirLight->GetSpecular();
		out << YAML::EndMap;
	}
	if (auto pointLight = entity->GetComponent<PointLight>())
	{
		out << YAML::Key << "Point Light";
		out << YAML::BeginMap;
		out << YAML::Key << "Linear" << YAML::Value << pointLight->GetLinear();
		out << YAML::Key << "Quadratic" << YAML::Value << pointLight->GetQuadratic();
		out << YAML::Key << "Ambient" << YAML::Value << pointLight->GetAmbient();
		out << YAML::Key << "Diffuse" << YAML::Value << pointLight->GetDiffuse();
		out << YAML::Key << "Specular" << YAML::Value << pointLight->GetSpecular();
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
		out << YAML::Key << "Ambient" << YAML::Value << spotLight->GetAmbient();
		out << YAML::Key << "Diffuse" << YAML::Value << spotLight->GetDiffuse();
		out << YAML::Key << "Specular" << YAML::Value << spotLight->GetSpecular();
		out << YAML::EndMap;
	}

	out << YAML::EndMap;
}
