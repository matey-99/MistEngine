#include "DirectionalLight.h"

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

void DirectionalLight::Use(glm::vec3 cameraPosition)
{
	auto shaderLibrary = MaterialManager::GetInstance()->GetShaderLibrary();
	for (auto shader : shaderLibrary->GetAllMaterialShaders())
	{
		shader->Use();

		shader->SetVec3("u_ViewPosition", cameraPosition);
		shader->SetVec3("u_DirectionalLight.direction", m_Direction);

		shader->SetVec3("u_DirectionalLight.ambient", m_Ambient);
		shader->SetVec3("u_DirectionalLight.diffuse", m_Diffuse);
		shader->SetVec3("u_DirectionalLight.specular", m_Specular);
	}
}
