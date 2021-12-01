#include "Light.h"

Light::Light(Entity* owner, Ref<UniformBuffer> uniformBuffer)
	: Component(owner), m_UniformBuffer(uniformBuffer)
{
	m_Color = glm::vec3(1.0f);
}

void Light::Begin()
{
}

void Light::Update()
{
	Use();
}

void Light::Destroy()
{
	SwitchOff();
}
