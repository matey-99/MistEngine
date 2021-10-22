#include "SpotLight.h"

#include <glm/gtc/type_ptr.hpp>

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
	//auto shaderLibrary = MaterialManager::GetInstance()->GetShaderLibrary();
	//for (auto shader : shaderLibrary->GetAllMaterialShaders())
	//{
	//	shader->Use();

	//	shader->SetVec3("u_SpotLights[" + std::to_string(m_Index) + "].ambient", glm::vec3(0.0f));
	//	shader->SetVec3("u_SpotLights[" + std::to_string(m_Index) + "].diffuse", glm::vec3(0.0f));
	//	shader->SetVec3("u_SpotLights[" + std::to_string(m_Index) + "].specular", glm::vec3(0.0f));
	//}
}

void SpotLight::Begin()
{

}

void SpotLight::Update()
{
}

void SpotLight::Use(Ref<UniformBuffer> uniformBuffer)
{
	uint32_t offset = GLSL_SPOT_LIGHTS_OFFSET;
	uniformBuffer->SetUniform(offset, sizeof(glm::vec3), glm::value_ptr(m_Entity->GetTransform()->GetWorldPosition()));
	uniformBuffer->SetUniform(offset + GLSL_VEC3_SIZE, sizeof(glm::vec3), glm::value_ptr(m_Direction));
	uniformBuffer->SetUniform(offset + (GLSL_VEC3_SIZE * 2), sizeof(glm::vec3), glm::value_ptr(m_Ambient));
	uniformBuffer->SetUniform(offset + (GLSL_VEC3_SIZE * 3), sizeof(glm::vec3), glm::value_ptr(m_Diffuse));
	uniformBuffer->SetUniform(offset + (GLSL_VEC3_SIZE * 4), sizeof(glm::vec3), glm::value_ptr(m_Specular));
	uniformBuffer->SetUniform(offset + (GLSL_VEC3_SIZE * 5) - GLSL_SCALAR_SIZE, sizeof(float), &m_Linear);
	uniformBuffer->SetUniform(offset + (GLSL_VEC3_SIZE * 5), sizeof(float), &m_Quadratic);
	uniformBuffer->SetUniform(offset + (GLSL_VEC3_SIZE * 5) + GLSL_SCALAR_SIZE, sizeof(float), &m_InnerCutOff);
	uniformBuffer->SetUniform(offset + (GLSL_VEC3_SIZE * 5) + (GLSL_SCALAR_SIZE * 2), sizeof(float), &m_OuterCutOff);
}

void SpotLight::SwitchOff()
{
	//auto shaderLibrary = MaterialManager::GetInstance()->GetShaderLibrary();
	//for (auto shader : shaderLibrary->GetAllMaterialShaders())
	//{
	//	shader->Use();

	//	shader->SetVec3("u_ViewPosition", glm::vec3(0.0f));

	//	shader->SetVec3("u_SpotLights[" + std::to_string(m_Index) + "].position", m_Entity->GetTransform()->GetWorldPosition());
	//	shader->SetVec3("u_SpotLights[" + std::to_string(m_Index) + "].direction", glm::vec3(0.0f));

	//	shader->SetVec3("u_SpotLights[" + std::to_string(m_Index) + "].ambient", glm::vec3(0.0f));
	//	shader->SetVec3("u_SpotLights[" + std::to_string(m_Index) + "].diffuse", glm::vec3(0.0f));
	//	shader->SetVec3("u_SpotLights[" + std::to_string(m_Index) + "].specular", glm::vec3(0.0f));

	//	shader->SetFloat("u_SpotLights[" + std::to_string(m_Index) + "].linear", 0.0f);
	//	shader->SetFloat("u_SpotLights[" + std::to_string(m_Index) + "].quadratic", 0.0f);

	//	shader->SetFloat("u_SpotLights[" + std::to_string(m_Index) + "].innerCutOff", 0.0f);
	//	shader->SetFloat("u_SpotLights[" + std::to_string(m_Index) + "].outerCutOff", 0.0f);
	//}
}
