#include "ShaderLibrary.h"

ShaderLibrary::ShaderLibrary()
{
	m_Shaders.insert(std::make_pair<std::string, Ref<Shader>>("Default", CreateRef<Shader>("res/shaders/Default.vert", "res/shaders/Default.frag")));
	m_Shaders.insert(std::make_pair<std::string, Ref<Shader>>("Texture", CreateRef<Shader>("res/shaders/Texture.vert", "res/shaders/Texture.frag")));
}

Ref<Shader> ShaderLibrary::GetShader(std::string name)
{
	return m_Shaders.find(name)->second;
}
