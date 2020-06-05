#include "Engine\Core\MiniginPCH.h"
#include "Engine\Core\Minigin.h"
#include "Engine\Managers\SceneManager.h"
#include "Engine\Managers\ResourceManager.h"
#include "Engine\Components\Components.h"
#include "Engine\Scene\GameObject.h"
#include "Engine\Scene\Scene.h"

void LoadScene();

int main(int, char* []) {
	MyEngine::Minigin engine;
	engine.Initialize();
	LoadScene();
	engine.Run();
	return 0;
}

void LoadScene()
{
	using namespace MyEngine;
	auto& scene = SceneManager::GetInstance()->CreateScene("Demo");

	GameObject* go = new GameObject();
	go->AddComponent(new RenderComponent(1));
	go->GetComponent<RenderComponent>()->AddTexture("background.jpg", true, 2, 2, 1.0f);
	scene.Add(go);

	go = new GameObject();
	go->GetComponent<TransformComponent>()->SetPosition(216.0f, 180.0f);
	go->AddComponent(new RenderComponent(1));
	go->GetComponent<RenderComponent>()->AddTexture("logo.png");
	scene.Add(go);

	go = new GameObject();
	go->GetComponent<TransformComponent>()->SetPosition(80.0f, 20.0f);
	go->AddComponent(new TextComponent("Programming 4 Assignment", ResourceManager::GetInstance()->LoadFont("Lingua.otf", 36), { 255, 255, 255 }));
	scene.Add(go);
	
	go = new GameObject();
	go->AddComponent(new TextComponent(" ", ResourceManager::GetInstance()->LoadFont("Lingua.otf", 36), { 255, 255, 0 }));
	go->AddComponent(new FPSComponent());
	scene.Add(go);
}