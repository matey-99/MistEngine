#include "SceneSerializer.h"

namespace YAML
{
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};
}


YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
	return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
	return out;
}

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
			Ref<Entity> e = scene->AddEntity(entity["Entity"].as<std::string>());
			
			auto transform = entity["Transform"];
			e->GetTransform()->ID = transform["ID"].as<uint64_t>();
			if (auto parent = transform["Parent"])
			{
				parentsIDs.push_back(parent.as<uint64_t>());
			}
			else
			{
				parentsIDs.push_back(0);
			}

			e->GetTransform()->Position = transform["Position"].as<glm::vec3>();
			e->GetTransform()->Rotation = transform["Rotation"].as<glm::vec3>();
			e->GetTransform()->Scale = transform["Scale"].as<glm::vec3>();
		}

		for (int i = 0; i < parentsIDs.size(); i++)
		{
			if (parentsIDs[i] == 0)
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
	out << YAML::EndMap;
}
