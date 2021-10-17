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
	uint64_t m_ID;
	std::string m_Name;
	Ref<Shader> m_Shader;
	MaterialType m_Type;

	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;
	float m_Shininess;

	bool depthTest;

public:
	Material(std::string name = "Default", Ref<Shader> shader = Ref<Shader>(), MaterialType type = MaterialType::Default);
	Material(uint64_t id, std::string name = "Default", Ref<Shader> shader = Ref<Shader>(), MaterialType type = MaterialType::Default);

	void Use();

	inline uint64_t GetID() const { return m_ID; }
	inline std::string GetName() const { return m_Name; }
	inline Ref<Shader> GetShader() const { return m_Shader; }
	inline glm::vec3 GetAmbient() const { return m_Ambient; }
	inline glm::vec3 GetDiffuse() const { return m_Diffuse; }
	inline glm::vec3 GetSpecular() const { return m_Specular; }
	inline float GetShininess() const { return m_Shininess; }

	inline void SetAmbient(glm::vec3 ambient) { m_Ambient = ambient; }
	inline void SetDiffuse(glm::vec3 diffuse) { m_Diffuse = diffuse; }
	inline void SetSpecular(glm::vec3 specular) { m_Specular = specular; }
	inline void SetShininess(float shininess) { m_Shininess = shininess; }

	friend class MaterialEditorPanel;
};