#pragma once
#include "Observer.h"

namespace cp
{
	class GameObject;
}

namespace Game
{
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

	struct PlayerScoreChange final : public cp::Observer
	{
		PlayerScoreChange() = default;
		virtual ~PlayerScoreChange() = default;

		PlayerScoreChange& operator=(const PlayerScoreChange&) = delete;
		PlayerScoreChange& operator=(PlayerScoreChange&&) = delete;
		PlayerScoreChange(const PlayerScoreChange&) = delete;
		PlayerScoreChange(PlayerScoreChange&&) = delete;

		virtual void OnNotify(const cp::GameObject* entity, cp::Event event) override;
	};

	struct PlayerOverlap final : public cp::Observer
	{
		PlayerOverlap() = default;
		virtual ~PlayerOverlap() = default;

		PlayerOverlap& operator=(const PlayerOverlap&) = delete;
		PlayerOverlap& operator=(PlayerOverlap&&) = delete;
		PlayerOverlap(const PlayerOverlap&) = delete;
		PlayerOverlap(PlayerOverlap&&) = delete;

		virtual void OnNotify(const cp::GameObject* entity, cp::Event event) override;
	};

	struct ProceedToNextLevel final : public cp::Observer
	{
		ProceedToNextLevel() = default;
		virtual ~ProceedToNextLevel() = default;

		ProceedToNextLevel& operator=(const ProceedToNextLevel&) = delete;
		ProceedToNextLevel& operator=(ProceedToNextLevel&&) = delete;
		ProceedToNextLevel(const ProceedToNextLevel&) = delete;
		ProceedToNextLevel(ProceedToNextLevel&&) = delete;

		virtual void OnNotify(const cp::GameObject * entity, cp::Event event) override;
	};

}

