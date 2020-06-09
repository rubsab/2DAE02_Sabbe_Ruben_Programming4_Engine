#pragma once
#include <XInput.h>
#include "../Helpers/Singleton.h"
#include <map>
#include <functional>

struct SDL_Window;

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
		InputManager() = default;
		~InputManager();
		void Init(SDL_Window* pWindow);
		bool ProcessSDLEvents() const;
		void ProcessInput();
		bool IsButtonState(const int buttonCode, const Hardware& hardWare, const ButtonState& buttonState, const int id = 0);
		bool IsPressed(const int buttonCode, const Hardware& hardWare, const int id = 0);
		bool IsDown(const int buttonCode, const Hardware& hardWare, const int id = 0);
		bool IsReleased(const int buttonCode, const Hardware& hardWare, const int id = 0);
		void AddCommand(const int buttonCode, const Hardware& hardWare, const Command* command, const int id = 0);
	private:
		void UpdateStates(const Hardware& hardWare);
		void UpdateState(const bool down, std::pair<const std::pair<const int, const int>, ButtonState>& button);
		void ExecuteCommands(const Hardware& hardWare, const std::map<std::pair<const int, const int>, std::vector<const Command*>>& mappings);
		void AddCommand(std::map<std::pair<const int, const int>, std::vector<const Command*>>& mappings, const int buttonCode, const Command* command, const int id);
		std::map<std::pair<const int, const int>, std::vector<const Command*>> m_ControllerMappings, m_KeyBoardMappings, m_MouseMappings;
		std::map<std::pair<const int, const int>, ButtonState> m_ControllerStates, m_KeyBoardStates, m_MouseStates;

		SDL_Window* m_pWindow = nullptr;
	};

}
