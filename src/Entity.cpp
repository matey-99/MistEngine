#include "Entity.h"

Entity::Entity(std::string name) : m_Name(name)
{
}

void Entity::Begin()
{
	m_Transform = CreateRef<Transform>(shared_from_this());
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
