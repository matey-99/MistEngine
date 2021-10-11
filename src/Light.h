#pragma once

#include <string>
#include <glm/glm.hpp>

#include "Component.h"
#include "Entity.h"
#include "Shader.h"
#include "Camera.h"
#include "ShaderLibrary.h"

class Light : public Component
{
private:
	Ref<Entity> m_Entity;
	Ref<ShaderLibrary> m_ShaderLibrary;
	Ref<Camera> m_Camera;

	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;

public:
	Light(Ref<Entity> entity, Ref<Camera> camera, Ref<ShaderLibrary> shaderLibrary);

	virtual void Begin() override;
	virtual void Update() override;

	void Use();

	inline void SetAmbient(glm::vec3 ambient) { m_Ambient = ambient; }
	inline void SetDiffuse(glm::vec3 diffuse) { m_Diffuse = diffuse; }
	inline void SetSpecular(glm::vec3 specular) { m_Specular = specular; }
};
