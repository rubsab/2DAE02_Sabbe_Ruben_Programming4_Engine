#include "MiniginPCH.h"
#include "InputManager.h"
#include <SDL.h>
#include "../Scene/GameObject.h"
#include "../Helpers/Logger.h"

MyEngine::InputManager::~InputManager()
{
	for (const std::pair<std::pair<const int, const int>, std::vector<Command*>>& pair : m_ControllerMappings)
	{
		for (Command* command : pair.second)
		{
			if (!command->IsDeleted)
			{
				command->IsDeleted = true;
				Safe_Delete(command);
			}
		}
	}
	
	for (const std::pair<std::pair<const int, const int>, std::vector<Command*>>& pair : m_KeyBoardMappings)
	{
		for (Command* command : pair.second)
		{
			if (!command->IsDeleted)
			{
				command->IsDeleted = true;
				Safe_Delete(command);
			}
		}
	}
	
	for (const std::pair<std::pair<const int, const int>, std::vector<Command*>>& pair : m_MouseMappings)
	{
		for (Command* command : pair.second)
		{
			if (!command->IsDeleted)
			{
				command->IsDeleted = true;
				Safe_Delete(command);
			}
		}
	}
}

void MyEngine::InputManager::Init(SDL_Window* pWindow)
{
	m_pWindow = pWindow;
}

bool MyEngine::InputManager::ProcessSDLEvents() const
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
	}
	return true;
}

void MyEngine::InputManager::ProcessInput()
{
	if (!(SDL_GetWindowFlags(m_pWindow) & (SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS)))
		return;
	UpdateStates(Hardware::KeyBoard);
	ExecuteCommands(Hardware::KeyBoard, m_KeyBoardMappings);
	UpdateStates(Hardware::Controller);
	ExecuteCommands(Hardware::Controller, m_ControllerMappings);
	UpdateStates(Hardware::Mouse);
	ExecuteCommands(Hardware::Mouse, m_MouseMappings);
}

bool MyEngine::InputManager::IsButtonState(const int buttonCode, const Hardware& hardWare, const ButtonState& buttonState, const int id)
{
	switch (hardWare)
	{
	case(Hardware::KeyBoard):
		return (m_KeyBoardStates[{buttonCode, id}] == buttonState);
		break;
	case(Hardware::Controller):
		return (m_ControllerStates[{buttonCode, id}] == buttonState);
		break;
	case(Hardware::Mouse):
		return (m_MouseStates[{buttonCode, id}] == buttonState);
		break;
	default:
		return false;
	}
}

bool MyEngine::InputManager::IsPressed(const int buttonCode, const Hardware& hardWare, const int id)
{
	return IsButtonState(buttonCode, hardWare, ButtonState::Pressed, id);
}

bool MyEngine::InputManager::IsDown(const int buttonCode, const Hardware& hardWare, const int id)
{
	return IsButtonState(buttonCode, hardWare, ButtonState::Down, id);
}

bool MyEngine::InputManager::IsReleased(const int buttonCode, const Hardware& hardWare, const int id)
{
	return IsButtonState(buttonCode, hardWare, ButtonState::Released, id);
}

void MyEngine::InputManager::AddCommand(std::vector<std::pair<const int, const Hardware>> buttons, Command* command, const int id)
{
	for (std::pair<const int, const Hardware>& button : buttons)
	{
		switch (button.second)
		{
		case(Hardware::KeyBoard):
			AddCommand(m_KeyBoardMappings, button.first, command, id);
			m_KeyBoardStates[{button.first, id}] = ButtonState::None;
			break;
		case(Hardware::Controller):
			AddCommand(m_ControllerMappings, button.first, command, id);
			m_ControllerStates[{button.first, id}] = ButtonState::None;
			break;
		case(Hardware::Mouse):
			AddCommand(m_MouseMappings, button.first, command, id);
			m_MouseStates[{button.first, id}] = ButtonState::None;
			break;
		default:
			break;
		}
	}
}

void MyEngine::InputManager::UpdateStates(const Hardware& hardWare)
{
	XINPUT_STATE* pState{};
	WORD buttons{};
	switch (hardWare)
	{
	case(Hardware::KeyBoard):
		for (std::pair<const std::pair<const int, const int>, ButtonState>& button : m_KeyBoardStates)
			UpdateState(GetKeyState(button.first.first) & 0x8000, button);
		break;
	case(Hardware::Controller):
		pState = new XINPUT_STATE();
		for (std::pair<const std::pair<const int, const int>, ButtonState>& button : m_ControllerStates)
		{
			XInputGetState(button.first.second, pState);
			buttons = pState->Gamepad.wButtons;
			UpdateState(buttons & WORD(button.first.first), button);
		}
		Safe_Delete(pState);
		break;
	case(Hardware::Mouse):
		for (std::pair<const std::pair<const int, const int>, ButtonState>& button : m_MouseStates)
			UpdateState(GetKeyState(button.first.first) & 0x80, button);
		break;
	default:
		break;
	}
}

void MyEngine::InputManager::UpdateState(const bool down, std::pair<const std::pair<const int, const int>, ButtonState>& button)
{
	if (down)
	{
		if (button.second == ButtonState::None || button.second == ButtonState::Pressed)
			button.second = ButtonState(int(button.second) + 1);
	}
	else
	{
		if (button.second == ButtonState::Down || button.second == ButtonState::Released)
			button.second = ButtonState((int(button.second) + 1) % 4);
	}
}

void MyEngine::InputManager::ExecuteCommands(const Hardware& hardWare, const std::map<std::pair<const int, const int>, std::vector<Command*>>& mappings)
{
	for (const std::pair<std::pair<const int, const int>, std::vector<Command*>>& pair : mappings)
	{
		for (const Command* command : pair.second)
		{
			if (IsButtonState(pair.first.first, hardWare, command->State, pair.first.second))
				command->Action();
		}
	}
}

void MyEngine::InputManager::AddCommand(std::map<std::pair<const int, const int>, std::vector<Command*>>& mappings, const int buttonCode, Command* command, const int id)
{
	for (const Command* pCommand : mappings[{buttonCode, id}])
	{
		if (pCommand == command)
		{
			Logger::LogWarning("Command already added to button");
			return;
		}
	}
	mappings[{buttonCode, id}].push_back(command);
}
