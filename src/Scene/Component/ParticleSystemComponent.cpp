#include "ParticleSystemComponent.h"
#include <GLFW/glfw3.h>
#include "Material/ShaderLibrary.h"
#include "Scene/Entity.h"
#include "Scene/Scene.h"

ParticleSystemComponent::ParticleSystemComponent(Entity* owner)
	: RenderComponent(owner)
{
	m_ParticlesCount = 1 << 20;
	m_Radius = 15.0f;
	m_Velocity = glm::vec3(0.0f, 2.0f, 0.0f);
	m_ParticleLifeTime = 3.0f;

	// noise
	m_NoiseSize = 16;
	m_NoiseFrequency = 10.0f;
	m_NoiseStrength = 0.001f;
	m_Damping = 0.95f;

	m_PositionBuffer = CreateRef<ShaderStorageBuffer<glm::vec4>>(m_ParticlesCount);
	m_VelocityBuffer = CreateRef<ShaderStorageBuffer<glm::vec4>>(m_ParticlesCount);
	m_IndexBuffer = CreateRef<ShaderStorageBuffer<uint32_t>>(m_ParticlesCount * 6);

	m_PositionBuffer->Bind();
	glm::vec4* positions = m_PositionBuffer->Map();
	for (size_t i = 0; i < m_ParticlesCount; i++)
	{
		glm::vec3 center = m_Owner->GetWorldPosition();

		float theta = 2 * glm::pi<float>() * ((float)rand() / RAND_MAX);
		float phi = glm::pi<float>() * ((float)rand() / RAND_MAX);
		float distance = sqrt((float)rand() / RAND_MAX);
		float x = center.x + distance * cos(theta) * sin(phi);
		float y = center.y + distance * cos(theta);
		float z = center.z + distance * sin(theta) * sin(phi);

		positions[i] = glm::vec4(x, y, z, 1.0f);
	}
	m_PositionBuffer->Unmap();
	m_PositionBuffer->Unbind();

	m_IndexBuffer->Bind();
	uint32_t* indices = m_IndexBuffer->Map();
	for (size_t i = 0; i < m_ParticlesCount; i++)
	{
		size_t index = i << 2;
		*(indices++) = index;
		*(indices++) = index + 1;
		*(indices++) = index + 2;
		*(indices++) = index;
		*(indices++) = index + 2;
		*(indices++) = index + 3;
	}
	m_IndexBuffer->Unmap();
	m_IndexBuffer->Unbind();

	m_NoiseTexture = GenerateNoiseTexture(m_NoiseSize, m_NoiseSize, m_NoiseSize, GL_RGBA8_SNORM);

	m_ComputeShader = CreateRef<ComputeShader>("res/shaders/Particle/StandardParticle.comp");
	m_ComputeShader->Use();
	m_ComputeShader->SetUint("u_ParticlesCount", m_ParticlesCount);
	m_ComputeShader->SetFloat("u_InverseNoiseSize", 1.0f / m_NoiseSize);
	m_ComputeShader->SetInt("u_NoiseTexture", 0);
	m_ComputeShader->SetFloat("u_NoiseFrequency", m_NoiseFrequency);
	m_ComputeShader->SetFloat("u_NoiseStrength", m_NoiseStrength);
	m_ComputeShader->SetFloat("u_Damping", m_Damping);
	m_ComputeShader->SetVec4("u_Attractor", glm::vec4(0.0f));

	glGenVertexArrays(1, &m_ParticleVAO);
}

void ParticleSystemComponent::Begin()
{
	
}

void ParticleSystemComponent::Update()
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_PositionBuffer->GetID());
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_VelocityBuffer->GetID());


	const float speed = 0.2f;
	glm::vec4 attractor = glm::vec4(sin(glfwGetTime() * speed), sin(glfwGetTime() * speed * 1.3f), cos(glfwGetTime() * speed), 0.02f);

	m_ComputeShader->Use();
	m_ComputeShader->SetVec4("u_Attractor", attractor);


	glDispatchCompute(m_ParticlesCount / 128, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void ParticleSystemComponent::PreRender()
{
}

void ParticleSystemComponent::Render()
{
	auto particleShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::PARTICLE, "StandardParticle");
	particleShader->Use();
	particleShader->SetMat4("u_Model", m_Owner->GetTransform().ModelMatrix);
	particleShader->SetMat4("u_View", m_Owner->GetScene()->GetCamera()->GetViewMatrix());
	particleShader->SetMat4("u_Projection", m_Owner->GetScene()->GetCamera()->GetProjectionMatrix());
	particleShader->SetVec2("u_SpriteSize", glm::vec2(0.015f, 0.015f));
	particleShader->SetBool("u_IsSprite", false);
	particleShader->SetInt("u_Sprite", 0);
	particleShader->SetVec4("u_Color", glm::vec4(0.4f, 6.0f, 7.0f, 1.0f));

	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glBindVertexArray(m_ParticleVAO);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_PositionBuffer->GetID());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer->GetID());

	glDrawElements(GL_TRIANGLES, m_ParticlesCount * 6, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);

	glBindVertexArray(0);


	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}

void ParticleSystemComponent::Destroy()
{
}

uint32_t ParticleSystemComponent::GenerateNoiseTexture(int width, int height, int depth, int internalFormat)
{
	srand(glfwGetTime());

	uint8_t* data = new uint8_t[width * height * depth * 4];
	uint8_t* ptr = data;
	for (int z = 0; z < depth; z++)
	{
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				*ptr++ = rand() & 0xff;
				*ptr++ = rand() & 0xff;
				*ptr++ = rand() & 0xff;
				*ptr++ = rand() & 0xff;
			}
		}
	}

	uint32_t texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_3D, texture);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);

	glTexImage3D(GL_TEXTURE_3D, 0, internalFormat, width, height, depth, 0, GL_RGBA, GL_BYTE, data);

	delete[] data;
	return texture;
}
