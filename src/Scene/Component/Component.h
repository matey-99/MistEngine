#pragma once

#include <string>
#include <glm/glm.hpp>

#include "typedefs.h"

class Component
{
public:
	virtual void Begin() = 0;
	virtual void Update() = 0;
};