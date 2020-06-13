#include "Engine\Core\MiniginPCH.h"
#include "Engine\Core\Minigin.h"
#include "Engine\Managers\SceneManager.h"
#include "Engine\Managers\ResourceManager.h"
#include "Engine\Managers\InputManager.h"
#include "Engine\Managers\SoundManager.h"
#include "Engine\Managers\PhysicsManager.h"
#include "Engine\Helpers\Sound.h"
#include "Engine\Components\Components.h"
#include "Engine\Scene\GameObject.h"
#include "Engine\Scene\Scene.h"
#include "DataHolder.h"
#include "Box2D.h"
#include "Factory.h"

void LoadScene();

int main(int, char* []) {
	int windowWidth = 640;

	MyEngine::Minigin engine;
	engine.Initialize("../Resources/", "Programming 4 assignment", windowWidth, windowWidth / 32 * 25, windowWidth / 32.0f, "MenuScene");
	MyEngine::PhysicsManager::GetInstance()->EnableDebugDrawing(true);
	DataHolder::GetInstance()->Init(windowWidth, windowWidth / 32 * 25);
	CreatePlayer(1, windowWidth / 32 * 25);
	CreatePlayer(2, windowWidth / 32 * 25);
	CreateMenu(windowWidth / 32 * 25);
	//CreateLevel(0, windowWidth / 32 * 25);
	//LevelManager::GetInstance()->Notify(MyEngine::Event(LevelManager::IncreaseLevel));
	//LoadScene();
	engine.Run();
	DataHolder::Release();
	LevelManager::Release();
	return 0;
}

class TestComponent final : public MyEngine::BaseComponent
{
public:
	TestComponent(MyEngine::GameObject* other) :m_pOther(other) {}

	void Update(const float) override
	{
		if (m_pGameObject->GetComponent<MyEngine::PhysicsComponent>()->IsOverlapping(m_pOther->GetComponent<MyEngine::PhysicsComponent>()))
			std::cout << "test" << std::endl;
	}
	void FixedUpdate(const float) override
	{
		std::cout << InputManager::GetInstance()->GetMousePos().x << " " << InputManager::GetInstance()->GetMousePos().y << std::endl;
	}
	void Render() const override
	{

	}
private:
	MyEngine::GameObject* m_pOther;
};

void LoadScene()
{
	using namespace MyEngine;
	Scene* scene = new Scene("Demo");
	SceneManager::GetInstance()->AddScene(scene);

	enum States : int
	{
		IDLE = 0,
		RUNNING = 1
	};

	MyEngine::GameObject* go = new MyEngine::GameObject({ 320.0f, 250.0f });
	go->SetState(States::IDLE);
	go->AddComponent(new RenderComponent());
	go->GetComponent<RenderComponent>()->AddTexture("background.jpg", false, false, 0, 0, 0.0f, 0, 0, { 0.5f, 0.5f }, States::IDLE);
	scene->Add(go);

	SoundManager::GetInstance()->LoadSoundEffect("GunShot.wav", Event(0));
	MyEngine::InputManager::GetInstance()->AddCommand({ {XINPUT_GAMEPAD_A, MyEngine::Hardware::Controller, 0}, {VK_LBUTTON, MyEngine::Hardware::Mouse} }, new MyEngine::Command{ []() { SoundManager::GetInstance()->Notify(Event(0)); }, MyEngine::ButtonState::Pressed });

	go = new MyEngine::GameObject({ 320.0f, 200.0f });
	go->AddComponent(new RenderComponent());
	go->GetComponent<RenderComponent>()->AddTexture("logo.png", true, false, 1, 3, 1.0f, 16, 50, { 0.5f, 0.5f }, -1, { 0.3333f, 0.0f }, { 0.3333f, 1.0f });
	scene->Add(go);
	
	go = new MyEngine::GameObject({ 320.0f, 300.0f });
	go->AddComponent(new RenderComponent());
	go->GetComponent<RenderComponent>()->AddTexture("logo.png", false, false, 0, 0, 0.0f, 50, 50, { 0.5f, 0.5f }, -1, { 0.3333f, 0.0f }, { 0.3333f, 1.0f });
	scene->Add(go);

	MyEngine::GameObject* testGo = new MyEngine::GameObject();
	testGo->AddComponent(new RenderComponent());
	testGo->GetComponent<RenderComponent>()->AddTexture("logo.png", false, false, 0, 0, 0.0f, 150, 50);
	testGo->AddComponent(new PhysicsComponent(MyEngine::PhysicsComponent::PhysicsType::Dynamic, { 300.0f, 300.0f }, 45.0f, 75.0f, 25.0f, 1.0f, 0.3f, 0.0f));
	scene->Add(testGo);

	go = new MyEngine::GameObject();
	go->AddComponent(new RenderComponent());
	go->GetComponent<RenderComponent>()->AddTexture("logo.png", false, false, 0, 0, 0.0f, 150, 50);
	go->AddComponent(new PhysicsComponent(MyEngine::PhysicsComponent::PhysicsType::Kinematic, { 300.0f, 10.0f }, 0.0f, 75.0f, 25.0f, 1.0f, 0.3f, 0.0f));
	go->AddComponent(new TestComponent(testGo));
	scene->Add(go);

	go = new MyEngine::GameObject({ 80.0f, 480.0f });
	go->AddComponent(new TextComponent("Programming 4 Assignment", ResourceManager::GetInstance()->LoadFont("Lingua.otf", 36), { 255, 255, 255 }, { 0.0f, 0.0f }));
	scene->Add(go);
	
	go = new MyEngine::GameObject({ 0.0f, 500.0f });
	go->AddComponent(new TextComponent(" ", ResourceManager::GetInstance()->LoadFont("Lingua.otf", 36), { 255, 255, 0 }, { 0.0f, 0.0f }));
	go->AddComponent(new FPSComponent());
	scene->Add(go);
}