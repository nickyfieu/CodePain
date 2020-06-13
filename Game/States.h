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

	struct ZenChanIdleState final : public cp::State
	{
		ZenChanIdleState() = default;
		virtual ~ZenChanIdleState() = default;

		ZenChanIdleState& operator=(const ZenChanIdleState&) = delete;
		ZenChanIdleState& operator=(ZenChanIdleState&&) = delete;
		ZenChanIdleState(const ZenChanIdleState&) = delete;
		ZenChanIdleState(ZenChanIdleState&&) = delete;

		virtual bool UpdateState(const cp::GameObject* gameObject) override;
	private:
		unsigned int m_LeftAnimName = std::hash<std::string>{}("WalkLeft");
		unsigned int m_RightAnimName = std::hash<std::string>{}("WalkRight");
	};

	struct ZenChanBubbleState final : public cp::State
	{
		ZenChanBubbleState() = default;
		virtual ~ZenChanBubbleState() = default;

		ZenChanBubbleState& operator=(const ZenChanBubbleState&) = delete;
		ZenChanBubbleState& operator=(ZenChanBubbleState&&) = delete;
		ZenChanBubbleState(const ZenChanBubbleState&) = delete;
		ZenChanBubbleState(ZenChanBubbleState&&) = delete;

		virtual bool UpdateState(const cp::GameObject* gameObject) override;
	private:
		unsigned int m_LeftAnimName = std::hash<std::string>{}("BubbleLeft");
		unsigned int m_RightAnimName = std::hash<std::string>{}("BubbleRight");
	};

	struct ZenChanDeathState final : public cp::State
	{
		ZenChanDeathState() = default;
		virtual ~ZenChanDeathState() = default;

		ZenChanDeathState& operator=(const ZenChanDeathState&) = delete;
		ZenChanDeathState& operator=(ZenChanDeathState&&) = delete;
		ZenChanDeathState(const ZenChanDeathState&) = delete;
		ZenChanDeathState(ZenChanDeathState&&) = delete;

		virtual bool UpdateState(const cp::GameObject* gameObject) override;
	private:
		unsigned int m_LeftAnimName = std::hash<std::string>{}("DieLeft");
		unsigned int m_RightAnimName = std::hash<std::string>{}("DieRight");
	};

}


