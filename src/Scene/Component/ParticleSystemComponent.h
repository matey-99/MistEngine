#pragma once

#include "Scene/Component/RenderComponent.h"
#include "Renderer/ShaderStorageBuffer.h"
#include "Renderer/ComputeShader.h"

class ParticleSystemComponent : public RenderComponent
{
public:
	ParticleSystemComponent(Entity* owner);

	virtual void Begin() override;
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void Destroy() override;

private:
	uint32_t GenerateNoiseTexture(int width, int height, int depth, int internalFormat);

private:
	Ref<ShaderStorageBuffer<glm::vec4>> m_PositionBuffer;
	Ref<ShaderStorageBuffer<glm::vec4>> m_VelocityBuffer;
	Ref<ShaderStorageBuffer<uint32_t>> m_IndexBuffer;

	Ref<ComputeShader> m_ComputeShader;

	uint32_t m_NoiseTexture;

	uint32_t m_ParticleVAO;

	uint32_t m_ParticlesCount;
	float m_Radius;
	glm::vec3 m_Velocity;
	float m_ParticleLifeTime;

	// noise
	int m_NoiseSize;
	float m_NoiseFrequency;
	float m_NoiseStrength;
	float m_Damping;


	friend class SceneSerializer;
};