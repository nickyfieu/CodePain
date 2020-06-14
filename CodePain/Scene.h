#pragma once
#include "SceneManager.h"
#include "GameObject.h"

namespace cp
{
	class InputHandler;
	class Scene final
	{
		friend Scene* SceneManager::CreateScene(const std::string& name);
	public:
		void Add(GameObject* object);

		void HandleInput(const cp::InputHandler& inputHandler);
		void Update(float elapsedSec);
		void FixedUpdate(float elapsedSec);
		void Render() const;

		~Scene();

		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		void ReserveGameObjects(size_t size);

		inline size_t GetAmountOfGameObjects() const { return m_AmountOfObjects; }
		inline size_t GetNameHash() const { return m_NameHash; }
		inline std::string GetName() const { return m_Name; }
		inline GameObject* GetGameObject(size_t index) const { return m_pObjects[index]; }

	private: 
		explicit Scene(const std::string& name);

		size_t m_AmountOfObjects{};
		size_t m_NameHash{};
		std::string m_Name{};
		std::vector <GameObject*> m_pObjects{};

	public:

		std::vector<GameObject*> GetAllGameObjectsOfType(GameObjectType type);
		void DeleteAllGameObjectsOfType(GameObjectType type);
		void DeleteGameObject(GameObject* ref);
		void SwitchLevel(size_t levelIndex);
	};

}
