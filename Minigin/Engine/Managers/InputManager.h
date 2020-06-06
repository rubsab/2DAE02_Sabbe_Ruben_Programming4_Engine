#pragma once
#include <XInput.h>
#include "../Helpers/Singleton.h"
#include <map>
#include <functional>

namespace MyEngine
{
	class GameObject;
	enum class ButtonState
	{
		None,
		Pressed,
		Down,
		Released
	};
	enum class Hardware
	{
		KeyBoard,
		Controller,
		Mouse
	};

	struct Command
	{
		std::function<void()> Action;
		ButtonState State;
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		~InputManager();
		bool ProcessSDLEvents() const;
		void ProcessInput();
		bool IsButtonState(const int buttonCode, const Hardware& hardWare, const ButtonState& buttonState);
		bool IsPressed(const int buttonCode, const Hardware& hardWare);
		bool IsDown(const int buttonCode, const Hardware& hardWare);
		bool IsReleased(const int buttonCode, const Hardware& hardWare);
		void AddCommand(const int buttonCode, const Hardware& hardWare, const Command* command);
	private:
		void UpdateStates(const Hardware& hardWare);
		void UpdateState(const bool down, std::pair<const int, ButtonState>& button);
		void ExecuteCommands(const Hardware& hardWare, const std::map<const int, std::vector<const Command*>>& mappings);
		void AddCommand(std::map<const int, std::vector<const Command*>>& mappings, const int buttonCode, const Command* command);
		std::map<const int, std::vector<const Command*>> m_ControllerMappings, m_KeyBoardMappings, m_MouseMappings;
		std::map<const int, ButtonState> m_ControllerStates, m_KeyBoardStates, m_MouseStates;
	};

}
