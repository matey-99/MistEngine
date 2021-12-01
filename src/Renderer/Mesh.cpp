#include "Mesh.h"

#include <glad/glad.h>

Mesh::Mesh(std::vector<Vertex> inVertices, std::vector<unsigned int> inIndices) 
	: vertices(inVertices), indices(inIndices)
{
	SetupMesh();
}

void Mesh::Render()
{
	//unsigned int diffuseNr = 1;
	//unsigned int specularNr = 1;
	//unsigned int normalNr = 1;
	//for (unsigned int i = 0; i < textures.size(); i++)
	//{
	//	glActiveTexture(GL_TEXTURE0 + i);

	//	std::string number;
	//	std::string name = textures[i].type;
	//	if (name == "diffuse")
	//		number = std::to_string(diffuseNr++);
	//	else if (name == "specular")
	//		number = std::to_string(specularNr++);
	//	else if (name == "normal")
	//		number = std::to_string(normalNr++);

	//	shader->SetInt(("u_Material." + name).c_str(), i);
	//	glBindTexture(GL_TEXTURE_2D, textures[i].m_ID);
	//}
	//glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// Vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	// Vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

	glBindVertexArray(0);
}


