#pragma once

#include <unordered_map>
#include <string>

#include "typedefs.h"
#include "Shader.h"

class ShaderLibrary
{
private:
	std::unordered_map<std::string, Ref<Shader>> m_Shaders;

public:
	ShaderLibrary();

	Ref<Shader> GetShader(std::string name);
	std::vector<Ref<Shader>> GetAllShaders();
};