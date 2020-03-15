#pragma once
#include "SceneManager.h"

namespace cp
{
	class GameObject;
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

	private: 
		explicit Scene(const std::string& name);

		std::string m_Name;
		std::vector <GameObject*> m_pObjects{};

		static unsigned int m_IdCounter; 
	};

}
