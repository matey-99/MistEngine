#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> inVertices, std::vector<unsigned int> inIndices, std::vector<Texture> inTextures) 
	: vertices(inVertices), indices(inIndices), textures(inTextures)
{
	SetupMesh();
}

void Mesh::Draw(Shader& shader)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		GL_CALL(glActiveTexture(GL_TEXTURE0 + i));

		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);

		shader.SetFloat(("material." + name + number).c_str(), i);
		GL_CALL(glBindTexture(GL_TEXTURE_2D, textures[i].id));
	}
	GL_CALL(glActiveTexture(GL_TEXTURE0));

	GL_CALL(glBindVertexArray(VAO));
	GL_CALL(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0));
	GL_CALL(glBindVertexArray(0));
}

void Mesh::SetupMesh()
{
	GL_CALL(glGenVertexArrays(1, &VAO));
	GL_CALL(glGenBuffers(1, &VBO));
	GL_CALL(glGenBuffers(1, &EBO));

	GL_CALL(glBindVertexArray(VAO));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));

	GL_CALL(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW));

	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW));

	// Vertex positions
	GL_CALL(glEnableVertexAttribArray(0));
	GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));

	// Vertex normals
	GL_CALL(glEnableVertexAttribArray(1));
	GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal)));

	// Vertex texture coords
	GL_CALL(glEnableVertexAttribArray(2));
	GL_CALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords)));

	GL_CALL(glBindVertexArray(0));
}


