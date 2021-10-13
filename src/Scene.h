#pragma once

#include "typedefs.h"
#include "Camera.h"
#include "Entity.h"
#include "ShaderLibrary.h"

class Scene
{
private:
	Ref<Camera> m_Camera;
	Ref<Entity> m_Root;
	std::vector<Ref<Entity>> m_Entities;
public:
	Scene();
	Scene(Ref<Camera> camera);

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

	inline Ref<Camera> GetCamera() const { return m_Camera; }
	inline std::vector<Ref<Entity>> GetEntities() const { return m_Entities; }
	inline Ref<Entity> GetRoot() const { return m_Root; }
};