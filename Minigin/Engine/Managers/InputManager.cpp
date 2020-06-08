#include "MiniginPCH.h"
#include "InputManager.h"
#include <SDL.h>
#include "../Scene/GameObject.h"
#include "../Helpers/Logger.h"

MyEngine::InputManager::~InputManager()
{
	for (std::pair<const int, std::vector<const Command*>> pair : m_ControllerMappings)
	{
		for (const Command* command : pair.second)
			delete command;
	}

	for (std::pair<const int, std::vector<const Command*>> pair : m_KeyBoardMappings)
	{
		for (const Command* command : pair.second)
			delete command;
	}

	for (std::pair<const int, std::vector<const Command*>> pair : m_MouseMappings)
	{
		for (const Command* command : pair.second)
			delete command;
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

bool MyEngine::InputManager::IsButtonState(const int buttonCode, const Hardware& hardWare, const ButtonState& buttonState)
{
	switch (hardWare)
	{
	case(Hardware::KeyBoard):
		return (m_KeyBoardStates[buttonCode] == buttonState);
		break;
	case(Hardware::Controller):
		return (m_ControllerStates[buttonCode] == buttonState);
		break;
	case(Hardware::Mouse):
		return (m_MouseStates[buttonCode] == buttonState);
		break;
	default:
		return false;
	}
}

bool MyEngine::InputManager::IsPressed(const int buttonCode, const Hardware& hardWare)
{
	return IsButtonState(buttonCode, hardWare, ButtonState::Pressed);
}

bool MyEngine::InputManager::IsDown(const int buttonCode, const Hardware& hardWare)
{
	return IsButtonState(buttonCode, hardWare, ButtonState::Down);
}

bool MyEngine::InputManager::IsReleased(const int buttonCode, const Hardware& hardWare)
{
	return IsButtonState(buttonCode, hardWare, ButtonState::Released);
}

void MyEngine::InputManager::AddCommand(const int buttonCode, const Hardware& hardWare, const Command* command)
{
	switch (hardWare)
	{
	case(Hardware::KeyBoard):
		AddCommand(m_KeyBoardMappings, buttonCode, command);
		m_KeyBoardStates[buttonCode] = ButtonState::None;
		break;
	case(Hardware::Controller):
		AddCommand(m_ControllerMappings, buttonCode, command);
		m_ControllerStates[buttonCode] = ButtonState::None;
		break;
	case(Hardware::Mouse):
		AddCommand(m_MouseMappings, buttonCode, command);
		m_MouseStates[buttonCode] = ButtonState::None;
		break;
	default:
		break;
	}
}

void MyEngine::InputManager::UpdateStates(const Hardware& hardWare)
{
	XINPUT_STATE* pState{};
	WORD buttons{};
	switch (hardWare)
	{
	case(Hardware::KeyBoard):
		for (std::pair<const int, ButtonState>& button : m_KeyBoardStates)
			UpdateState(GetKeyState(button.first) & 0x8000, button);
		break;
	case(Hardware::Controller):
		pState = new XINPUT_STATE();
		XInputGetState(1, pState);
		buttons = pState->Gamepad.wButtons;
		delete pState;
		for (std::pair<const int, ButtonState>& button : m_ControllerStates)
			UpdateState(buttons & WORD(button.first), button);
		break;
	case(Hardware::Mouse):
		for (std::pair<const int, ButtonState>& button : m_MouseStates)
			UpdateState(GetKeyState(button.first) & 0x80, button);
		break;
	default:
		break;
	}
}

void MyEngine::InputManager::UpdateState(const bool down, std::pair<const int, ButtonState>& button)
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

void MyEngine::InputManager::ExecuteCommands(const Hardware& hardWare, const std::map<const int, std::vector<const Command*>>& mappings)
{
	for (const std::pair<const int, std::vector<const Command*>>& pair : mappings)
	{
		for (const Command* command : pair.second)
		{
			if (IsButtonState(pair.first, hardWare, command->State))
				command->Action();
		}
	}
}

void MyEngine::InputManager::AddCommand(std::map<const int, std::vector<const Command*>>& mappings, const int buttonCode, const Command* command)
{
	for (const Command* pCommand : mappings[buttonCode])
	{
		if (pCommand == command)
		{
			Logger::GetInstance()->LogWarning("Command already added to button");
			return;
		}
	}
	mappings[buttonCode].push_back(command);
}
