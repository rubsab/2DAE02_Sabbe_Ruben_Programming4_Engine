#include "MiniginPCH.h"
#include "InputManager.h"
#include "../Scene/GameObject.h"
#include "../Helpers/Logger.h"
#include "SceneManager.h"
#include "../Scene/Scene.h"

MyEngine::InputManager::~InputManager()
{
	DestroyMap(m_ControllerMappings);
	DestroyMap(m_KeyBoardMappings);
	DestroyMap(m_MouseMappings);
}

void MyEngine::InputManager::Init(SDL_Window* pWindow, const std::string& ignoreScene)
{
	m_pWindow = pWindow;
	m_ShouldQuit = false;
	m_IgnoreScene = ignoreScene;
}

bool MyEngine::InputManager::ProcessSDLEvents() const
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT || m_ShouldQuit) {
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
	m_OldMousePos = m_MousePos;
	SDL_GetMouseState(&m_MousePos.x, &m_MousePos.y);
}

bool MyEngine::InputManager::IsButtonState(const Button& button, const ButtonState& buttonState)
{
	switch (button.HardWare)
	{
	case(Hardware::KeyBoard):
		return (m_KeyBoardStates[{button.ButtonCode, button.Id}] == buttonState);
		break;
	case(Hardware::Controller):
		return (m_ControllerStates[{button.ButtonCode, button.Id}] == buttonState);
		break;
	case(Hardware::Mouse):
		return (m_MouseStates[{button.ButtonCode, button.Id}] == buttonState);
		break;
	default:
		return false;
	}
}

bool MyEngine::InputManager::IsPressed(const Button& button)
{
	return IsButtonState(button, ButtonState::Pressed);
}

bool MyEngine::InputManager::IsDown(const Button& button)
{
	return IsButtonState(button, ButtonState::Down);
}

bool MyEngine::InputManager::IsReleased(const Button& button)
{
	return IsButtonState(button, ButtonState::Released);
}

void MyEngine::InputManager::AddCommand(std::vector<Button> buttons, Command* command)
{
	for (const Button& button : buttons)
	{
		switch (button.HardWare)
		{
		case(Hardware::KeyBoard):
			AddCommand(m_KeyBoardMappings, button.ButtonCode, command, button.Id);
			m_KeyBoardStates[{button.ButtonCode, button.Id}] = ButtonState::None;
			break;
		case(Hardware::Controller):
			AddCommand(m_ControllerMappings, button.ButtonCode, command, button.Id);
			m_ControllerStates[{button.ButtonCode, button.Id}] = ButtonState::None;
			break;
		case(Hardware::Mouse):
			AddCommand(m_MouseMappings, button.ButtonCode, command, button.Id);
			m_MouseStates[{button.ButtonCode, button.Id}] = ButtonState::None;
			break;
		default:
			break;
		}
	}
}

const SDL_Point MyEngine::InputManager::GetMousePos() const
{
	int height;
	SDL_GetWindowSize(m_pWindow, nullptr, &height);
	return { m_MousePos.x, height - m_MousePos.y };
}

const SDL_Point MyEngine::InputManager::GetMouseMovement() const
{
	return { m_MousePos.x - m_OldMousePos.x, m_OldMousePos.y - m_MousePos.y };
}

void MyEngine::InputManager::RemoveCommandsByIdentifierName(const std::string& idName)
{
	ClearCommmandsByIDName(idName, m_ControllerMappings);
	ClearCommmandsByIDName(idName, m_KeyBoardMappings);
	ClearCommmandsByIDName(idName, m_MouseMappings);
}

void MyEngine::InputManager::DestroyMap(std::map<std::pair<const int, const int>, std::vector<Command*>>& map)
{
	for (std::pair<const std::pair<const int, const int>, std::vector<Command*>>& pair : map)
	{
		for (size_t i{}; i < pair.second.size(); i++)
		{
			if (pair.second[i] && !pair.second[i]->IsDeleted)
			{
				pair.second[i]->IsDeleted = true;
				Safe_Delete(pair.second[i]);
			}
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
		for (std::pair<const std::pair<const int, const int>, ButtonState>& button : m_ControllerStates)
		{
			pState = new XINPUT_STATE();
			XInputGetState(button.first.second, pState);
			buttons = pState->Gamepad.wButtons;
			UpdateState(buttons & WORD(button.first.first), button);
			Safe_Delete(pState);
		}
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
			if (IsButtonState({ pair.first.first, hardWare, pair.first.second }, command->State) && ((command->SceneName.empty() && SceneManager::GetInstance()->GetActiveScene()->GetName() != m_IgnoreScene) || command->SceneName == SceneManager::GetInstance()->GetActiveScene()->GetName()))
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

void MyEngine::InputManager::ClearCommmandsByIDName(const std::string& idName, std::map<std::pair<const int, const int>, std::vector<Command*>>& map)
{
	for (std::pair<const std::pair<const int, const int>, std::vector<Command*>>& pair : map)
	{
		for (size_t commandCounter{}; commandCounter < pair.second.size(); commandCounter++)
		{
			if (pair.second[commandCounter]->SceneName == idName)
			{
				Safe_Delete(pair.second[commandCounter]);
				pair.second[commandCounter] = pair.second.back();
				pair.second.pop_back();
				commandCounter--;
			}
		}
	}
}
