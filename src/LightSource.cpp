#include "LightSource.h"

LightSource::LightSource(std::string name, LightSourceType type, Ref<Shader> shader, Ref<ShaderLibrary> shaderLibrary, Ref<Camera> camera)
	: m_Name(name), m_Type(type), m_Shader(shader), m_ShaderLibrary(shaderLibrary), m_Camera(camera)
{
	m_Transform = Ref<Transform>();
}

void LightSource::Initialize()
{
	m_Transform = CreateRef<Transform>(shared_from_this());
}

void LightSource::Update()
{
	if (m_Transform->Parent)
		m_Transform->CalculateModelMatrix(m_Transform->Parent->ModelMatrix);
	else
		m_Transform->CalculateModelMatrix();
}

void LightSource::Use()
{
	for (auto shader : m_ShaderLibrary->GetAllShaders())
	{
		shader->Use();

		shader->SetVec3("u_ViewPosition", m_Camera->Position);
		shader->SetVec3("u_Light.position", m_Transform->Position);

		shader->SetVec3("u_Light.ambient", m_Ambient);
		shader->SetVec3("u_Light.diffuse", m_Diffuse);
		shader->SetVec3("u_Light.specular", m_Specular);
	}

}
