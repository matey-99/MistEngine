#include "Material.h"


Material::Material(std::string name, Ref<Shader> shader)
	: m_Name(name), m_Shader(shader)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution distribution(1, 999999999);

	m_ID = distribution(gen);

	m_Color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_Shininess = 32.0f;
}

Material::Material(uint64_t id, std::string name, Ref<Shader> shader)
	: m_ID(id), m_Name(name), m_Shader(shader)
{

}

void Material::Use()
{
	m_Shader->Use();

	m_Shader->SetVec3("u_Material.color", m_Color);
	m_Shader->SetFloat("u_Material.shininess", m_Shininess);
}

