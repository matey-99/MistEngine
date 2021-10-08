#include "Entity.h"

Entity::Entity(const char* path) : Model(path)
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
