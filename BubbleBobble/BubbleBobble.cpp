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

int main(int, char* []) {
	int windowWidth = 640;

	MyEngine::Minigin engine;
	engine.Initialize("../Resources/", "Programming 4 assignment", windowWidth, windowWidth / 32 * 25, windowWidth / 32.0f, "MenuScene");
	MyEngine::PhysicsManager::GetInstance()->EnableDebugDrawing(true);
	DataHolder::GetInstance()->Init(windowWidth, windowWidth / 32 * 25);
	SceneManager::GetInstance()->AddScene(new Scene("BufferScene"));
	CreateSounds();
	CreateMenu(windowWidth / 32 * 25);
	SceneManager::GetInstance()->SetSceneActive("MenuScene");
	SoundManager::GetInstance()->Notify(MyEngine::Event(SoundEvents::SoundTrack), true);
	engine.Run();
	DataHolder::Release();
	LevelManager::Release();
	return 0;
}