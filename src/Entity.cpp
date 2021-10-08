#include "Entity.h"

Entity::Entity(const char* path, std::string name) : Model(path), m_Name(name)
{
	m_Transform = CreateRef<Transform>();
}

void Entity::Update()
{
	if (m_Transform->Parent)
		m_Transform->CalculateModelMatrix(m_Transform->Parent->ModelMatrix);
	else
		m_Transform->CalculateModelMatrix();
}
