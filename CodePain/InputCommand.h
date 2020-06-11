#pragma once
#include "GameObject.h"

namespace cp
{
	class Command abstract
	{
	public:
		Command() = default;
		virtual ~Command() = default;

		Command(const Command& other) = delete;
		Command(Command&& other) = delete;
		Command& operator=(const Command& other) = delete;
		Command& operator=(Command&& other) = delete;

		virtual void Execute(const GameObject* actor) = 0;
	};
}