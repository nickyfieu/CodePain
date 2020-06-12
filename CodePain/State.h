#pragma once
#include "GameObject.h"

namespace cp
{
	
	struct State abstract
	{
		State() = default;
		virtual ~State() = default;

		State& operator=(const State&) = delete;
		State& operator=(State&&) = delete;
		State(const State&) = delete;
		State(State&&) = delete;

		// if bool returns true means update will delete current state and replace it with newState
		virtual bool UpdateState(const GameObject* gameObject) = 0;
	};
}