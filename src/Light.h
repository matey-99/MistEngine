#pragma once

#include <string>
#include <glm/glm.hpp>

#include "Component.h"
#include "Entity.h"
#include "Shader.h"
#include "Camera.h"

enum class LightType
{
	Point, Directional
};

class Light : public Component
{
private:
	Ref<Entity> m_Entity;

	LightType m_LightType;

	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;

public:
	Light(Ref<Entity> entity);

	virtual void Begin() override;
	virtual void Update() override;

	void Use(glm::vec3 cameraPosition);

	inline LightType GetLightType() const { return m_LightType; }
	inline glm::vec3 GetAmbient() const { return m_Ambient; }
	inline glm::vec3 GetDiffuse() const { return m_Diffuse; }
	inline glm::vec3 GetSpecular() const { return m_Specular; }

	inline void SetLightType(LightType type) { m_LightType = type; }
	inline void SetAmbient(glm::vec3 ambient) { m_Ambient = ambient; }
	inline void SetDiffuse(glm::vec3 diffuse) { m_Diffuse = diffuse; }
	inline void SetSpecular(glm::vec3 specular) { m_Specular = specular; }

	friend class EntityDetailsPanel;
};
