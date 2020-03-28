#include "MiniginPCH.h"
#include "InputManager.h"
#include <SDL.h>
#include "../Scene/GameObject.h"
#include "../Helpers/Logger.h"


MyEngine::InputManager::~InputManager()
{
	for (std::pair<ControllerButton, std::vector<Command*>> pair : m_ControllerMappings)
	{
		for (Command* command : pair.second)
			delete command;
	}

	for (std::pair<KeyBoardKey, std::vector<Command*>> pair : m_KeyBoardMappings)
	{
		for (Command* command : pair.second)
			delete command;
	}
}

bool MyEngine::InputManager::ProcessSDLEvents()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
	}
	return true;
}

void MyEngine::InputManager::ProcessInput(GameObject* object)
{
	for (std::pair<ControllerButton, std::vector<Command*>> pair : m_ControllerMappings)
	{
		if (IsPressed(pair.first))
		{
			for (Command* command : pair.second)
				command->Execute(object);
		}
	}

	for (std::pair<KeyBoardKey, std::vector<Command*>> pair : m_KeyBoardMappings)
	{
		if (IsPressed(pair.first))
		{
			for (Command* command : pair.second)
				command->Execute(object);
		}
	}
}

bool MyEngine::InputManager::IsPressed(ControllerButton button) const
{
	XINPUT_STATE* pState = new XINPUT_STATE();
	XInputGetState(1, pState);
	WORD buttons = pState->Gamepad.wButtons;
	delete pState;
	return buttons & WORD(button);
}

bool MyEngine::InputManager::IsPressed(KeyBoardKey key) const
{
	return(GetKeyState(int(key)) & 0x8000);
}

void MyEngine::InputManager::AddCommand(ControllerButton button, Command* command)
{
	for (Command* pCommand : m_ControllerMappings[button])
	{
		if (typeid(*pCommand) == typeid(*command))
		{
			Logger::GetInstance()->LogWarning("Command already added to button (Created memory leak)");
			return;
		}
	}
	m_ControllerMappings[button].push_back(command);
}

void MyEngine::InputManager::AddCommand(KeyBoardKey key, Command* command)
{
	for (Command* pCommand : m_KeyBoardMappings[key])
	{
		if (typeid(*pCommand) == typeid(*command))
		{
			Logger::GetInstance()->LogWarning("Command already added to key (Created memory leak)");
			return;
		}
	}
	m_KeyBoardMappings[key].push_back(command);
}

