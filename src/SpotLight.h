#pragma once

#include "Light.h"

class SpotLight : public Light
{
private:
	int m_Index;

	glm::vec3 m_Direction;

	float m_Linear;
	float m_Quadratic;

	float m_InnerCutOff;
	float m_OuterCutOff;

public:
	SpotLight(Ref<Entity> entity);
	~SpotLight();

	virtual void Begin() override;
	virtual void Update() override;

	virtual void Use(glm::vec3 cameraPosition) override;

	inline void SetIndex(int index) { m_Index = index; }

	inline glm::vec3 GetDirection() const { return m_Direction; }
	inline float GetInnerCutOff() const { return m_InnerCutOff; }
	inline float GetOuterCutOff() const { return m_OuterCutOff; }
	inline float GetLinear() const { return m_Linear; }
	inline float GetQuadratic() const { return m_Quadratic; }

	inline void SetDirection(glm::vec3 direction) { m_Direction = direction; }
	inline void SetInnerCutOff(float innerCutOff) { m_InnerCutOff = innerCutOff; }
	inline void SetOuterCutOff(float outerCutOff) { m_OuterCutOff = outerCutOff; }
	inline void SetLinear(float linear) { m_Linear = linear; }
	inline void SetQuadratic(float quadratic) { m_Quadratic = quadratic; }

	friend class EntityDetailsPanel;
};
