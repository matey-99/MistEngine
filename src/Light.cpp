#include "Light.h"
#include "MaterialManager.h"

Light::Light(Ref<Entity> entity)
	: m_Entity(entity)
{
	m_LightType = LightType::Point;
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

void Light::Use(glm::vec3 cameraPosition)
{
	auto shaderLibrary = MaterialManager::GetInstance()->GetShaderLibrary();
	for (auto shader : shaderLibrary->GetAllShaders())
	{
		shader->Use();

		shader->SetVec3("u_ViewPosition", cameraPosition);
		shader->SetVec3("u_Light.position", m_Entity->GetTransform()->Position);

		shader->SetVec3("u_Light.ambient", m_Ambient);
		shader->SetVec3("u_Light.diffuse", m_Diffuse);
		shader->SetVec3("u_Light.specular", m_Specular);
	}

}
