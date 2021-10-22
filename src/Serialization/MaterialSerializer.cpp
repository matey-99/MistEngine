#include "MaterialSerializer.h"

#include "yaml.h"


void MaterialSerializer::Serialize(Ref<Material> material)
{
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "Material" << YAML::Value << material->GetName();
	out << YAML::Key << "ID" << YAML::Value << material->GetID();
	out << YAML::Key << "Shader" << YAML::Value << material->GetShader()->GetName();
	out << YAML::Key << "Color" << YAML::Value << material->GetColor();
	out << YAML::Key << "Shininess" << YAML::Value << material->GetShininess();
	out << YAML::EndMap;

	std::ofstream file("../../res/materials/" + material->GetName() + ".mat");
	file << out.c_str();
	file.close();
}

Ref<Material> MaterialSerializer::Deserialize(std::string path, Ref<ShaderLibrary> shaderLibrary)
{
	std::ifstream file(path);
	std::stringstream ss;
	ss << file.rdbuf();

	YAML::Node data = YAML::Load(ss.str());
	if (!data["Material"])
	{
		std::cout << "Cannot load material!" << std::endl;
		return Ref<Material>();
	}

	std::string name = data["Material"].as<std::string>();
	uint64_t id = data["ID"].as<uint64_t>();
	std::string shader = data["Shader"].as<std::string>();
	glm::vec3 color = data["Color"].as<glm::vec3>();
	float shininess = data["Shininess"].as<float>();

	Ref<Material> material = CreateRef<Material>(id, name, shaderLibrary->GetMaterialShader(shader));
	material->SetColor(color);
	material->SetShininess(shininess);

	return material;
}
