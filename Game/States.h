#pragma once
#include "State.h"
#include <string>

namespace cp
{
	class GameObject;
}

namespace Game
{

	struct IdleState final : public cp::State
	{
		IdleState() = default;
		virtual ~IdleState() = default;

		IdleState& operator=(const IdleState&) = delete;
		IdleState& operator=(IdleState&&) = delete;
		IdleState(const IdleState&) = delete;
		IdleState(IdleState&&) = delete;

		virtual bool UpdateState(const cp::GameObject* gameObject) override;
	};

	// moves left right and (up at random)
	struct EnemyMovingState1 final : public cp::State
	{
		EnemyMovingState1() = default;
		virtual ~EnemyMovingState1() = default;

		EnemyMovingState1& operator=(const EnemyMovingState1&) = delete;
		EnemyMovingState1& operator=(EnemyMovingState1&&) = delete;
		EnemyMovingState1(const EnemyMovingState1&) = delete;
		EnemyMovingState1(EnemyMovingState1&&) = delete;

		virtual bool UpdateState(const cp::GameObject* gameObject) override;
	private:
		unsigned int m_LeftAnimName = (unsigned int)std::hash<std::string>{}("WalkLeft");
		unsigned int m_RightAnimName = (unsigned int)std::hash<std::string>{}("WalkRight");
	};

	// only moves left right
	struct EnemyMovingState2 final : public cp::State
	{
		EnemyMovingState2() = default;
		virtual ~EnemyMovingState2() = default;

		EnemyMovingState2& operator=(const EnemyMovingState2&) = delete;
		EnemyMovingState2& operator=(EnemyMovingState2&&) = delete;
		EnemyMovingState2(const EnemyMovingState2&) = delete;
		EnemyMovingState2(EnemyMovingState2&&) = delete;

		virtual bool UpdateState(const cp::GameObject* gameObject) override;
	private:
		unsigned int m_LeftAnimName = (unsigned int)std::hash<std::string>{}("WalkLeft");
		unsigned int m_RightAnimName = (unsigned int)std::hash<std::string>{}("WalkRight");
	};

}


