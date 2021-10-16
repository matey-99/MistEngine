#include "Scene.h"
#include "Model.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "MaterialManager.h"

Scene::Scene()
{
	m_Camera = CreateRef<Camera>(glm::vec3(0.0f, 0.0f, 5.0f));

	m_Root = Ref<Entity>();
	m_Entities = std::vector<Ref<Entity>>();
}

Scene::Scene(Ref<Camera> camera) : m_Camera(camera)
{
	m_Root = Ref<Entity>();
	m_Entities = std::vector<Ref<Entity>>();
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
	for (auto entity : m_Entities)
	{
		entity->Update();
	}
}

void Scene::Draw()
{
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
	for (auto shader : shaderLibrary->GetAllShaders())
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
			model->GetMaterial()->Use();

			Ref<Shader> sh = model->GetMaterial()->GetShader();

			glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
			sh->SetMat4("u_Projection", projection);

			glm::mat4 view = glm::lookAt(m_Camera->Position, m_Camera->Position + m_Camera->Front, m_Camera->Up);
			sh->SetMat4("u_View", view);

			sh->SetMat4("u_Model", entity->GetTransform()->ModelMatrix);
			model->Draw(sh);
		}
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
