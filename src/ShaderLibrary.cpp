#include "ShaderLibrary.h"

ShaderLibrary::ShaderLibrary()
{
	m_Shaders.insert(std::make_pair<std::string, Ref<Shader>>("Default", CreateRef<Shader>("res/shaders/Default.vert", "res/shaders/Default.frag")));
	m_Shaders.insert(std::make_pair<std::string, Ref<Shader>>("Texture", CreateRef<Shader>("res/shaders/Texture.vert", "res/shaders/Texture.frag")));
	m_Shaders.insert(std::make_pair<std::string, Ref<Shader>>("LightSource", CreateRef<Shader>("res/shaders/LightSource.vert", "res/shaders/LightSource.frag")));
}

Ref<Shader> ShaderLibrary::GetShader(std::string name)
{
	return m_Shaders.find(name)->second;
}

std::vector<Ref<Shader>> ShaderLibrary::GetAllShaders()
{
	std::vector<Ref<Shader>> result = std::vector<Ref<Shader>>();
	for (auto shader : m_Shaders)
	{
		result.push_back(shader.second);
	}

	return result;
}
