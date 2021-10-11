#include "Entity.h"

Entity::Entity(std::string name) : m_Name(name)
{
	m_Transform = CreateRef<Transform>();
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
