#include "CodePainPCH.h"
#include <Windows.h>
#include <Xinput.h>
#include "InputManager.h"
#include "Logger.h"
#if defined(_DEBUG)
	#include "Imgui\imgui.h"
	#include "Imgui_Sdl\imgui_impl_sdl.h"
#endif
#include <algorithm>
#include <SDL.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "XInput.lib")

void cp::InputManager::Initialize()
{
	m_AmountOfControllerStates = XUSER_MAX_COUNT;
	m_ControllerStates.resize(m_AmountOfControllerStates);
	m_PreviousControllerStates.resize(m_AmountOfControllerStates);
	for (size_t i = 0; i < m_AmountOfControllerStates; i++)
	{
		m_PreviousControllerStates[i] = 0x0000;
		m_ControllerStates[i] = 0x0000;
	}

	std::vector<float> baseAxisValues;
	baseAxisValues.resize(m_AmountOfControllerStates, 0.f);
	m_ControllerAxisValues.insert(ControllerAxisData{ ControllerAxis::LeftTrigger,baseAxisValues });
	m_ControllerAxisValues.insert(ControllerAxisData{ ControllerAxis::RightTrigger,baseAxisValues });
	m_ControllerAxisValues.insert(ControllerAxisData{ ControllerAxis::LeftThumbStickX,baseAxisValues });
	m_ControllerAxisValues.insert(ControllerAxisData{ ControllerAxis::LeftThumbStickY,baseAxisValues });
	m_ControllerAxisValues.insert(ControllerAxisData{ ControllerAxis::RightThumbStickX,baseAxisValues });
	m_ControllerAxisValues.insert(ControllerAxisData{ ControllerAxis::RightThumbStickY,baseAxisValues });
	m_PreviousControllerAxisValues.insert(ControllerAxisData{ ControllerAxis::LeftTrigger,baseAxisValues });
	m_PreviousControllerAxisValues.insert(ControllerAxisData{ ControllerAxis::RightTrigger,baseAxisValues });
	m_PreviousControllerAxisValues.insert(ControllerAxisData{ ControllerAxis::LeftThumbStickX,baseAxisValues });
	m_PreviousControllerAxisValues.insert(ControllerAxisData{ ControllerAxis::LeftThumbStickY,baseAxisValues });
	m_PreviousControllerAxisValues.insert(ControllerAxisData{ ControllerAxis::RightThumbStickX,baseAxisValues });
	m_PreviousControllerAxisValues.insert(ControllerAxisData{ ControllerAxis::RightThumbStickY,baseAxisValues });
}

bool cp::InputManager::ProcessInput()
{
	ProcessControllerInput();
	ProcessKeyboardKeyInput();
	ProcessMouseButtonInput();
	ProcessImGuiInput();
	return !ProcessQuit();
}

bool cp::InputManager::ProcessQuit()
{
	SDL_Event e;
	SDL_PollEvent(&e);
	return e.type == SDL_QUIT;
}

void cp::InputManager::ProcessControllerInput()
{
	DWORD dwResult;
	for (DWORD i = 0; i < m_AmountOfControllerStates; i++)
	{
		// setting previous values
		m_PreviousControllerStates[i] = m_ControllerStates[i];
		m_PreviousControllerAxisValues[ControllerAxis::LeftTrigger][i]		= m_ControllerAxisValues[ControllerAxis::LeftTrigger][i];
		m_PreviousControllerAxisValues[ControllerAxis::RightTrigger][i]		= m_ControllerAxisValues[ControllerAxis::RightTrigger][i];
		m_PreviousControllerAxisValues[ControllerAxis::LeftThumbStickX][i]	= m_ControllerAxisValues[ControllerAxis::LeftThumbStickX][i];
		m_PreviousControllerAxisValues[ControllerAxis::LeftThumbStickY][i]	= m_ControllerAxisValues[ControllerAxis::LeftThumbStickY][i];
		m_PreviousControllerAxisValues[ControllerAxis::RightThumbStickX][i]	= m_ControllerAxisValues[ControllerAxis::RightThumbStickX][i];
		m_PreviousControllerAxisValues[ControllerAxis::RightThumbStickY][i] = m_ControllerAxisValues[ControllerAxis::RightThumbStickY][i];

		// getting new state
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE)); // default initializing state to 0 
		dwResult = XInputGetState(i, &state); // get the state
		if (dwResult == ERROR_SUCCESS) // Controller is connected 
		{
			m_ControllerStates[i] = state.Gamepad.wButtons;
			//trigger value ranges from [0, 255]
			m_ControllerAxisValues[ControllerAxis::LeftTrigger][i] = (float)state.Gamepad.bLeftTrigger / m_MaxTriggerValue;
			m_ControllerAxisValues[ControllerAxis::RightTrigger][i] = (float)state.Gamepad.bRightTrigger / m_MaxTriggerValue;
			//thumb stick value ranges from [-32768 to +32767]
			//to be able to have +1 instead of 0.999 i will devide by 32767
			//thats the reason why i will be doing a std::fmaxf
			float lftx = std::max<float>(-1.f, (float)state.Gamepad.sThumbLX / m_MaxAxisValue);
			float lfty = std::max<float>(-1.f, (float)state.Gamepad.sThumbLY / m_MaxAxisValue);
			float rhtx = std::max<float>(-1.f, (float)state.Gamepad.sThumbRX / m_MaxAxisValue);
			float rhty = std::max<float>(-1.f, (float)state.Gamepad.sThumbRY / m_MaxAxisValue);
			m_ControllerAxisValues[ControllerAxis::LeftThumbStickX][i] = (abs(lftx) < m_AxisDreadZone) ? 0.f : lftx;
			m_ControllerAxisValues[ControllerAxis::LeftThumbStickY][i] = (abs(lfty) < m_AxisDreadZone) ? 0.f : lfty;
			m_ControllerAxisValues[ControllerAxis::RightThumbStickX][i] = (abs(rhtx) < m_AxisDreadZone) ? 0.f : rhtx;
			m_ControllerAxisValues[ControllerAxis::RightThumbStickY][i] = (abs(rhty) < m_AxisDreadZone) ? 0.f : rhty;
		}
		else
		{
			m_ControllerStates[i] = 0x0000;
			//trigger value ranges from [0, 255]
			m_ControllerAxisValues[ControllerAxis::LeftTrigger][i] = 0.f;
			m_ControllerAxisValues[ControllerAxis::RightTrigger][i] = 0.f;
			//thumb stick value ranges from [-32768 to +32767]
			m_ControllerAxisValues[ControllerAxis::LeftThumbStickX][i] = 0.f;
			m_ControllerAxisValues[ControllerAxis::LeftThumbStickY][i] = 0.f;
			m_ControllerAxisValues[ControllerAxis::RightThumbStickX][i] = 0.f;
			m_ControllerAxisValues[ControllerAxis::RightThumbStickY][i] = 0.f;
		}
	}
}

void cp::InputManager::ProcessKeyboardKeyInput()
{
	for (KeyData keyData : m_PreviousKeyStates)
	{
		m_PreviousKeyStates[keyData.first] = m_KeyStates[keyData.first];
	}
	for (KeyData keyData : m_KeyStates)
	{
		m_KeyStates[keyData.first] = GetKeyState((int)keyData.first);
	}
}

void cp::InputManager::AddKeyboardKey(KeyboardKey key)
{
	unsigned short value = 0;
	KeyData newKey = KeyData{ key, value };
	m_KeyStates.insert(newKey);
	m_PreviousKeyStates.insert(newKey);
}

void cp::InputManager::ProcessMouseButtonInput()
{
	for (MouseButtonData mouseData : m_PreviousMouseButtonStates)
	{
		m_PreviousMouseButtonStates[mouseData.first] = m_MouseButtonStates[mouseData.first];
	}

	for (MouseButtonData mouseData : m_MouseButtonStates)
	{
		m_MouseButtonStates[mouseData.first] = GetKeyState((int)mouseData.first);
	}
}

void cp::InputManager::ProcessImGuiInput()
{
#if defined(_DEBUG)
	SDL_Event e;
	SDL_PollEvent(&e);
	ImGui_ImplSDL2_ProcessEvent(&e);
	ImGuiIO& io = ImGui::GetIO();

	int wheel = 0, mouseX = 0, mouseY = 0;
	const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
	if (e.type == SDL_WINDOWEVENT)
	{
		if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
		{
			io.DisplaySize.x = static_cast<float>(e.window.data1);
			io.DisplaySize.y = static_cast<float>(e.window.data2);
		}
	}
	else if (e.type == SDL_MOUSEWHEEL)
	{
		wheel = e.wheel.y;
	}

	io.DeltaTime = 1.0f / 60.0f;
	io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
	io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
	io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
	io.MouseWheel = static_cast<float>(wheel);
#endif
}

void cp::InputManager::AddMouseButton(MouseButton button)
{
	unsigned short value = 0;
	MouseButtonData newButton = MouseButtonData{ button, value };
	m_MouseButtonStates.insert(newButton);
	m_PreviousMouseButtonStates.insert(newButton);
}

bool cp::InputManager::IsPressed(ControllerButton button, int controllerID) const
{
#if defined(_DEBUG)
	if (!IsValidController(controllerID))
		return false;
#endif
	return CheckControllerButton(button,controllerID) && !CheckPrevControllerButton(button, controllerID);
}

bool cp::InputManager::IsPressed(KeyboardKey key) const
{
#if defined(_DEBUG)
	if (!DoesKeyExist(key))
		return false;
#endif
	return CheckIsDown(key) && !CheckIsPrevDown(key);
}

bool cp::InputManager::IsPressed(MouseButton button) const
{
#if defined(_DEBUG)
	if (!DoesButtonExist(button))
		return false;
#endif
	return CheckIsDown(button) && !CheckIsPrevDown(button);
}

bool cp::InputManager::IsHeld(ControllerButton button, int controllerID) const
{
#if defined(_DEBUG)
	if (!IsValidController(controllerID))
		return false;
#endif
	return CheckControllerButton(button, controllerID) && CheckPrevControllerButton(button, controllerID);
}

bool cp::InputManager::IsHeld(KeyboardKey key) const
{
#if defined(_DEBUG)
	if (!DoesKeyExist(key))
		return false;
#endif
	return CheckIsDown(key) && CheckIsPrevDown(key);
}

bool cp::InputManager::IsHeld(MouseButton button) const
{
#if defined(_DEBUG)
	if (!DoesButtonExist(button))
		return false;
#endif
	return CheckIsDown(button) && CheckIsPrevDown(button);
}

bool cp::InputManager::IsReleased(ControllerButton button, int controllerID) const
{
#if defined(_DEBUG)
	if (!IsValidController(controllerID))
		return false;
#endif
	return !CheckControllerButton(button, controllerID) && CheckPrevControllerButton(button, controllerID);
}

bool cp::InputManager::IsReleased(KeyboardKey key) const
{
#if defined(_DEBUG)
	if (!DoesKeyExist(key))
		return false;
#endif
	return !CheckIsDown(key) && CheckIsPrevDown(key);
}

bool cp::InputManager::IsReleased(MouseButton button) const
{
#if defined(_DEBUG)
	if (!DoesButtonExist(button))
		return false;
#endif
	return !CheckIsDown(button) && CheckIsPrevDown(button);
}

float cp::InputManager::GetControllerAxisValue(ControllerAxis axis, int controllerID) const
{
#if defined(_DEBUG)
	if (!IsValidController(controllerID))
		return 0.f;
#endif
	return m_ControllerAxisValues.at(axis)[controllerID];
}

float cp::InputManager::GetPreviousControllerAxisValue(ControllerAxis axis, int controllerID) const
{
#if defined(_DEBUG)
	if (!IsValidController(controllerID))
		return 0.f;
#endif
	return m_PreviousControllerAxisValues.at(axis)[controllerID];
}

bool cp::InputManager::IsValidController(int controllerID) const
{
	if (controllerID < 0 || controllerID > (int)m_AmountOfControllerStates)
	{
		Logger::GetInstance().Log(cp::LogLevel::Error, "InputManager::CheckControllerID Invalid ID " + std::to_string(controllerID));
		return false;
	}
	return true;
}

// see Controller Buttons to understand (1 << (int)button)
bool cp::InputManager::CheckControllerButton(ControllerButton button, int controllerID) const
{
	return m_ControllerStates[controllerID] & (1 << (int)button);
}

// see Controller Buttons to understand (1 << (int)button)
bool cp::InputManager::CheckPrevControllerButton(ControllerButton button, int controllerID) const
{
	return m_PreviousControllerStates[controllerID] & (1 << (int)button);
}

bool cp::InputManager::DoesKeyExist(KeyboardKey key) const
{
	if (m_KeyStates.find(key) != m_KeyStates.cend())
		return true;
	Logger::GetInstance().Log(cp::LogLevel::Warning, "InputManager::DoesKeyExist key with code " + std::to_string((int)key) + " does not exist!");
	return false;
}

bool cp::InputManager::CheckIsDown(KeyboardKey key) const
{
	return m_KeyStates.at(key) & 0x8000;
}

// not used just implemented
bool cp::InputManager::CheckIsToggled(KeyboardKey key) const 
{
	return m_KeyStates.at(key) & 0x0001;
}

bool cp::InputManager::CheckIsPrevDown(KeyboardKey key) const
{
	return m_PreviousKeyStates.at(key) & 0x8000;
}

bool cp::InputManager::CheckIsPrevToggled(KeyboardKey key) const
{
	return m_PreviousKeyStates.at(key) & 0x0001;
}

bool cp::InputManager::DoesButtonExist(MouseButton button) const
{
	if (m_MouseButtonStates.find(button) != m_MouseButtonStates.cend())
		return true;
	Logger::GetInstance().Log(cp::LogLevel::Warning, "InputManager::DoesButtonExist button with code " + std::to_string((int)button) + " does not exist!");
	return false;
}

bool cp::InputManager::CheckIsDown(MouseButton button) const
{
	return m_MouseButtonStates.at(button) & 0x8000;
}

bool cp::InputManager::CheckIsToggled(MouseButton button) const
{
	return m_MouseButtonStates.at(button) & 0x0001;
}

bool cp::InputManager::CheckIsPrevDown(MouseButton button) const
{
	return m_PreviousMouseButtonStates.at(button) & 0x8000;
}

bool cp::InputManager::CheckIsPrevToggled(MouseButton button) const
{
	return m_PreviousMouseButtonStates.at(button) & 0x0001;
}
