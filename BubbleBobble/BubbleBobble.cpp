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