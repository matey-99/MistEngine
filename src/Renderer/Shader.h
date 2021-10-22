#pragma once

#include <glm/glm.hpp>

class Shader
{
public:
	unsigned int id;

private:
	std::string m_Name;

public:
	Shader(std::string name, const char* vertexPath, const char* fragmentPath);
	~Shader();
	void Use() const;

	inline std::string GetName() const { return m_Name; }

	// uniforms
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetVec3(const std::string& name, glm::vec3& vec) const;
	void SetVec4(const std::string& name, glm::vec4& vec) const;
	void SetMat4(const std::string& name, glm::mat4& mat) const;
	
private:
	unsigned int CompileShader(unsigned int type, const char* source);
	void PrintUniforms();
};