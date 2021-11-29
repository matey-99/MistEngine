#include "StaticMeshComponent.h"

#include "Importer/MeshImporter.h"
#include "Importer/MaterialImporter.h"

StaticMeshComponent::StaticMeshComponent() 
	: StaticMeshComponent("../../res/models/defaults/default_cube.obj")
{
}

StaticMeshComponent::StaticMeshComponent(std::string path) 
	: m_Path(path)
{
	LoadMesh(path);

	for (int i = 0; i < m_Meshes.size(); i++)
		LoadMaterial("../../res/materials/Default.mat");
}

StaticMeshComponent::StaticMeshComponent(std::string path, std::vector<std::string> materialsPaths) 
	: m_Path(path), m_MaterialsPaths(materialsPaths)
{
	LoadMesh(path);

	for (auto path : m_MaterialsPaths)
		m_Materials.push_back(MaterialImporter::GetInstance()->ImportMaterial(path));

}

void StaticMeshComponent::Draw(bool depthOnly)
{
	if (depthOnly)
	{
		for (int i = 0; i < m_Meshes.size(); i++)
		{
			m_Meshes.at(i).Draw();
		}

		return;
	}

	if (!m_MultipleMaterials && m_Materials.at(0))
	{
		m_Materials.at(0)->Use();

		for (auto mesh : m_Meshes)
		{
			mesh.Draw();
		}

		return;
	}

	for (int i = 0; i < m_Meshes.size(); i++)
	{
		if (m_Materials.size() > i)
			m_Materials.at(i)->Use();

		m_Meshes.at(i).Draw();
	}
}

void StaticMeshComponent::Begin()
{

}

void StaticMeshComponent::Update()
{

}

uint32_t StaticMeshComponent::GetRenderedVerticesCount()
{
	uint32_t vertices = 0;
	for (auto mesh : m_Meshes)
	{
		vertices += mesh.vertices.size();
	}

	return vertices;
}

void StaticMeshComponent::LoadMesh(std::string path)
{
	m_Path = path;
	m_Meshes = MeshImporter::GetInstance()->ImportMesh(path);
}

void StaticMeshComponent::LoadMaterial(std::string path)
{
	m_Materials.push_back(MaterialImporter::GetInstance()->ImportMaterial(path));
	m_MaterialsPaths.push_back(path);
}

void StaticMeshComponent::ChangeMesh(std::string path)
{
	LoadMesh(path);

	m_Materials.clear();
	m_MaterialsPaths.clear();

	for (int i = 0; i < m_Meshes.size(); i++)
		LoadMaterial("../../res/materials/Default.mat");
}

void StaticMeshComponent::ChangeMaterial(int index, std::string path)
{
	m_MaterialsPaths.at(index) = path;
	m_Materials.at(index) = MaterialImporter::GetInstance()->ImportMaterial(path);
}