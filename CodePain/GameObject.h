#pragma once
#include "BaseComponent.h"
#include "Components.h"
#include <vector>
#include <functional>

namespace cp
{
	enum class GameObjectType
	{
		none = 0,
		level = 1,
		Player1 = 2, // takes keyboard / moouse input
		Player2 = 3, // takes controller 1 input
		Player3 = 4, // takes controller 2 input
		Player4 = 5, // takes controller 3 input
		Player5 = 6, // takes controller 4 input
	};

	typedef std::function<void (GameObject* selfObject, GameObject* otherObject, CollisionBox::CollisionSide side)> CollisionCallback;
	
	class GameObject final
	{
	public:

		void Update(float elapsedSec);
		void FixedUpdate(float elapsedSec);
		void Render() const;

		void RemoveComponent(class BaseComponent* pToRemove);
		void AddComponent(class BaseComponent* pToAdd);

		// game object type is to be able to search for a block of types easier
		GameObject(GameObjectType type = GameObjectType::none);
		~GameObject();

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		inline bool GetIsActive() const { return m_IsActive; }
		inline GameObjectType GetType() const { return m_Type; }

		void SetActive(bool active);

		void SetCollisionCallback(CollisionCallback callback);
		// note do not give in functions that delete the original gameobjects in the callback function! (safety)
		void OnCollisionCallback(GameObject* self, GameObject* other, CollisionBox::CollisionSide side);

	private:
		CollisionCallback m_CollisionCallback;

		GameObjectType m_Type;

		bool m_IsActive{ true };
		size_t m_AmountOfComponents{};
		std::vector<BaseComponent*> m_pComponents;

	public:
		template <class T>
		bool HasComponent(ComponentType type) const
		{
			return GetComponent<T>(type) != nullptr;
		}

		template <class T>
		T* GetComponent(ComponentType type) const
		{
			for (BaseComponent* component : m_pComponents)
			{
				if (component->GetComponentType() == type)
					return static_cast<T*>(component);
			}

			return nullptr;
		}

		template <class T>
		std::vector<T*> GetAllComponentsOfType(ComponentType type) const
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
