#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "typedefs.h"

class Transform : public std::enable_shared_from_this<Transform>
{
public:
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;

	glm::mat4 ModelMatrix = glm::mat4(1.0f);

	Ref<Transform> Parent;
	std::vector<Ref<Transform>> Children;

public:
	Transform(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), Ref<Transform> parent = Ref<Transform>());

	void SetParent(Ref<Transform> parent);

	void CalculateModelMatrix();
	void CalculateModelMatrix(const glm::mat4& parentGlobalModelMatrix);


	Ref<Transform> GetReference();

private:
	void AddChild(Ref<Transform> child);
	glm::mat4 GetLocalModelMatrix();
};