#pragma once

#include "glm/glm.hpp"
#include "Model.h"
#include "Transform.h"

class Entity : public Model
{
private:
	std::string m_Name;
	Ref<Transform> m_Transform;

public:
	Entity(const char* path, std::string name);

	void Update();

	inline std::string GetName() const { return m_Name; }
	inline Ref<Transform> GetTransform() const { return m_Transform; }
};
