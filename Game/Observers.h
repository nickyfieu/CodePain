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

	struct TopBottomLevelCollision final : public cp::Observer
	{
		TopBottomLevelCollision() = default;
		virtual ~TopBottomLevelCollision() = default;

		TopBottomLevelCollision& operator=(const TopBottomLevelCollision&) = delete;
		TopBottomLevelCollision& operator=(TopBottomLevelCollision&&) = delete;
		TopBottomLevelCollision(const TopBottomLevelCollision&) = delete;
		TopBottomLevelCollision(TopBottomLevelCollision&&) = delete;

		virtual void OnNotify(const cp::GameObject* entity, cp::Event event) override;
	};


	struct PlayerHealthChange final : public cp::Observer
	{
		PlayerHealthChange() = default;
		virtual ~PlayerHealthChange() = default;

		PlayerHealthChange& operator=(const PlayerHealthChange&) = delete;
		PlayerHealthChange& operator=(PlayerHealthChange&&) = delete;
		PlayerHealthChange(const PlayerHealthChange&) = delete;
		PlayerHealthChange(PlayerHealthChange&&) = delete;

		virtual void OnNotify(const cp::GameObject* entity, cp::Event event) override;
	};

}

