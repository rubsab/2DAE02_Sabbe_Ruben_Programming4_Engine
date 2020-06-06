#include "Engine\Core\MiniginPCH.h"
#include "Engine\Core\Minigin.h"
#include "Engine\Managers\SceneManager.h"
#include "Engine\Managers\ResourceManager.h"
#include "Engine\Managers\InputManager.h"
#include "Engine\Managers\SoundManager.h"
#include "Engine\Helpers\Sound.h"
#include "Engine\Components\Components.h"
#include "Engine\Scene\GameObject.h"
#include "Engine\Scene\Scene.h"
#include "LevelDataParser.h"

void LoadScene();

int main(int, char* []) {
	MyEngine::Minigin engine;
	engine.Initialize("../Resources/");
	std::vector<Level> levels;
	LevelDataParser::ParseFile("../Resources/BBSprites/FixedLevelData.dat", levels);
	LoadScene();
	engine.Run();
	return 0;
}

void LoadScene()
{
	enum States : int
	{
		IDLE = 0,
		RUNNING = 1
	};

	using namespace MyEngine;
	auto& scene = SceneManager::GetInstance()->CreateScene("Demo");

	GameObject* go = new GameObject();
	go->SetState(States::IDLE);
	go->AddComponent(new RenderComponent(1));
	go->GetComponent<RenderComponent>()->AddTexture("background.jpg", States::IDLE);
	scene.Add(go);

	SoundEffect* sound = SoundManager::GetInstance()->LoadSoundEffect("GunShot.wav");
	MyEngine::InputManager::GetInstance()->AddCommand(VK_LBUTTON, MyEngine::Hardware::Mouse, new MyEngine::Command{ [sound]() { sound->Play(0); }, MyEngine::ButtonState::Pressed });

	go = new GameObject();
	go->GetComponent<TransformComponent>()->SetPosition({ 216.0f, 180.0f });
	go->AddComponent(new RenderComponent(1));
	go->GetComponent<RenderComponent>()->AddTexture("logo.png", 1, 3, 1.0f, 50, 50);
	scene.Add(go);

	go = new GameObject();
	go->GetComponent<TransformComponent>()->SetPosition({ 80.0f, 20.0f });
	go->AddComponent(new TextComponent("Programming 4 Assignment", ResourceManager::GetInstance()->LoadFont("Lingua.otf", 36), { 255, 255, 255 }));
	scene.Add(go);
	
	go = new GameObject();
	go->AddComponent(new TextComponent(" ", ResourceManager::GetInstance()->LoadFont("Lingua.otf", 36), { 255, 255, 0 }));
	go->AddComponent(new FPSComponent());
	scene.Add(go);
}