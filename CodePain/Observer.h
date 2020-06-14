#pragma once
#include "GameObject.h"

namespace cp
{
	enum class Event
	{
		EVENT_HP_CHANGE = 0,
		EVENT_GAME_OVER = 1,
		EVENT_RESTART = 2,
		EVENT_MOVE_LEFT = 3,
		EVENT_MOVE_RIGHT = 4,
		EVENT_JUMP = 5,
		EVENT_SHOOT = 6,
		EVENT_COLLISION_COLLIDE = 7,
		EVENT_COLLISION_OVERLAP = 8,
		EVENT_ANIMATION_FINISHED = 9,
		EVENT_IDLE = 10,
		EVENT_SPAWN_ENEMIES = 11,
		// unique for the game manager object
		EVENT_OBJ_DESTROYED = 12,
	};

	struct Observer abstract
	{
		Observer() = default;
		virtual ~Observer() = default;

		Observer& operator=(const Observer&) = delete;
		Observer& operator=(Observer&&) = delete;
		Observer(const Observer&) = delete;
		Observer(Observer&&) = delete;

		// !!!WARNING!!!
		// as on notify is ran on a seperate thread
		// if you run allot of LOGGER::LOG each cycle
		// the chance of IMGUI crashing on you gets bigger!
		// prefer printf / std::cout to debug output in the on notify function
		virtual void OnNotify(const GameObject* entity, Event event) = 0;
	};
}