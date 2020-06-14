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

	class StartP1Game final : public cp::Command
	{
	public:
		virtual void Execute(const cp::GameObject* actor) override;
	};

	class StartP2Game final : public cp::Command
	{
	public:
		virtual void Execute(const cp::GameObject* actor) override;
	};

	class Start1v1Game final : public cp::Command
	{
	public:
		virtual void Execute(const cp::GameObject* actor) override;
	};


	class ControllerJumpCommand final : public cp::Command
	{
	public:
		virtual void Execute(const cp::GameObject* actor) override;
	};

	class ControllerHorizontalCommand final : public cp::Command
	{
	public:
		virtual void Execute(const cp::GameObject* actor) override;
	};

	class ControllerFIreCommand final : public cp::Command
	{
	public:
		virtual void Execute(const cp::GameObject* actor) override;
	};
}