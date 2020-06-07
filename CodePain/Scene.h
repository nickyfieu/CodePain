#pragma once
#include "SceneManager.h"
#include "GameObject.h"

namespace cp
{
	class Scene final
	{
		friend Scene* SceneManager::CreateScene(const std::string& name);
	public:
		void Add(GameObject* object);

		void Update(float elapsedSec);
		void Render() const;

		~Scene();

		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		inline std::string GetName() const { return m_Name; }
		void ReserveGameObjects(size_t size);

		size_t GetAmountOfGameObjects() const 
		{
			return m_AmountOfObjects;
		}

		GameObject* GetGameObject(size_t index) const
		{
			return m_pObjects[index];
		}

	private: 
		explicit Scene(const std::string& name);

		size_t m_AmountOfObjects{};
		std::string m_Name{};
		std::vector <GameObject*> m_pObjects{};

	public:

		std::vector<GameObject*> GetAllGameObjectsOfType(GameObjectType type);
	};

}
