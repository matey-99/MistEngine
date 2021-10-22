#include "PointLight.h"

#include <glm/gtc/type_ptr.hpp>

#include "Scene/Scene.h"
#include "Renderer/UniformBuffer.h"
#include "Material/MaterialManager.h"

PointLight::PointLight(Ref<Entity> entity, Ref<UniformBuffer> uniformBuffer) 
	: Light(entity, uniformBuffer)
{
	m_Index = entity->GetScene()->GetComponentsCount<PointLight>();

	m_Linear = 0.09f;
	m_Quadratic = 0.032f;
}

PointLight::~PointLight()
{
	SwitchOff();
}

void PointLight::Begin()
{

}

void PointLight::Update()
{
}

void PointLight::Use()
{
	uint32_t offset = GLSL_POINT_LIGHTS_OFFSET + (GLSL_POINT_LIGHT_SIZE * m_Index);
	m_UniformBuffer->SetUniform(offset, sizeof(glm::vec3), glm::value_ptr(m_Entity->GetTransform()->GetWorldPosition()));
	m_UniformBuffer->SetUniform(offset + GLSL_VEC3_SIZE, sizeof(glm::vec3), glm::value_ptr(m_Ambient));
	m_UniformBuffer->SetUniform(offset + (GLSL_VEC3_SIZE * 2), sizeof(glm::vec3), glm::value_ptr(m_Diffuse));
	m_UniformBuffer->SetUniform(offset + (GLSL_VEC3_SIZE * 3), sizeof(glm::vec3), glm::value_ptr(m_Specular));
	m_UniformBuffer->SetUniform(offset + (GLSL_VEC3_SIZE * 4) - GLSL_SCALAR_SIZE, sizeof(float), &m_Linear);
	m_UniformBuffer->SetUniform(offset + (GLSL_VEC3_SIZE * 4), sizeof(float), &m_Quadratic);
}

void PointLight::SwitchOff()
{
	uint32_t offset = GLSL_POINT_LIGHTS_OFFSET + (GLSL_POINT_LIGHT_SIZE * m_Index);
	m_UniformBuffer->SetUniform(offset, sizeof(glm::vec3), glm::value_ptr(glm::vec3(0.0f)));
	m_UniformBuffer->SetUniform(offset + GLSL_VEC3_SIZE, sizeof(glm::vec3), glm::value_ptr(glm::vec3(0.0f)));
	m_UniformBuffer->SetUniform(offset + (GLSL_VEC3_SIZE * 2), sizeof(glm::vec3), glm::value_ptr(glm::vec3(0.0f)));
	m_UniformBuffer->SetUniform(offset + (GLSL_VEC3_SIZE * 3), sizeof(glm::vec3), glm::value_ptr(glm::vec3(0.0f)));
	m_UniformBuffer->SetUniform(offset + (GLSL_VEC3_SIZE * 4) - GLSL_SCALAR_SIZE, sizeof(float), (void*)0);
	m_UniformBuffer->SetUniform(offset + (GLSL_VEC3_SIZE * 4), sizeof(float), (void*)0);
}
