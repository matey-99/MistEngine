#include "Entity.h"

Entity::Entity(std::string path, std::string name) : Model(path.c_str()), m_Path(path), m_Name(name)
{
	m_Transform = Ref<Transform>();
}

void Entity::Initialize()
{
	m_Transform = CreateRef<Transform>(shared_from_this());
}

void Entity::Update()
{
	if (m_Transform->Parent)
		m_Transform->CalculateModelMatrix(m_Transform->Parent->ModelMatrix);
	else
		m_Transform->CalculateModelMatrix();
}
