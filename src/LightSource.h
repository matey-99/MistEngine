#pragma once

#include <string>
#include <glm/glm.hpp>

#include "Transform.h"
#include "Shader.h"
#include "Camera.h"
#include "ShaderLibrary.h"

enum class LightSourceType
{
	Ambient, Diffuse, Specular
};

class LightSource : public std::enable_shared_from_this<LightSource>
{
private:
	std::string m_Name;
	LightSourceType m_Type;
	Ref<Shader> m_Shader;
	Ref<ShaderLibrary> m_ShaderLibrary;
	Ref<Transform> m_Transform;
	Ref<Camera> m_Camera;

	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;

public:
	LightSource(std::string name, LightSourceType type, Ref<Shader> shader, Ref<ShaderLibrary> shaderLibrary, Ref<Camera> camera);

	void Initialize();

	void Update();
	void Use();

	inline std::string GetName() const { return m_Name; }
	inline Ref<Shader> GetShader() const { return m_Shader; }
	inline Ref<Transform> GetTransform() const { return m_Transform; }

	inline void SetAmbient(glm::vec3 ambient) { m_Ambient = ambient; }
	inline void SetDiffuse(glm::vec3 diffuse) { m_Diffuse = diffuse; }
	inline void SetSpecular(glm::vec3 specular) { m_Specular = specular; }
};
