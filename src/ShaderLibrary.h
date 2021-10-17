#pragma once

#include <unordered_map>
#include <string>

#include "typedefs.h"
#include "Shader.h"

class ShaderLibrary
{
private:
	std::unordered_map<std::string, Ref<Shader>> m_MaterialShaders;
	std::unordered_map<std::string, Ref<Shader>> m_SkyboxShaders;

public:
	ShaderLibrary();

	Ref<Shader> GetMaterialShader(std::string name);
	Ref<Shader> GetSkyboxShader(std::string name);


	std::vector<Ref<Shader>> GetAllMaterialShaders();
	std::vector<Ref<Shader>> GetAllSkyboxShaders();
};