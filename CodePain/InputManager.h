#pragma once
#include <XInput.h>
#include "Singleton.h"
#include <vector>
#include <unordered_map>

namespace cp
{
	enum class ControllerButton : int
	{							//bitmask
		DpadUp = 0,				//1<<0	0x0001
		DpadDown = 1,			//1<<1	0x0002
		DpadLeft = 2,			//1<<2	0x0004
		DpadRight = 3,			//1<<3	0x0008
		Start = 4,				//1<<4	0x0010
		Back = 5,				//1<<5	0x0020
		LeftThumb = 6,			//1<<6	0x0040
		RightThumb = 7,			//1<<7	0x0080
		LeftShoulder = 8,		//1<<8	0x0100
		RightShoulder = 9,		//1<<9	0x0200
		ButtonA = 12,			//1<<12	0x1000
		ButtonB = 13,			//1<<13	0x2000
		ButtonX = 14,			//1<<14	0x4000
		ButtonY = 15,			//1<<15	0x8000
	};

	enum class ControllerAxis : int
	{
		LeftTrigger = 0,
		RightTrigger = 1,
		LeftThumbStickX = 2,
		LeftThumbStickY = 3,
		RightThumbStickX = 4,
		RightThumbStickY = 5,
	};

	typedef std::pair<ControllerAxis, std::vector<float>> ControllerAxisData;

	enum class MouseButton : unsigned char
	{
		LMB = 0x01,
		RMB = 0x02,
		MMB = 0x04,
		XB1 = 0x05,
		XB2 = 0x06,
	};

	typedef std::pair<MouseButton, unsigned short> MouseButtonData;

	enum class KeyboardKey : unsigned char
	{
		Cancel = 0x03,
		KeyBack = 0x08,
		Tab = 0x09,
		Clear = 0x0C,
		Return = 0x0D,
		Shift = 0x10,
		Control = 0x11,
		Menu = 0x12,
		Pause = 0x13,
		Capital = 0x14,
		KeyEscape = 0x1B,
		SpaceBar = 0x20,
		Prior = 0x21,
		Next = 0x22,
		End = 0x23,
		Home = 0x24,
		Left = 0x25,
		Up = 0x26,
		Right = 0x27,
		Down = 0x28,
		Select = 0x29,
		Print = 0x2A,
		Execute = 0x2B,
		SnapShot = 0x2C,
		Insert = 0x2D,
		Delete = 0x2E,
		Help = 0x2F,
		Key0 = 0x30,
		Key1 = 0x31,
		Key2 = 0x32,
		Key3 = 0x33,
		Key4 = 0x34,
		Key5 = 0x35,
		Key6 = 0x36,
		Key7 = 0x37,
		Key8 = 0x38,
		Key9 = 0x39,
		KeyA = 0x41,
		KeyB = 0x42,
		KeyC = 0x43,
		KeyD = 0x44,
		KeyE = 0x45,
		KeyF = 0x46,
		KeyG = 0x47,
		KeyH = 0x48,
		KeyI = 0x49,
		KeyJ = 0x4A,
		KeyK = 0x4B,
		KeyL = 0x4C,
		KeyM = 0x4D,
		KeyN = 0x4E,
		KeyO = 0x4F,
		KeyP = 0x50,
		KeyQ = 0x51,
		KeyR = 0x52,
		KeyS = 0x53,
		KeyT = 0x54,
		KeyU = 0x55,
		KeyV = 0x56,
		KeyW = 0x57,
		KeyX = 0x58,
		KeyY = 0x59,
		KeyZ = 0x5A,
		LeftWin = 0x5B,
		RightWin = 0x5C,
		Apps = 0x5D,
		KeySleep = 0x5F,
		NumPad0 = 0x60,
		NumPad1 = 0x61,
		NumPad2 = 0x62,
		NumPad3 = 0x63,
		NumPad4 = 0x64,
		NumPad5 = 0x65,
		NumPad6 = 0x66,
		NumPad7 = 0x67,
		NumPad8 = 0x68,
		NumPad9 = 0x69,
		Multiply = 0x6A,
		Add = 0x6B,
		Separator = 0x6C,
		Subtract = 0x6D,
		Decimal = 0x6E,
		Divide = 0x6F,
		F1 = 0x70,
		F2 = 0x71,
		F3 = 0x72,
		F4 = 0x73,
		F5 = 0x74,
		F6 = 0x75,
		F7 = 0x76,
		F8 = 0x77,
		F9 = 0x78,
		F10 = 0x79,
		F11 = 0x7A,
		F12 = 0x7B,
		F13 = 0x7C,
		F14 = 0x7D,
		F15 = 0x7E,
		F16 = 0x7F,
		F17 = 0x80,
		F18 = 0x81,
		F19 = 0x82,
		F20 = 0x83,
		F21 = 0x84,
		F22 = 0x85,
		F23 = 0x86,
		F24 = 0x87,
		NumLock = 0x90,
		ScrollLock = 0x91,
		LeftShift = 0xA0,
		RightShift = 0xA1,
		LeftCtrl = 0xA2,
		RightCtrl = 0xA3,
		LeftMenu = 0xA4,
		RightMenu = 0xA5,
		VolumeMute = 0xAD,
		VolumeDown = 0xAE,
		VolumeUp = 0xAF,
		OEM_1 = 0xBA,
		OEM_Plus = 0xBB,
		OEM_Comma = 0xBC,
		OEM_Minus = 0xBD,
		OEM_Period = 0xBE,
		OEM_2 = 0xBF,
		OEM_3 = 0xC0,
		OEM_4 = 0xDB,
		OEM_5 = 0xDC,
		OEM_6 = 0xDD,
		OEM_7 = 0xDE,
		OEM_8 = 0xDF,
		OEM_102 = 0xE2
	};

	typedef std::pair<KeyboardKey, unsigned short> KeyData;

	class InputManager final : public Singleton<InputManager>
	{
	public:
		void Initialize();

		bool ProcessInput();

		void AddKeyboardKey(KeyboardKey key);
		void AddMouseButton(MouseButton button);

		bool IsPressed(ControllerButton button, int controllerID) const;
		bool IsPressed(KeyboardKey key) const;
		bool IsPressed(MouseButton button) const;

		bool IsHeld(ControllerButton button, int controllerID) const;
		bool IsHeld(KeyboardKey key) const;
		bool IsHeld(MouseButton button) const;

		bool IsReleased(ControllerButton button, int controllerID) const;
		bool IsReleased(KeyboardKey key) const;
		bool IsReleased(MouseButton button) const;

		float GetControllerAxisValue(ControllerAxis axis, int controllerID) const;

	private:
		bool ProcessQuit();

		void ProcessControllerInput();
		void ProcessKeyboardKeyInput();
		void ProcessMouseButtonInput();
		void ProcessImGuiInput();

		bool IsValidController(int controllerID) const;
		bool CheckControllerButton(ControllerButton button, int controllerID)const;
		bool CheckPrevControllerButton(ControllerButton button, int controllerID)const;

		bool DoesKeyExist(KeyboardKey key) const;
		bool CheckIsDown(KeyboardKey key) const;
		bool CheckIsToggled(KeyboardKey key) const;
		bool CheckIsPrevDown(KeyboardKey key) const;
		bool CheckIsPrevToggled(KeyboardKey key) const;

		bool DoesButtonExist(MouseButton button) const;
		bool CheckIsDown(MouseButton button) const;
		bool CheckIsToggled(MouseButton button) const;
		bool CheckIsPrevDown(MouseButton button) const;
		bool CheckIsPrevToggled(MouseButton button) const;

		size_t m_AmountOfControllerStates = 0;
		std::vector<unsigned short> m_ControllerStates;
		std::vector<unsigned short> m_PreviousControllerStates;

		const float m_AxisDreadZone = 0.05f;
		const float m_MaxTriggerValue = 255.f;
		const float m_MaxAxisValue = 32767.f;
		std::unordered_map<ControllerAxis, std::vector<float>> m_ControllerAxisValues;

		std::unordered_map<MouseButton, unsigned short> m_MouseButtonStates;
		std::unordered_map<MouseButton, unsigned short> m_PreviousMouseButtonStates;

		std::unordered_map<KeyboardKey, unsigned short> m_KeyStates;
		std::unordered_map<KeyboardKey, unsigned short> m_PreviousKeyStates;
	};

}
