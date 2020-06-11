#include "CodePainPCH.h"
#include "InputManager.h"
#include "InputHandler.h"
#include "Logger.h"

void cp::InputHandler::Cleanup()
{
	if (!m_Keys.empty())
	{
		for (auto toDeleteParrent : m_Keys)
		{
			for (auto toDelete : toDeleteParrent.second)
			{
				SAFE_DELETE(toDelete.second);
			}
		}
		m_Keys.clear();
	}

	if (!m_Buttons.empty())
	{
		for (auto toDeleteParrent : m_Buttons)
		{
			for (auto toDelete : toDeleteParrent.second)
			{
				SAFE_DELETE(toDelete.second);
			}
		}
		m_Buttons.clear();
	}

	if (!m_Controllers.empty())
	{
		for (auto toDeleteParrent : m_Controllers)
		{
			for (auto toDelete : toDeleteParrent.second)
			{
				SAFE_DELETE(toDelete.second);
			}
		}
		m_Controllers.clear();
	}
}

void cp::InputHandler::HandleInput(const GameObject* actor) const
{
	if (actor == nullptr)
		return;

	cp::GameObjectType type = actor->GetType();

	if (type != cp::GameObjectType::Player1 &&
		type != cp::GameObjectType::Player2 &&
		type != cp::GameObjectType::Player3 &&
		type != cp::GameObjectType::Player4 &&
		type != cp::GameObjectType::Player5)
		return;

	if (type == cp::GameObjectType::Player1)
	{
		// keyboard key handling
		for (const auto& key : m_Keys)
		{
			for (const auto& state : key.second)
			{
				if (state.first == InputState::Pressed)
				{
					if (IsPressed(key.first))
						Execute(key.first, state.first, actor);
				}
				else if (state.first == InputState::Hold)
				{
					if (IsHeld(key.first))
						Execute(key.first, state.first, actor);
				}
				else if (state.first == InputState::Released)
				{
					if (IsReleased(key.first))
						Execute(key.first, state.first, actor);
				}
			}
		}

		// mouse key handling
		for (const auto& button : m_Buttons)
		{
			for (const auto& state : button.second)
			{
				if (state.first == InputState::Pressed)
				{
					if (IsPressed(button.first))
						Execute(button.first, state.first, actor);
				}
				else if (state.first == InputState::Hold)
				{
					if (IsHeld(button.first))
						Execute(button.first, state.first, actor);
				}
				else if (state.first == InputState::Released)
				{
					if (IsReleased(button.first))
						Execute(button.first, state.first, actor);
				}
			}
		}
	}
	else 
	{
		for (const auto& button : m_Controllers)
		{
			for (const auto& state : button.second)
			{
				if (state.first == InputState::Pressed)
				{
					if (IsPressed(button.first, (int)type - (int) cp::GameObjectType::Player2))
						Execute(button.first, state.first, actor);
				}
				else if (state.first == InputState::Hold)
				{
					if (IsHeld(button.first, (int)type - (int)cp::GameObjectType::Player2))
						Execute(button.first, state.first, actor);
				}
				else if (state.first == InputState::Released)
				{
					if (IsReleased(button.first, (int)type - (int)cp::GameObjectType::Player2))
						Execute(button.first, state.first, actor);
				}
			}
		}
	}

}

void cp::InputHandler::AddInput(ControllerButton button, InputState state, Command* command)
{
#if defined(_DEBUG)
	if (m_Controllers[button][state] != nullptr)
		cp::Logger::GetInstance().Log(cp::LogLevel::Warning, "InputHandler::AddInput button state already had a command!");
#endif
	m_Controllers[button][state] = command;

}

void cp::InputHandler::AddInput(KeyboardKey key, InputState state, Command* command)
{
#if defined(_DEBUG)
	if (m_Keys[key][state] != nullptr)
		cp::Logger::GetInstance().Log(cp::LogLevel::Warning, "InputHandler::AddInput key state already had a command!");
#endif
	m_Keys[key][state] = command;
	cp::InputManager::GetInstance().AddKeyboardKey(key);
}

void cp::InputHandler::AddInput(MouseButton button, InputState state, Command* command)
{
#if defined(_DEBUG)
	if (m_Buttons[button][state] != nullptr)
		cp::Logger::GetInstance().Log(cp::LogLevel::Warning, "InputHandler::AddInput button state already had a command!");
#endif
	m_Buttons[button][state] = command;
	cp::InputManager::GetInstance().AddMouseButton(button);
}

bool cp::InputHandler::IsPressed(ControllerButton button, int controllerId) const
{
	return InputManager::GetInstance().IsPressed(button, controllerId);
}

bool cp::InputHandler::IsPressed(KeyboardKey key) const
{
	return InputManager::GetInstance().IsPressed(key);
}

bool cp::InputHandler::IsPressed(MouseButton button) const
{
	return InputManager::GetInstance().IsPressed(button);
}

bool cp::InputHandler::IsHeld(ControllerButton button, int controllerId) const
{
	return InputManager::GetInstance().IsHeld(button, controllerId);
}

bool cp::InputHandler::IsHeld(KeyboardKey key) const
{
	return InputManager::GetInstance().IsHeld(key);
}

bool cp::InputHandler::IsHeld(MouseButton button) const
{
	return InputManager::GetInstance().IsHeld(button);
}

bool cp::InputHandler::IsReleased(ControllerButton button, int controllerId) const
{
	return InputManager::GetInstance().IsReleased(button, controllerId);
}

bool cp::InputHandler::IsReleased(KeyboardKey key) const
{
	return InputManager::GetInstance().IsReleased(key);
}

bool cp::InputHandler::IsReleased(MouseButton button) const
{
	return InputManager::GetInstance().IsReleased(button);
}

void cp::InputHandler::Execute(ControllerButton button, InputState state, const GameObject* actor) const
{
	Command* pCommand = m_Controllers.at(button).at(state);
	if (pCommand)
		pCommand->Execute(actor);
}

void cp::InputHandler::Execute(KeyboardKey key, InputState state, const GameObject* actor) const
{
	Command* pCommand = m_Keys.at(key).at(state);
	if (pCommand)
		pCommand->Execute(actor);
}

void cp::InputHandler::Execute(MouseButton button, InputState state, const GameObject* actor) const
{
	Command* pCommand = m_Buttons.at(button).at(state);
	if (pCommand)
		pCommand->Execute(actor);
}
