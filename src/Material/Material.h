#pragma once

#include <string>
#include <glm/glm.hpp>

#include "typedefs.h"
#include "Renderer/Shader.h"

template <typename T>
struct MaterialParameter
{
public:
	std::string name;
	T value;
};

struct MaterialFloatParameter : public MaterialParameter<float>
{
};

struct MaterialVectorParameter : public MaterialParameter<glm::vec3>
{
};

struct MaterialTextureParameter : public MaterialParameter<glm::vec3>
{
};

class Material
{
private:
	uint64_t m_ID;
	std::string m_Name;
	Ref<Shader> m_Shader;

	glm::vec3 m_Color;
	float m_Shininess;

public:
	Material(std::string name = "Default", Ref<Shader> shader = Ref<Shader>());
	Material(uint64_t id, std::string name = "Default", Ref<Shader> shader = Ref<Shader>());

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