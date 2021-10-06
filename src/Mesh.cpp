#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> inVertices, std::vector<unsigned int> inIndices, std::vector<Texture> inTextures) 
	: vertices(inVertices), indices(inIndices), textures(inTextures)
{
	SetupMesh();
}

void Mesh::SetupMesh()
{
}


