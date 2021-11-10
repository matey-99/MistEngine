#pragma once

#include <unordered_map>
#include <string>

#include "typedefs.h"
#include "Renderer/Shader.h"

class ShaderLibrary
{
private:
	std::unordered_map<std::string, Ref<Shader>> m_MaterialShaders;
	std::unordered_map<std::string, Ref<Shader>> m_SkyboxShaders;
	std::unordered_map<std::string, Ref<Shader>> m_OtherShaders;

public:
	ShaderLibrary();

	Ref<Shader> GetMaterialShader(std::string name);
	Ref<Shader> GetSkyboxShader(std::string name);
	Ref<Shader> GetOtherShader(std::string name);


	std::vector<Ref<Shader>> GetAllMaterialShaders();
	std::vector<Ref<Shader>> GetAllSkyboxShaders();
};