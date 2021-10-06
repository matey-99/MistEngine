#pragma once

#include "Renderer.h"
#include <glm/glm.hpp>

class Shader
{
public:
	unsigned int id;

	Shader(const char* vertexPath, const char* fragmentPath);
	void Use() const;

	// uniforms
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetMat4(const std::string& name, glm::mat4& mat) const;
	
private:
	unsigned int CompileShader(unsigned int type, const char* source);
};