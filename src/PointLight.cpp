#include "PointLight.h"

#include "MaterialManager.h"

PointLight::PointLight(Ref<Entity> entity) : Light(entity)
{
	m_Index = 0;

	m_Linear = 0.09f;
	m_Quadratic = 0.032f;
}

PointLight::~PointLight()
{
	auto shaderLibrary = MaterialManager::GetInstance()->GetShaderLibrary();
	for (auto shader : shaderLibrary->GetAllShaders())
	{
		shader->Use();

		shader->SetVec3("u_PointLights[" + std::to_string(m_Index) + "].ambient", glm::vec3(0.0f));
		shader->SetVec3("u_PointLights[" + std::to_string(m_Index) + "].diffuse", glm::vec3(0.0f));
		shader->SetVec3("u_PointLights[" + std::to_string(m_Index) + "].specular", glm::vec3(0.0f));
	}
}

void PointLight::Begin()
{

}

void PointLight::Update()
{
}

void PointLight::Use(glm::vec3 cameraPosition)
{
	auto shaderLibrary = MaterialManager::GetInstance()->GetShaderLibrary();
	for (auto shader : shaderLibrary->GetAllShaders())
	{
		shader->Use();

		shader->SetVec3("u_ViewPosition", cameraPosition);
		shader->SetVec3("u_PointLights[" + std::to_string(m_Index) + "].position", m_Entity->GetTransform()->GetWorldPosition());

		shader->SetVec3("u_PointLights[" + std::to_string(m_Index) + "].ambient", m_Ambient);
		shader->SetVec3("u_PointLights[" + std::to_string(m_Index) + "].diffuse", m_Diffuse);
		shader->SetVec3("u_PointLights[" + std::to_string(m_Index) + "].specular", m_Specular);

		shader->SetFloat("u_PointLights[" + std::to_string(m_Index) + "].linear", m_Linear);
		shader->SetFloat("u_PointLights[" + std::to_string(m_Index) + "].quadratic", m_Quadratic);
	}

}
