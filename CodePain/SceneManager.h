#pragma once
#include "Singleton.h"
#include <list>
#include <string>

namespace cp
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene* CreateScene(const std::string& name);

		virtual ~SceneManager();

		SceneManager(const SceneManager& other) = delete;
		SceneManager(SceneManager&& other) = delete;
		SceneManager& operator=(const SceneManager& other) = delete;
		SceneManager& operator=(SceneManager&& other) = delete;

		void Update(float elapsedSec);
		void Render() const;

		inline Scene* GetActiveScene() const { return m_pActiveScene; }
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		Scene* m_pActiveScene;
		std::list<Scene*> m_Scenes;
	public:

		bool SetActiveScene(const std::string& name);
	};
}
