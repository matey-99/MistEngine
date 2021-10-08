#pragma once

#include "typedefs.h"
#include "Camera.h"
#include "Entity.h"

class Scene
{
private:
	Ref<Camera> m_Camera;
	std::vector<Ref<Entity>> m_Entities;

public:
	Scene();
	Scene(Ref<Camera> camera);

	void Update();
	void Draw(Shader& shader);

	Ref<Entity> AddEntity(const char* path);
	Ref<Entity> AddEntity(const char* path, Ref<Transform> parent);
	void RemoveEntity(Ref<Entity> entity);

	inline Ref<Camera> GetCamera() const { return m_Camera; }
	inline std::vector<Ref<Entity>> GetEntities() const { return m_Entities; }
};