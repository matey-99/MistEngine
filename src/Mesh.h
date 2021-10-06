#pragma once

#include "Vertex.h"
#include "Texture.h"
#include "Shader.h"

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> inVertices, std::vector<unsigned int> inIndices, std::vector<Texture> inTextures);
	void Draw(Shader& shader);

private:
	unsigned int VAO, VBO, EBO;

	void SetupMesh();
};