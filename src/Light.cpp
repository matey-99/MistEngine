#include "Light.h"

Light::Light(Ref<Entity> entity)
	: m_Entity(entity)
{
	m_Ambient = glm::vec3(0.2f);
	m_Diffuse = glm::vec3(0.5f);
	m_Specular = glm::vec3(1.0f);
}

void Light::Begin()
{
}

void Light::Update()
{
}
