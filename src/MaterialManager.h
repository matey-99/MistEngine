#pragma once

#include <unordered_map>
#include <string>
#include <mutex>

#include "typedefs.h"
#include "Material.h"
#include "ShaderLibrary.h"

class MaterialManager
{
private:
	static Ref<MaterialManager> s_Instance;
	static std::mutex s_Mutex;

	std::vector<Ref<Material>> m_Materials;
	Ref<ShaderLibrary> m_ShaderLibrary;

public:
	MaterialManager();
	~MaterialManager() {};

	MaterialManager(MaterialManager& other) = delete;
	void operator=(const MaterialManager&) = delete;

	static Ref<MaterialManager> GetInstance();

	Ref<Material> LoadMaterial(std::string path);
	Ref<Material> CreateMaterial(std::string name, std::string shaderName);

	inline Ref<ShaderLibrary> GetShaderLibrary() const { return m_ShaderLibrary; }
};