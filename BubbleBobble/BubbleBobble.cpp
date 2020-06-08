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
#include "LevelDataParser.h"
#include "Box2D.h"

void LoadScene();

int main(int, char* []) {
	MyEngine::Minigin engine;
	engine.Initialize("../Resources/", "Programming 4 assignment", 640, 500, 20.0f);
	std::vector<Level> levels;
	LevelDataParser::ParseFile("../Resources/BBSprites/FixedLevelData.dat", levels);
	MyEngine::PhysicsManager::GetInstance()->EnableDebugDrawing(true);
	LoadScene();
	engine.Run();
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
	auto& scene = SceneManager::GetInstance()->CreateScene("Demo");

	enum States : int
	{
		IDLE = 0,
		RUNNING = 1
	};

	GameObject* go = new GameObject({ 320.0f, 250.0f });
	go->SetState(States::IDLE);
	go->AddComponent(new RenderComponent());
	go->GetComponent<RenderComponent>()->AddTexture("background.jpg", false, 0, 0, 0.0f, 0, 0, { 0.5f, 0.5f }, States::IDLE);
	scene.Add(go);

	SoundEffect* sound = SoundManager::GetInstance()->LoadSoundEffect("GunShot.wav");
	MyEngine::InputManager::GetInstance()->AddCommand(VK_LBUTTON, MyEngine::Hardware::Mouse, new MyEngine::Command{ [sound]() { sound->Play(0); }, MyEngine::ButtonState::Pressed });

	go = new GameObject({ 320.0f, 200.0f });
	go->AddComponent(new RenderComponent());
	go->GetComponent<RenderComponent>()->AddTexture("logo.png", true, 1, 3, 1.0f, 50, 50);
	scene.Add(go);

	GameObject* testGo = new GameObject();
	testGo->AddComponent(new RenderComponent());
	testGo->GetComponent<RenderComponent>()->AddTexture("logo.png", false, 0, 0, 0.0f, 150, 50);
	testGo->AddComponent(new PhysicsComponent(MyEngine::PhysicsComponent::PhysicsType::Dynamic, 300.0f, 300.0f, 45.0f, 75.0f, 25.0f, 1.0f, 0.3f, 0.2f, 0x0001));
	//testGo->GetComponent<PhysicsComponent>()->SetTrigger(true);
	scene.Add(testGo);

	go = new GameObject();
	go->AddComponent(new RenderComponent());
	go->GetComponent<RenderComponent>()->AddTexture("logo.png", false, 0, 0, 0.0f, 150, 50);
	go->AddComponent(new PhysicsComponent(MyEngine::PhysicsComponent::PhysicsType::Kinematic, 300.0f, 10.0f, 0.0f, 75.0f, 25.0f, 1.0f, 0.3f, 0.2f, 0x0001));
	go->AddComponent(new TestComponent(testGo));
	scene.Add(go);

	go = new GameObject({ 80.0f, 480.0f });
	go->AddComponent(new TextComponent("Programming 4 Assignment", ResourceManager::GetInstance()->LoadFont("Lingua.otf", 36), { 255, 255, 255 }, { 0.0f, 0.0f }));
	scene.Add(go);
	
	go = new GameObject({ 0.0f, 500.0f });
	go->AddComponent(new TextComponent(" ", ResourceManager::GetInstance()->LoadFont("Lingua.otf", 36), { 255, 255, 0 }, { 0.0f, 0.0f }));
	go->AddComponent(new FPSComponent());
	scene.Add(go);
}