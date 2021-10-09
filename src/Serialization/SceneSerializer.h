#pragma once

#include "Scene.h"

class SceneSerializer
{
public:
	void Serialize(Ref<Scene> scene);
	Ref<Scene> Deserialize(std::string path);

private:
	std::string GetData(std::string line);
};