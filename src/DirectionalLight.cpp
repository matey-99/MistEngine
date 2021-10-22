#include "DirectionalLight.h"

#include "UniformBuffer.h"
#include "glm/gtc/type_ptr.hpp"
#include "MaterialManager.h"

DirectionalLight::DirectionalLight(Ref<Entity> entity) : Light(entity)
{
	m_Direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::~DirectionalLight()
{
	auto shaderLibrary = MaterialManager::GetInstance()->GetShaderLibrary();
	for (auto shader : shaderLibrary->GetAllMaterialShaders())
	{
		shader->Use();

		shader->SetVec3("u_DirectionalLight.ambient", glm::vec3(0.0f));
		shader->SetVec3("u_DirectionalLight.diffuse", glm::vec3(0.0f));
		shader->SetVec3("u_DirectionalLight.specular", glm::vec3(0.0f));
	}
}

void DirectionalLight::Begin()
{
}

void DirectionalLight::Update()
{
}

void DirectionalLight::Use(Ref<UniformBuffer> uniformBuffer)
{
	uint32_t offset = GLSL_DIRECTIONAL_LIGHT_OFFSET;
	uniformBuffer->SetUniform(offset, sizeof(glm::vec3), glm::value_ptr(m_Direction));
	uniformBuffer->SetUniform(offset + GLSL_VEC3_SIZE, sizeof(glm::vec3), glm::value_ptr(m_Ambient));
	uniformBuffer->SetUniform(offset + (GLSL_VEC3_SIZE * 2), sizeof(glm::vec3), glm::value_ptr(m_Diffuse));
	uniformBuffer->SetUniform(offset + (GLSL_VEC3_SIZE * 3), sizeof(glm::vec3), glm::value_ptr(m_Specular));
}

void DirectionalLight::SwitchOff()
{
	//auto shaderLibrary = MaterialManager::GetInstance()->GetShaderLibrary();
	//for (auto shader : shaderLibrary->GetAllMaterialShaders())
	//{
	//	shader->Use();

	//	shader->SetVec3("u_ViewPosition", glm::vec3(0.0f));
	//	shader->SetVec3("u_DirectionalLight.direction", glm::vec3(0.0f));

	//	shader->SetVec3("u_DirectionalLight.ambient", glm::vec3(0.0f));
	//	shader->SetVec3("u_DirectionalLight.diffuse", glm::vec3(0.0f));
	//	shader->SetVec3("u_DirectionalLight.specular", glm::vec3(0.0f));
	//}
}
