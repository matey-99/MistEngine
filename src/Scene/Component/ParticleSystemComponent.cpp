#include "ParticleSystemComponent.h"
#include <GLFW/glfw3.h>
#include "Material/ShaderLibrary.h"

ParticleSystemComponent::ParticleSystemComponent(Entity* owner)
	: RenderComponent(owner)
{
	m_ParticlesCount = 1024;
	m_Radius = 20.0f;
	m_Velocity = 2.0f;

	m_PositionBuffer = CreateRef<ShaderStorageBuffer<glm::vec4>>(m_ParticlesCount);
	m_VelocityBuffer = CreateRef<ShaderStorageBuffer<glm::vec4>>(m_ParticlesCount);
	m_IndexBuffer = CreateRef<ShaderStorageBuffer<uint32_t>>(m_ParticlesCount * 6);

	m_IndexBuffer->Bind();
	uint32_t* indices = m_IndexBuffer->Map();
	for (int i = 0; i < m_ParticlesCount; i++)
	{
		int index = i << 2;
		*(indices++) = index;
		*(indices++) = index + 1;
		*(indices++) = index + 2;
		*(indices++) = index;
		*(indices++) = index + 2;
		*(indices++) = index + 3;
	}
	m_IndexBuffer->Unmap();
	m_IndexBuffer->Unbind();
	
	m_ComputeShader = CreateRef<ComputeShader>("res/shaders/Particle/StandardParticle.comp");
	m_ComputeShader->Use();
	m_ComputeShader->SetUint("u_ParticlesCount", m_ParticlesCount);
}

void ParticleSystemComponent::Begin()
{
}

void ParticleSystemComponent::Update()
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_PositionBuffer->GetID());
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_VelocityBuffer->GetID());

	m_ComputeShader->Use();
	glDispatchCompute(m_ParticlesCount / 128, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void ParticleSystemComponent::PreRender()
{
}

void ParticleSystemComponent::Render()
{
	/*auto standardShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::MATERIAL, "Standard");
	standardShader->Use();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_PositionBuffer->GetID());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer->GetID());

	glDrawElements(GL_TRIANGLES, m_ParticlesCount * 6, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glDisable(GL_BLEND);*/
}

void ParticleSystemComponent::Destroy()
{
}
