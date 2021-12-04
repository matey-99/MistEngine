#include "Light.h"

Light::Light(Entity* owner, Ref<UniformBuffer> vertexUniformBuffer, Ref<UniformBuffer> fragmentUniformBuffer)
	: RenderComponent(owner), m_VertexUniformBuffer(vertexUniformBuffer), m_FragmentUniformBuffer(fragmentUniformBuffer)
{
	m_Color = glm::vec3(1.0f);
	m_ShadowsEnabled = true;
}

void Light::Begin()
{
}

void Light::Update()
{
	Use();
}

void Light::PreRender()
{
	if (m_ShadowsEnabled)
		RenderShadowMap();
}

void Light::Render(ViewMode viewMode)
{
}

void Light::Destroy()
{
	SwitchOff();
}
