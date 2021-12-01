#include "SpotLight.h"

#include <glm/gtc/type_ptr.hpp>

#include "Scene/Scene.h"

SpotLight::SpotLight(Entity* owner, Ref<UniformBuffer> uniformBuffer)
	: Light(owner, uniformBuffer)
{
	m_Index = owner->GetScene()->GetComponentsCount<SpotLight>();

	m_Direction = glm::vec3(0.0f, 0.0f, -1.0f);

	m_Linear = 0.09f;
	m_Quadratic = 0.032f;

	m_InnerCutOff = 0.91f;
	m_OuterCutOff = 0.82f;
}

SpotLight::~SpotLight()
{
	SwitchOff();
}

void SpotLight::Use()
{
	uint32_t offset = GLSL_SPOT_LIGHTS_OFFSET + (GLSL_SPOT_LIGHT_SIZE * m_Index);
	m_UniformBuffer->SetUniform(offset, sizeof(glm::vec3), glm::value_ptr(m_Owner->GetWorldPosition()));
	m_UniformBuffer->SetUniform(offset + GLSL_VEC3_SIZE, sizeof(glm::vec3), glm::value_ptr(m_Direction));
	m_UniformBuffer->SetUniform(offset + (GLSL_VEC3_SIZE * 2), sizeof(glm::vec3), glm::value_ptr(m_Color));
	m_UniformBuffer->SetUniform(offset + (GLSL_VEC3_SIZE * 3) - GLSL_SCALAR_SIZE, sizeof(float), &m_InnerCutOff);
	m_UniformBuffer->SetUniform(offset + (GLSL_VEC3_SIZE * 3), sizeof(float), &m_OuterCutOff);
}

void SpotLight::SwitchOff()
{
	uint32_t offset = GLSL_SPOT_LIGHTS_OFFSET + (GLSL_SPOT_LIGHT_SIZE * m_Index);
	m_UniformBuffer->SetUniform(offset, sizeof(glm::vec3), glm::value_ptr(glm::vec3(0.0f)));
	m_UniformBuffer->SetUniform(offset + GLSL_VEC3_SIZE, sizeof(glm::vec3), glm::value_ptr(glm::vec3(0.0f)));
	m_UniformBuffer->SetUniform(offset + (GLSL_VEC3_SIZE * 2), sizeof(glm::vec3), glm::value_ptr(glm::vec3(0.0f)));
	m_UniformBuffer->SetUniform(offset + (GLSL_VEC3_SIZE * 3) - GLSL_SCALAR_SIZE, sizeof(float), (void*)0);
	m_UniformBuffer->SetUniform(offset + (GLSL_VEC3_SIZE * 3), sizeof(float), (void*)0);
}
