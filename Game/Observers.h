#pragma once
#include "Observer.h"

namespace cp
{
	class GameObject;
}

namespace Game
{

	struct IdleEvent final : public cp::Observer
	{
		IdleEvent() = default;
		virtual ~IdleEvent() = default;

		IdleEvent& operator=(const IdleEvent&) = delete;
		IdleEvent& operator=(IdleEvent&&) = delete;
		IdleEvent(const IdleEvent&) = delete;
		IdleEvent(IdleEvent&&) = delete;

		virtual void OnNotify(const cp::GameObject* entity, cp::Event event) override;
	};

	struct SpawnLevelEnemies final : public cp::Observer
	{
		SpawnLevelEnemies() = default;
		virtual ~SpawnLevelEnemies() = default;

		SpawnLevelEnemies& operator=(const SpawnLevelEnemies&) = delete;
		SpawnLevelEnemies& operator=(SpawnLevelEnemies&&) = delete;
		SpawnLevelEnemies(const SpawnLevelEnemies&) = delete;
		SpawnLevelEnemies(SpawnLevelEnemies&&) = delete;

		virtual void OnNotify(const cp::GameObject* entity, cp::Event event) override;
	};

	struct BottomLevelCollision final : public cp::Observer
	{
		BottomLevelCollision() = default;
		virtual ~BottomLevelCollision() = default;

		BottomLevelCollision& operator=(const BottomLevelCollision&) = delete;
		BottomLevelCollision& operator=(BottomLevelCollision&&) = delete;
		BottomLevelCollision(const BottomLevelCollision&) = delete;
		BottomLevelCollision(BottomLevelCollision&&) = delete;

		virtual void OnNotify(const cp::GameObject* entity, cp::Event event) override;
	};

}

