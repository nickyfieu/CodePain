#pragma once
#include <list>
#include "BaseComponent.h"

namespace cp
{
	class GameObject final
	{
	public:
		void Update(float elapsedSec);
		void Render() const;

		void AddComponent(BaseComponent* pToAdd);

		GameObject();
		~GameObject();

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		// List is used for deleting components that are in the middle of the list
		std::list<BaseComponent*> m_pComponents;


		template <class T>
		bool HasComponent(ComponentType type)
		{
			return GetComponent<T>(type) != nullptr;
		}

		template <class T>
		T* GetComponent(ComponentType type)
		{
			for (BaseComponent* component : m_pComponents)
			{
				if (component->GetComponentType() == type)
					return static_cast<T*>(component);
			}

			return nullptr;
		}

		template <class T>
		std::vector<T*> GetAllComponentsOfType(ComponentType type)
		{
			std::vector<T*> componentsOfType;
			for (BaseComponent* component : m_pComponents)
			{
				if (component->GetComponentType() == type)
					componentsOfType.push_back(static_cast<T*>(component));
			}

			return componentsOfType;
		}
	};
}
