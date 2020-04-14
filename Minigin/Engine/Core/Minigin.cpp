#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include <thread>
#include "../Managers/InputManager.h"
#include "../Managers/SceneManager.h"
#include "../Graphics/Renderer.h"
#include "../Managers/ResourceManager.h"
#include <SDL.h>
#include "../Scene/GameObject.h"
#include "../Scene/Scene.h"
#include "../Components/Components.h"
#include <vld.h>
#include "../Helpers/Logger.h"

using namespace std;
using namespace std::chrono;
const float MyEngine::Minigin::SecPerFrame = 0.016f;

void MyEngine::Minigin::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}
	Renderer::GetInstance()->Init(m_Window);
	Logger::GetInstance()->Initialize();
	ResourceManager::GetInstance()->Init("../Data/");
}

/**
 * Code constructing the scene world starts here
 */
void MyEngine::Minigin::LoadGame() const
{
	auto& scene = SceneManager::GetInstance()->CreateScene("Demo");

	GameObject* go = new GameObject();
	go->AddComponent(new TransformComponent());
	go->AddComponent(new RenderComponent(1));
	go->GetComponent<RenderComponent>()->AddTexture(ResourceManager::GetInstance()->LoadTexture("background.jpg"));
	scene.Add(go);

	go = new GameObject();
	go->AddComponent(new TransformComponent({ 216.0f, 180.0f }));
	go->AddComponent(new RenderComponent(1));
	go->GetComponent<RenderComponent>()->AddTexture(ResourceManager::GetInstance()->LoadTexture("logo.png"));
	scene.Add(go);

	go = new GameObject();
	go->AddComponent(new TransformComponent({ 80.0f, 20.0f }));
	go->AddComponent(new RenderComponent(1));
	go->AddComponent(new TextComponent("Programming 4 Assignment", ResourceManager::GetInstance()->LoadFont("Lingua.otf", 36), { 255, 255, 255 }));
	go->GetComponent<RenderComponent>()->AddTexture(go->GetComponent<TextComponent>()->GetTexture());
	scene.Add(go);

	go = new GameObject();
	go->AddComponent(new TransformComponent());
	go->AddComponent(new RenderComponent(1));
	go->AddComponent(new TextComponent(" ", ResourceManager::GetInstance()->LoadFont("Lingua.otf", 36), { 255, 255, 0 }));
	go->GetComponent<RenderComponent>()->AddTexture(go->GetComponent<TextComponent>()->GetTexture());
	go->AddComponent(new FPSComponent());
	scene.Add(go);
}

void MyEngine::Minigin::Cleanup()
{
	Renderer::GetInstance()->Destroy();
	SDL_DestroyWindow(m_Window);
	Renderer::Release();
	SceneManager::Release();
	InputManager::Release();
	ResourceManager::Release();
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
