#pragma once

#include <string>
#include <glm/glm.hpp>

#include "typedefs.h"
#include "Shader.h"

class Material
{
private:
	std::string m_Name;
	glm::vec4 m_Color;
	Ref<Shader> m_Shader;

public:
	Material(std::string name = "Default", Ref<Shader> shader = Ref<Shader>());

	void Use();

	inline Ref<Shader> GetShader() const { return m_Shader; }
	inline glm::vec4 GetColor() const { return m_Color; }
	inline void SetColor(glm::vec4 color) { m_Color = color; }
};