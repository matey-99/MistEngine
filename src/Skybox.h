#pragma once

#include <vector>
#include <string>

#include "typedefs.h"
#include "Shader.h"
#include "Framebuffer.h"

class Skybox
{
private:
	uint32_t m_ID;
	uint32_t m_VAO;
	uint32_t m_VBO;

	Ref<Shader> m_Shader;

public:
	Skybox();
	Skybox(std::vector<std::string> faces);

	void LoadCubemap(std::vector<std::string> faces);

	void Render(glm::mat4 viewProjectionMatrix);

	inline uint32_t GetID() const { return m_ID; }

private:
	void SetupMesh();
};