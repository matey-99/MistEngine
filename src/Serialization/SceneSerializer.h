#pragma once

#include "Scene.h"

#include <yaml-cpp/yaml.h>

class SceneSerializer
{
public:
	void Serialize(Ref<Scene> scene);
	Ref<Scene> Deserialize(std::string path);

private:
	void SerializeEntity(YAML::Emitter& out, Ref<Entity> entity);
};