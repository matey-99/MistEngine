#pragma once

#include <unordered_map>
#include <string>

#include "typedefs.h"
#include "Renderer/Shader.h"

enum class ShaderType
{
	MATERIAL, POST_PROCESSING, SKYBOX, CALCULATION
};

class ShaderLibrary
{
private:
	std::unordered_map<std::string, Ref<Shader>> m_MaterialShaders;
	std::unordered_map<std::string, Ref<Shader>> m_PostProcessingShaders;
	std::unordered_map<std::string, Ref<Shader>> m_SkyboxShaders;
	std::unordered_map<std::string, Ref<Shader>> m_CalculationShaders;

public:
	ShaderLibrary();

	Ref<Shader> GetShader(ShaderType type, std::string name);

	std::vector<Ref<Shader>> GetAllMaterialShaders();
	inline std::unordered_map<std::string, Ref<Shader>> GetMaterialShaders() const { return m_MaterialShaders; }
};