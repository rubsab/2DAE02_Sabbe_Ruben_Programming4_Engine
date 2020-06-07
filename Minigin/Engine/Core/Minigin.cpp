#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include <thread>
#include "../Managers/InputManager.h"
#include "../Managers/SceneManager.h"
#include "../Managers/ResourceManager.h"
#include "../Managers/SoundManager.h"
#include "../Graphics/Renderer.h"
#include <SDL.h>
#include "../Scene/GameObject.h"
#include "../Scene/Scene.h"
#include "../Components/Components.h"
#include <vld.h>
#include "../Helpers/Logger.h"

using namespace std;
using namespace std::chrono;
const float MyEngine::Minigin::SecPerFrame = 0.016f;

void MyEngine::Minigin::Initialize(const std::string& dataPath, const char* windowTitle, const int windowWidth, const int windowHeight)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_Window = SDL_CreateWindow(
		windowTitle,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		windowWidth,
		windowHeight,
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}
	Renderer::GetInstance()->Init(m_Window);
	Logger::GetInstance()->Initialize();
	ResourceManager::GetInstance()->Init(dataPath + "Images/");
	SoundManager::GetInstance()->Init(dataPath + "Sound/");
}

/**
 * Code constructing the scene world starts here
 */
void MyEngine::Minigin::LoadGame() const
{
}

void MyEngine::Minigin::Cleanup()
{
	Renderer::GetInstance()->Destroy();
	SDL_DestroyWindow(m_Window);
	Renderer::Release();
	SceneManager::Release();
	InputManager::Release();
	ResourceManager::Release();
	SoundManager::Release();
	Logger::Release();
	m_Window = nullptr;
	SDL_Quit();
}

void MyEngine::Minigin::Run()
{

	bool doContinue = true;
	float lag = 0.0f;
	auto lastTime = high_resolution_clock::now();
	while (doContinue)
	{
		const auto currentTime = high_resolution_clock::now();
		float deltaTime = duration<float>(currentTime - lastTime).count();
		lastTime = currentTime;
		lag += deltaTime;
		doContinue = InputManager::GetInstance()->ProcessSDLEvents();
		InputManager::GetInstance()->ProcessInput();
		SceneManager::GetInstance()->Update(deltaTime);
		while (lag >= SecPerFrame)
		{
			SceneManager::GetInstance()->FixedUpdate(SecPerFrame);
			lag -= SecPerFrame;
		}
		Renderer::GetInstance()->Render();
	}

	Cleanup();
}
