#include "Light.h"

Light::Light(Ref<Entity> entity, Ref<Camera> camera, Ref<ShaderLibrary> shaderLibrary)
	: m_Entity(entity), m_Camera(camera), m_ShaderLibrary(shaderLibrary)
{
	m_LightType = LightType::Point;
}

void Light::Begin()
{
}

void Light::Update()
{
}

void Light::Use()
{
	for (auto shader : m_ShaderLibrary->GetAllShaders())
	{
		shader->Use();

		shader->SetVec3("u_ViewPosition", m_Camera->Position);
		shader->SetVec3("u_Light.position", m_Entity->GetTransform()->Position);

		shader->SetVec3("u_Light.ambient", m_Ambient);
		shader->SetVec3("u_Light.diffuse", m_Diffuse);
		shader->SetVec3("u_Light.specular", m_Specular);
	}

}
