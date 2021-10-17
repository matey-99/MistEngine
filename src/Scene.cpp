#include "Scene.h"
#include "Model.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "MaterialManager.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Scene::Scene()
{
	m_Framebuffer = CreateRef<Framebuffer>(1280, 720);

	m_Camera = CreateRef<Camera>(glm::vec3(0.0f, 0.0f, 5.0f));

	m_Root = Ref<Entity>();
	m_Entities = std::vector<Ref<Entity>>();

	m_BackgroundColor = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);

	m_CameraVertexUniformBuffer = CreateRef<UniformBuffer>(sizeof(glm::mat4), 0);
	m_CameraFragmentUniformBuffer = CreateRef<UniformBuffer>(sizeof(glm::vec3), 1);

	std::vector<std::string> faces
	{
		"res/textures/skybox/right.jpg",
		"res/textures/skybox/left.jpg",
		"res/textures/skybox/top.jpg",
		"res/textures/skybox/bottom.jpg",
		"res/textures/skybox/front.jpg",
		"res/textures/skybox/back.jpg"
	};
	m_Skybox = CreateRef<Skybox>(faces);
}

void Scene::Begin()
{
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

	int pointLightsCount = 0;
	int spotLightsCount = 0;
	for (auto entity : m_Entities)
	{
		if (entity->GetComponent<PointLight>())
		{
			pointLightsCount++;
		}
		if (entity->GetComponent<SpotLight>())
		{
			spotLightsCount++;
		}
	}
	auto shaderLibrary = MaterialManager::GetInstance()->GetShaderLibrary();
	for (auto shader : shaderLibrary->GetAllMaterialShaders())
	{
		shader->Use();
		shader->SetInt("u_PointLightsCount", pointLightsCount);
		shader->SetInt("u_SpotLightsCount", spotLightsCount);
	}

	int pointLightIndex = 0;
	int spotLightIndex = 0;
	for (auto entity : m_Entities)
	{
		if (auto dirLight = entity->GetComponent<DirectionalLight>())
		{
			dirLight->Use(m_Camera->Position);
		}
		if (auto pointLight = entity->GetComponent<PointLight>())
		{
			pointLight->SetIndex(pointLightIndex);
			pointLight->Use(m_Camera->Position);

			pointLightIndex++;
		}
		if (auto spotLight = entity->GetComponent<SpotLight>())
		{
			spotLight->SetIndex(spotLightIndex);
			spotLight->Use(m_Camera->Position);

			spotLightIndex++;
		}

		auto model = entity->GetComponent<Model>();
		if (model)
		{
			auto material = model->GetMaterial();
			material->Use();
			material->GetShader()->SetMat4("u_Model", entity->GetTransform()->ModelMatrix);
			model->Draw();
		}
	}

	if (m_IsSkybox)
	{
		glm::mat4 skyboxProjection = m_Camera->GetProjectionMatrix();
		glm::mat4 skyboxView = glm::mat4(glm::mat3(m_Camera->GetViewMatrix()));
		glm::mat4 skyboxViewProjection = skyboxProjection * skyboxView;

		m_Skybox->Render(skyboxViewProjection);
	}
}

Ref<Entity> Scene::AddRoot()
{
	Ref<Entity> root = CreateRef<Entity>("Root");
	m_Root = root;
	m_Entities.push_back(root);

	return root;
}

Ref<Entity> Scene::AddEntity(std::string name)
{
	Ref<Entity> entity = CreateRef<Entity>(name);
	entity->GetTransform()->SetParent(m_Root->GetTransform());
	m_Entities.push_back(entity);

	return entity;
}

Ref<Entity> Scene::AddEntity(std::string path, std::string name)
{
	Ref<Entity> entity = CreateRef<Entity>(name);
	entity->GetTransform()->SetParent(m_Root->GetTransform());
	entity->AddComponent<Model>(path.c_str());
	m_Entities.push_back(entity);

	return entity;
}

Ref<Entity> Scene::AddEntity(std::string path, std::string name, Ref<Transform> parent)
{
	Ref<Entity> entity = CreateRef<Entity>(name);
	entity->GetTransform()->SetParent(parent);
	entity->AddComponent<Model>(path.c_str());
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

Ref<Entity> Scene::FindEntity(uint64_t transformID)
{
	for (auto entity : m_Entities)
	{
		if (entity->GetTransform()->ID == transformID)
			return entity;
	}

	return Ref<Entity>();
}

Ref<Transform> Scene::FindTransform(uint64_t id)
{
	for (auto entity : m_Entities)
	{
		if (entity->GetTransform()->ID == id)
			return entity->GetTransform();
	}

	return Ref<Transform>();
}
