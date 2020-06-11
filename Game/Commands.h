#pragma once
#include "InputCommand.h"

namespace cp
{
	class GameObject;
}

namespace Game
{
	class JumpCommand final : public cp::Command
	{
	public:
		virtual void Execute(const cp::GameObject* actor) override;
	};

	class LeftCommand final : public cp::Command
	{
	public:
		virtual void Execute(const cp::GameObject* actor) override;
	};

	class RightCommand final : public cp::Command
	{
	public:
		virtual void Execute(const cp::GameObject* actor) override;
	};

	class FireCommand final : public cp::Command
	{
	public:
		virtual void Execute(const cp::GameObject* actor) override;
	};

}