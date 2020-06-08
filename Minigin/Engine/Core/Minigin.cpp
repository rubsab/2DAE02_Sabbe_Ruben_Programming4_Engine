#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include <thread>
#include "../Managers/InputManager.h"
#include "../Managers/SceneManager.h"
#include "../Managers/ResourceManager.h"
#include "../Managers/SoundManager.h"
#include "../Managers/PhysicsManager.h"
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

void MyEngine::Minigin::Initialize(const std::string& dataPath, const char* windowTitle, const int windowWidth, const int windowHeight, const float ppm)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_pWindow = SDL_CreateWindow(
		windowTitle,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		windowWidth,
		windowHeight,
		SDL_WINDOW_OPENGL
	);
	if (m_pWindow == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}
	Renderer::GetInstance()->Init(m_pWindow, { 0, 0, 0, 255 });
	Logger::GetInstance()->Init();
	InputManager::GetInstance()->Init(m_pWindow);
	ResourceManager::GetInstance()->Init(dataPath + "Images/");
	SoundManager::GetInstance()->Init(dataPath + "Sound/");
	PhysicsManager::GetInstance()->Init({ 0.0f, -9.81f }, 8, 3, ppm, { 255, 0, 0, 255 });
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
	SDL_DestroyWindow(m_pWindow);
	Renderer::Release();
	SceneManager::Release();
	InputManager::Release();
	ResourceManager::Release();
	SoundManager::Release();
	PhysicsManager::Release();
	Logger::Release();
	m_pWindow = nullptr;
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
			PhysicsManager::GetInstance()->FixedUpdate(SecPerFrame);
			lag -= SecPerFrame;
		}
		Renderer::GetInstance()->Render();
	}

	Cleanup();
}
