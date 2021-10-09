#pragma once

#include "typedefs.h"
#include "Camera.h"
#include "Entity.h"

class Scene
{
private:
	Ref<Camera> m_Camera;
	std::vector<Ref<Entity>> m_Entities;
	Ref<ShaderLibrary> m_ShaderLibrary;

public:
	Scene();
	Scene(Ref<Camera> camera);

	void Update();
	void Draw();

	Ref<Entity> AddEntity(std::string path, std::string name);
	Ref<Entity> AddEntity(std::string path, std::string name, Ref<Transform> parent);
	void RemoveEntity(Ref<Entity> entity);
	Ref<Entity> FindEntity(std::string name);

	inline Ref<Camera> GetCamera() const { return m_Camera; }
	inline std::vector<Ref<Entity>> GetEntities() const { return m_Entities; }
	inline Ref<ShaderLibrary> GetShaderLibrary() const { return m_ShaderLibrary; }
};