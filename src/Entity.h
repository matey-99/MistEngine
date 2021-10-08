#pragma once

#include "glm/glm.hpp"
#include "Model.h"
#include "Transform.h"

class Entity : public Model
{
private:
	Ref<Transform> m_Transform;

	
	
public:
	Entity(const char* path);

	void Update();

	inline Ref<Transform> GetTransform() const { return m_Transform; }
};
