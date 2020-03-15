#pragma once
#include <list>

namespace cp
{
	class BaseComponent;
	class GameObject final
	{
	public:
		void Update(float elapsedSec);
		void Render() const;

		void AddComponent(BaseComponent* pToAdd);

		GameObject();
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		// List is used for deleting components that are in the middle of the list
		std::list<BaseComponent*> m_pComponents;


		template <class T>
		bool HasComponent()
		{
			return GetComponent<T>() != nullptr;
		}

		template <class T>
		T* GetComponent()
		{
			const type_info& typeInfo = typeid(T);

			for (BaseComponent* component : m_pComponents)
			{
				if (component && (typeid(*component) == typeInfo))
					return static_cast<T*>(component);
			}

			return nullptr;
		}
	};
}
