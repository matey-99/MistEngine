#include "Transform.h"

Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, Ref<Transform> parent)
	: Position(position), Rotation(rotation), Scale(scale), Parent(parent)
{

}

void Transform::SetParent(Ref<Transform> parent)
{
	Parent = parent;
	Parent->AddChild(GetReference());
}

void Transform::CalculateModelMatrix()
{
	ModelMatrix = GetLocalModelMatrix();
}
void Transform::CalculateModelMatrix(const glm::mat4& parentGlobalModelMatrix)
{
	ModelMatrix = parentGlobalModelMatrix * GetLocalModelMatrix();
}

void Transform::AddChild(Ref<Transform> child)
{
	Children.emplace_back(child);
}

Ref<Transform> Transform::GetReference()
{
	return shared_from_this();
}

glm::mat4 Transform::GetLocalModelMatrix()
{
	const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	const glm::mat4 rotationMatrix = transformY * transformX * transformZ;

	return glm::translate(glm::mat4(1.0f), Position) * rotationMatrix * glm::scale(glm::mat4(1.0f), Scale);
}