#pragma once

#include <string>
#include <glm/glm.hpp>

#include "typedefs.h"
#include "Shader.h"

enum class MaterialType
{
	Default, Texture
};

class Material
{
private:
	std::string m_Name;
	Ref<Shader> m_Shader;
	MaterialType m_Type;

	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;
	float m_Shininess;

public:
	Material(std::string name = "Default", Ref<Shader> shader = Ref<Shader>(), MaterialType type = MaterialType::Default);

	void Use();

	inline Ref<Shader> GetShader() const { return m_Shader; }

	inline void SetAmbient(glm::vec3 ambient) { m_Ambient = ambient; }
	inline void SetDiffuse(glm::vec3 diffuse) { m_Diffuse = diffuse; }
	inline void SetSpecular(glm::vec3 specular) { m_Specular = specular; }
	inline void SetShininess(float shininess) { m_Shininess = shininess; }
};