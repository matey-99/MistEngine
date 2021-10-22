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

	glm::vec3 m_Color;
	float m_Shininess;

	bool depthTest;

public:
	Material(std::string name = "Default", Ref<Shader> shader = Ref<Shader>(), MaterialType type = MaterialType::Default);
	Material(uint64_t id, std::string name = "Default", Ref<Shader> shader = Ref<Shader>(), MaterialType type = MaterialType::Default);

	void Use();

	inline uint64_t GetID() const { return m_ID; }
	inline std::string GetName() const { return m_Name; }
	inline Ref<Shader> GetShader() const { return m_Shader; }
	inline glm::vec3 GetColor() const { return m_Color; }
	inline float GetShininess() const { return m_Shininess; }

	inline void SetColor(glm::vec3 color) { m_Color = color; }
	inline void SetShininess(float shininess) { m_Shininess = shininess; }

	friend class MaterialEditorPanel;
};