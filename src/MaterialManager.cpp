#include "MaterialManager.h"

#include "Serialization/MaterialSerializer.h"

Ref<MaterialManager> MaterialManager::s_Instance{};
std::mutex MaterialManager::s_Mutex;

MaterialManager::MaterialManager()
{
	m_Materials = std::vector<Ref<Material>>();
	m_ShaderLibrary = CreateRef<ShaderLibrary>();
}

Ref<MaterialManager> MaterialManager::GetInstance()
{
	std::lock_guard<std::mutex> lock(s_Mutex);
	if (s_Instance == nullptr)
		s_Instance = CreateRef<MaterialManager>();

	return s_Instance;
}

Ref<Material> MaterialManager::LoadMaterial(std::string path)
{
	Ref<Material> material = MaterialSerializer::Deserialize(path, m_ShaderLibrary);

	for (auto mat : m_Materials)
	{
		if (material->GetID() == mat->GetID())
			return mat;
	}

	m_Materials.push_back(material);
	return material;
}

Ref<Material> MaterialManager::CreateMaterial(std::string name, std::string shaderName)
{
	Ref<Material> material = CreateRef<Material>(name, m_ShaderLibrary->GetMaterialShader(shaderName));
	m_Materials.push_back(material);
	MaterialSerializer::Serialize(material);

	return material;
}
