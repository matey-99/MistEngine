#pragma once

#include "glm/glm.hpp"
#include "Shader.h"
#include "Texture.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 tangent;
};

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	Mesh(std::vector<Vertex> inVertices, std::vector<unsigned int> inIndices);
	void Render();

private:
	unsigned int VAO, VBO, EBO;

	void SetupMesh();
};