#pragma once
#include "State.h"

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

}


