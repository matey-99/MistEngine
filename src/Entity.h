#pragma once

#include "glm/glm.hpp"
#include "Model.h"
#include "Transform.h"

class Entity : public Model, public std::enable_shared_from_this<Entity>
{
private:
	std::string m_Name;
	Ref<Transform> m_Transform;

public:
	Entity(const char* path, std::string name);
	void Initialize();

	void Update();

	inline std::string GetName() const { return m_Name; }
	inline Ref<Transform> GetTransform() const { return m_Transform; }
};
