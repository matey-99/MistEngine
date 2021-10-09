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

	Ref<Entity> AddEntity(std::string path, std::string name);
	Ref<Entity> AddEntity(std::string path, std::string name, Ref<Transform> parent);
	void RemoveEntity(Ref<Entity> entity);
	Ref<Entity> FindEntity(std::string name);

	inline Ref<Camera> GetCamera() const { return m_Camera; }
	inline std::vector<Ref<Entity>> GetEntities() const { return m_Entities; }
};