#pragma once
#include "Singleton.h"

namespace cp
{
	class GameObject;
	class GameManager final : public Singleton<GameManager>
	{
	public:
		virtual ~GameManager();

		GameManager(const GameManager& other) = delete;
		GameManager(GameManager&& other) = delete;
		GameManager& operator=(const GameManager& other) = delete;
		GameManager& operator=(GameManager&& other) = delete;

		void Initialize();

		inline size_t GetCurrentLevel() const { return m_CurrentLevel; }
		inline GameObject* GetManagerObj() const { return m_ManagerObj; }
		
		void SetCurrentLevel(size_t currentLevel);

	private:
		friend class Singleton<GameManager>;
		GameManager() = default;

		GameObject* m_ManagerObj;
		size_t m_CurrentLevel;
	};
}