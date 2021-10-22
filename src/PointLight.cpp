#include "PointLight.h"

#include <glm/gtc/type_ptr.hpp>

#include "Scene.h"
#include "UniformBuffer.h"
#include "MaterialManager.h"

PointLight::PointLight(Ref<Entity> entity) : Light(entity)
{
	m_Index = entity->GetScene()->GetComponentsCount<PointLight>();

	m_Linear = 0.09f;
	m_Quadratic = 0.032f;
}

PointLight::~PointLight()
{
	//auto shaderLibrary = MaterialManager::GetInstance()->GetShaderLibrary();
	//for (auto shader : shaderLibrary->GetAllMaterialShaders())
	//{
	//	shader->Use();

	//	shader->SetVec3("u_PointLights[" + std::to_string(m_Index) + "].ambient", glm::vec3(0.0f));
	//	shader->SetVec3("u_PointLights[" + std::to_string(m_Index) + "].diffuse", glm::vec3(0.0f));
	//	shader->SetVec3("u_PointLights[" + std::to_string(m_Index) + "].specular", glm::vec3(0.0f));
	//}
}

void PointLight::Begin()
{

}

void PointLight::Update()
{
}

void PointLight::Use(Ref<UniformBuffer> uniformBuffer)
{
	uint32_t offset = GLSL_POINT_LIGHTS_OFFSET;
	uniformBuffer->SetUniform(offset, sizeof(glm::vec3), glm::value_ptr(m_Entity->GetTransform()->GetWorldPosition()));
	uniformBuffer->SetUniform(offset + GLSL_VEC3_SIZE, sizeof(glm::vec3), glm::value_ptr(m_Ambient));
	uniformBuffer->SetUniform(offset + (GLSL_VEC3_SIZE * 2), sizeof(glm::vec3), glm::value_ptr(m_Diffuse));
	uniformBuffer->SetUniform(offset + (GLSL_VEC3_SIZE * 3), sizeof(glm::vec3), glm::value_ptr(m_Specular));
	uniformBuffer->SetUniform(offset + (GLSL_VEC3_SIZE * 4) - GLSL_SCALAR_SIZE, sizeof(float), &m_Linear);
	uniformBuffer->SetUniform(offset + (GLSL_VEC3_SIZE * 4), sizeof(float), &m_Quadratic);
}

void PointLight::SwitchOff()
{
	//auto shaderLibrary = MaterialManager::GetInstance()->GetShaderLibrary();
	//for (auto shader : shaderLibrary->GetAllMaterialShaders())
	//{
	//	shader->Use();

	//	shader->SetVec3("u_ViewPosition", glm::vec3(0.0f));
	//	shader->SetVec3("u_PointLights[" + std::to_string(m_Index) + "].position", m_Entity->GetTransform()->GetWorldPosition());

	//	shader->SetVec3("u_PointLights[" + std::to_string(m_Index) + "].ambient", glm::vec3(0.0f));
	//	shader->SetVec3("u_PointLights[" + std::to_string(m_Index) + "].diffuse", glm::vec3(0.0f));
	//	shader->SetVec3("u_PointLights[" + std::to_string(m_Index) + "].specular", glm::vec3(0.0f));

	//	shader->SetFloat("u_PointLights[" + std::to_string(m_Index) + "].linear", 0.0f);
	//	shader->SetFloat("u_PointLights[" + std::to_string(m_Index) + "].quadratic", 0.0f);
	//}
}
