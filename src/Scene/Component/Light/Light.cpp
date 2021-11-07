#include "Light.h"

Light::Light(Ref<Entity> entity, Ref<UniformBuffer> uniformBuffer)
	: m_Entity(entity), m_UniformBuffer(uniformBuffer)
{
	m_Color = glm::vec3(1.0f);
}

void Light::Begin()
{
}

void Light::Update()
{
}
