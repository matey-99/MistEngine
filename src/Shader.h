#pragma once

#include "Renderer.h"

class Shader
{
public:
	unsigned int id;

	Shader(const char* vertexPath, const char* fragmentPath);
	void Use() const;
	
private:
	unsigned int CompileShader(unsigned int type, const char* source);
};