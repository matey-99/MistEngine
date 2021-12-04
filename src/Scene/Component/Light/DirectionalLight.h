#pragma once

#include "Light.h"

class DirectionalLight : public Light
{
private:
	glm::vec3 m_Direction;

public:
	DirectionalLight(Entity* owner, Ref<UniformBuffer> vertexUniformBuffer, Ref<UniformBuffer> fragmentUniformBuffer);
	~DirectionalLight();

	virtual void Use() override;
	virtual void SwitchOff() override;
	
	virtual void RenderShadowMap() override;

	inline glm::vec3 GetDirection() const { return m_Direction; }

	inline void SetDirection(glm::vec3 direction) { m_Direction = direction; }

	friend class EntityDetailsPanel;
};
