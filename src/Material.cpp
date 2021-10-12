#include "Material.h"


Material::Material(std::string name, Ref<Shader> shader, MaterialType type)
	: m_Name(name), m_Shader(shader), m_Type(type)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution distribution(1, 999999999);

	m_ID = distribution(gen);

	m_Ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	m_Diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	m_Specular = glm::vec3(1.0f, 1.0f, 1.0f);
	m_Shininess = 32.0f;
}

Material::Material(uint64_t id, std::string name, Ref<Shader> shader, MaterialType type)
	: m_ID(id), m_Name(name), m_Shader(shader), m_Type(type)
{

}

void Material::Use()
{
	m_Shader->Use();

	if (m_Type == MaterialType::Default)
	{
		m_Shader->SetVec3("u_Material.ambient", m_Ambient);
		m_Shader->SetVec3("u_Material.diffuse", m_Diffuse);
		m_Shader->SetVec3("u_Material.specular", m_Specular);
	}

	m_Shader->SetFloat("u_Material.shininess", m_Shininess);
}

