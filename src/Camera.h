#pragma once

#include "glm/glm.hpp"

enum class CameraMovement
{
	Forward, Backward, Left, Right
};

class Camera
{
public:
	glm::vec3 Position;

	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;

	float Yaw;
	float Pitch;

	float MovementSpeed;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f),
		   glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f, float movementSpeed = 1.0f);

	void Update();

	void Move(CameraMovement movementDirection, float deltaTime);
	void Move(float xoffset, float yoffset, float deltaTime);
	void Move(float yoffset, float deltaTime);

	void Rotate(float yaw, float pitch);

private:
	glm::vec3 CalculateFrontVector();
	glm::vec3 CalculateRightVector();
};

