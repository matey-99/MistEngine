#pragma once

#include "typedefs.h"
#include "Camera.h"
#include "Entity.h"
#include "Material/ShaderLibrary.h"
#include "Renderer/UniformBuffer.h"
#include "Renderer/Framebuffer.h"
#include "Skybox.h"

class Scene
{
private:
	Ref<Camera> m_Camera;
	Ref<Entity> m_Root;
	std::vector<Ref<Entity>> m_Entities;
	Ref<Skybox> m_Skybox;
	glm::vec4 m_BackgroundColor;

	Ref<UniformBuffer> m_CameraVertexUniformBuffer;
	Ref<UniformBuffer> m_CameraFragmentUniformBuffer;
	Ref<UniformBuffer> m_LightsUniformBuffer;

	unsigned int m_IrradianceMap;

	bool m_IsSkybox = true;

public:
	Scene();

	void Begin();
	void Update();
	void Draw();

	void RenderEntity(Ref<Entity> entity);

	Ref<Entity> AddRoot();
	Ref<Entity> AddEntity(std::string name);
	Ref<Entity> AddEntity(std::string path, std::string name);
	Ref<Entity> AddEntity(std::string path, std::string name, Ref<Transform> parent);

	void RemoveEntity(Ref<Entity> entity);
	Ref<Entity> FindEntity(std::string name);
	Ref<Entity> FindEntity(uint64_t transformID);
	Ref<Transform> FindTransform(uint64_t id);

	template<typename T>
	int GetComponentsCount()
	{
		int count = 0;
		for (auto entity : m_Entities)
		{
			if (entity->GetComponent<T>())
				count++;
		}

		return count;
	}

	inline Ref<Camera> GetCamera() const { return m_Camera; }
	inline Ref<Entity> GetRoot() const { return m_Root; }
	inline std::vector<Ref<Entity>> GetEntities() const { return m_Entities; }
	inline glm::vec4* GetBackgroundColor() { return &m_BackgroundColor; }
	inline bool IsSkybox() { return m_IsSkybox; }

	inline void RenderSkybox(bool render) { m_IsSkybox = true; }

	friend class SceneSerializer;
	friend class WorldSettingsPanel;
	friend class EntityDetailsPanel;
};