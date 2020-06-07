#include "CodePainPCH.h"
#include "InputManager.h"
#include "Logger.h"
#include <SDL.h>
#ifdef _DEBUG
	#include "Imgui\imgui.h"
	#include "Imgui_Sdl\imgui_impl_sdl.h"
#endif

bool cp::InputManager::ProcessInput()
{
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(0, &m_CurrentState);

#ifdef _DEBUG
	ImGuiIO& io = ImGui::GetIO();
#endif

	int wheel = 0;
	UNREFERENCED_PARAMETER(wheel);

	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
#ifdef _DEBUG
		ImGui_ImplSDL2_ProcessEvent(&e);
#endif
		if (e.type == SDL_QUIT)
		{
			return false;
		}
		else if (e.type == SDL_WINDOWEVENT)
		{
			if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
			{
#ifdef _DEBUG
				io.DisplaySize.x = static_cast<float>(e.window.data1);
				io.DisplaySize.y = static_cast<float>(e.window.data2);
#endif
			}
		}
		else if (e.type == SDL_MOUSEWHEEL)
		{
			wheel = e.wheel.y;
		}
		else if (e.type == SDL_KEYDOWN) 
		{

		}
		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			
		}
	}

	int mouseX, mouseY;
	const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
	UNREFERENCED_PARAMETER(mouseX);
	UNREFERENCED_PARAMETER(mouseY);
	UNREFERENCED_PARAMETER(buttons);

#ifdef _DEBUG
	io.DeltaTime = 1.0f / 60.0f;
	io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
	io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
	io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
	io.MouseWheel = static_cast<float>(wheel);
	
	if (io.MouseDown[1] == true)
		Logger::GetInstance().Log(cp::LogLevel::Info, "Mouse position: x " + std::to_string(io.MousePos.x) + " y " + std::to_string(io.MousePos.y), true);

#endif

	return true;
}

bool cp::InputManager::IsPressed(ControllerButton button) const
{
	switch (button)
	{
	case ControllerButton::ButtonA:
		return m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_A;
	case ControllerButton::ButtonB:
		return m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_B;
	case ControllerButton::ButtonX:
		return m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_X;
	case ControllerButton::ButtonY:
		return m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_Y;
	default: return false;
	}
}

