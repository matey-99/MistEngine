#pragma once

#include "Light.h"

class PointLight : public Light
{
private:
	int m_Index;

	float m_Linear;
	float m_Quadratic;

	uint32_t m_ShadowMap;
	std::vector<glm::mat4> m_LightViews;
	float m_FarPlane;

public:
	PointLight(Entity* owner, Ref<UniformBuffer> vertexUniformBuffer, Ref<UniformBuffer> fragmentUniformBuffer);
	~PointLight();

	virtual void Use() override;
	virtual void SwitchOff() override;

	virtual void RenderShadowMap() override;

	inline void SetIndex(int index) { m_Index = index; }

	inline float GetLinear() const { return m_Linear; }
	inline float GetQuadratic() const { return m_Quadratic; }

	inline uint32_t GetShadowMap() const { return m_ShadowMap; }
	inline std::vector<glm::mat4> GetLightViews() const { return m_LightViews; }
	inline float GetFarPlane() const { return m_FarPlane; }

	inline void SetLinear(float linear) { m_Linear = linear; }
	inline void SetQuadratic(float quadratic) { m_Quadratic = quadratic; }

	friend class EntityDetailsPanel;
};
