#include "Material.h"


Material::Material(std::string name, Ref<Shader> shader) : m_Name(name), m_Shader(shader)
{
	m_Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

void Material::Use()
{
	m_Shader->Use();

	m_Shader->SetVec4("u_Color", m_Color);
}

