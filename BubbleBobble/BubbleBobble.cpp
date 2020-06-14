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

void LoadTestScene()
{
	using namespace MyEngine;
	Scene* scene = new Scene("Demo", false);
	SceneManager::GetInstance()->AddScene(scene);

	GameObject* go = new GameObject({ 320.f , 250.f });
	go->AddComponent(new RenderComponent());
	go->GetComponent<RenderComponent>()->AddTexture(ResourceManager::GetInstance()->LoadTexture("background.jpg"));
	scene->Add(go);

	go = new GameObject({ 536.f, 320.f });
	//x + half, y-half if not work test out
	RenderComponent* pRenderComp = new RenderComponent();
	pRenderComp->AddTexture(ResourceManager::GetInstance()->LoadTexture("logo.png"));
	go->AddComponent(pRenderComp);
	scene->Add(go);

	go = new GameObject({ 80.f, 480.f });
	go->AddComponent(new TextComponent("Programming 4 Assignment", ResourceManager::GetInstance()->LoadFont("Lingua.otf", 36), { 255,255,255 }));
	scene->Add(go);

	go = new GameObject({ 0.f, 500.f });
	go->AddComponent(new TextComponent(" ", ResourceManager::GetInstance()->LoadFont("Lingua.otf", 18), { 255, 255,0 }));
	go->AddComponent(new FPSComponent());
	scene->Add(go);

	go = new GameObject();
	RenderComponent* pSecondRenderComp = new RenderComponent();
	pSecondRenderComp->AddTexture(ResourceManager::GetInstance()->LoadTexture("logo.png"));
	go->AddComponent(pSecondRenderComp);
	go->AddComponent(new PhysicsComponent(PhysicsComponent::PhysicsType::Dynamic, { 320.f, 250.f }, 45.f, 104.f, 32.5f, 10.f, 0.3f, 0.0f));
	scene->Add(go);

	go = new GameObject();
	RenderComponent* pThirdRenderComp = new RenderComponent();
	pThirdRenderComp->AddTexture(ResourceManager::GetInstance()->LoadTexture("logo.png"));
	go->AddComponent(pThirdRenderComp);
	go->AddComponent(new PhysicsComponent(PhysicsComponent::PhysicsType::Kinematic, { 320.f, 10.f }, 0.f, 104.f, 32.5f, 10.f, 0.3f, 0.0f));
	scene->Add(go);
}

int main(int, char* []) {
	int windowWidth = 640;

	MyEngine::Minigin engine;
	engine.Initialize("../Resources/", "Programming 4 assignment", windowWidth, windowWidth / 32 * 25, windowWidth / 32.0f, "MenuScene");
	MyEngine::PhysicsManager::GetInstance()->EnableDebugDrawing(true);
	LoadTestScene();
	//DataHolder::GetInstance()->Init(windowWidth, windowWidth / 32 * 25);
	//SceneManager::GetInstance()->AddScene(new Scene("BufferScene"));
	//CreateSounds();
	//CreateMenu(windowWidth / 32 * 25);
	//SceneManager::GetInstance()->SetSceneActive("MenuScene");
	//SoundManager::GetInstance()->Notify(MyEngine::Event(SoundEvents::SoundTrack), true);
	engine.Run();
	//DataHolder::Release();
	//LevelManager::Release();
	return 0;
}