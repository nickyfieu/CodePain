#pragma once
#include "State.h"
#include <string>

namespace cp
{
	class GameObject;
}

namespace Game
{

	struct PlayerMovingState final : public cp::State
	{
		PlayerMovingState() = default;
		virtual ~PlayerMovingState() = default;

		PlayerMovingState& operator=(const PlayerMovingState&) = delete;
		PlayerMovingState& operator=(PlayerMovingState&&) = delete;
		PlayerMovingState(const PlayerMovingState&) = delete;
		PlayerMovingState(PlayerMovingState&&) = delete;

		virtual bool UpdateState(const cp::GameObject * gameObject) override;
	private:
		unsigned int m_LeftAnimName = (unsigned int)std::hash<std::string>{}("WalkLeft");
		unsigned int m_RightAnimName = (unsigned int)std::hash<std::string>{}("WalkRight");
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

	// only moves left right up constantly
	struct EnemyMovingState3 final : public cp::State
	{
		EnemyMovingState3() = default;
		virtual ~EnemyMovingState3() = default;

		EnemyMovingState3& operator=(const EnemyMovingState3&) = delete;
		EnemyMovingState3& operator=(EnemyMovingState3&&) = delete;
		EnemyMovingState3(const EnemyMovingState3&) = delete;
		EnemyMovingState3(EnemyMovingState3&&) = delete;

		virtual bool UpdateState(const cp::GameObject* gameObject) override;
	private:
		unsigned int m_LeftAnimName = (unsigned int)std::hash<std::string>{}("WalkLeft");
		unsigned int m_RightAnimName = (unsigned int)std::hash<std::string>{}("WalkRight");

		bool m_HasHitGround = false;
	};

	// flying enemies moves left right up down  at a speed and have a bool move up / move down
	struct EnemyMovingState4 final : public cp::State
	{
		EnemyMovingState4(bool isMovingDown, float horizontalForce, float verticalForce);
		virtual ~EnemyMovingState4() = default;

		EnemyMovingState4& operator=(const EnemyMovingState4&) = delete;
		EnemyMovingState4& operator=(EnemyMovingState4&&) = delete;
		EnemyMovingState4(const EnemyMovingState4&) = delete;
		EnemyMovingState4(EnemyMovingState4&&) = delete;

		virtual bool UpdateState(const cp::GameObject* gameObject) override;

	private:
		unsigned int m_LeftAnimName = (unsigned int)std::hash<std::string>{}("WalkLeft");
		unsigned int m_RightAnimName = (unsigned int)std::hash<std::string>{}("WalkRight");

		bool m_IsMovingDown = false;
		float m_HorizontalForce = 0.f;
		float m_VerticalForce = 0.f;
	};

}


