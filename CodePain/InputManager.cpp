#include "CodePainPCH.h"
#include "InputManager.h"
#include "Logger.h"
#ifdef _DEBUG
	#include "Imgui\imgui.h"
	#include "Imgui_Sdl\imgui_impl_sdl.h"
#endif

bool cp::InputManager::ProcessInput()
{
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(0, &m_CurrentState);

	int wheel = 0;
	UNREFERENCED_PARAMETER(wheel);

	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
#ifdef _DEBUG
		ImGui_Input(e);
#endif
		if (e.type == SDL_QUIT)
		{
			return false;
		}
		else if (e.type == SDL_WINDOWEVENT)
		{
			if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
			{
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

void cp::InputManager::ImGui_Input(const SDL_Event& sdlEvent)
{
	ImGuiIO& io = ImGui::GetIO();
#ifdef _Debug
	ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
#endif

	int wheel = 0, mouseX = 0, mouseY = 0;
	
	if (sdlEvent.type == SDL_WINDOWEVENT)
	{
		if (sdlEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
		{
			io.DisplaySize.x = static_cast<float>(sdlEvent.window.data1);
			io.DisplaySize.y = static_cast<float>(sdlEvent.window.data2);
		}
	}
	else if (sdlEvent.type == SDL_MOUSEWHEEL)
	{
		wheel = sdlEvent.wheel.y;
	}

	const int buttons = SDL_GetMouseState(&mouseX, &mouseY);

	io.DeltaTime = 1.0f / 60.0f;
	io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
	io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
	io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
	io.MouseWheel = static_cast<float>(wheel);

	if (io.MouseDown[1] == true)
		Logger::GetInstance().Log(cp::LogLevel::Info, "Mouse position: x " + std::to_string(io.MousePos.x) + " y " + std::to_string(io.MousePos.y), true);
}

