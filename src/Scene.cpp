#include "Scene.h"

Scene::Scene()
{
	m_Camera = CreateRef<Camera>(glm::vec3(0.0f, 0.0f, 5.0f));
	m_Entities = std::vector<Ref<Entity>>();
	m_LightSources = std::vector<Ref<LightSource>>();
	m_ShaderLibrary = CreateRef<ShaderLibrary>();
}

Scene::Scene(Ref<Camera> camera) : m_Camera(camera)
{
	m_Entities = std::vector<Ref<Entity>>();
	m_LightSources = std::vector<Ref<LightSource>>();
	m_ShaderLibrary = CreateRef<ShaderLibrary>();
}

void Scene::Update()
{
	for (auto source : m_LightSources)
	{
		source->Update();
	}

	for (auto entity : m_Entities)
	{
		entity->Update();
	}
}

void Scene::Draw()
{
	for (auto source : m_LightSources)
	{
		source->Use();
	}

	for (auto entity : m_Entities)
	{
		entity->GetMaterial()->Use();

		Ref<Shader> sh = entity->GetMaterial()->GetShader();

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		sh->SetMat4("u_Projection", projection);

		glm::mat4 view = glm::lookAt(m_Camera->Position, m_Camera->Position + m_Camera->Front, m_Camera->Up);
		sh->SetMat4("u_View", view);

		sh->SetMat4("u_Model", entity->GetTransform()->ModelMatrix);
		entity->Draw(sh);
	}
}

Ref<Entity> Scene::AddEntity(std::string path, std::string name)
{
	Ref<Entity> entity = CreateRef<Entity>(path, CreateRef<Material>("Default", m_ShaderLibrary->GetShader("Default")), name);
	entity->Initialize();
	m_Entities.push_back(entity);

	return entity;
}

Ref<Entity> Scene::AddEntity(std::string path, std::string name, Ref<Transform> parent)
{
	Ref<Entity> entity = CreateRef<Entity>(path, CreateRef<Material>("Default", m_ShaderLibrary->GetShader("Default")), name);
	entity->Initialize();
	entity->GetTransform()->SetParent(parent);
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

Ref<LightSource> Scene::AddLightSource(std::string name, LightSourceType type)
{
	Ref<LightSource> lightSource = CreateRef<LightSource>(name, type, m_ShaderLibrary->GetShader("LightSource"), m_ShaderLibrary, m_Camera);
	lightSource->Initialize();
	m_LightSources.push_back(lightSource);

	return lightSource;
}

Ref<LightSource> Scene::FindLightSource(std::string name)
{
	for (auto source : m_LightSources)
	{
		if (source->GetName() == name)
			return source;
	}

	return Ref<LightSource>();
}
