#include "InstanceRenderedMeshComponent.h"

#include "Importer/MeshImporter.h"
#include "Importer/MaterialImporter.h"

#include "Scene/Entity.h"
#include "Scene/Scene.h"
#include "Light/PointLight.h"
#include "Light/SpotLight.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

InstanceRenderedMeshComponent::InstanceRenderedMeshComponent(Entity* owner)
	: InstanceRenderedMeshComponent(owner, "../../res/models/defaults/default_cube.obj")
{
}

InstanceRenderedMeshComponent::InstanceRenderedMeshComponent(Entity* owner, std::string path)
	: RenderComponent(owner), m_Path(path)
{
	LoadMesh(path);

	for (int i = 0; i < m_Meshes.size(); i++)
		LoadMaterial("../../res/materials/DefaultInstanced.mat");


	srand(glfwGetTime());
	for (uint32_t i = 0; i < m_Amount; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);

		float angle = (float)i / (float)m_Amount * 360.0f;
		float displacement = (rand() % (int)(2 * m_Offset * 100)) / 100.0f - m_Offset;
		float x = sin(angle) * m_Radius + displacement;
		displacement = (rand() % (int)(2 * m_Offset * 100)) / 100.0f - m_Offset;
		float y = displacement * 0.4f;
		displacement = (rand() % (int)(2 * m_Offset * 100)) / 100.0f - m_Offset;
		float z = cos(angle) * m_Radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		float scale = (rand() % 20) / 100.0f + 0.05f;
		model = glm::scale(model, glm::vec3(scale));

		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		m_ModelMatrices.push_back(model);
	}

	uint32_t buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, m_Amount * sizeof(glm::mat4), &m_ModelMatrices[0], GL_STATIC_DRAW);

	for (int i = 0; i < m_Meshes.size(); i++)
	{
		glBindVertexArray(m_Meshes[i].GetVAO());

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);

		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));

		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));

		glEnableVertexAttribArray(7);
		glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glVertexAttribDivisor(7, 1);

		glBindVertexArray(0);
	}
}

InstanceRenderedMeshComponent::InstanceRenderedMeshComponent(Entity* owner, std::string path, std::vector<std::string> materialsPaths)
	: RenderComponent(owner), m_Path(path), m_MaterialsPaths(materialsPaths)
{
	LoadMesh(path);

	for (auto path : m_MaterialsPaths)
		m_Materials.push_back(MaterialImporter::GetInstance()->ImportMaterial(path));

}

void InstanceRenderedMeshComponent::Begin()
{
	
}

void InstanceRenderedMeshComponent::Update()
{

}

void InstanceRenderedMeshComponent::PreRender()
{
}

void InstanceRenderedMeshComponent::Render()
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
		glBindTexture(GL_TEXTURE_2D, Renderer::GetInstance()->GetDirectionalLightShadowMapFramebuffer()->GetDepthAttachment());
		material->GetShader()->SetInt("u_DirectionalLightShadowMap", 23);

		auto pointLights = m_Owner->GetScene()->GetComponents<PointLight>();
		for (int i = 0; i < MAX_POINT_LIGHTS; i++)
		{
			if (i < pointLights.size())
			{
				auto shadowMap = Cast<PointLight>(pointLights[i])->GetShadowMap();

				glActiveTexture(GL_TEXTURE0 + 24 + i);
				glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap);
				material->GetShader()->SetInt("u_PointLightShadowMaps[" + std::to_string(i) + "]", 24 + i);
			}
			else
			{
				glActiveTexture(GL_TEXTURE0 + 24 + i);
				glBindTexture(GL_TEXTURE_CUBE_MAP, m_Owner->GetScene()->m_IrradianceMap);
				material->GetShader()->SetInt("u_PointLightShadowMaps[" + std::to_string(i) + "]", 24 + i);
			}
		}

		auto spotLights = m_Owner->GetScene()->GetComponents<SpotLight>();
		for (int i = 0; i < MAX_SPOT_LIGHTS; i++)
		{
			if (i < spotLights.size())
			{
				auto shadowMap = Cast<SpotLight>(spotLights[i])->GetShadowMap();

				glActiveTexture(GL_TEXTURE0 + 24 + MAX_POINT_LIGHTS + i);
				glBindTexture(GL_TEXTURE_2D, shadowMap);
				material->GetShader()->SetInt("u_SpotLightShadowMaps[" + std::to_string(i) + "]", 24 + MAX_POINT_LIGHTS + i);
			}
			else
			{
				glActiveTexture(GL_TEXTURE0 + 24 + MAX_POINT_LIGHTS + i);
				glBindTexture(GL_TEXTURE_2D, m_Owner->GetScene()->m_BRDFLUT);
				material->GetShader()->SetInt("u_SpotLightShadowMaps[" + std::to_string(i) + "]", 24 + MAX_POINT_LIGHTS + i);
			}
		}

		material->GetShader()->SetMat4("u_Model", m_Owner->GetTransform().ModelMatrix);
	}
	if (!m_MultipleMaterials && m_Materials.at(0))
	{
		m_Materials.at(0)->Use();

		for (auto mesh : m_Meshes)
		{
			mesh.RenderInstanced(m_Amount);
		}

		return;
	}

	for (int i = 0; i < m_Meshes.size(); i++)
	{
		if (m_Materials.size() > i)
			m_Materials.at(i)->Use();

		m_Meshes.at(i).RenderInstanced(m_Amount);
	}

}

void InstanceRenderedMeshComponent::Destroy()
{
}

uint32_t InstanceRenderedMeshComponent::GetRenderedVerticesCount()
{
	uint32_t vertices = 0;
	for (auto mesh : m_Meshes)
	{
		vertices += mesh.vertices.size();
	}

	return vertices;
}

void InstanceRenderedMeshComponent::LoadMesh(std::string path)
{
	m_Path = path;
	m_Meshes = MeshImporter::GetInstance()->ImportMesh(path);
}

void InstanceRenderedMeshComponent::LoadMaterial(std::string path)
{
	m_Materials.push_back(MaterialImporter::GetInstance()->ImportMaterial(path));
	m_MaterialsPaths.push_back(path);
}

void InstanceRenderedMeshComponent::ChangeMesh(std::string path)
{
	LoadMesh(path);

	m_Materials.clear();
	m_MaterialsPaths.clear();

	for (int i = 0; i < m_Meshes.size(); i++)
		LoadMaterial("../../res/materials/Default.mat");
}

void InstanceRenderedMeshComponent::ChangeMaterial(int index, std::string path)
{
	m_MaterialsPaths.at(index) = path;
	m_Materials.at(index) = MaterialImporter::GetInstance()->ImportMaterial(path);
}