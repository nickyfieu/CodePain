#pragma once
#include "Singleton.h"
#include "AudioLocator.h"

namespace cp
{
	class GameObject;
	class GameManager final : public Singleton<GameManager>
	{
	public:

		enum class GameState
		{
			Menu,
			Game,
		};

		virtual ~GameManager();

		GameManager(const GameManager& other) = delete;
		GameManager(GameManager&& other) = delete;
		GameManager& operator=(const GameManager& other) = delete;
		GameManager& operator=(GameManager&& other) = delete;

		void Initialize();

		inline size_t GetCurrentLevel() const { return m_CurrentLevel; }
		inline GameObject* GetManagerObj() const { return m_ManagerObj; }
		// channel starts from 1
		size_t GetChannelVolume(int channel) const;

		void SetCurrentLevel(size_t currentLevel);
		// volume is a value between 0 - 100
		// channel if -1 means all channels otherwise starts from 1
		void SetChannelVolume(int channel, size_t volume);

		GameState GetState() const { return m_CurrentState; }
		void SetGameState(GameState state) { m_CurrentState = state; }

	private:
		bool IsValidChannel(int channel) const;

		friend class Singleton<GameManager>;
		GameManager() = default;

		GameObject* m_ManagerObj;
		size_t m_CurrentLevel = 1;
		// variable between 0 - 100
		size_t m_ChannelVolume[CHANNELS]{STARTVOLUME,STARTVOLUME};

		GameState m_CurrentState = GameState::Menu;
	};
}