#pragma once

#include "glm/glm.hpp"
#include "Model.h"
#include "Transform.h"

class Entity : public Model, public std::enable_shared_from_this<Entity>
{
private:
	std::string m_Path;
	std::string m_Name;
	Ref<Transform> m_Transform;

public:
	Entity(std::string path, Ref<Material> material, std::string name);
	void Initialize();

	void Update();

	inline std::string GetPath() const { return m_Path; }
	inline std::string GetName() const { return m_Name; }
	inline Ref<Transform> GetTransform() const { return m_Transform; }
};
