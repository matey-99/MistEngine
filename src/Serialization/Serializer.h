#pragma once

#include <string>
#include "typedefs.h"

template<typename T>
class Serializer
{
public:
	virtual void Serialize(Ref<T> object) = 0;
	virtual Ref<T> Deserialize(std::string path) = 0;
};