#pragma once
#include <list>
#include "BaseComponent.h"
#include "Components.h"

namespace cp
{
	enum class GameObjectType
	{
		// not a specified type ( default constructor )
		none = 0,
		// a level game object
		level = 1,
		// a enemy game object
		enemy = 2,
		// a player game object
		players = 3,
	};

	class GameObject final
	{
	public:
		void Update(float elapsedSec);
		void Render() const;

		void AddComponent(BaseComponent* pToAdd);

		GameObject();
		GameObject(GameObjectType type);
		~GameObject();

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		inline GameObjectType GetType() const { return m_Type; }

		void SetActive(bool active);
		inline bool GetIsActive() const { return m_IsActive; }

	private:
		GameObjectType m_Type;

		bool m_IsActive{ true };

		// List is used for deleting components that are in the middle of the list
		std::list<BaseComponent*> m_pComponents;

	public:
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
