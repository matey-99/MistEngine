#include "SpotLight.h"

#include "Scene.h"
#include "MaterialManager.h"

SpotLight::SpotLight(Ref<Entity> entity) : Light(entity)
{
	m_Index = entity->GetScene()->GetComponentsCount<SpotLight>();

	m_Direction = glm::vec3(0.0f, 0.0f, -1.0f);

	m_Linear = 0.09f;
	m_Quadratic = 0.032f;

	m_InnerCutOff = 0.91f;
	m_OuterCutOff = 0.82f;
}

SpotLight::~SpotLight()
{
	auto shaderLibrary = MaterialManager::GetInstance()->GetShaderLibrary();
	for (auto shader : shaderLibrary->GetAllMaterialShaders())
	{
		shader->Use();

		shader->SetVec3("u_SpotLights[" + std::to_string(m_Index) + "].ambient", glm::vec3(0.0f));
		shader->SetVec3("u_SpotLights[" + std::to_string(m_Index) + "].diffuse", glm::vec3(0.0f));
		shader->SetVec3("u_SpotLights[" + std::to_string(m_Index) + "].specular", glm::vec3(0.0f));
	}
}

void SpotLight::Begin()
{

}

void SpotLight::Update()
{
}

void SpotLight::Use(glm::vec3 cameraPosition)
{
	auto shaderLibrary = MaterialManager::GetInstance()->GetShaderLibrary();
	for (auto shader : shaderLibrary->GetAllMaterialShaders())
	{
		shader->Use();

		shader->SetVec3("u_ViewPosition", cameraPosition);

		shader->SetVec3("u_SpotLights[" + std::to_string(m_Index) + "].position", m_Entity->GetTransform()->GetWorldPosition());
		shader->SetVec3("u_SpotLights[" + std::to_string(m_Index) + "].direction", m_Direction);

		shader->SetVec3("u_SpotLights[" + std::to_string(m_Index) + "].ambient", m_Ambient);
		shader->SetVec3("u_SpotLights[" + std::to_string(m_Index) + "].diffuse", m_Diffuse);
		shader->SetVec3("u_SpotLights[" + std::to_string(m_Index) + "].specular", m_Specular);

		shader->SetFloat("u_SpotLights[" + std::to_string(m_Index) + "].linear", m_Linear);
		shader->SetFloat("u_SpotLights[" + std::to_string(m_Index) + "].quadratic", m_Quadratic);

		shader->SetFloat("u_SpotLights[" + std::to_string(m_Index) + "].innerCutOff", m_InnerCutOff);
		shader->SetFloat("u_SpotLights[" + std::to_string(m_Index) + "].outerCutOff", m_OuterCutOff);
	}

}

void SpotLight::SwitchOff()
{
	auto shaderLibrary = MaterialManager::GetInstance()->GetShaderLibrary();
	for (auto shader : shaderLibrary->GetAllMaterialShaders())
	{
		shader->Use();

		shader->SetVec3("u_ViewPosition", glm::vec3(0.0f));

		shader->SetVec3("u_SpotLights[" + std::to_string(m_Index) + "].position", m_Entity->GetTransform()->GetWorldPosition());
		shader->SetVec3("u_SpotLights[" + std::to_string(m_Index) + "].direction", glm::vec3(0.0f));

		shader->SetVec3("u_SpotLights[" + std::to_string(m_Index) + "].ambient", glm::vec3(0.0f));
		shader->SetVec3("u_SpotLights[" + std::to_string(m_Index) + "].diffuse", glm::vec3(0.0f));
		shader->SetVec3("u_SpotLights[" + std::to_string(m_Index) + "].specular", glm::vec3(0.0f));

		shader->SetFloat("u_SpotLights[" + std::to_string(m_Index) + "].linear", 0.0f);
		shader->SetFloat("u_SpotLights[" + std::to_string(m_Index) + "].quadratic", 0.0f);

		shader->SetFloat("u_SpotLights[" + std::to_string(m_Index) + "].innerCutOff", 0.0f);
		shader->SetFloat("u_SpotLights[" + std::to_string(m_Index) + "].outerCutOff", 0.0f);
	}
}
