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
	Ref<ShaderStorageBuffer<glm::vec4>> m_PositionBuffer;
	Ref<ShaderStorageBuffer<glm::vec4>> m_VelocityBuffer;
	Ref<ShaderStorageBuffer<uint32_t>> m_IndexBuffer;

	uint32_t m_ProgramPipeline;
	Ref<ComputeShader> m_ComputeShader;

	uint32_t m_ParticlesCount;
	uint32_t m_Radius;
	uint32_t m_Velocity;
};