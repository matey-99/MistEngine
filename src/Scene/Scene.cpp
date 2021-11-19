#include "Scene.h"
#include "Component/StaticMeshComponent.h"
#include "Component/Light/DirectionalLight.h"
#include "Component/Light/PointLight.h"
#include "Component/Light/SpotLight.h"
#include "Material/MaterialManager.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include "Renderer/Renderer.h"

Scene::Scene()
{
	m_Camera = CreateRef<Camera>(glm::vec3(0.0f, 0.0f, 5.0f));

	m_Root = Ref<Entity>();
	m_Entities = std::vector<Ref<Entity>>();

	m_BackgroundColor = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);

	m_CameraVertexUniformBuffer = CreateRef<UniformBuffer>(sizeof(glm::mat4), 0);
	m_CameraFragmentUniformBuffer = CreateRef<UniformBuffer>(GLSL_VEC3_SIZE, 1);
	m_LightsUniformBuffer = CreateRef<UniformBuffer>(GLSL_SCALAR_SIZE * 2
		+ GLSL_DIRECTIONAL_LIGHT_SIZE
		+ (GLSL_POINT_LIGHT_SIZE * MAX_POINT_LIGHTS)
		+ (GLSL_SPOT_LIGHT_SIZE * MAX_SPOT_LIGHTS), 2);

	std::vector<std::string> faces
	{
		"res/textures/skybox/right.jpg",
		"res/textures/skybox/left.jpg",
		"res/textures/skybox/top.jpg",
		"res/textures/skybox/bottom.jpg",
		"res/textures/skybox/front.jpg",
		"res/textures/skybox/back.jpg"
	};
	m_Skybox = Skybox::CreateFromEquirectangularMap("res/textures/equirectangularMap/equirectangularMap2.hdr");
	m_IrradianceMap = m_Skybox->GetIrradianceMap();
	m_PrefilterMap = m_Skybox->GetPrefilterMap();
	m_BRDFLUT = m_Skybox->GetBRDFLUT();
}

void Scene::Begin()
{
	m_Root->CalculateModelMatrix();

	for (auto entity : m_Entities)
	{
		entity->Begin();
	}
}

void Scene::Update()
{
	m_Camera->Update();

	for (auto entity : m_Entities)
	{
		entity->Update();
	}
}

void Scene::Draw()
{
	m_CameraVertexUniformBuffer->SetUniform(0, sizeof(glm::mat4), glm::value_ptr(m_Camera->GetViewProjectionMatrix()));
	m_CameraFragmentUniformBuffer->SetUniform(0, sizeof(glm::vec3), glm::value_ptr(m_Camera->Position));

	int pointLightsCount = GetComponentsCount<PointLight>();
	int spotLightsCount = GetComponentsCount<SpotLight>();

	m_LightsUniformBuffer->SetUniform(0, GLSL_SCALAR_SIZE, &pointLightsCount);
	m_LightsUniformBuffer->SetUniform(GLSL_SCALAR_SIZE, GLSL_SCALAR_SIZE, &spotLightsCount);

	RenderEntity(GetRoot());

	if (m_IsSkybox)
	{
		glm::mat4 skyboxProjection = m_Camera->GetProjectionMatrix();
		glm::mat4 skyboxView = glm::mat4(glm::mat3(m_Camera->GetViewMatrix()));
		glm::mat4 skyboxViewProjection = skyboxProjection * skyboxView;

		m_Skybox->Render(skyboxView, skyboxProjection);
	}
}

void Scene::RenderEntity(Ref<Entity> entity)
{
	if (!entity->IsEnable())
	{
		if (auto light = entity->GetComponent<Light>())
			light->SwitchOff();

		return;
	}

	if (auto dirLight = entity->GetComponent<DirectionalLight>())
	{
		dirLight->Use();
	}
	if (auto pointLight = entity->GetComponent<PointLight>())
	{
		pointLight->Use();
	}
	if (auto spotLight = entity->GetComponent<SpotLight>())
	{
		spotLight->Use();
	}

	if (auto mesh = entity->GetComponent<StaticMeshComponent>())
	{
		for (auto material : mesh->GetMaterials())
		{
			material->Use();
			glActiveTexture(GL_TEXTURE0 + 20);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_IrradianceMap);
			material->GetShader()->SetInt("u_IrradianceMap", 20);
			glActiveTexture(GL_TEXTURE0 + 21);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_PrefilterMap);
			material->GetShader()->SetInt("u_PrefilterMap", 21);
			glActiveTexture(GL_TEXTURE0 + 22);
			glBindTexture(GL_TEXTURE_2D, m_BRDFLUT);
			material->GetShader()->SetInt("u_BRDFLUT", 22);
			material->GetShader()->SetMat4("u_Model", entity->GetTransform().ModelMatrix);
		}
		mesh->Draw();
	}

	if (!entity->GetChildren().empty())
	{
		for (auto child : entity->GetChildren())
		{
			int pointIndex, spotIndex;
			RenderEntity(CreateRef<Entity>(*child));
		}
	}
}

Ref<Entity> Scene::AddRoot()
{
	Ref<Entity> root = Entity::Create(this, "Root");
	m_Root = root;
	m_Entities.push_back(root);

	return root;
}

Ref<Entity> Scene::AddEntity(std::string name)
{
	Ref<Entity> entity = Entity::Create(this, name);
	entity->SetParent(m_Root.get());
	m_Entities.push_back(entity);

	return entity;
}

Ref<Entity> Scene::AddEntity(uint64_t id, std::string name)
{
	Ref<Entity> entity = Entity::Create(this, id, name);
	Entity* root = m_Root.get();
	entity->SetParent(root);
	m_Entities.push_back(entity);

	return entity;
}

Ref<Entity> Scene::AddEntity(std::string path, std::string name)
{
	Ref<Entity> entity = Entity::Create(this, name);
	entity->SetParent(m_Root.get());
	entity->AddComponent<StaticMeshComponent>(path.c_str());
	m_Entities.push_back(entity);

	return entity;
}

Ref<Entity> Scene::AddEntity(std::string path, std::string name, Ref<Entity> parent)
{
	Ref<Entity> entity = Entity::Create(this, name);
	entity->SetParent(parent.get());
	entity->AddComponent<StaticMeshComponent>(path.c_str());
	m_Entities.push_back(entity);

	return entity;
}

void Scene::RemoveEntity(Ref<Entity> entity)
{
}

Ref<Entity> Scene::FindEntity(std::string name)
{
	for (auto entity : m_Entities)
	{
		if (entity->GetName() == name)
			return entity;
	}

	return Ref<Entity>();
}

Ref<Entity> Scene::FindEntity(uint64_t id)
{
	for (auto entity : m_Entities)
	{
		if (entity->GetID() == id)
			return entity;
	}

	return Ref<Entity>();
}
