#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "typedefs.h"
#include "Component.h"

class Entity;

class Transform : public Component, public std::enable_shared_from_this<Transform>
{
public:
	uint64_t ID;

	glm::vec3 LocalPosition;
	glm::vec3 LocalRotation;
	glm::vec3 LocalScale;

	glm::mat4 ModelMatrix = glm::mat4(1.0f);

	Ref<Transform> Parent;
	std::vector<Ref<Transform>> Children;

public:
	Transform(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), Ref<Transform> parent = Ref<Transform>());

	Transform(uint64_t id, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), Ref<Transform> parent = Ref<Transform>());

	virtual void Begin() override;
	virtual void Update() override;

	glm::vec3 GetWorldPosition();
	glm::vec3 GetWorldRotation();
	glm::vec3 GetWorldScale();

	void SetParent(Ref<Transform> parent);

	void CalculateModelMatrix();
	void CalculateModelMatrix(const glm::mat4& parentGlobalModelMatrix);

	unsigned int FindDepth(Ref<Transform> transform);

	Ref<Transform> GetReference();

private:
	void AddChild(Ref<Transform> child);
	void RemoveChild(Ref<Transform> child);
	glm::mat4 GetLocalModelMatrix();
};