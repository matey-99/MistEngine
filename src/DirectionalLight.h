#pragma once

#include "Light.h"

class DirectionalLight : public Light
{
private:
	glm::vec3 m_Direction;

public:
	DirectionalLight(Ref<Entity> entity);
	~DirectionalLight();

	virtual void Begin() override;
	virtual void Update() override;

	virtual void Use(glm::vec3 cameraPosition) override;

	inline glm::vec3 GetDirection() const { return m_Direction; }

	inline void SetDirection(glm::vec3 direction) { m_Direction = direction; }

	friend class EntityDetailsPanel;
};
