#pragma once
#include <XInput.h>
#include "../Helpers/Singleton.h"
#include <map>
#include "../Helpers/Commands.h"

namespace MyEngine
{
	class GameObject;
	enum class ControllerButton
	{
		ButtonA = XINPUT_GAMEPAD_A,
		ButtonB = XINPUT_GAMEPAD_B,
		ButtonX = XINPUT_GAMEPAD_X,
		ButtonY = XINPUT_GAMEPAD_Y
	};

	enum class KeyBoardKey
	{
		KeyUp = VK_UP,
		KeyDown = VK_DOWN,
		KeyLeft = VK_LEFT,
		KeyRight = VK_RIGHT,
		KeyW = 'W',
		KeyS = 'S',
		KeyA = 'A',
		KeyD = 'D',
		KeyShift = VK_SHIFT,
		KeyControl = VK_CONTROL,
		KeySpace = VK_SPACE
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		~InputManager();
		bool ProcessSDLEvents();
		void ProcessInput(GameObject* object);
		bool IsPressed(ControllerButton button) const;
		bool IsPressed(KeyBoardKey key) const;
		void AddCommand(ControllerButton button, Command* command);
		void AddCommand(KeyBoardKey key, Command* command);
	private:
		std::map<ControllerButton, std::vector<Command*>> m_ControllerMappings;
		std::map<KeyBoardKey, std::vector<Command*>> m_KeyBoardMappings;
	};

}
