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

}

