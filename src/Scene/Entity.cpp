#include "Entity.h"

#include "Scene.h"

Ref<Entity> Entity::Create(Scene* scene, std::string name)
{
	Ref<Entity> entity = CreateRef<Entity>(scene, name);
	entity->m_Transform = CreateRef<Transform>(entity);

	return entity;
}

Entity::Entity(Scene* scene, std::string name)
	: m_Scene(scene), m_Name(name)
{
	m_Transform = Ref<Transform>();
}

void Entity::Begin()
{
	m_Transform->Begin();

	for (auto component : m_Components)
	{
		component->Begin();
	}
}

void Entity::Update()
{
	m_Transform->Update();

	for (auto component : m_Components)
	{
		component->Update();
	}
}

void Entity::SetEnable(bool enable)
{
	m_Enable = enable;
	//if (!m_Transform->Children.empty())
	//{
	//	for (auto child : m_Transform->Children)
	//		child->GetEntity()->SetEnable(enable);

	//}
}
