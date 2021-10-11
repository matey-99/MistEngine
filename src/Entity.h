#pragma once

#include "glm/glm.hpp"
#include "Component.h"
#include "Transform.h"

class Entity : public std::enable_shared_from_this<Entity>
{
private:
	std::string m_Name;
	Ref<Transform> m_Transform;
	std::vector<Ref<Component>> m_Components;

public:
	Entity(std::string name);

	void Begin();
	void Update();

	template<typename T, typename ... Args>
	void AddComponent(Args&& ... args)
	{
		m_Components.push_back(CreateRef<T>(std::forward<Args>(args)...));
	}

	template<typename T>
	Ref<T> GetComponent() const
	{
		for (auto component : m_Components)
		{
			if (Ref<T> comp = std::dynamic_pointer_cast<T>(component))
				return comp;
		}

		return Ref<T>();
	}

	inline std::string GetName() const { return m_Name; }
	inline Ref<Transform> GetTransform() const { return m_Transform; }
};
