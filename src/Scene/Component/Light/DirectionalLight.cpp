#include "DirectionalLight.h"

#include "Renderer/UniformBuffer.h"
#include "glm/gtc/type_ptr.hpp"
#include "Material/MaterialManager.h"

DirectionalLight::DirectionalLight(Ref<Entity> entity, Ref<UniformBuffer> uniformBuffer)
	: Light(entity, uniformBuffer)
{
	m_Direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::~DirectionalLight()
{
	SwitchOff();
}

void DirectionalLight::Begin()
{
}

void DirectionalLight::Update()
{
}

void DirectionalLight::Use()
{
	uint32_t offset = GLSL_DIRECTIONAL_LIGHT_OFFSET;
	m_UniformBuffer->SetUniform(offset, sizeof(glm::vec3), glm::value_ptr(m_Direction));
	m_UniformBuffer->SetUniform(offset + GLSL_VEC3_SIZE, sizeof(glm::vec3), glm::value_ptr(m_Color));
}

void DirectionalLight::SwitchOff()
{
	uint32_t offset = GLSL_DIRECTIONAL_LIGHT_OFFSET;
	m_UniformBuffer->SetUniform(offset, sizeof(glm::vec3), glm::value_ptr(glm::vec3(0.0f)));
	m_UniformBuffer->SetUniform(offset + GLSL_VEC3_SIZE, sizeof(glm::vec3), glm::value_ptr(glm::vec3(0.0f)));
}