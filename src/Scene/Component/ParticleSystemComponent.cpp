#include "ParticleSystemComponent.h"
#include <GLFW/glfw3.h>

ParticleSystemComponent::ParticleSystemComponent(Entity* owner)
	: RenderComponent(owner)
{
	m_ParticlesCount = 1024 * 1024;
	m_Radius = 20.0f;
	m_Velocity = 2.0f;

	m_PositionBuffer = CreateRef<ShaderStorageBuffer<glm::vec4>>(m_ParticlesCount);
	m_VelocityBuffer = CreateRef<ShaderStorageBuffer<glm::vec4>>(m_ParticlesCount);

	srand(glfwGetTime());

	m_PositionBuffer->Bind();
	glm::vec4* positions = m_PositionBuffer->Map();
	for (int i = 0; i < m_ParticlesCount; i++)
	{
		positions[i].x = ((float)rand() / RAND_MAX) * m_Radius;
		positions[i].y = ((float)rand() / RAND_MAX) * m_Radius;
		positions[i].z = ((float)rand() / RAND_MAX) * m_Radius;
		positions[i].w = 1.0f;
	}
	m_PositionBuffer->Unmap();
	m_PositionBuffer->Unbind();

	m_VelocityBuffer->Bind();
	glm::vec4* velocities = m_VelocityBuffer->Map();
	for (int i = 0; i < m_ParticlesCount; i++)
	{
		velocities[i].x = ((float)rand() / RAND_MAX) * m_Velocity;
		velocities[i].y = ((float)rand() / RAND_MAX) * m_Velocity;
		velocities[i].z = ((float)rand() / RAND_MAX) * m_Velocity;
		velocities[i].w = 0.0f;
	}
	m_VelocityBuffer->Unmap();
	m_VelocityBuffer->Unbind();


}

void ParticleSystemComponent::Begin()
{
}

void ParticleSystemComponent::Update()
{
}

void ParticleSystemComponent::PreRender()
{
}

void ParticleSystemComponent::Render()
{
}

void ParticleSystemComponent::Destroy()
{
}
