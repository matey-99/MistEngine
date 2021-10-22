#pragma once

#include "glm/glm.hpp"
#include "Scene/Component/Component.h"
#include "Scene/Component/Transform.h"

class Scene;

class Entity : public std::enable_shared_from_this<Entity>
{
private:
	Scene* m_Scene;

	std::string m_Name;
	Ref<Transform> m_Transform;
	std::vector<Ref<Component>> m_Components;

	bool m_Enable = true;

public:
	static Ref<Entity> Create(Scene* scene, std::string name);

	Entity(Scene* scene, std::string name);

	void Begin();
	void Update();

	template<typename T, typename ... Args>
	Ref<T> AddComponent(Args&& ... args)
	{
		Ref<T> comp = CreateRef<T>(std::forward<Args>(args)...);
		m_Components.push_back(comp);

		return comp;
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

	template<typename T>
	void RemoveComponent()
	{
		for (auto component : m_Components)
		{
			if (std::dynamic_pointer_cast<T>(component))
			{
				m_Components.erase(std::remove(m_Components.begin(), m_Components.end(), component), m_Components.end());
			}
		}
	}

	inline Scene* GetScene() const { return m_Scene; }
	inline std::string GetName() const { return m_Name; }
	inline Ref<Transform> GetTransform() const { return m_Transform; }
	inline bool IsEnable() const { return m_Enable; }

	void SetEnable(bool enable);

	friend class SceneHierarchyPanel;
	friend class EntityDetailsPanel;
};
