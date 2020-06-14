#pragma once
#include <XInput.h>
#include "../Helpers/Singleton.h"
#include <map>
#include <functional>
#include <SDL.h>
#include <string>
#include <vector>

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

	class OldCommand 
	{
	public:
		OldCommand(const ButtonState state) { m_State = state; }
		virtual ~OldCommand() {}
		virtual void Execute(GameObject* obj) = 0;
		const ButtonState GetState() const { return m_State; }
	private:
		ButtonState m_State;
	};

	struct Command final
	{
		std::function<void()> Action;
		ButtonState State;
		std::string SceneName;
		bool IsDeleted = false;
	};

	struct Button
	{
		int ButtonCode;
		Hardware HardWare;
		int Id;
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager() = default;
		~InputManager();
		void Init(SDL_Window* pWindow, const std::string& ignoreScene);
		bool ProcessSDLEvents() const;
		void ProcessInput();
		bool IsButtonState(const Button& button, const ButtonState& buttonState);
		bool IsPressed(const Button& button);
		bool IsDown(const Button& button);
		bool IsReleased(const Button& button);
		void AddCommand(std::vector<Button> buttons, Command* command);
		void Quit() { m_ShouldQuit = true; }
		const SDL_Point GetMousePos() const;
		const SDL_Point GetMouseMovement() const;
		void RemoveCommandsByIdentifierName(const std::string& sceneName);
	private:
		void DestroyMap(std::map<std::pair<const int, const int>, std::vector<Command*>>& map);
		void UpdateStates(const Hardware& hardWare);
		void UpdateState(const bool down, std::pair<const std::pair<const int, const int>, ButtonState>& button);
		void ExecuteCommands(const Hardware& hardWare, const std::map<std::pair<const int, const int>, std::vector<Command*>>& mappings);
		void AddCommand(std::map<std::pair<const int, const int>, std::vector<Command*>>& mappings, const int buttonCode, Command* command, const int id);
		void ClearCommmandsByIDName(const std::string& sceneName, std::map<std::pair<const int, const int>, std::vector<Command*>>& map);
		std::map<std::pair<const int, const int>, std::vector<Command*>> m_ControllerMappings, m_KeyBoardMappings, m_MouseMappings;
		std::map<std::pair<const int, const int>, ButtonState> m_ControllerStates, m_KeyBoardStates, m_MouseStates;

		SDL_Window* m_pWindow = nullptr;
		SDL_Point m_MousePos, m_OldMousePos;
		bool m_ShouldQuit;
		std::string m_IgnoreScene;
	};

}
