#include "Scene.h"

Scene::Scene()
{
	m_Camera = CreateRef<Camera>(glm::vec3(0.0f, 0.0f, 5.0f));
	m_Entities = std::vector<Ref<Entity>>();
}

Scene::Scene(Ref<Camera> camera) : m_Camera(camera)
{
	m_Entities = std::vector<Ref<Entity>>();
}

void Scene::Update()
{
	for (auto entity : m_Entities)
	{
		entity->Update();
	}
}

void Scene::Draw(Shader& shader)
{
	shader.Use();

	glm::mat4 view = glm::lookAt(m_Camera->Position, m_Camera->Position + m_Camera->Front, m_Camera->Up);
	shader.SetMat4("view", view);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	shader.SetMat4("projection", projection);

	for (auto entity : m_Entities)
	{
		shader.SetMat4("model", entity->GetTransform()->ModelMatrix);
		entity->Draw(shader);
	}
}

Ref<Entity> Scene::AddEntity(const char* path, std::string name)
{
	Ref<Entity> entity = CreateRef<Entity>(path, name);
	m_Entities.push_back(entity);

	return entity;
}

Ref<Entity> Scene::AddEntity(const char* path, std::string name, Ref<Transform> parent)
{
	Ref<Entity> entity = CreateRef<Entity>(path, name);
	entity->GetTransform()->SetParent(parent);
	m_Entities.push_back(entity);

	return entity;
}

void Scene::RemoveEntity(Ref<Entity> entity)
{
}
