#include "Transform.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Entity.h"

Transform::Transform(Ref<Entity> entity, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, Ref<Transform> parent)
	: m_Entity(entity), LocalPosition(position), LocalRotation(rotation), LocalScale(scale), Parent(parent)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution distribution(1, 999999999);

	ID = distribution(gen);
}

Transform::Transform(Ref<Entity> entity, uint64_t id, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, Ref<Transform> parent)
	: m_Entity(entity), ID(id), LocalPosition(position), LocalRotation(rotation), LocalScale(scale), Parent(parent)
{
}

void Transform::Begin()
{
}

void Transform::Update()
{
	if (Parent)
		CalculateModelMatrix(Parent->ModelMatrix);
	else
		CalculateModelMatrix();
}

glm::vec3 Transform::GetWorldPosition()
{
	return LocalPosition + (Parent ? Parent->GetWorldPosition() : glm::vec3(0.0f));
}

glm::vec3 Transform::GetWorldRotation()
{
	return LocalRotation + (Parent ? Parent->GetWorldRotation() : glm::vec3(0.0f));
}

glm::vec3 Transform::GetWorldScale()
{
	return LocalScale + (Parent ? Parent->GetWorldScale() : glm::vec3(0.0f));

}

void Transform::SetWorldPosition(glm::vec3 position)
{
	LocalPosition = position - (Parent ? Parent->GetWorldPosition() : glm::vec3(0.0f));
}

void Transform::SetWorldRotation(glm::vec3 rotation)
{
	LocalRotation = rotation - (Parent ? Parent->GetWorldRotation() : glm::vec3(0.0f));
}

void Transform::SetWorldScale(glm::vec3 scale)
{
	LocalScale = (Parent ? scale / Parent->GetWorldScale() : scale);
}

void Transform::SetParent(Ref<Transform> parent)
{
	if (Parent)
		Parent->RemoveChild(shared_from_this());

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

unsigned int Transform::FindDepth(Ref<Transform> transform)
{
	if (!transform->Parent)
		return 0;

	return FindDepth(transform->Parent) + 1;
}

void Transform::AddChild(Ref<Transform> child)
{
	Children.emplace_back(child);
}

void Transform::RemoveChild(Ref<Transform> child)
{
	Children.erase(std::remove(Children.begin(), Children.end(), child));
}

Ref<Transform> Transform::GetReference()
{
	return shared_from_this();
}

glm::mat4 Transform::GetLocalModelMatrix()
{
	glm::mat4 rotation = glm::toMat4(glm::quat(glm::radians(LocalRotation)));
	return glm::translate(glm::mat4(1.0f), LocalPosition) * rotation * glm::scale(glm::mat4(1.0f), LocalScale);
}