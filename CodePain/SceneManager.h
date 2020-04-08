#pragma once
#include "Singleton.h"
#include <list>

namespace cp
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene* CreateScene(const std::string& name);

		virtual ~SceneManager();

		void Update(float elapsedSec);
		void Render();
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::list<Scene*> m_Scenes;
	};
}
