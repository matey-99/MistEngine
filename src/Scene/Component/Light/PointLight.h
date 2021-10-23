#pragma once

#include "Light.h"

class PointLight : public Light
{
private:
	int m_Index;

	float m_Linear;
	float m_Quadratic;

	Ref<UniformBuffer> m_PBRUniformBuffer;

public:
	PointLight(Ref<Entity> entity, Ref<UniformBuffer> uniformBuffer);
	~PointLight();

	virtual void Begin() override;
	virtual void Update() override;

	virtual void Use() override;
	virtual void SwitchOff() override;

	inline void SetIndex(int index) { m_Index = index; }

	inline float GetLinear() const { return m_Linear; }
	inline float GetQuadratic() const { return m_Quadratic; }

	inline void SetLinear(float linear) { m_Linear = linear; }
	inline void SetQuadratic(float quadratic) { m_Quadratic = quadratic; }

	inline void SetPBRUniformBuffer(Ref<UniformBuffer> ubo) { m_PBRUniformBuffer = ubo; }

	friend class EntityDetailsPanel;
};
