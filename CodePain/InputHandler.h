#pragma once
#include "Singleton.h"
#include "GameObject.h"
#include "InputCommand.h"
#include "InputManager.h"

namespace cp
{
	enum class InputState : int
	{
		Pressed = 0,
		Hold = 1,
		Released = 2,
	};

	typedef std::pair<InputState, Command*> inputCommand;

	class InputHandler : public Singleton<InputHandler>
	{
	public:
		void Cleanup();

		void HandleInput(const GameObject* actor) const;

		void AddInput(ControllerAxis axie, Command* command);
		void AddInput(ControllerButton button, InputState state, Command* command);
		void AddInput(KeyboardKey key, InputState state, Command* command);
		void AddInput(MouseButton button, InputState state, Command* command);

		inline Command* GetInputCommand(ControllerAxis axie) const { return m_ControllerAxies.at(axie); }
		inline Command* GetInputCommand(ControllerButton button, InputState state) const { return m_Controllers.at(button).at(state); }
		inline Command* GetInputCommand(KeyboardKey key, InputState state) const { return m_Keys.at(key).at(state); }
		inline Command* GetInputCommand(MouseButton button, InputState state) const { return m_Buttons.at(button).at(state); }

	private:
		bool IsPressed(ControllerButton button, int controllerId) const;
		bool IsPressed(KeyboardKey key) const;
		bool IsPressed(MouseButton button) const;

		bool IsHeld(ControllerButton button, int controllerId) const;
		bool IsHeld(KeyboardKey key) const;
		bool IsHeld(MouseButton button) const;

		bool IsReleased(ControllerButton button, int controllerId) const;
		bool IsReleased(KeyboardKey key) const;
		bool IsReleased(MouseButton button) const;

		bool IsActive(ControllerAxis axis, int controllerId) const;

		void Execute(ControllerAxis axie, const GameObject* actor) const;
		void Execute(ControllerButton button, InputState state, const GameObject* actor) const;
		void Execute(KeyboardKey key, InputState state, const GameObject* actor) const;
		void Execute(MouseButton button, InputState state, const GameObject* actor) const;

		std::unordered_map< ControllerAxis, Command*> m_ControllerAxies;
		std::unordered_map< ControllerButton, std::unordered_map<InputState, Command*>> m_Controllers;
		std::unordered_map< KeyboardKey, std::unordered_map<InputState, Command*>> m_Keys;
		std::unordered_map< MouseButton, std::unordered_map<InputState, Command*>> m_Buttons;
	};
}