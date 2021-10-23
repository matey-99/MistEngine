#include "ShaderLibrary.h"

ShaderLibrary::ShaderLibrary()
{
	m_MaterialShaders.insert(std::make_pair<std::string, Ref<Shader>>("Standard", CreateRef<Shader>("Standard", "res/shaders/Standard.vert", "res/shaders/Standard.frag")));
	m_MaterialShaders.insert(std::make_pair<std::string, Ref<Shader>>("StandardPBR", CreateRef<Shader>("StandardPBR", "res/shaders/StandardPBR.vert", "res/shaders/StandardPBR.frag")));
	m_MaterialShaders.insert(std::make_pair<std::string, Ref<Shader>>("Texture", CreateRef<Shader>("Texture", "res/shaders/Texture.vert", "res/shaders/Texture.frag")));
	m_MaterialShaders.insert(std::make_pair<std::string, Ref<Shader>>("Viewport", CreateRef<Shader>("Viewport", "res/shaders/Viewport.vert", "res/shaders/Viewport.frag")));
	m_MaterialShaders.insert(std::make_pair<std::string, Ref<Shader>>("Shiny", CreateRef<Shader>("Shiny", "res/shaders/Shiny.vert", "res/shaders/Shiny.frag")));
	m_MaterialShaders.insert(std::make_pair<std::string, Ref<Shader>>("Refraction", CreateRef<Shader>("Refraction", "res/shaders/Refraction.vert", "res/shaders/Refraction.frag")));

	m_SkyboxShaders.insert(std::make_pair<std::string, Ref<Shader>>("Skybox", CreateRef<Shader>("Skybox", "res/shaders/Skybox.vert", "res/shaders/Skybox.frag")));
}

Ref<Shader> ShaderLibrary::GetMaterialShader(std::string name)
{
	return m_MaterialShaders.find(name)->second;
}

Ref<Shader> ShaderLibrary::GetSkyboxShader(std::string name)
{
	return m_SkyboxShaders.find(name)->second;
}

std::vector<Ref<Shader>> ShaderLibrary::GetAllMaterialShaders()
{
	std::vector<Ref<Shader>> result = std::vector<Ref<Shader>>();
	for (auto shader : m_MaterialShaders)
	{
		result.push_back(shader.second);
	}

	return result;
}

std::vector<Ref<Shader>> ShaderLibrary::GetAllSkyboxShaders()
{
	std::vector<Ref<Shader>> result = std::vector<Ref<Shader>>();
	for (auto shader : m_SkyboxShaders)
	{
		result.push_back(shader.second);
	}

	return result;
}
