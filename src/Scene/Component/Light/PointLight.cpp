#include "PointLight.h"

#include <glm/gtc/type_ptr.hpp>

#include "Scene/Scene.h"
#include "Renderer/UniformBuffer.h"

PointLight::PointLight(Entity* owner, Ref<UniformBuffer> vertexUniformBuffer, Ref<UniformBuffer> fragmentUniformBuffer)
	: Light(owner, vertexUniformBuffer, fragmentUniformBuffer)
{
	m_Index = owner->GetScene()->GetComponentsCount<PointLight>();

	m_Linear = 0.09f;
	m_Quadratic = 0.032f;

	for (int i = 0; i < 6; i++)
		m_LightViews.push_back(glm::mat4(0.0f));
}

PointLight::~PointLight()
{
	SwitchOff();
}

void PointLight::Use()
{
	uint32_t offset = GLSL_POINT_LIGHTS_OFFSET + (GLSL_POINT_LIGHT_SIZE * m_Index);
	m_FragmentUniformBuffer->SetUniform(offset, sizeof(glm::vec3), glm::value_ptr(m_Owner->GetWorldPosition()));
	m_FragmentUniformBuffer->SetUniform(offset + GLSL_VEC3_SIZE, sizeof(glm::vec3), glm::value_ptr(m_Color));

	glm::mat4 lightProjection = glm::perspective(glm::radians(90.0f), 1.0f, 1.0f, 25.0f);
	m_LightViews.at(0) = (lightProjection * glm::lookAt(m_Owner->GetWorldPosition(), m_Owner->GetWorldPosition() + glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0)));
	m_LightViews.at(1) = (lightProjection * glm::lookAt(m_Owner->GetWorldPosition(), m_Owner->GetWorldPosition() + glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0)));
	m_LightViews.at(2) = (lightProjection * glm::lookAt(m_Owner->GetWorldPosition(), m_Owner->GetWorldPosition() + glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0)));
	m_LightViews.at(3) = (lightProjection * glm::lookAt(m_Owner->GetWorldPosition(), m_Owner->GetWorldPosition() + glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0)));
	m_LightViews.at(4) = (lightProjection * glm::lookAt(m_Owner->GetWorldPosition(), m_Owner->GetWorldPosition() + glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0)));
	m_LightViews.at(5) = (lightProjection * glm::lookAt(m_Owner->GetWorldPosition(), m_Owner->GetWorldPosition() + glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0)));
}

void PointLight::SwitchOff()
{
	uint32_t offset = GLSL_POINT_LIGHTS_OFFSET + (GLSL_POINT_LIGHT_SIZE * m_Index);
	m_FragmentUniformBuffer->SetUniform(offset, sizeof(glm::vec3), glm::value_ptr(glm::vec3(0.0f)));
	m_FragmentUniformBuffer->SetUniform(offset + GLSL_VEC3_SIZE, sizeof(glm::vec3), glm::value_ptr(glm::vec3(0.0f)));
}

void PointLight::RenderShadowMap()
{
	Renderer::GetInstance()->RenderShadowMap(m_Owner->GetScene(), this);
}
