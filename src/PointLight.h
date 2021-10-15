#pragma once

#include "Light.h"

class PointLight : public Light
{
private:
	int m_Index;

	float m_Linear;
	float m_Quadratic;

public:
	PointLight(Ref<Entity> entity);
	~PointLight();

	virtual void Begin() override;
	virtual void Update() override;

	virtual void Use(glm::vec3 cameraPosition) override;

	inline void SetIndex(int index) { m_Index = index; }

	inline float GetLinear() const { return m_Linear; }
	inline float GetQuadratic() const { return m_Quadratic; }

	inline void SetLinear(float linear) { m_Linear = linear; }
	inline void SetQuadratic(float quadratic) { m_Quadratic = quadratic; }

	friend class EntityDetailsPanel;
};
