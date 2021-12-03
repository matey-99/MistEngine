#include "StaticMeshComponent.h"

#include "Importer/MeshImporter.h"
#include "Importer/MaterialImporter.h"

#include "Scene/Entity.h"
#include "Scene/Scene.h"

#include <glad/glad.h>

StaticMeshComponent::StaticMeshComponent(Entity* owner)
	: StaticMeshComponent(owner, "../../res/models/defaults/default_cube.obj")
{
}

StaticMeshComponent::StaticMeshComponent(Entity* owner, std::string path)
	: RenderComponent(owner), m_Path(path)
{
	LoadMesh(path);

	for (int i = 0; i < m_Meshes.size(); i++)
		LoadMaterial("../../res/materials/Default.mat");
}

StaticMeshComponent::StaticMeshComponent(Entity* owner, std::string path, std::vector<std::string> materialsPaths)
	: RenderComponent(owner), m_Path(path), m_MaterialsPaths(materialsPaths)
{
	LoadMesh(path);

	for (auto path : m_MaterialsPaths)
		m_Materials.push_back(MaterialImporter::GetInstance()->ImportMaterial(path));

}

void StaticMeshComponent::Begin()
{

}

void StaticMeshComponent::Update()
{

}

void StaticMeshComponent::Render(ViewMode viewMode)
{
	switch (viewMode)
	{
	case ViewMode::Lit:
	{
		for (auto material : GetMaterials())
		{
			material->Use();
			glActiveTexture(GL_TEXTURE0 + 20);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_Owner->GetScene()->m_IrradianceMap);
			material->GetShader()->SetInt("u_IrradianceMap", 20);
			glActiveTexture(GL_TEXTURE0 + 21);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_Owner->GetScene()->m_PrefilterMap);
			material->GetShader()->SetInt("u_PrefilterMap", 21);
			glActiveTexture(GL_TEXTURE0 + 22);
			glBindTexture(GL_TEXTURE_2D, m_Owner->GetScene()->m_BRDFLUT);
			material->GetShader()->SetInt("u_BRDFLUT", 22);
			glActiveTexture(GL_TEXTURE0 + 23);
			glBindTexture(GL_TEXTURE_2D, Renderer::GetInstance()->GetShadowMapFramebuffer()->GetDepthAttachment());
			material->GetShader()->SetInt("u_ShadowMap", 23);

			material->GetShader()->SetMat4("u_LightSpaceMatrix", m_Owner->GetScene()->m_LightSpace);
			material->GetShader()->SetMat4("u_Model", m_Owner->GetTransform().ModelMatrix);
		}
		if (!m_MultipleMaterials && m_Materials.at(0))
		{
			m_Materials.at(0)->Use();

			for (auto mesh : m_Meshes)
			{
				mesh.Render();
			}

			return;
		}

		for (int i = 0; i < m_Meshes.size(); i++)
		{
			if (m_Materials.size() > i)
				m_Materials.at(i)->Use();

			m_Meshes.at(i).Render();
		}

		break;
	}
	case ViewMode::Wireframe:
	{
		break;
	}
	case ViewMode::SceneDepth:
	{
		auto depthShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::CALCULATION, "SceneDepth");
		depthShader->Use();
		depthShader->SetMat4("u_LightSpace", m_Owner->GetScene()->m_LightSpace);
		//depthShader->SetMat4("u_LightSpace", m_Owner->GetScene()->GetCamera()->GetViewProjectionMatrix());
		depthShader->SetMat4("u_Model", m_Owner->GetTransform().ModelMatrix);

		for (auto mesh : m_Meshes)
			mesh.Render();

		break;
	}
	}
}

void StaticMeshComponent::Destroy()
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