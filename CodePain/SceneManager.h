#pragma once
#include "Singleton.h"
#include <vector>
#include <string>

namespace cp
{
	class Scene;
	class InputHandler;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene* CreateScene(const std::string& name);

		virtual ~SceneManager();

		SceneManager(const SceneManager& other) = delete;
		SceneManager(SceneManager&& other) = delete;
		SceneManager& operator=(const SceneManager& other) = delete;
		SceneManager& operator=(SceneManager&& other) = delete;

		void HandleInput(const cp::InputHandler& inputHandler);
		void Update(float elapsedSec);
		void FixedUpdate(float elapsedSec);
		void Render() const;

		inline Scene* GetActiveScene() const { return m_pActiveScene; }
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		size_t m_AmountOfScenes;
		Scene* m_pActiveScene;
		std::vector<Scene*> m_Scenes;
	public:

		bool SetActiveScene(const std::string& name);
	};
}
