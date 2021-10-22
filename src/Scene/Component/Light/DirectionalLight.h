#pragma once

#include "Light.h"

class DirectionalLight : public Light
{
private:
	glm::vec3 m_Direction;

public:
	DirectionalLight(Ref<Entity> entity, Ref<UniformBuffer> uniformBuffer);
	~DirectionalLight();

	virtual void Begin() override;
	virtual void Update() override;

	virtual void Use() override;
	virtual void SwitchOff() override;

	inline glm::vec3 GetDirection() const { return m_Direction; }

	inline void SetDirection(glm::vec3 direction) { m_Direction = direction; }

	friend class EntityDetailsPanel;
};
