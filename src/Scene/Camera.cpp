#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float yaw, float pitch, float movementSpeed)
	: Position(position), Front(front), Up(up), Yaw(yaw), Pitch(pitch), MovementSpeed(movementSpeed)
{
	Right = CalculateRightVector();

	FieldOfView = 45.0f;
	AspectRactio = glm::vec2(16.0f, 9.0f);
	Near = 0.1f;
	Far = 100.0f;
}

void Camera::Update()
{
	Front = CalculateFrontVector();
	Right = CalculateRightVector();
}

void Camera::Move(CameraMovement movementDirection, float deltaTime)
{
	switch (movementDirection)
	{
	case CameraMovement::Forward:
		Position += Front * MovementSpeed * deltaTime;
		break;
	case CameraMovement::Backward:
		Position -= Front * MovementSpeed * deltaTime;
		break;
	case CameraMovement::Left:
		Position -= Right * MovementSpeed * deltaTime;
		break;
	case CameraMovement::Right:
		Position += Right * MovementSpeed * deltaTime;
		break;
	}
}

void Camera::Move(float xoffset, float yoffset, float deltaTime)
{
	Position += xoffset * MovementSpeed * Right * deltaTime;
	Position += yoffset * MovementSpeed * Up * deltaTime;
}

void Camera::Move(float yoffset, float deltaTime)
{
	Position += (float)yoffset * 3 * MovementSpeed * deltaTime * Front;
}

void Camera::Rotate(float yaw, float pitch)
{
	Yaw += yaw;
	Pitch += pitch;

	Yaw = glm::mod(Yaw, 360.0f);
	Pitch = glm::clamp(Pitch, -89.0f, 89.0f);
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::GetProjectionMatrix()
{
	return glm::perspective(glm::radians(FieldOfView), AspectRactio.x / AspectRactio.y, Near, Far);
}

glm::mat4 Camera::GetViewProjectionMatrix()
{
	return GetProjectionMatrix() * GetViewMatrix();
}

glm::vec3 Camera::CalculateFrontVector()
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	direction.y = sin(glm::radians(Pitch));
	direction.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	return glm::normalize(direction);
}

glm::vec3 Camera::CalculateRightVector()
{
	return glm::normalize(glm::cross(Front, Up));
}
