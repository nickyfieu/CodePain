#pragma once
#include <XInput.h>
#include "Singleton.h"
#include <SDL.h>

namespace cp
{
	enum class ControllerButton
	{
		ButtonA,
		ButtonB,
		ButtonX,
		ButtonY
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		bool IsPressed(ControllerButton button) const;
	private:
		XINPUT_STATE m_CurrentState{};


		void ImGui_Input(const SDL_Event& event);
	};

}
