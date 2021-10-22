#pragma once

#include <string>
#include <glm/glm.hpp>

#include "Scene/Component/Component.h"
#include "Scene/Entity.h"
#include "Renderer/Shader.h"
#include "Scene/Camera.h"
#include "Renderer/UniformBuffer.h"

#define MAX_POINT_LIGHTS 32
#define MAX_SPOT_LIGHTS 32

#define GLSL_SCALAR_SIZE 4
#define GLSL_VEC3_SIZE 16
#define GLSL_DIRECTIONAL_LIGHT_SIZE 64
#define GLSL_POINT_LIGHT_SIZE 80
#define GLSL_SPOT_LIGHT_SIZE 96

#define GLSL_LIGHTS_COUNTS_OFFSET 0
#define GLSL_DIRECTIONAL_LIGHT_OFFSET 16
#define GLSL_POINT_LIGHTS_OFFSET (GLSL_DIRECTIONAL_LIGHT_OFFSET + GLSL_DIRECTIONAL_LIGHT_SIZE)
#define GLSL_SPOT_LIGHTS_OFFSET (GLSL_POINT_LIGHTS_OFFSET + (GLSL_POINT_LIGHT_SIZE * MAX_POINT_LIGHTS))

class UniformBuffer;

class Light : public Component
{
protected:
	Ref<Entity> m_Entity;
	Ref<UniformBuffer> m_UniformBuffer;

	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;

public:
	Light(Ref<Entity> entity, Ref<UniformBuffer> uniformBuffer);

	virtual void Begin() override;
	virtual void Update() override;

	virtual void Use() = 0;
	virtual void SwitchOff() = 0;

	inline glm::vec3 GetAmbient() const { return m_Ambient; }
	inline glm::vec3 GetDiffuse() const { return m_Diffuse; }
	inline glm::vec3 GetSpecular() const { return m_Specular; }

	inline void SetAmbient(glm::vec3 ambient) { m_Ambient = ambient; }
	inline void SetDiffuse(glm::vec3 diffuse) { m_Diffuse = diffuse; }
	inline void SetSpecular(glm::vec3 specular) { m_Specular = specular; }

	friend class EntityDetailsPanel;
};
