#include "DirectionalLight.h"

#include "Renderer/UniformBuffer.h"
#include "glm/gtc/type_ptr.hpp"
#include "Material/ShaderLibrary.h"

DirectionalLight::DirectionalLight(Entity* owner, Ref<UniformBuffer> vertexUniformBuffer, Ref<UniformBuffer> fragmentUniformBuffer)
	: Light(owner, vertexUniformBuffer, fragmentUniformBuffer)
{
	m_Direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::~DirectionalLight()
{
	SwitchOff();
}

void DirectionalLight::Use()
{
	uint32_t offset = GLSL_DIRECTIONAL_LIGHT_OFFSET;
	m_FragmentUniformBuffer->SetUniform(offset, sizeof(glm::vec3), glm::value_ptr(m_Direction));
	m_FragmentUniformBuffer->SetUniform(offset + GLSL_VEC3_SIZE, sizeof(glm::vec3), glm::value_ptr(m_Color));
	m_FragmentUniformBuffer->SetUniform(offset + (GLSL_VEC3_SIZE * 2) - GLSL_SCALAR_SIZE, sizeof(bool), &m_ShadowsEnabled);

	glm::vec3 position = glm::clamp(m_Direction * -20.0f, glm::vec3(0.0001f), glm::vec3(10000.0f));
	glm::mat4 lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 20.0f);
	glm::mat4 lightView = glm::lookAt(position,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	m_LightSpace = lightProjection * lightView;
	m_VertexUniformBuffer->SetUniform(0, sizeof(glm::mat4), glm::value_ptr(m_LightSpace));
}

void DirectionalLight::SwitchOff()
{
	uint32_t offset = GLSL_DIRECTIONAL_LIGHT_OFFSET;
	m_FragmentUniformBuffer->SetUniform(offset, sizeof(glm::vec3), glm::value_ptr(glm::vec3(0.0f)));
	m_FragmentUniformBuffer->SetUniform(offset + GLSL_VEC3_SIZE, sizeof(glm::vec3), glm::value_ptr(glm::vec3(0.0f)));
}

void DirectionalLight::RenderShadowMap()
{
	Renderer::GetInstance()->RenderShadowMap(m_Owner->GetScene(), this);
}
