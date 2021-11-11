#pragma once

#include <vector>
#include <string>

#include "typedefs.h"
#include "Renderer/Shader.h"
#include "Renderer/Framebuffer.h"

enum class SkyboxMapType
{
	CUBE_MAP, EQUIRECTANGULAR_MAP
};

class Skybox
{
private:
	uint32_t m_ID;
	uint32_t m_VAO;
	uint32_t m_VBO;
	uint32_t m_IrradianceMap;
	uint32_t m_PrefilterMap;

	Ref<Shader> m_Shader;

public:
	Skybox();
	Skybox(std::vector<std::string> faces);

	static Ref<Skybox> CreateFromFaces(std::vector<std::string> faces);
	static Ref<Skybox> CreateFromEquirectangularMap(std::string path);

	void LoadCubemap(std::vector<std::string> faces);

	void Render(glm::mat4 viewProjectionMatrix);
	void Render(glm::mat4 view, glm::mat4 projection);

	inline uint32_t GetID() const { return m_ID; }
	inline unsigned int GetIrradianceMap() const { return m_IrradianceMap; }

private:
	void SetupMesh();
};