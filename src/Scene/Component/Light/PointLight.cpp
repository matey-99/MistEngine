#include "PointLight.h"

#include <glm/gtc/type_ptr.hpp>

#include "Scene/Scene.h"
#include "Renderer/UniformBuffer.h"

PointLight::PointLight(Entity* owner, Ref<UniformBuffer> uniformBuffer)
	: Light(owner, uniformBuffer)
{
	m_Index = owner->GetScene()->GetComponentsCount<PointLight>();

	m_Linear = 0.09f;
	m_Quadratic = 0.032f;
}

PointLight::~PointLight()
{
	SwitchOff();
}

void PointLight::Use()
{
	uint32_t offset = GLSL_POINT_LIGHTS_OFFSET + (GLSL_POINT_LIGHT_SIZE * m_Index);
	m_UniformBuffer->SetUniform(offset, sizeof(glm::vec3), glm::value_ptr(m_Owner->GetWorldPosition()));
	m_UniformBuffer->SetUniform(offset + GLSL_VEC3_SIZE, sizeof(glm::vec3), glm::value_ptr(m_Color));
}

void PointLight::SwitchOff()
{
	uint32_t offset = GLSL_POINT_LIGHTS_OFFSET + (GLSL_POINT_LIGHT_SIZE * m_Index);
	m_UniformBuffer->SetUniform(offset, sizeof(glm::vec3), glm::value_ptr(glm::vec3(0.0f)));
	m_UniformBuffer->SetUniform(offset + GLSL_VEC3_SIZE, sizeof(glm::vec3), glm::value_ptr(glm::vec3(0.0f)));
}
