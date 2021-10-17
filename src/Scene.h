#pragma once

#include "typedefs.h"
#include "Camera.h"
#include "Entity.h"
#include "ShaderLibrary.h"
#include "UniformBuffer.h"
#include "Framebuffer.h"
#include "Skybox.h"

class Scene
{
private:
	Ref<Framebuffer> m_Framebuffer;

	Ref<Camera> m_Camera;
	Ref<Entity> m_Root;
	std::vector<Ref<Entity>> m_Entities;
	Ref<Skybox> m_Skybox;
	glm::vec4 m_BackgroundColor;

	Ref<UniformBuffer> m_CameraVertexUniformBuffer;
	Ref<UniformBuffer> m_CameraFragmentUniformBuffer;

	bool m_IsSkybox = true;

public:
	Scene();

	void Begin();
	void Update();
	void Draw();

	Ref<Entity> AddRoot();
	Ref<Entity> AddEntity(std::string name);
	Ref<Entity> AddEntity(std::string path, std::string name);
	Ref<Entity> AddEntity(std::string path, std::string name, Ref<Transform> parent);

	void RemoveEntity(Ref<Entity> entity);
	Ref<Entity> FindEntity(std::string name);
	Ref<Entity> FindEntity(uint64_t transformID);
	Ref<Transform> FindTransform(uint64_t id);

	inline Ref<Framebuffer> GetFramebuffer() const { return m_Framebuffer; }
	inline Ref<Camera> GetCamera() const { return m_Camera; }
	inline Ref<Entity> GetRoot() const { return m_Root; }
	inline std::vector<Ref<Entity>> GetEntities() const { return m_Entities; }
	inline glm::vec4* GetBackgroundColor() { return &m_BackgroundColor; }
	inline bool IsSkybox() { return m_IsSkybox; }

	inline void RenderSkybox(bool render) { m_IsSkybox = true; }

	friend class WorldSettingsPanel;
};